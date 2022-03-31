#include "../include/pfm.h"
#include <vector>
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;


int main()
{
    //std::string file_path = (fs::path(".") / "test_files" / "test.pfm").string();
    std::string file_path = (fs::path("test") / "test_files" / "test.pfm").string();
    std::cout << file_path << std::endl;
    std::vector<std::vector<float>> mat_write = {{3.54, 12.41, 1.15}, {1.59, 9.11, 10.43}, {12.13, 4.35, 7.29}};
    pfm::io::write_pfm(file_path, mat_write, 1);

    //std::vector<float> mat_read;
    //int height, width, scale;
    //bool color;
    int scale;
    std::vector<std::vector<float>> mat_read;

    pfm::io::read_pfm(file_path, mat_read, scale);

    for (int i = 0; i < mat_read.size(); i++) {
        for (int j = 0; j < mat_read[i].size(); j++)
            std::cout << mat_read[i][j] << " ";

        std::cout << std::endl;
    }

    return 0;
}