#include "gtest/gtest.h"

#include "settings.h"

#include <string>

using sopmq::node::settings;

int main(int argc, char **argv) {
    printf("Running main() from test-main.cpp\n");
    testing::InitGoogleTest(&argc, argv);
    
    for (int i = 0; i < argc; ++i)
    {
        std::string argstr(argv[i]);
        
        if (argstr.length() > 5 && argstr.substr(0, 5) == "cass=")
        {
            //cassandra server specified
            std::string seed = argstr.substr(5);
            printf("Cassandra seed specified: %s\n", seed.c_str());
            settings::instance().cassandraSeeds.push_back(seed);
        }
    }
    
    return RUN_ALL_TESTS();
}

