#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

class Image
{
public:
    int cols, rows, max_value, n_bytes;
    char* data;

    Image(int, int);
    Image(const char*);
    ~Image();

    void save(const char*);

    void get_pixel(int, int, int&, int&, int&);

    void set_pixel(int, int, int, int, int);
    void set_all(int, int, int);

    Image section(int, int, int, int);
private:
    int get_address(int, int);
};

Image::Image(int _cols, int _rows)
{
    cols = _cols;
    rows = _rows;
    n_bytes = _cols * _rows * 3;
    data = new char[n_bytes];
}

Image::Image(const char* filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Error opening file" << std::endl;
        exit(1);
    }

    char magic_number[2];
    file.read(magic_number, 2);
    if (magic_number[0] != 'P' || magic_number[1] != '6')
    {
        std::cout << "Error: not a PPM file" << std::endl;
        exit(1);
    }

    file >> cols >> rows >> max_value;
    n_bytes = cols * rows * 3;
    data = new char[n_bytes];

    file.read(data, n_bytes);

    file.close();

    std::cout << "Image loaded from " << filename << std::endl;
}

Image::~Image() {}

void Image::save(const char* filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Error opening file" << std::endl;
        exit(1);
    }

    file << "P6" << std::endl;
    file << cols << " " << rows << " " << 255 << std::endl;
    file.write(data, n_bytes);

    file.close();

    std::cout << "Image saved to " << filename << std::endl;
}

int Image::get_address(int x, int y) {
    if (x < 0 || x >= cols || y < 0 || y >= rows) {
        std::cout << "Error: pixel out of bounds" << std::endl;
        std::cout << x << " " << y << std::endl;
        exit(1);
    }

    return (y * cols + x) * 3;
}

void Image::get_pixel(int x, int y, int& red, int& green, int& blue)
{
    int index = get_address(x, y);
    red = (int)data[index];
    green = (int)data[index + 1];
    blue = (int)data[index + 2];
}

void Image::set_pixel(int x, int y, int red, int green, int blue)
{
    int index = get_address(x, y);
    data[index] = red;
    data[index + 1] = green;
    data[index + 2] = blue;
}

void Image::set_all(int red, int green, int blue)
{
    for (int i = 0; i < n_bytes; i += 3)
    {
        data[i] = red;
        data[i + 1] = green;
        data[i + 2] = blue;
    }
}

Image Image::section(int x, int y, int width, int height)
{
    Image section(width, height);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int index = get_address(x + i, y + j);
            section.set_pixel(i, j, data[index], data[index + 1], data[index + 2]);
        }
    }

    return section;
}
