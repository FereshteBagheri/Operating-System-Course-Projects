#include "filters.hpp"

#include <algorithm>
#include <cmath>

#include "bmp.hpp"


BMP purple_haze_filter(const BMP& input, const float (&coefficients)[3][3]) {
    BMP output = input;
    int height = output.get_height();
    int width = output.get_width();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Pixel& pixel = output(i, j);

            float newR = coefficients[0][0] * pixel.R_ + coefficients[0][1] * pixel.G_ + coefficients[0][2] * pixel.B_;
            float newG = coefficients[1][0] * pixel.R_ + coefficients[1][1] * pixel.G_ + coefficients[1][2] * pixel.B_;
            float newB = coefficients[2][0] * pixel.R_ + coefficients[2][1] * pixel.G_ + coefficients[2][2] * pixel.B_;

            pixel.R_ = std::min(std::max<int>(newR, 0), 255);
            pixel.G_ = std::min(std::max<int>(newG, 0), 255);
            pixel.B_ = std::min(std::max<int>(newB, 0), 255);
        }
    }

    return output;
}

BMP vertical_mirror_filter(const BMP& input) {
    BMP output = input;
    int height = output.get_height();
    int width = output.get_width();

    for (int i = 0; i < height / 2; ++i) {
        for (int j = 0; j < width; ++j) {
            std::swap(output(i, j), output(height - i - 1, j));
        }
    }

    return output;  
}




BMP kernel_filter(const BMP& input, const float kernel[3][3]) {
    BMP output = input; 
    int height = output.get_height();
    int width = output.get_width();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float newR = 0, newG = 0, newB = 0;

            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {
                    int ni = i + ki;
                    int nj = j + kj;

                    if (ni < 0 || ni >= height) {
                        ni = i;
                    }
                    if (nj < 0 || nj >= width) {
                        nj = j;
                    }

                    const Pixel& neighbor = input(ni, nj);

                    newR += kernel[ki + 1][kj + 1] * neighbor.R_;
                    newG += kernel[ki + 1][kj + 1] * neighbor.G_;
                    newB += kernel[ki + 1][kj + 1] * neighbor.B_;
                }
            }

            Pixel& pixel = output(i, j);
            pixel.R_ = std::min(std::max<int>(newR, 0), 255);
            pixel.G_ = std::min(std::max<int>(newG, 0), 255);
            pixel.B_ = std::min(std::max<int>(newB, 0), 255);
        }
    }

    return output;
}



BMP add_diagonal_lines(const BMP& input) {
    BMP output = input;
    int height = output.get_height();
    int width = output.get_width();


    Pixel white;
    white.R_ = 255;
    white.G_ = 255;
    white.B_ = 255;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {

            if (i + j == width || i + j == width - height / 2 || i + j == width + height / 2) {
                output(i, j) = white;
            }
        }
    }

    return output;
}


