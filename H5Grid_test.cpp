
#include "H5Grid.hpp"
#include "gtest/gtest.h"

TEST(H5GridTest, OpenClose)
{
    H5Grid h0, h1;
    int nx = 10;
    int ny = 20;
    int nz = 0;
    int stat;

    // check that it opens
    stat = h0.open("test2.h5", "w", nx, ny, nz);
    ASSERT_EQ(stat, 0);

    // check that it cant open anything else
    stat = h0.open("test3.h5", "w", nx, ny, nz);
    EXPECT_NE(stat, 0);

    stat = h0.close();
    EXPECT_EQ(stat, 0);

    // check for already closed error
    stat = h0.close();
    EXPECT_NE(stat, 0);

    nx = 1;
    ny = 1;
    nz = 1;

    // check open in read mode
    stat = h1.open("test2.h5", "r", nx, ny, nz);
    ASSERT_EQ(stat, 0);
    EXPECT_EQ(nx, 10);
    EXPECT_EQ(ny, 20);
    EXPECT_EQ(nz, 0);
    h1.close();
}


TEST(H5GridTest, SetGetAttr)
{
    H5Grid h5;
    int nx = 10;
    int ny = 20;
    int nz = 0;
    h5.open("test.h5", "w", nx, ny, nz);

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

TEST(H5GridTest, WriteReadData)
{
    H5Grid h0, h1;
    int i;
    int nx = 10;
    int ny = 20;
    int nz = 0;
    double * data = (double *) malloc(sizeof(double)*nx*ny);
    int stat;

    stat = h0.open("test4.h5", "w", nx, ny, nz);
    ASSERT_EQ(stat, 0);

    for (i=0; i<nx*ny; i++) data[i] = i;

    // check error for nonexistent dataset
    /* not yet implemented
    stat = h0.read_dataset("/c/frame_000", data);
    EXPECT_NE(stat, 0);
    */

    stat = h0.write_dataset("/c/frame_000", data);
    ASSERT_EQ(stat, 0);

    stat = h0.read_dataset("/c/frame_000", data);
    ASSERT_EQ(stat, 0);
    EXPECT_EQ(data[10], 10);


    for (i=0; i<nx*ny; i++) data[i]++;

    // check for overwriting 
    stat = h0.write_dataset("/c/frame_000", data);
    EXPECT_EQ(stat, 1);

    stat = h0.read_dataset("/c/frame_000", data);
    ASSERT_EQ(stat, 0);
    EXPECT_EQ(data[10], 11);

    stat = h0.close();
    ASSERT_EQ(stat, 0);

    free(data);
}

TEST(H5GridTest, list)
{
    int nx = 10;
    int ny = 20;
    int nz = 0;
    int stat;
    std::vector<std::string> list;

    double * data = (double *) malloc(sizeof(double)*nx*ny);

    H5Grid h5;
    stat = h5.open("test5.h5", "w", nx, ny, nz);
    ASSERT_EQ(stat, 0);
    h5.write_dataset("/data1", data);
    h5.write_dataset("/data2", data);
    h5.write_dataset("/data3", data);
    h5.write_dataset("/data4", data);
    stat = h5.list("/", list);
    ASSERT_EQ(stat, 0);

    EXPECT_EQ(list.size(), 4);

    h5.write_dataset("/path/to/my/data1", data);
    h5.write_dataset("/path/to/my/data2", data);
    h5.write_dataset("/path/to/my/data3", data);

    h5.list("/path/to/my/", list);
    EXPECT_EQ(list.size(), 3);

    h5.close();
}

TEST(H5GridTest, ListRead)
{
    int nx = 10;
    int ny = 20;
    int nz = 0;
    int stat;
    std::vector<std::string> list;

    double * data = new double [nx*ny];
    H5Grid h5;
    stat = h5.open("test6.h5", "w", nx, ny, nz);
    ASSERT_EQ(stat, 0);

    h5.write_dataset("/c", data);
    h5.write_dataset("/phi", data);
    h5.write_dataset("/eta_00", data);
    h5.write_dataset("/eta_01", data);

    stat = h5.list("/", list);
    ASSERT_EQ(stat, 0);

    h5.close();
    delete [] data;
}

TEST(H5GridTest, Append)
{
    int nx = 5;
    int ny = 10;
    int nz = 15;
    int stat;

    double * data = new double [nx*ny*nz];
    H5Grid h5;
    stat = h5.open("test7.h5", "w", nx, ny, nz);
    ASSERT_EQ(stat, 0);
    stat = h5.write_dataset("/phi/000000", data);
    ASSERT_EQ(stat, 0);
    h5.close();

    h5.open("test7.h5", "a", nx, ny, nz);
    h5.write_dataset("/phi/000001", data);
    h5.close();
}

int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "H5GridTest.Append";
    return RUN_ALL_TESTS();
}
