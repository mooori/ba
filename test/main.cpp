#include <gtest/gtest.h>

/**
 * Put main in separate file which is then linked, to ensure that
 * RUN_ALL_TESTS is run only once.
 */

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
