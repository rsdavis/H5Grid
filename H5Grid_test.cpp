
#include "H5Grid.hpp"
#include "gtest/gtest.h"


/*
TEST(Open, WriteMode) {
    H5Grid h5;
    int ndims = 3;
    int dims[ndims];
    dims[0] = 100;
    dims[1] = 120;
    dims[2] = 140;
    h5.open("test.h5", "w", dims, ndims);
    h5.close();
}

TEST(Open, ReadMode) {
    int dims[3];
    int ndims;
    H5Grid h5;
    h5.open("test.h5", "r", dims, ndims);
    EXPECT_EQ(ndims, 3);
    h5.close();
}

TEST(WriteTest, Writes) {
    H5Grid h5;
    h5.open("test.h5", "w", dims, 3);
    write_dataset(dataset_name, dataset);
}
*/


int main(int argc, char ** argv) {
    /*
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    */
    int dims[3] = {100, 120, 140};
    int ndims = 3;
    int value;
    H5Grid h5;
    h5.open("test.h5", "w", dims, ndims);
    h5.set_attribute("/path/to/my/Nx", 100);
    int stat = h5.get_attribute("/path/to/my/Nx", value);
    printf("stat = %d\n", stat);
    h5.close();
    std::cout << value << std::endl;
    return 0;
}
