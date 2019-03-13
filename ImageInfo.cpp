#include "ImageInfo.h"


ImageInfo::ImageInfo(char * header){

    setImageHeader(header);

}

void ImageInfo::setImageHeader(char * header){
    memcpy(allInfo , header , 40);
}


DWORD ImageInfo::getWidth(){

    DWORD width;
    BYTE * temp = allInfo + 4;
    memcpy(&width , temp , sizeof(width));
    return width;
}
DWORD ImageInfo::getHeight(){

    DWORD height;
    BYTE * temp = allInfo + 8;
    memcpy(&height , temp , sizeof(height));
    return height;
}

WORD ImageInfo::getBitCount(){
    WORD bitCount;
    BYTE * temp = allInfo + 14;
    memcpy(&bitCount , temp , sizeof(bitCount));
    return bitCount;
}

DWORD ImageInfo::getBiSize(){
    DWORD size;
    BYTE * temp = allInfo + 20;
    memcpy(&size , temp , sizeof(size));
    return size;
}

void ImageInfo::setSize(DWORD size){
        BYTE * temp = allInfo + 20;
        memcpy(temp , &size , 4);
}

BYTE* ImageInfo::getAllInfo()
{
    return allInfo;
}

void ImageInfo::setWidth(int width){
        BYTE * temp = allInfo + 4;
        memcpy(temp , &width , 4);
}

void ImageInfo::setHeight(int height){
        BYTE * temp = allInfo + 8;
        memcpy(temp , &height , 4);
}
