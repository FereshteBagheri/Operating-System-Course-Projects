#ifndef FILTERS_HPP
#define FILTERS_HPP

#include "bmp.hpp"

const int THREAD_NUM = 4;

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


struct thread_args {
    BMP& output;
    const BMP& input;
    int start_row;
    int end_row;
};

BMP purple_haze_filter(const BMP& input);
BMP kernel_filter(BMP& input);
BMP vertical_mirror_filter(const BMP& input);
BMP add_diagonal_lines(const BMP& input);

#endif
