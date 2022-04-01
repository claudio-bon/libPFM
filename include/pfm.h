#include <vector>
#include <string>
#include <cstddef>


namespace pfm
{
    namespace io
    {
        /**Handles both color images and grayscale images. If mat is a color image, the parameter color must be set to true*/
        void write_pfm(std::string path, float* mat, int height, int width, bool color, int scale = 1);
        /**Handles grayscale images*/
        void write_pfm(std::string path, float** mat, int height, int width, int scale = 1);
        /**Handles color images*/
        void write_pfm(std::string path, float*** mat, int height, int width, int scale = 1);

        /**Handles both color images and grayscale images. If mat is a color image, the parameter color must be set to true*/
        void write_pfm(std::string path, std::vector<float>& mat, int height, int width, bool color, int scale = 1);
        /**Handles grayscale images*/
        void write_pfm(std::string path, std::vector<std::vector<float>>& mat, int scale = 1);
        /**Handles color images*/
        void write_pfm(std::string path, std::vector<std::vector<std::vector<float>>>& mat, int scale = 1);


        /**Reads a pfm file and store its content in a flat vector*/
        void read_pfm(std::string path, std::vector<float>& mat, int& height, int& width, bool& color, int& scale);
        void read_pfm(std::string path, std::vector<float>& mat, int& height, int& width, bool& color);
        /**Reads a pfm file and store its content in a 2D matrix*/
        void read_pfm(std::string path, std::vector<std::vector<float>>& mat, int& scale);
        void read_pfm(std::string path, std::vector<std::vector<float>>& mat);
        /**Reads a pfm file and store its content in a 3D matrix*/
        void read_pfm(std::string path, std::vector<std::vector<std::vector<float>>>& mat, int& scale);
        void read_pfm(std::string path, std::vector<std::vector<std::vector<float>>>& mat);
    }
}
