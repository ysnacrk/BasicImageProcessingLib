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
        void cropImage(int x1, int y1 , int x2 , int y2 );
        void zeroMatrix(int width , int height);
        void zoomImage(int row , int column);

    private:

        BYTE * data;
        BYTE * grayData;
        
        BYTE * zeroData;
        BYTE * zoomData;
        BYTE * tempMatrix;

        int padding;
};
