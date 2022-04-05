#include <vector>

namespace pfm
{
    namespace io
    {
        const int channels = 3;

        /**Checks what kind of endienness the system is currently using.
         * Taken from: https://github.com/microsoft/AirSim/blob/77a879e8b6459543caa1ab867e277f4958becb91/AirLib/include/common/common_utils/Utils.hpp#L681*/
        bool is_little_endian();

        void vertical_flip(std::vector<float>& flipped_mat, std::vector<float>& mat, int height, int width, bool color);
    }
}