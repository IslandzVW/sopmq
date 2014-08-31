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

#include "gtest/gtest.h"

#include "user_account.h"
#include "settings.h"
#include "util.h"

#include <cryptopp/sha.h>
#include <condition_variable>
#include <mutex>



using sopmq::node::settings;
using sopmq::node::user_account;
using sopmq::shared::util;

TEST(AccountTest, TestCreateAccount)
{
    if (settings::instance().cassandraSeeds.size() == 0) return;
    
    const char* const USERNAME = "test user";
    const char* const PASSWORD = "password";
    
    user_account::create(USERNAME, PASSWORD, 1);
    
    
    unsigned char hashResult[CryptoPP::SHA256::DIGESTSIZE];
    CryptoPP::SHA256 sha;
    sha.CalculateDigest(&hashResult[0], (unsigned char*)USERNAME, strlen(USERNAME));
    
    std::string nameHash = util::hex_encode(hashResult, CryptoPP::SHA256::DIGESTSIZE);
    
    std::mutex m;
    std::condition_variable cv;
    
    user_account foundAcct;
    bool returned = false;
    bool ufound = false;
    
    user_account::find(nameHash, [&] (bool found, user_account acct) {
        std::lock_guard<std::mutex> lk(m);
        ufound = found;
        returned = true;
        foundAcct = acct;
        cv.notify_one();
    });
    
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [&] {return returned == true;});
    
    ASSERT_EQ(true, ufound);
    ASSERT_EQ(1, foundAcct.user_level());
    ASSERT_EQ(USERNAME, foundAcct.username());
}

TEST(AccountTest, TestAccountAuthorized)
{
    if (settings::instance().cassandraSeeds.size() == 0) return;
    
    const char* const USERNAME = "test user";
    const char* const PASSWORD = "password";
    
    user_account::create(USERNAME, PASSWORD, 1);
    
    
    unsigned char hashResult[CryptoPP::SHA256::DIGESTSIZE];
    CryptoPP::SHA256 sha;
    sha.CalculateDigest(&hashResult[0], (unsigned char*)USERNAME, strlen(USERNAME));
    
    std::string nameHash = util::hex_encode(hashResult, CryptoPP::SHA256::DIGESTSIZE);
    
    sha.Restart();
    sha.CalculateDigest(&hashResult[0], (unsigned char*)PASSWORD, strlen(PASSWORD));
    
    std::string pwHash = util::hex_encode(hashResult, CryptoPP::SHA256::DIGESTSIZE);
    
    std::mutex m;
    std::condition_variable cv;
    
    auto challengeBytes = util::random_bytes(1024);
    auto pwHashAndChallenge = pwHash + challengeBytes;
    
    sha.Restart();
    sha.CalculateDigest(&hashResult[0], (unsigned char*)pwHashAndChallenge.c_str(), pwHashAndChallenge.length());
    
    auto challengeResponse = util::hex_encode(hashResult, CryptoPP::SHA256::DIGESTSIZE);
    
    bool uauthd = false;
    bool returned = false;
    
    user_account::is_authorized(nameHash, challengeBytes, challengeResponse, [&] (bool authd) {
        std::lock_guard<std::mutex> lk(m);
        returned = true;
        uauthd = authd;
        cv.notify_one();
    });
    
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [&] {return returned == true;});
    
    ASSERT_EQ(true, uauthd);
}