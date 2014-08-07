sopmq
=====

Scalable optionally persistent message queue

### Windows note

You must specify the paths to various dependencies under windows. An example would be:

cmake -D LIBCQL_LIBRARY=D:\lib\cpp-driver\build\x64\Debug\cassandra.lib -D LIBCQL_INCLUDE_DIR=D:\lib\cpp-driver\include -D CRYPTOPP_INCLUDE_DIR=D:\lib\cryptopp562 -D CRYPTOPP_LIBRARY=D:\lib\cryptopp562\cryptopp\x64\DLL_Output\Debug\cryptopp.lib -D CRYPTOPP_ROOT_DIR=D:\lib\cryptopp562\cryptopp -D PROTOBUF_LIBRARY=D:\lib\protobuf-2.5.0\vsprojects\Release\libprotobuf.lib -D PROTOBUF_INCLUDE_DIR=D:\lib\protobuf-2.5.0\src .. -G "Visual Studio 11 Win64"

More details on building later as the project matures

#### Additional notes

This project is in the very early stages of development and is not yet functional.