sopmq
=====

Scalable optionally persistent message queue

This project is in the very early stages of development and is not yet functional

SOPMQ's design is intended to provide an at least once message queue with partition tolerance. SOPMQ models a CP system.

### Windows note

This project uses the EditorConfig visual studio plugin to maintain tab and spacing settings.

You must specify the paths to various dependencies under windows. An example would be:

cmake -D LIBCASSANDRA_LIBRARY=D:\lib\cpp-driver\build\Debug\cassandra.lib -D LIBCASSANDRA_INCLUDE_DIR=D:\lib\cpp-driver\include -D CRYPTOPP_INCLUDE_DIR=D:\lib\cryptopp562 -D CRYPTOPP_LIBRARY=D:\lib\cryptopp562\cryptopp\x64\DLL_Output\Debug\cryptopp.lib -D CRYPTOPP_ROOT_DIR=D:\lib\cryptopp562\cryptopp -D PROTOBUF_LIBRARY=D:\lib\protobuf-2.5.0\vsprojects\x64\Debug\libprotobuf.lib -D PROTOBUF_INCLUDE_DIR=D:\lib\protobuf-2.5.0\src -D OPENSSL_ROOT_DIR=D:\lib\openssl-1.0.1h-bin .. -G "Visual Studio 11 Win64"

More details on building later as the project matures

