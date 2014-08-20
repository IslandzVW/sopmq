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

namespace sopmq {
    namespace node {
        
        ///
        /// A user account on this sopmq cluster
        ///
        class user_account
        {
            ///
            /// Returns whether or not a user is authorized to log in
            ///
            /// \param nameHashHexString A hex version of the sha256 hash of the username
            /// \param challengeBytes The bytes that were sent as part of the auth challenge
            /// \param passwordChallengeHashHexString The result of the sha256(sha256(pw) + challenge)
            ///
            static bool is_authorized(const std::string& nameHashHexString,
                                      const std::string& challengeBytes,
                                      const std::string& passwordChallengeHashHexString);
            
            ///
            /// Creates a new user, writes it to the datastore, and returns it
            ///
            static user_account create(const std::string& userName, const std::string& password);
        };
        
    }
}

#endif /* defined(__sopmq__user_account__) */
