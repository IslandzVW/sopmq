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

/*
 * Random selector from https://gist.github.com/cbsmith/5538174
 *
 */

#ifndef Project_Header_h
#define Project_Header_h

#include <random>
#include <iterator>

namespace sopmq {
    namespace shared {

        template <typename RandomGenerator = std::default_random_engine>
        struct random_selector
        {
            //On most platforms, you probably want to use std::random_device("/dev/urandom")()
            random_selector(RandomGenerator g = RandomGenerator(std::random_device()()))
            : gen(g) {}
            
            template <typename Iter>
            Iter select(Iter start, Iter end) {
                std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
                std::advance(start, dis(gen));
                return start;
            }
            
            //convenience function
            template <typename Iter>
            Iter operator()(Iter start, Iter end) {
                return select(start, end);
            }
            
            //convenience function that works on anything with a sensible begin() and end(), and returns with a ref to the value type
            template <typename Container>
            auto operator()(const Container& c) -> decltype(*begin(c))& {
                return *select(begin(c), end(c));
            }
            
        private:
            RandomGenerator gen;
        };
    }
}

#endif
