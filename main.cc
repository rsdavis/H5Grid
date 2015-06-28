

class H5Grid {
    private:
    public:
        int open(const char * filename, const char * mode, int * dims, int ndims);
        int read_dataset(const char * dataset_name, double * dataset);
        int write_dataset(const char * dataset_name, double * dataset);
        int set_attribute(const char * attr_name, int * attr_value);
        int get_attribute(const char * attr_name, int * attr_value);
        void close();
};

int H5Grid :: open(const char * filename, const char * mode, int * dims, int ndims)
{
    return 0;
}

int H5Grid :: read_dataset(const char * dataset_name, double * dataset)
{
    return 0;
}

int H5Grid :: write_dataset(const char * dataset_name, double * dataset)
{
    return 0;
}

int H5Grid :: set_attribute(const char * attr_name, int * attr_value)
{
    return 0;
}

int H5Grid :: get_attribute(const char * attr_name, int * attr_value)
{
    return 0;
}

void H5Grid :: close()
{
}

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
