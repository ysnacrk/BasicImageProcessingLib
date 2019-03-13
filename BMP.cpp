#include "BMP.h"

BMP::BMP(){
    imageinfo = new ImageInfo;
    fileinfo = new FileInfo;
}
void BMP::readImage(string filename){


    ifstream file(filename , ios::binary); //opening file

    char filetemp[14];                     //store file informations
    char imagetemp[40];

    file.read(filetemp , sizeof(filetemp)); //read first 14 byte
    fileinfo->setFileHeader(filetemp);      //send allData
    file.read(imagetemp , sizeof(imagetemp));
    imageinfo->setImageHeader(imagetemp);
    fileinfo  ->  setOffSet(54);

    file.seekg(54);    //move cursor offset

    padding = 0;

    while((imageinfo->getWidth()*3+padding)%4 != 0) padding++; //calculate padding

    data = new BYTE[imageinfo -> getBiSize()];  //determine data size
    BYTE * pointerOfData = data;                //point data
    BYTE buffer[imageinfo->getWidth()*3];       //temprature memory

    for(int i=0;i<(int)(imageinfo->getBiSize()/sizeof(buffer));i++){

        file.read((char*)buffer,sizeof(buffer));        //read first row image
        memcpy(pointerOfData,buffer,sizeof(buffer));    //copy buffer adresses
        pointerOfData += sizeof(buffer);                //move pointer
        file.read((char*)buffer,padding);               //move cursor
    }

    file.close();
}

void BMP::saveGrayScale(string filename){

    grayData = new BYTE[imageinfo->getBiSize()/3];
    BYTE * iterator = grayData;
    BYTE* pointerOfData = grayData;


    for(int i=0;i<(int)imageinfo->getBiSize();i+=3){
        *iterator = BYTE((data[i]*0.21+data[i+1]*0.72+data[i+2]*0.07));
        iterator++;
    }

    ofstream file(filename + ".bmp" , ios::binary);
    file.write((char *)fileinfo->getAllHeader() , 14);
    file.write((char *)imageinfo->getAllInfo() , 40);


    unsigned int pixelNumber = 0;

    for(int i=0;i<(int)imageinfo->getBiSize()/3;i++){

        file.write((char*)pointerOfData,1);
        file.write((char*)pointerOfData,1);
        file.write((char*)pointerOfData++,1);

        pixelNumber++;

        if(pixelNumber == imageinfo->getWidth()){
            BYTE pad = 0;
            for(int i=0;i<padding;i++) file.write((char*)&pad,1);
            pixelNumber = 0;
        }
    }

    file.close();
}
void BMP::saveGrayScale(BYTE * temp , string filename){

    BYTE * pointerOfData = temp;

    ofstream file(filename + ".bmp" , ios::binary);
    file.write((char *)fileinfo->getAllHeader() , 14);
    file.write((char *)imageinfo->getAllInfo() , 40);

    unsigned int pixelNumber = 0;

    for(int i=0;i<(int)imageinfo->getBiSize()/3;i++){

        file.write((char*)pointerOfData,1);
        file.write((char*)pointerOfData,1);
        file.write((char*)pointerOfData++,1);

        pixelNumber++;

        if(pixelNumber == imageinfo->getWidth()){
            BYTE pad = 0;
            for(int i=0;i<padding;i++) file.write((char*)&pad,1);
            pixelNumber = 0;
        }
    }

    file.close();
}

void BMP::saveImage(string filename){

    ofstream file(filename + ".bmp" , ios::binary);
    file.write((char *)fileinfo->getAllHeader() , 14);
    file.write((char *)imageinfo->getAllInfo() , 40);

    BYTE * pointerOfData = data;
    unsigned int pixelNumber = 0;

    for(int i=0;i<(int)imageinfo->getBiSize();i++){

        file.write((char*)pointerOfData++,1);

        pixelNumber++;
        if(pixelNumber >= imageinfo->getWidth() * 3){
            BYTE pad = 0;
            for(int i=0;i<padding;i++) file.write((char*)&pad,1);
            pixelNumber = 0;
            i += padding;
        }
    }

    file.close();
}

void BMP::cropImage(int x1, int y1 , int x2 , int y2){

    int height = abs(y1 - y2);
    int width =  abs(x1 - x2);
    int iter = 0;


    BYTE * zeroData = new BYTE[width * height];
    BYTE * iterator = zeroData;

    for(int i = y1 ; i < y2 ; i++ ){
        for(int j = x1 ; j < x2 ; j++ ){
            *iterator = grayData[i * width + j];
            iterator++;
        }

    }
    float zoomMatrix [9] = { 0.25 , 0.50 , 0.25 , 0.50 , 1 , 0.50 , 0.25 , 0.50 , 0.25 } ;

    convolutionMatrix(zeroData ,width , height , zoomMatrix ,  3 , 3);
}


BYTE * BMP::convolutionMatrix(BYTE * buffer ,int width , int height , float *  convolutionMatrix, int row , int column){



    int width2 = (2 * width ) + 1 ;
    int height2 = (2 * height ) + 1 ;

    BYTE * tempMatrix = new BYTE[ width2 * height2];
    BYTE  * iterator = buffer;

    for(int i = 0 ; i <  height2; i++){
        for(int j = 0 ; j < width2 ; j++){

            if( (i % 2) == 0 || (j % 2) == 0){
                tempMatrix[i * width2 + j] = 0;
            }
            else{
                tempMatrix[i * width2 + j] = *iterator;
                iterator++;
            }
        }
    }

    buffer = new BYTE [width2 * height2];

    //matrisi sıfırlamamız gerek

    for(int i = 0 ; i < height2 ; i++){
        for (int j = 0 ; j < width2 ; j++){
            buffer[i * width2 + j] = 255;
        }
    }

    int sum = 0;

    for(int i = 0 ; i < height2 - 2 ; i++){
        for(int j = 0 ; j < width2 - 2 ; j++){
            for(int k = 0 ; k < row ; k++){
                for(int m = 0 ; m < column ; m++){

                    sum += tempMatrix[(width2 * k) + m + j + (i * width2)] * convolutionMatrix[3 * k + m];

                    // (width * k + m) --> move convolution matrix
                    // j --> move convolution matrix x axis
                    // (i*width) --> move convolution matrix y axis
                }
            }
            buffer[(i + 1) * width2 + (j + 1)] = sum;
            sum = 0;
        }
    }

    BMP * bmp = new BMP;

    int newPadding = 0;

    while((width2*3+newPadding)%4 != 0) newPadding++;

    DWORD imageSize = width2 * height2 * 3 + 54;

    bmp -> fileinfo->setFileHeader((char*)fileinfo->getAllHeader());
    bmp -> fileinfo->setSize(imageSize);
    bmp -> padding = newPadding;
    bmp -> imageinfo -> setImageHeader((char *)imageinfo->getAllInfo());
    bmp -> imageinfo -> setHeight(height2);
    bmp -> imageinfo -> setWidth(width2);
    bmp -> imageinfo -> setSize(imageSize - 54);

    bmp -> saveGrayScale(buffer , "masked");

    return buffer;
}
