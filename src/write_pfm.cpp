#include "../include/pfm.h"
#include "../include/utils.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstring>

namespace pfm
{
    namespace io
    {

        void write_pfm(std::string path, std::vector<float>& mat, int height, int width, bool color, int scale)
        {
            std::vector<float> flipped_mat(height*width*(color ? 3 : 1));
            vertical_flip(flipped_mat, mat, height, width, color);

            std::ofstream file;
            file.open(path, std::ios::binary);
            
            file.exceptions(file.failbit);

            file << (color ? "PF" : "Pf") << "\n";

            file << width << " " << height << "\n";

            if (is_little_endian()) scale = -scale;

            file << scale << "\n";

            for (int i = 0; i < flipped_mat.size(); i++) {
                file.write(reinterpret_cast<char*>(&flipped_mat[i]), sizeof(float));
            }

            file.close();
        }

        void write_pfm(std::string path, std::vector<std::vector<float>>& mat, int scale)
        {
            if (mat.size() < 1) throw std::logic_error("mat should have at least a 1 row.");
            int height = mat.size();
            int width = mat[0].size();

            std::vector<float> flat_mat(height*width);
            for (int i = 0; i < height; i++) {
                if (mat[i].size() != width) throw std::logic_error("All mat rows should have the same length.");

                for (int j = 0; j < width; j++) {
                    flat_mat[i*width + j] = mat[i][j];
                }
            }
            write_pfm(path, flat_mat, height, width, false, scale);
        }
        void write_pfm(std::string path, std::vector<std::vector<std::vector<float>>>& mat, int scale)
        {
            if (mat.size() < 1) throw std::logic_error("mat should have at least a 1 row.");
            int height = mat.size();
            int width = mat[0].size();

            std::vector<float> flat_mat(height*width*channels);
            for (int i = 0; i < height; i++) {
                if (mat[i].size() != width) throw std::logic_error("All mat rows should have the same length.");

                for (int j = 0; j < width; j++) {
                    if (mat[i][j].size() != channels) throw std::logic_error("All mat elements should have 3 channels.");

                    for (int k = 0; k < channels; k++) {
                        flat_mat[i*width*channels + j*channels + k] = mat[i][j][k];
                    }
                }
            }
            write_pfm(path, flat_mat, height, width, true, scale);
        }

        void write_pfm(std::string path, float* mat, int height, int width, bool color, int scale)
        {
            std::vector<float> flat_mat(height*width*(color ? 3 : 1));
            for (int i = 0; i < flat_mat.size(); i++) {
                 flat_mat[i] = mat[i];
            }
            write_pfm(path, flat_mat, height, width, color, scale);
        }
        void write_pfm(std::string path, float** mat, int height, int width, int scale)
        {
            std::vector<float> flat_mat(height*width);
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    flat_mat[i*width + j] = mat[i][j];
                }
            }
            write_pfm(path, flat_mat, height, width, false, scale);
        }
        void write_pfm(std::string path, float*** mat, int height, int width, int scale)
        {
            std::vector<float> flat_mat(height*width*channels);
            
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    for (int k = 0; k < channels; k++)
                    flat_mat[i*width*channels + j*channels + k] = mat[i][j][k];
                }
            }
            write_pfm(path, flat_mat, height, width, true, scale);
        }

    }
}