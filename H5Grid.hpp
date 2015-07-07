
#include <iostream>
#include <string>
#include "hdf5.h"

//! A simple interface for storing/accessing grid data in hdf5 files

class H5Grid {
    private:
        hid_t file_id;
        void create_group(const char * dataset_name);
    public:
        int open(const char * filename, const char * mode, int * dims, int &ndims);
        int read_dataset(const char * dataset_name, double * dataset);
        int write_dataset(const char * dataset_name, double * dataset);
        int set_attribute(std::string attr_name, int attr_value);
        int get_attribute(const char * attr_name, int * attr_value);
        void close();
};

void H5Grid :: create_group(const char * dataset_name)
{
    char str[256];
    char * pch;
    char path[256] = "";

    strncpy(str, dataset_name, 256);
    pch = strtok(str, "/");
    
    while (pch != NULL)
    {
        sprintf(path, "%s/%s", path, pch);
        pch = strtok(NULL, "/");

        htri_t group_exists = H5Lexists(file_id, path, H5P_DEFAULT);

        if (pch!=NULL && group_exists==0) {
            hid_t grp_id = H5Gcreate(file_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
            H5Gclose(grp_id);
        }
    }
}


int H5Grid :: open(const char * filename, const char * mode, int * dims, int &ndims)
{
    /**
    * @param filename Name of the hdf5 file
    * @param mode "r", "w", or "a"
    * @param dims an array of extents of the grid to be stored
    * @param ndims the number of dimensions of the grid
    **/


    unsigned read, write, append;

    read = (mode[0] == 'r');
    write = (mode[0] == 'w');
    append = (mode[0] == 'a');

    // check that one is true
    if ( (read || write || append) != 1) return 1;


    if ( write ) {
        file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        
        // write the dims to an file attribute
        hsize_t attr_dim = ndims;
        hid_t space_id = H5Screate_simple(1, &attr_dim, NULL);
        hid_t attr_id = H5Acreate(file_id, "DIMS", H5T_NATIVE_INT, space_id, H5P_DEFAULT, H5P_DEFAULT);
        H5Awrite(attr_id, H5T_NATIVE_INT, dims);
        H5Sclose(space_id);
        H5Aclose(attr_id);

    } else if (read || append) {
        FILE * file_exists = fopen(filename, "r");
        if (file_exists == NULL) return 2;
        else fclose(file_exists);

        if (H5Fis_hdf5(filename) < 1) return 3;

        if (read)
            file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
        if (append)
            file_id = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);

        hsize_t attr_dim[1];
        hid_t attr_id = H5Aopen(file_id, "DIMS", H5P_DEFAULT);
        H5Aread(attr_id, H5T_NATIVE_INT, dims);
        hid_t space_id = H5Aget_space(attr_id);
        int stat = H5Sget_simple_extent_dims(space_id, attr_dim, NULL);
        ndims = (int) (attr_dim[0]);
        H5Sclose(space_id);
        H5Aclose(attr_id);
    }

    return 0;
}

int H5Grid :: read_dataset(const char * dataset_name, double * dataset)
{
    /**
    * @param dataset_name name of the dataset
    * @param dataset pointer to the first element of data
    **/
    return 0;
}

int H5Grid :: write_dataset(const char * dataset_name, double * dataset)
{
    /**
    * @param dataset_name name of the dataset
    * @param dataset pointer to the first element of data
    **/
    return 0;
}

void parse (std::string attr_name, std::string &path, std::string &name)
{
    size_t start = 0;
    size_t end = 0;

    while (end != std::string::npos)
    {
        end = attr_name.find("/", start);
        path = attr_name.substr(0,start);
        name = attr_name.substr(start,end-start);
        start = end + 1;
    }
}

int H5Grid :: set_attribute(std::string attr_name, int  attr_value)
{
    /**
    * @param attr_name Name/address of the attribute
    * @param attr_value Value of the attribute
    **/

    std::string path;
    std::string name;
    parse(attr_name, path, name);

    create_group(attr_name.c_str());
    hid_t space_id = H5Screate(H5S_SCALAR);
    hid_t attr_id = H5Acreate_by_name(file_id, 
                              path.c_str(),
                              name.c_str(), 
                              H5T_NATIVE_INT, 
                              space_id, 
                              H5P_DEFAULT, 
                              H5P_DEFAULT,
                              H5P_DEFAULT);

    H5Awrite(attr_id, H5T_NATIVE_INT, &attr_value);
    H5Aclose(attr_id);
    H5Sclose(space_id);

    return 0;
}

int H5Grid :: get_attribute(const char * attr_name, int * attr_value)
{
    /**
    * @param attr_name Name/address of the attribute
    * @param attr_value Value of the attribute
    **/

    return 0;
}

void H5Grid :: close()
{
}

/*
int main(int argc, char ** argv)
{

    int dims[3];
    dims[0] = 100;
    dims[1] = 100;
    dims[2] = 100;


    H5Grid out;
    out.open("out.h5", "w", dims, 3);
    return 0;
}
*/
