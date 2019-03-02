#include "BMP.h"


int main(){
    BMP bmp;
    bmp.readImage("sample.bmp");
    bmp.saveGrayScale("grayScale");
    bmp.cropImage(0,0,1000,500);
}