#ifndef FILTERS_HPP
#define FILTERS_HPP

#include "bmp.hpp"


const float purple_haze_coefficients[3][3] = {
    {0.5, 0.3, 0.5},
    {0.16, 0.5, 0.16},
    {0.6, 0.2, 0.8}
};

const float gaussian_blur_kernel[3][3] = {
    {1/16.0f, 2/16.0f, 1/16.0f},
    {2/16.0f, 4/16.0f, 2/16.0f},
    {1/16.0f, 2/16.0f, 1/16.0f}
};

BMP purple_haze_filter(const BMP& input, const float (&coefficients)[3][3]);
BMP kernel_filter(const BMP& input, const float kernel[3][3]);

BMP emboss_filter(const BMP& input_pic);

BMP vertical_mirror_filter(const BMP& input);

BMP draw_rhombus(BMP input_pic);
BMP add_diagonal_lines(const BMP& input);

#endif
