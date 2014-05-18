//
//  Bitmap.h
//  open-safari
//
//  Created by Darren Tsung on 5/17/14.
//  Copyright (c) 2014 Lambawoof. All rights reserved.
//

#ifndef __open_safari__Bitmap__
#define __open_safari__Bitmap__

#include <iostream>
#include <string>

namespace tdogl {
    /** 
     A bitmap image (a 2D grid of pixels)
     
     Not directly related to OpenGL, but can be used to make OpenGL textures
     using tdogl::texture
     */
    class Bitmap {
    public:
        /**
         Format represents the number of channels per pixel (amount of data defining the pixel)
         and the order of the channels
         
         Each channel is one byte
         */
        enum Format {
            Format_Grayscale = 1, /**< one channel, grayscale */
            Format_GrayscaleAlpha = 2, /**< two channels, grayscale and alpha */
            Format_RGB = 3, /**< three channels, red green and blue */
            Format_RGBA = 4, /**< four channels, RGB and alpha */
        };
        
        /**
         Creates a new image with the specified height, width and format.
         
         Width and height are in pixels. Image will contain random garbage if pixels == NULL
         */
        Bitmap(unsigned width, unsigned height, Format format, const unsigned char* pixels = NULL);
        ~Bitmap();
        
        /**
         Tries to load a file into tdogl::Bitmap
         */
        static Bitmap bitmapFromFile(std::string filePath);
        
        /** width in pixels */
        unsigned width() const;
        
        /** height in pixels */
        unsigned height() const;
        
        /** bitmap's format */
        Format format() const;
        
        /** 
         Pointer to the raw pixel data of the bitmap
         
         Each channel is 1 byte. The number and meaning of channels is set by the format 
         of the image. The pointer starts at the top row of the image and then goes right
         and down. 
         i.e. c0r0, c1r0, c2r0, ..., c0r1, c1r1, c2r1, etc.
         */
        unsigned char* pixelBuffer() const;
        
        /**
         @return the pointer to the pixel at the specified column and row
         */
        unsigned char* getPixel(unsigned int column, unsigned int row) const;
        
        /**
         Sets the raw pixel data at the pixel specified by the column and row
         */
        void setPixel(unsigned int column, unsigned int row, unsigned char* pixel);
        
        /** 
         Reverses the row data of the bitmap, so the image is upside down of what it was previously
         */
        void flipVertically();
        
        /**
         Rotates the image 90-degrees counter-clockwise
         */
        void rotate90CounterClockwise();
        
        /** 
         Copies a rectangular area from the bitmap src to this bitmap.
         
         If the srcCol, srcRow, width, height are all 0, the entire source will be 
         copied (full width and height). 
         
         If the source bitmap has a different format than the destination, the pixels
         will be converted to match the destination bitmap.
         
         Throws an exception if the src and destination are the same and the source 
         and destination rectangles are the same. Use the copy functionality if you want to
         copy a bitmap.
         */
        void copyRectFromBitmap(const Bitmap& src,
                            unsigned srcCol,
                            unsigned srcRow,
                            unsigned dstCol,
                            unsigned dstRow,
                            unsigned width,
                            unsigned height);
        
        /** copy constructor */
        Bitmap(const Bitmap& other);
        
        /** assignment operation */
        Bitmap& operator = (const Bitmap& other);
        
    private:
        Format _format;
        unsigned _width, _height;
        unsigned char* _pixels;
        
        void _set(unsigned width, unsigned height, Format format, const unsigned char* pixels);
        static void _getPixelOffset(unsigned col, unsigned row, unsigned width, unsigned height, Format format);
    };
}


#endif /* defined(__open_safari__Bitmap__) */
