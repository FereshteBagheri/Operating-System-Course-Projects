#include <algorithm>
#include <cmath>
#include <pthread.h>
#include <vector>

#include "bmp.hpp"
#include "filters.hpp"


void* apply_kernel_thread(void* args) {
    thread_args& threadArgs = *static_cast<thread_args*>(args);
    BMP& output = threadArgs.output;
    const BMP& input = threadArgs.input;
    int start_row = threadArgs.start_row;
    int end_row = threadArgs.end_row;

    int height = input.get_height();
    int width = input.get_width();

    for (int i = start_row; i < end_row; i++) {
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

                    newR += gaussian_blur_kernel[ki + 1][kj + 1] * neighbor.R_;
                    newG += gaussian_blur_kernel[ki + 1][kj + 1] * neighbor.G_;
                    newB += gaussian_blur_kernel[ki + 1][kj + 1] * neighbor.B_;
                }
            }

            Pixel& pixel = output(i, j);
            pixel.R_ = std::min(std::max<int>(newR, 0), 255);
            pixel.G_ = std::min(std::max<int>(newG, 0), 255);
            pixel.B_ = std::min(std::max<int>(newB, 0), 255);
        }
    }

    return NULL;
}


BMP kernel_filter(BMP& input) {
    BMP output = input; 
    int height = output.get_height();

    int num_threads = THREAD_NUM;
    std::vector<pthread_t> threads(num_threads);
    std::vector<thread_args*> threadArgs(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        int start_row = i * height / num_threads;
        int end_row = (i == num_threads - 1) ? height : start_row + height / num_threads;

        threadArgs[i] = new thread_args{output, input, start_row, end_row};

        pthread_create(&threads[i], NULL, apply_kernel_thread, threadArgs[i]);
    }

    for (auto& thread : threads) {
        pthread_join(thread, NULL);
    }

    for(auto& args : threadArgs) {
        delete args;
    }

    return output;
}


void* apply_purple_haze_thread(void* args) {
    thread_args& threadArgs = *static_cast<thread_args*>(args);
    BMP& output = threadArgs.output;
    const BMP& input = threadArgs.input;
    int start_row = threadArgs.start_row;
    int end_row = threadArgs.end_row;

    int width = input.get_width();

    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < width; ++j) {
            Pixel& pixel = output(i, j);

            float newR = purple_haze_coefficients[0][0] * pixel.R_ + purple_haze_coefficients[0][1] * pixel.G_ + purple_haze_coefficients[0][2] * pixel.B_;
            float newG = purple_haze_coefficients[1][0] * pixel.R_ + purple_haze_coefficients[1][1] * pixel.G_ + purple_haze_coefficients[1][2] * pixel.B_;
            float newB = purple_haze_coefficients[2][0] * pixel.R_ + purple_haze_coefficients[2][1] * pixel.G_ + purple_haze_coefficients[2][2] * pixel.B_;

            pixel.R_ = std::min(std::max<int>(newR, 0), 255);
            pixel.G_ = std::min(std::max<int>(newG, 0), 255);
            pixel.B_ = std::min(std::max<int>(newB, 0), 255);
        }
    }

    return NULL;
}

BMP purple_haze_filter(const BMP& input) {
    BMP output = input; 
    int height = output.get_height();

    int num_threads = THREAD_NUM;
    std::vector<pthread_t> threads(num_threads);
    std::vector<thread_args*> threadArgs(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        int start_row = i * height / num_threads;
        int end_row = (i == num_threads - 1) ? height : start_row + height / num_threads;

        threadArgs[i] = new thread_args{output, input, start_row, end_row};

        pthread_create(&threads[i], NULL, apply_purple_haze_thread, threadArgs[i]);
    }

    for (auto& thread : threads) {
        pthread_join(thread, NULL);
    }

    for(auto& args : threadArgs) {
        delete args;
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


