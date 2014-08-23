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

#ifndef __sopmq__user_account__
#define __sopmq__user_account__

#include <string>
#include <functional>

namespace sopmq {
    namespace node {
        
        ///
        /// A user account on this sopmq cluster
        ///
        class user_account
        {
        public:
            ///
            /// \brief Returns whether or not a user is authorized to log in
            ///
            /// \param nameHashHexString A hex version of the sha256 hash of the username
            /// \param challengeBytes The bytes that were sent as part of the auth challenge
            /// \param passwordChallengeHashHexString The result of the sha256(sha256(pw) + challenge)
            /// \param authCallback Callback executed with the authorization status for the user
            ///
            static void is_authorized(const std::string& nameHashHexString,
                                      const std::string& challengeBytes,
                                      const std::string& passwordChallengeHashHexString,
                                      std::function<void(bool)> authCallback);
            
            ///
            /// \brief Creates a new user, writes it to the datastore, and returns it
            ///
            static user_account create(const std::string& userName, const std::string& password,
                                       int userLevel);
            
            ///
            /// \brief Finds a user by username hash
            ///
            static void find(const std::string& nameHashHexString,
                             std::function<void(bool, user_account)> findCallback);
            
        public:
            user_account();
            user_account(const std::string& nameHash, const std::string& userName,
                         const std::string& pwHash, int level);
            virtual ~user_account();
            
            ///
            /// \brief Returns the username for this account
            ///
            const std::string& username() const;
            
            ///
            /// \brief Returns the hex representation of the sha hash of the name
            ///
            const std::string& name_hash() const;
            
            ///
            /// \brief Returns the hex representation of the sha hash of the password
            ///
            const std::string& pw_hash() const;
            
            ///
            /// \brief Returns the user level of this account
            ///
            int user_level() const;
            
        private:
            std::string _name_hash;
            std::string _username;
            std::string _pw_hash;
            int _user_level;
        };
        
    }
}

#endif /* defined(__sopmq__user_account__) */
