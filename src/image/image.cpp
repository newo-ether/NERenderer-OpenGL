// image.cpp

#define STB_IMAGE_IMPLEMENTATION
#include "../../third-party/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../third-party/stb/stb_image_write.h"

#include "include/image.hpp"

Image::Image(int width, int height, ImageChannel channel):
    image(new unsigned char [width * height * channel]),
    width(width),
    height(height),
    channel(channel),
    size(width * height * channel) { }

Image::Image(unsigned char *array, int width, int height, ImageChannel channel):
    image(array),
    width(width),
    height(height),
    channel(channel),
    size(width * height * channel) { }

Image::Image(const char *filename, ImageChannel channel) {
    int originalChannel;
    image = stbi_load(filename, &width, &height, &originalChannel, channel);
}

Image::~Image() {
    delete [] image;
}

unsigned char * Image::GetArray() {
    return image;
}

int Image::GetWidth() {
    return width;
}

int Image::GetHeight() {
    return height;
}

ImageChannel Image::GetChannel() {
    return channel;
}

int Image::GetSize() {
    return size;
}

void Image::Save(const char *filename, ImageFormat format) {
    if (format == PNG) {
        stbi_write_png(filename, width, height, channel, image, width * channel);
    }
    else if (format == JPG) {
        stbi_write_jpg(filename, width, height, channel, image, 100);
    }
    else if (format == BMP) {
        stbi_write_bmp(filename, width, height, channel, image);
    }
}
