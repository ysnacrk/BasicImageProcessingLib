#include "BMP.h"


int main(){
    BMP bmp;
    bmp.readImage("test.bmp");
    bmp.saveGrayScale("grayScale");
    bmp.cropImage(0,0,512,512);
}