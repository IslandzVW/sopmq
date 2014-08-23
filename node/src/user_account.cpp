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

#include "user_account.h"

#include "util.h"
#include "cassandra_storage.h"

#include <cryptopp/sha.h>

using sopmq::shared::util;
using sopmq::node::storage::cassandra_storage;

namespace sopmq {
    namespace node {
        
        user_account user_account::create(const std::string& userName, const std::string& password,
                                          int userLevel)
        {
            unsigned char hashResult[CryptoPP::SHA256::DIGESTSIZE];
            
            CryptoPP::SHA256 sha;
            
            sha.CalculateDigest(&hashResult[0], (unsigned char*)userName.c_str(), userName.length());
            std::string userHex = util::hex_encode(hashResult, CryptoPP::SHA256::DIGESTSIZE);
            
            sha.Restart();
            
            sha.CalculateDigest(&hashResult[0], (unsigned char*)password.c_str(), password.length());
            std::string pwHex = util::hex_encode(hashResult, CryptoPP::SHA256::DIGESTSIZE);
            
            cassandra_storage::instance().create_user(userHex, userName, pwHex, userLevel);
            
            return user_account(userHex, userName, pwHex, userLevel);
        }
        
        void user_account::is_authorized(const std::string& nameHashHexString,
                                         const std::string& challengeBytes,
                                         const std::string& passwordChallengeHashHexString,
                                         std::function<void(bool)> authCallback)
        {
            auto userLookupCb = [=](const storage::find_user_result& fur)
            {
                if (!fur.user_found || fur.account.user_level() == 0)
                {
                    authCallback(false);
                }
                
                //we need to take the sha256 of the stored password hash and the
                //auth challenge and compare that to the challenge response that
                //was passed in
                CryptoPP::SHA256 sha;
                
                unsigned char goodHash[CryptoPP::SHA256::DIGESTSIZE];
                std::string goodPwHashAndChallenge(fur.account.pw_hash() + challengeBytes);
                
                sha.CalculateDigest(&goodHash[0], (unsigned char*)goodPwHashAndChallenge.c_str(),
                                    goodPwHashAndChallenge.length());
                
                unsigned char givenHash[CryptoPP::SHA256::DIGESTSIZE];
                sha.CalculateDigest(&givenHash[0], (unsigned char*)passwordChallengeHashHexString.c_str(),
                                    passwordChallengeHashHexString.length());
                
                if (memcmp(goodHash, givenHash, CryptoPP::SHA256::DIGESTSIZE) == 0)
                {
                    authCallback(true);
                }
                else
                {
                    authCallback(false);
                }
            };
            
            cassandra_storage::instance().find_user(nameHashHexString, userLookupCb);
        }
        
        void user_account::find(const std::string& nameHashHexString,
                                std::function<void(bool, user_account)> findCallback)
        {
            auto userLookupCb = [=](const storage::find_user_result& fur)
            {
                if (!fur.user_found)
                {
                    findCallback(false, user_account());
                }
                else
                {
                    findCallback(true, fur.account);
                }
                
            };
            
            cassandra_storage::instance().find_user(nameHashHexString, userLookupCb);
        }
        
        user_account::user_account()
        : _user_level(0)
        {
            
        }
        
        user_account::user_account(const std::string& nameHash, const std::string& userName,
                                   const std::string& pwHash, int level)
        : _name_hash(nameHash), _username(userName), _pw_hash(pwHash), _user_level(level)
        {
            
        }
        
        user_account::~user_account()
        {
            
        }
        
        const std::string& user_account::username() const
        {
            return _username;
        }
        
        const std::string& user_account::name_hash() const
        {
            return _name_hash;
        }
        
        const std::string& user_account::pw_hash() const
        {
            return _pw_hash;
        }
        
        int user_account::user_level() const
        {
            return _user_level;
        }
        
    }
}