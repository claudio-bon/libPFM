#include "../include/pfm.h"
#include "utils.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <regex>

namespace pfm
{
    namespace io
    {
        /**Trim string from both ends(in place)
        Inspired by: https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring*/
        static inline void trim(std::string &s) {
            //Trim from start
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
            //Trim from end
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), s.end());
        }

        static inline std::string rdln(std::ifstream& file) {
            std::string s;
            std::getline(file, s);
            trim(s);
            return s;
        }

        /**Taken from: https://stackoverflow.com/questions/2782725/converting-float-values-from-big-endian-to-little-endian*/
        float reverse_float(float in_float)
        {
            float ret_val;
            char* float_to_convert = (char*)&in_float;
            char* return_float = (char*)&ret_val;

            //Swap the bytes into a temporary buffer
            return_float[0] = float_to_convert[3];
            return_float[1] = float_to_convert[2];
            return_float[2] = float_to_convert[1];
            return_float[3] = float_to_convert[0];

            return ret_val;
        }

        void read_pfm(std::string path, std::vector<float>& mat, int& height, int& width, bool& color, int& scale)
        {
            std::ifstream file;
            file.open(path, std::ios::binary);
            
            file.exceptions(file.failbit); //Throws an exception if the file has failed to be opened

            //Read the header
            std::string header = rdln(file);
            if (header == "PF") color = true;
            else if (header == "Pf") color = false;
            else throw std::logic_error("Not a PFM file.");

            //Read the width and height
            std::string dim = rdln(file);
            std::regex dim_regex("([0-9]+) ([0-9]+)");
            if(std::regex_match(dim, dim_regex)) {
                /**Inspired by : https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c*/
                std::istringstream dim_ss(dim);
                if (!(dim_ss >> width >> height)) { throw std::logic_error("Malformed PDF header."); }
            }
            else {
                throw std::logic_error("Malformed PDF header.");
            }
            
            //Check if the endianness of the numbers in the file is compatible with the one currently in use
            std::string scale_str = rdln(file);
            std::istringstream scale_ss(scale_str);
            scale_ss >> scale;
            bool invert = ((scale < 0) != is_little_endian());

            //Read the rest of the content
            std::vector<float> flipped_mat(height*width*(color ? 3 : 1));
            float val;
            for (int i = 0; i < flipped_mat.size(); i++) {
                file.read(reinterpret_cast<char*>(&val), sizeof(float));
                if (invert) val = reverse_float(val);
                flipped_mat[i] = val;
            }

            file.close();

            //Flip back
            mat = std::vector<float>(height*width*(color ? 3 : 1));
            vertical_flip(mat, flipped_mat, height, width, color);
        }
        void read_pfm(std::string path, std::vector<float>& mat, int& height, int& width, bool& color)
        {
            int scale;
            read_pfm(path, mat, height, width, color, scale);
        }

        void read_pfm(std::string path, std::vector<std::vector<float>>& mat, int& scale)
        {
            int height, width;
            bool color;
            std::vector<float> flat_mat;
            read_pfm(path, flat_mat, height, width, color, scale);
            
            if (color) throw std::logic_error("The file " + path + "contains a color image.");
            else {
                mat = std::vector<std::vector<float>>(height);
                for (int i = 0; i < height; i++) {
                    mat[i] = std::vector<float>(width);
                    for (int j = 0; j < width; j++) {
                        mat[i][j] = flat_mat[i*width + j];
                    }
                }
            }
        }
        void read_pfm(std::string path, std::vector<std::vector<float>>& mat)
        {
            int scale;
            read_pfm(path, mat, scale);
        }

        void read_pfm(std::string path, std::vector<std::vector<std::vector<float>>>& mat, int& scale)
        {
            int height, width;
            bool color;
            std::vector<float> flat_mat;
            read_pfm(path, flat_mat, height, width, color, scale);

            if (!color) throw std::logic_error("The file " + path + "contains a grayscale image.");
            else {
                mat = std::vector<std::vector<std::vector<float>>>(height);
                for (int i = 0; i < height; i++) {
                    mat[i] = std::vector<std::vector<float>>(width);
                    for (int j = 0; j < width; j++) {
                        mat[i][j] = std::vector<float>(channels);
                        for (int k = 0; k < channels; k++) {
                            mat[i][j][k] = flat_mat[i*width*channels + j*channels + k];
                        }
                    }
                }
            }
        }
        void read_pfm(std::string path, std::vector<std::vector<std::vector<float>>>& mat)
        {
            int scale;
            read_pfm(path, mat, scale);
        }

        void read_pfm(std::string path, float* mat, int& height, int& width, bool& color, int& scale) {
            std::vector<float> mat_tmp;
            read_pfm(path, mat_tmp, height, width, color, scale);
            if (mat == nullptr) mat = new float[mat_tmp.size()];
            for (int i = 0; i < mat_tmp.size(); i++)
                mat[i] = mat_tmp[i];
        }
        void read_pfm(std::string path, float* mat, int& height, int& width, bool& color) {
            int scale;
            read_pfm(path, mat, height, width, color, scale);
        }

        void read_pfm(std::string path, float** mat, int& height, int& width, int& scale) {
            std::vector < std::vector<float>> mat_tmp;
            read_pfm(path, mat_tmp, scale);
            height = mat_tmp.size();
            if (height == 0) throw std::logic_error("Loaded file is empty.");
            width = mat_tmp[0].size();

            if (mat == nullptr) mat = new float* [height];
            for (int i = 0; i < height; i++) {
                if (mat_tmp[i].size() != width) throw std::logic_error("The number of columns is inconsistent among different rows.");
                mat[i] = new float[width];
                for (int j = 0; j < width; j++)
                    mat[i][j] = mat_tmp[i][j];
            }
        }
        void read_pfm(std::string path, float** mat, int& height, int& width) {
            int scale;
            read_pfm(path, mat, height, width, scale);
        }

        void read_pfm(std::string path, float*** mat, int& height, int& width, int& scale) {
            std::vector < std::vector < std::vector<float>>> mat_tmp;
            read_pfm(path, mat_tmp, scale);
            height = mat_tmp.size();
            if (height == 0) throw std::logic_error("Loaded file is empty.");
            width = mat_tmp[0].size();
            if (width == 0) throw std::logic_error("Loaded file has some issues.");
            int channels = mat_tmp[0][0].size();

            if (mat == nullptr) mat = new float** [height];
            for (int i = 0; i < height; i++) {
                if (mat_tmp[i].size() != width) throw std::logic_error("The number of columns is inconsistent among different rows.");
                mat[i] = new float* [width];
                for (int j = 0; j < width; j++) {
                    if (mat_tmp[i][j].size() != channels) throw std::logic_error("The number of channels is inconsistent among different columns.");
                    mat[i][j] = new float [channels];
                    for (int k = 0; k < channels; k++)
                        mat[i][j][k] = mat_tmp[i][j][k];
                }
            }
        }
        void read_pfm(std::string path, float*** mat, int& height, int& width) {
            int scale;
            read_pfm(path, mat, height, width, scale);
        }
    }
}