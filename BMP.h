#include<iostream>
#include<string>
#include<fstream>
#include <cstdint>
#include <cstring>
#include <math.h>
#include <algorithm>
#include "FileInfo.h"
#include "ImageInfo.h"

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef uint32_t DWORD;

class BMP{

    public:
        BMP();
        
        int numberOffield = 0;
        int * objectLabelNumbers;
        FileInfo * fileinfo;
        ImageInfo * imageinfo;
        
        void init();


        void readImage(string filename);
        void saveGrayScale(string filename);
        void saveGrayScale(BYTE * ,string filename);
        void saveImage(string filename);
        
        void cropImage(BYTE *  image , int width , int height ,int x1, int y1 , int x2 , int y2 , int indice);
        void drawLine(BYTE * image , int width , int height , int x1 , int y1 , int x2 , int y2);
        void drawRect(BYTE * label , BYTE  * image , int width , int height);
        void objectCout(BYTE * label , int width , int  height);
        
        double moment(BYTE * buffer , int width , int height , int pvalue , int qvalue);
        double centralMoment(BYTE * buffer , int width , int height , int pvalue , int qvalue);
        double normalizedMoment(BYTE * buffer , int width , int height , int pvalue , int qvalue);
        void huInvariantMoments(BYTE * buffer , int width , int height , int pvalue , int qvalue);

        BYTE * convolutionMatrix(BYTE * , int width , int height , float * , int row , int column);
        BYTE * kMeans(BYTE *data , int width , int height , int K);
        BYTE * erosion(BYTE* image, int width, int height) ;
        BYTE * dilation(BYTE* image, int width, int height);
        BYTE * labeling(BYTE* image, int width , int height);

        int * histogram();            

    private:

        BYTE * data;
        BYTE * grayData;
        double  * h;
        
        BYTE * tempMatrix;

        int padding;
};





