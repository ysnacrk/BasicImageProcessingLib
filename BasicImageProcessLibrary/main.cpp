#include "BMP.h"


int main(){

    BMP bmp;
    bmp.readImage("sample.bmp");
    bmp.saveGrayScale("samplegray");
}