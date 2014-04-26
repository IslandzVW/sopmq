//
//  endpoint.h
//  Project
//
//  Created by David Daeschler on 4/26/14.
//
//

#ifndef __Project__endpoint__
#define __Project__endpoint__

#include <string>

namespace sopmq {
    namespace node {
        namespace net {
            
            ///
            /// The endpoint protocol
            ///
            enum class endpoint_proto { SOPMQv1 };
            
            ///
            /// The endpoint for a node
            ///
            class endpoint
            {
            private:
                std::string _hostname;
                unsigned short _port;
                endpoint_proto _proto;
                
            public:
                ///
                /// Constructs an endpoint by parsing a uri
                ///
                endpoint(const std::string& uri);
                
                ///
                /// dtor
                ///
                virtual ~endpoint();

                
                ///
                /// The host name for this endpoint
                ///
                const std::string& host_name() const;
                
                ///
                /// The port for this endpoint
                ///
                unsigned short port() const;
                
                ///
                /// The protocol this endpoint speaks
                ///
                endpoint_proto protocol() const;
            };
            
        }
    }
}

#endif /* defined(__Project__endpoint__) */
