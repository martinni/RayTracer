#pragma once

#include <string>

#include <pixel.h>

class PpmFile
{
  public:
    PpmFile(const std::string &fileName, unsigned int width, unsigned int height, int colorRange);

    void writePixels(const std::vector<Pixel> &pixels);

  private:
    std::string d_fileName;
    unsigned int d_width;
    unsigned int d_height;
    int d_colorRange;
};
