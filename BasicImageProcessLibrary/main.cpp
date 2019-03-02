#include "BMP.h"


int main(){
    BMP bmp;
    bmp.readImage("asd.bmp");
    bmp.saveGrayScale("ttt");
    bmp.cropImage(0,0,800,600);
}