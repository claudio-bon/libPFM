#include "../include/utils.h"

namespace pfm
{
    namespace io
    {
        bool is_little_endian()
        {
            int intval = 1;
            unsigned char* uval = reinterpret_cast<unsigned char*>(&intval);
            return uval[0] == 1;
        }


        void vertical_flip(std::vector<float>& flipped_mat, std::vector<float>& mat, int height, int width, bool color)
        {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (!color) flipped_mat[i*width + j] = mat[i*width + j];
                    else {
                        for (int k = 0; k < channels; k++)
                            flipped_mat[i*width*channels + j*channels + k] = mat[i*width*channels + j*channels + k];
                    }
                }
            }
        }
    }
}