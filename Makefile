
gtest = /Volumes/DATA/Software/gtest-1.7.0
hdf5 = /usr/local/Cellar/hdf5/1.8.14

default:
	g++ -isystem $(gtest)/include -I$(gtest) -pthread -c $(gtest)/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	g++ -Wall -isystem $(gtest)/include -pthread H5Grid_test.cpp libgtest.a -o a.out -I$(hdf5)/include -L$(hdf5)/lib -lhdf5
