#include "ppmFile.h"

#include <fstream>
#include <iostream>
#include <vector>

PpmFile::PpmFile(const std::string &fileName, unsigned int width, unsigned int height,
                 int colorRange)
    : d_fileName(fileName), d_width(width), d_height(height), d_colorRange(colorRange)
{
    std::ofstream ofs(d_fileName, std::ios::out | std::ios::binary | std::ios::trunc);
    ofs << "P3\n" << d_width << " " << height << "\n" << d_colorRange << "\n";
    ofs.close();
}

void PpmFile::writePixels(const std::vector<Pixel> &pixels)
{
    if (pixels.size() != d_width * d_height)
    {
        std::cerr << "Number of pixels doesn't fit image size. Got " << pixels.size()
                  << " pixels for image of size " << d_width << "*" << d_height << std::endl;
    }

    std::ofstream ofs(d_fileName, std::ios::out | std::ios::binary | std::ios::app);
    for (unsigned int i = 0; i < d_width * d_height; ++i)
    {
        ofs << static_cast<int>(pixels[i].color.red * d_colorRange) << " "
            << static_cast<int>(pixels[i].color.green * d_colorRange) << " "
            << static_cast<int>(pixels[i].color.blue * d_colorRange) << "\n";
    }
    ofs.close();
}
