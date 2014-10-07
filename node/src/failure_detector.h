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

#ifndef __sopmq__failure_detector__
#define __sopmq__failure_detector__

#include <boost/chrono/system_clocks.hpp>
#include <boost/circular_buffer.hpp>

namespace sopmq {
    namespace node {

        ///
        /// Given a set of ping times, this class predicts when a server has failed
        ///
        /// Based on the work done in:
        /// http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.80.7427&rep=rep1&type=pdf
        ///
        /// However like the apache cassandra implementation, this one uses an exponential distribution.
        /// There is a bit of information on this class in the docs directory
        ///
        class failure_detector
        {
        public:
            enum state 
            {
                UP,
                DOWN
            };

        public:
            ///
            /// Constructs a new failure detector with the given PHI threashold and initial interval values
            /// The initial interval value should be high enough that nodes aren't marked down right when
            /// this class is initialized, but low enough that it doesnt take a long time to detected a failure
            /// if one happens close to time this class is initializing
            ///
            failure_detector(int failureThreshold, boost::chrono::milliseconds initialIntervalValue);
            virtual ~failure_detector();

            ///
            /// Records a liveliness heartbeat
            ///
            void heartbeat();

            ///
            /// Returns the failure state for the monitored node based on our
            /// historical information
            ///
            state interpret() const;

        private:
            ///
            /// The number of intervals we keep to perform a running average on
            ///
            static const int SAMPLE_SIZE = 1000;

            ///
            /// The PHI number at or above which we consider ourselves failed
            ///
            int _failure_threshold;

            ///
            /// The last time heartbeat() was called
            ///
            boost::chrono::steady_clock::time_point _last_heartbeat;

            ///
            /// The history of heartbeat intervals we have received from the node
            ///
            boost::circular_buffer<boost::chrono::milliseconds::rep> _intervals;


            ///
            /// Calculates the PHI value from the given intervals, last heartbeat, and time now
            ///
            float current_phi() const;

            ///
            /// Calculates the current average from the intervals
            ///
            float current_interval_average() const;

            ///
            /// Calculates the current interval since the last heartbeat
            ///
            boost::chrono::milliseconds current_interval() const;
        };

    }
}

#endif