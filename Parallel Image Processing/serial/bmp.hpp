#ifndef BMP_HPP
#define BMP_HPP



#include <iostream>
#include <vector>

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;


struct Pixel {
    unsigned char R_;
    unsigned char G_;
    unsigned char B_;
};


#pragma pack(push, 1)
    typedef struct tagBITMAPFILEHEADER {
        WORD bfType;
        DWORD bfSize;
        WORD bfReserved1;
        WORD bfReserved2;
        DWORD bfOffBits;
    } BITMAPFILEHEADER;

#pragma pack(pop)
#pragma pack(push, 1)
    typedef struct tagBITMAPINFOHEADER {
        DWORD biSize;
        LONG biWidth;
        LONG biHeight;
        WORD biPlanes;
        WORD biBitCount;
        DWORD biCompression;
        DWORD biSizeImage;
        LONG biXPelsPerMeter;
        LONG biYPelsPerMeter;
        DWORD biClrUsed;
        DWORD biClrImportant;
    } BITMAPINFOHEADER;
#pragma pack(pop)


class BMP {

public:


    BMP() = default;
    BMP(const BMP& other);

    int get_height() const;
    int get_width() const;

    Pixel& operator()(int row, int column);
    //if it was not necessary delete one of them

    const Pixel& operator()(int row, int column) const;

    friend std::istream& operator>>(std::istream& stream, BMP& bmp);
    friend std::ostream& operator<<(std::ostream& stream, BMP& bmp);

private:
    std::vector<std::vector<Pixel>> pixels_;
    
    BITMAPFILEHEADER bitmap_file_header_;
    BITMAPINFOHEADER bitmap_info_header_;

    void read_headers(std::istream& read_stream);
    void read_bitmap(std::istream& read_stream);

    void write_headers(std::ostream& write_stream);
    void write_bitmap(std::ostream& write_stream);

    int get_data_pos_from_pixel_loc(int row, int col);
};







#endif