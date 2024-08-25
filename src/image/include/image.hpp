// image.hpp

#pragma once

enum ImageFormat {
    PNG, JPG, BMP
};

enum ImageChannel {
    GREY=1, GREYALPHA=2, RGB=3, RGBA=4
};

class Image {
    public:
        Image(int width, int height, ImageChannel channel);
        Image(unsigned char *array, int width, int height, ImageChannel channel);
        Image(const char *filename, ImageChannel channel);
        ~Image();

        unsigned char * GetArray();
        int GetWidth();
        int GetHeight();
        ImageChannel GetChannel();
        int GetSize();
        void Save(const char *filename, ImageFormat format);
    
    private:
        unsigned char *image;
        int width;
        int height;
        ImageChannel channel;
        int size;
};
