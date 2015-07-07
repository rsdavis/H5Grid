
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


int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
