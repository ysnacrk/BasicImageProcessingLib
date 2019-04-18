#include "BMP.h"


int main(){
    BMP bmp;
    bmp.readImage("index.bmp");
    bmp.saveGrayScale("grayScale");
    bmp.init();
}
