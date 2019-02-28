#include "BMP.h"


int main(){
    BMP bmp;

    bmp.readImage("/home/yasin/Resimler/dog.bmp");
    bmp.saveGrayScale("qq2223");
    bmp.cropImage(0 , 0, 5 , 5);
}