#include<iostream>
#include<string>
#include<fstream>
#include <cstdint>
#include <cstring>
#include "FileInfo.h"
#include "ImageInfo.h"

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef uint32_t DWORD;

class BMP{
    public:
        BMP();
        FileInfo * fileinfo;
        ImageInfo * imageinfo;

        void readImage(string filename);
        void saveGrayScale(string filename);
        void saveImage(string filename);

    private:
        BYTE * data;
        int padding;
};
