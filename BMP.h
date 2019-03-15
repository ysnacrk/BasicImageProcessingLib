#include<iostream>
#include<string>
#include<fstream>
#include <cstdint>
#include <cstring>
#include <math.h>
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
        void saveGrayScale(BYTE * ,string filename);
        void saveImage(string filename);
        void cropImage(int x1, int y1 , int x2 , int y2 );
        int * histogram();
        BYTE * convolutionMatrix(BYTE * , int width , int height , float * , int row , int column);
        BYTE * kMeans(BYTE *data , int width , int height , int K);
    private:

        BYTE * data;
        BYTE * grayData;


        BYTE * tempMatrix;

        int padding;
};


