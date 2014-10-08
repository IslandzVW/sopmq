/*
 * SOPMQ - Scalable optionally persistent message queue
 * Copyright 2014 InWorldz, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "failure_detector.h"


#include <numeric>

namespace bc = boost::chrono;

namespace sopmq {
    namespace node {

        failure_detector::failure_detector(int failureThreshold, bc::milliseconds initialValue)
            : _failure_threshold(failureThreshold), 
            _last_heartbeat(bc::steady_clock::now()),
            _intervals(SAMPLE_SIZE, SAMPLE_SIZE, initialValue.count())
        {

        }

        failure_detector::~failure_detector()
        {
        }

        void failure_detector::heartbeat()
        {
            auto now = bc::steady_clock::now();

            bc::milliseconds interval = this->current_interval(now);
            _last_heartbeat = now;

            _intervals.push_back(interval.count());
        }

        failure_detector::state failure_detector::interpret() const
        {
            return this->interpret(bc::steady_clock::now());
        }

        failure_detector::state failure_detector::interpret(bc::steady_clock::time_point compareTo) const
        {
            if (this->current_phi(compareTo) >= _failure_threshold)
            {
                return DOWN;
            }
            else
            {
                return UP;
            }
        }

        float failure_detector::current_phi(bc::steady_clock::time_point compareTo) const
        {
            //based on the simplification in apache cassandra
            //PHI_FACTOR = 1.0 / Math.log(10.0)
            const float PHI_FACTOR = 0.43429448190f;

            bc::milliseconds interval = this->current_interval(compareTo);
            auto phi = interval.count() / this->current_interval_average();

            return PHI_FACTOR * phi;
        }

        float failure_detector::current_interval_average() const
        {
            return std::accumulate(_intervals.begin(), _intervals.end(), 0) / (float) SAMPLE_SIZE;
        }

        bc::milliseconds failure_detector::current_interval(bc::steady_clock::time_point compareTo) const
        {
            return bc::duration_cast<bc::milliseconds>(compareTo - _last_heartbeat);
        }
    }
}