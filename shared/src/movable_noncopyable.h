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

#ifndef sopmq_movable_noncopyable_h
#define sopmq_movable_noncopyable_h

namespace sopmq {
    namespace shared {
        
        namespace noncopyable_  // protection from unintended ADL
        {
            class movable_noncopyable
            {
            protected:
                movable_noncopyable() {}
                movable_noncopyable(movable_noncopyable&&) {};
                ~movable_noncopyable() {}
            private:  // emphasize the following members are private
                movable_noncopyable( const movable_noncopyable& );
                const movable_noncopyable& operator=( const movable_noncopyable& );
            };
        }
        
        typedef noncopyable_::movable_noncopyable movable_noncopyable;
        
    }
}

#endif
