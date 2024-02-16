#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>

#include "bmp.hpp"
#include "filters.hpp"

int main(int argc, char const* argv[]) {
    std::string file_name(argv[1]);
    BMP bmp;

    auto program_start_clk = std::chrono::high_resolution_clock::now();

    auto start_clk = std::chrono::high_resolution_clock::now();
    std::ifstream input_file(file_name);
    input_file >> bmp;
    input_file.close();
    auto end_clk = std::chrono::high_resolution_clock::now();
    auto time_taken = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
        end_clk - start_clk);
    std::cout << "Read: " << time_taken.count() << '\n';

    start_clk = std::chrono::high_resolution_clock::now();
    bmp = vertical_mirror_filter(bmp);
    end_clk = std::chrono::high_resolution_clock::now();
    time_taken = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
        end_clk - start_clk);
    std::cout << "Flip: " << time_taken.count() << '\n';

    start_clk = std::chrono::high_resolution_clock::now();
    bmp = kernel_filter(bmp, gaussian_blur_kernel);
    end_clk = std::chrono::high_resolution_clock::now();
    time_taken = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
        end_clk - start_clk);
    std::cout << "Blur: " << time_taken.count() << '\n';

    start_clk = std::chrono::high_resolution_clock::now();
    bmp = purple_haze_filter(bmp, purple_haze_coefficients);
    end_clk = std::chrono::high_resolution_clock::now();
    time_taken = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
        end_clk - start_clk);
    std::cout << "Purple: " << time_taken.count() << '\n';

    start_clk = std::chrono::high_resolution_clock::now();
    bmp = add_diagonal_lines(bmp);
    end_clk = std::chrono::high_resolution_clock::now();
    time_taken = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
        end_clk - start_clk);
    std::cout << "Lines: " << time_taken.count() << '\n';

    std::ofstream output_file("output.bmp");
    output_file << bmp;
    output_file.close();

    auto program_end_clk = std::chrono::high_resolution_clock::now();
    auto total_time_taken = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(
        program_end_clk - program_start_clk);
    std::cout << "Execution: " << total_time_taken.count() << '\n';

    return 0;
}
