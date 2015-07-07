
gtest = /Volumes/DATA/Software/gtest-1.7.0
hdf5 = /Volumes/DATA/Software/hdf5/1.8.14

default: libgtest.a a.out

libgtest.a: gtest-all.o
	g++ -isystem $(gtest)/include -I$(gtest) -pthread -c $(gtest)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o

a.out: H5Grid_test.cpp H5Grid.hpp
	g++ -Wall -isystem $(gtest)/include -pthread H5Grid_test.cpp libgtest.a -o a.out -I$(hdf5)/include -L$(hdf5)/lib -lhdf5 -lz
