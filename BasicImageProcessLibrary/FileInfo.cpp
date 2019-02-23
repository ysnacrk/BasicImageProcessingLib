#include "FileInfo.h"

FileInfo::FileInfo(char * header){
    setFileHeader(header);
}

void FileInfo::setFileHeader(char * header){
    memcpy(allHeader , header , 14);
}

WORD FileInfo::getType(){
    WORD type;
    memcpy(&type , allHeader , sizeof(type));      

    return type;
}

DWORD FileInfo::getSize(){
    DWORD size;
    BYTE *temp = allHeader + 2;
    memcpy(&size , temp , sizeof(size));

    return size;
}

DWORD FileInfo::getOffSet(){
    DWORD offset;
    BYTE *temp = allHeader + 10;
    memcpy(&offset , temp , sizeof(offset));

    return offset;
}


BYTE * FileInfo::getAllHeader(){
    return allHeader;
}


void FileInfo::setSize(DWORD size){
    BYTE * temp = allHeader + 2;
    memcpy(&temp , &size , 4);
}