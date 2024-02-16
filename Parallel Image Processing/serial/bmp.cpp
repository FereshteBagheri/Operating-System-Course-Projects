#include "bmp.hpp"

#include <string.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

BMP::BMP(const BMP& other)
    : pixels_(other.pixels_),
      bitmap_file_header_(other.bitmap_file_header_),
      bitmap_info_header_(other.bitmap_info_header_) {}


int BMP::get_height() const {
    return bitmap_info_header_.biHeight;
}

int BMP::get_width() const {
    return bitmap_info_header_.biWidth;
}

Pixel& BMP::operator()(int row, int column) {
    return pixels_[row][column];
}

const Pixel& BMP::operator()(int row, int column) const {
    return pixels_[row][column];
}

std::istream& operator>>(std::istream& stream, BMP& bmp) {
    bmp.read_headers(stream);
    bmp.read_bitmap(stream);
    return stream;
}

std::ostream& operator<<(std::ostream& stream, BMP& bmp) {
    bmp.write_headers(stream);
    bmp.write_bitmap(stream);
    return stream;
}

void BMP::read_headers(std::istream& read_stream) {
    read_stream.read(
        (char*)(&bitmap_file_header_),
        sizeof(BITMAPFILEHEADER));

    if (bitmap_file_header_.bfType != 0x4D42)
        throw std::runtime_error("Not a bmp file.");

    read_stream.read(
        (char*)(&bitmap_info_header_),
        sizeof(BITMAPINFOHEADER));
}

void BMP::read_bitmap(std::istream& read_stream) {
    read_stream.seekg(bitmap_file_header_.bfOffBits, std::ios::beg);
    pixels_.assign(bitmap_info_header_.biHeight, std::vector<Pixel>(bitmap_info_header_.biWidth));
    unsigned char* img_buffer = new unsigned char[bitmap_info_header_.biSizeImage];
    read_stream.read(
        (char*)img_buffer,
        bitmap_info_header_.biSizeImage);
    for (int i = 0; i < bitmap_info_header_.biHeight; ++i) {
        for (int j = 0; j < bitmap_info_header_.biWidth; ++j) {
            unsigned char* temp_pixels = &img_buffer[get_data_pos_from_pixel_loc(i, j)];
            pixels_[i][j] = {temp_pixels[2], temp_pixels[1], temp_pixels[0]};
        }
    }
    delete[] img_buffer;
}

void BMP::write_headers(std::ostream& write_stream) {
    write_stream.write(
        (char*)(&bitmap_file_header_),
        sizeof(BITMAPFILEHEADER));
    write_stream.write(
        (char*)(&bitmap_info_header_),
        sizeof(BITMAPINFOHEADER));

    DWORD cur_stream_point = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    while (cur_stream_point < bitmap_file_header_.bfOffBits) {
        write_stream << '\0';
        ++cur_stream_point;
    }
}

void BMP::write_bitmap(std::ostream& write_stream) {
    char* img_buffer = new char[bitmap_info_header_.biSizeImage];
    for (int i = 0; i < bitmap_info_header_.biHeight; ++i) {
        for (int j = 0; j < bitmap_info_header_.biWidth; ++j) {
            char* temp_pixel = &img_buffer[get_data_pos_from_pixel_loc(i, j)];
            temp_pixel[0] = pixels_[i][j].B_;
            temp_pixel[1] = pixels_[i][j].G_;
            temp_pixel[2] = pixels_[i][j].R_;
        }
    }
    write_stream.write(
        img_buffer,
        bitmap_info_header_.biSizeImage);
    delete[] img_buffer;
}

int BMP::get_data_pos_from_pixel_loc(int row, int col) {
    int row_len = std::ceil((3 * bitmap_info_header_.biWidth) / 4) * 4;
    int height = bitmap_info_header_.biHeight - row - 1;
    int width = col * 3;
    int result = height * row_len + width;
    return result;
}
