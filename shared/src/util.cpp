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

#include "util.h"

#include "MurmurHash3/MurmurHash3.h"

#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

#include <random>
#include <limits>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/static_assert.hpp>

#include <cassert>

using namespace CryptoPP;

namespace sopmq {
    namespace shared {
        
        std::string util::hex_encode(const unsigned char* input, int inputSize)
        {
            std::string encoded;
            
            StringSource ss(input, inputSize, true, new HexEncoder(new StringSink(encoded)));
            
            return encoded;
        }
        
        std::string util::random_bytes(int count)
        {
            
            std::random_device rng;
            std::uniform_int_distribution<> dist(std::numeric_limits<char>::min(),
                                                           std::numeric_limits<char>::max());
            
            std::string outstr;
            outstr.reserve(count);
            
            for(int i = 0; i < count; ++i) {
                outstr += (char)dist(rng);
            }
            
            return outstr;
        }
        
        boost::uuids::uuid util::random_uuid()
        {
            boost::uuids::random_generator gen;
            return gen();
        }

        uint128 util::murmur_hash3(const void * key, int len)
        {
            const int SEED = -147483647;

            uint128 hash;
            BOOST_STATIC_ASSERT(sizeof(hash) == 16);
            MurmurHash3_x64_128(key, len, SEED, &hash.lo);

            return hash;
        }
        
        std::string* util::uuid_to_bytes(const boost::uuids::uuid& id)
        {
            return new std::string(id.begin(), id.end());
        }

        util::util()
        {
            
        }
        
        util::~util()
        {
            
        }
    }
}