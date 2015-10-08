#include "H5Grid.hpp"
#include "gtest/gtest.h"
#include <vector>

TEST(H5GridTest, OpenClose)
{
    H5Grid h0, h1, h2;
    int stat;

    stat = h0.open("test1.h5", "w");
    ASSERT_EQ(stat, 0);

    stat = h0.open("test2.h5", "w");
    ASSERT_NE(stat, 0);

    stat = h0.close();
    ASSERT_EQ(stat, 0);

    stat = h0.close();
    ASSERT_NE(stat, 0);

    stat = h1.open("test1.h5", "r");
    ASSERT_EQ(stat, 0);
    stat = h1.close();
    ASSERT_EQ(stat, 0);

    stat = h2.open("test1.h5", "a");
    ASSERT_EQ(stat, 0);
    stat = h2.close();
    ASSERT_EQ(stat, 0);


}

TEST(H5GridTest, ReadWrite)
{
    H5Grid h0, h1;
    int ndims = 2;
    int dims[2] = {10,20};
    std::vector<int> h5dims;
    h5dims.assign(dims, dims+2); 
    double * data1 = new double [dims[0]*dims[1]];
    double * data2 = new double [dims[0]*dims[1]];
    int stat;

    stat = h0.open("test4.h5", "w");
    ASSERT_EQ(stat, 0);

    for (int i=0; i<dims[0]*dims[1]; i++) data1[i] = i;

    stat = h0.write_dataset("c/frame_000", data1, dims, ndims);
    ASSERT_EQ(stat, 0);

    stat = h0.read_dataset("/c/frame_000", data2);
    ASSERT_EQ(stat, 0);

    for (int i=0; i<dims[0]*dims[1]; i++) 
        EXPECT_EQ(data1[i], data2[i]);


    delete [] data1;
    delete [] data2;
}

TEST(H5GridTest, Append)
{
    H5Grid h5;
    int ndims = 2;
    int dims[2] = {10,20};
    std::vector<int> h5dims;
    h5dims.assign(dims, dims+2);
    int stat;

    double * data = new double [dims[0]*dims[1]];

    stat = h5.open("test5.h5", "w");
    ASSERT_EQ(stat, 0);
    stat = h5.write_dataset("phi/frame_0", data, dims, ndims);
    ASSERT_EQ(stat, 0);
    h5.close();

    stat = h5.open("test5.h5", "a");
    ASSERT_EQ(stat, 0);
    stat = h5.write_dataset("phi/frame_1", data, dims, ndims);
    ASSERT_EQ(stat, 0);
    h5.close();

}

TEST(H5GridTest, OverwriteFile)
{
    H5Grid h5;
    int ndims = 2;
    int dims[2] = {10,20};
    std::vector<int> h5dims;
    h5dims.assign(dims, dims+2);
    int stat;
    double * data = new double [dims[0]*dims[1]];

    stat = h5.open("test6.h5", "w");
    ASSERT_EQ(stat, 0);
    stat = h5.write_dataset("phi", data, dims, ndims);
    ASSERT_EQ(stat, 0);
    h5.close();

    stat = h5.open("test6.h5", "w");
    ASSERT_EQ(stat, 0);
    stat = h5.write_dataset("phi", data, dims, ndims);
    ASSERT_EQ(stat, 0);
    h5.close();
}

TEST(H5GridTest, list)
{
    int ndims = 2;
    int dims[2] = {10, 20};
    int stat;
    std::vector<std::string> list;

    double * data = (double *) malloc(sizeof(double)*dims[0]*dims[1]);

    H5Grid h5;
    stat = h5.open("test7.h5", "w");
    ASSERT_EQ(stat, 0);
    h5.write_dataset("/data1", data, dims, ndims);
    h5.write_dataset("/data2", data, dims, ndims);
    h5.write_dataset("/data3", data, dims, ndims);
    h5.write_dataset("/data4", data, dims, ndims);
    stat = h5.list("/", list);
    ASSERT_EQ(stat, 0);

    EXPECT_EQ(list.size(), 4);
    EXPECT_EQ(list[0], "data1");
    EXPECT_EQ(list[1], "data2");
    EXPECT_EQ(list[2], "data3");
    EXPECT_EQ(list[3], "data4");

    h5.write_dataset("/path/to/my/data1", data, dims, ndims);
    h5.write_dataset("/path/to/my/data2", data, dims, ndims);
    h5.write_dataset("/path/to/my/data3", data, dims, ndims);

    h5.list("/path/to/my/", list);
    EXPECT_EQ(list.size(), 3);

    h5.close();
}

TEST(H5GridTest, SetGetAttr)
{
    H5Grid h5;

    h5.open("test2.h5", "w");

    int temp = 0;
    int stat;
    stat = h5.set_attribute("Temp", 50);
    EXPECT_NE(stat, 0);

    stat = h5.set_attribute("/Temp", 50);
    ASSERT_EQ(stat, 0);

    stat = h5.get_attribute("/Temp", temp);
    ASSERT_EQ(stat, 0);
    EXPECT_EQ(temp, 50);

    stat = h5.set_attribute("/path/to/my/crazy/attr", 100);
    ASSERT_EQ(stat, 0);
    int attr;
    stat = h5.get_attribute("/path/to/my/crazy/attr", attr);
    EXPECT_EQ(attr, 100);
    
    h5.close();
}

TEST(H5GridTest, GetDims)
{
    int ndims = 2;
    int dims[2] = {10,20};
    int stat;

    double * data = new double [dims[0]*dims[1]];

    H5Grid h5;
    h5.open("test3.h5", "w");
    h5.write_dataset("phi", data, dims, ndims);
    h5.close();

    int test_ndims;
    int test_dims[10];

    h5.open("test3.h5", "r");
    stat = h5.get_ndims("phi", test_ndims);
    ASSERT_EQ(stat, 0);
    EXPECT_EQ(ndims, test_ndims);

    stat = h5.get_dims("phi", test_dims);
    ASSERT_EQ(stat, 0);
    EXPECT_EQ(test_dims[0], 10);
    EXPECT_EQ(test_dims[1], 20);
    h5.close();

}

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
