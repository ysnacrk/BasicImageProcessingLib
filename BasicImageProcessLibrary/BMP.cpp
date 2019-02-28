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
    
    int row = abs(y1 - y2);
    int column =  abs(x1 - x2);
    int iter = 0;

    zeroData = new BYTE[column * row];

    for(int i = y1 ; i < y2 ; i++ ){
        for(int j = x1 ; j < x2 ; j++ ){
            if( j <= x2 && j >= x1){
                zeroData[iter++] = grayData[i * column + j];
                cout<<(int)grayData[i * column + j];
                cout<<" ";
            }
        }
        cout<<endl;
    }
    for(int i = 0 ; i < row ; i++){
        for (int j = 0 ; j < column ; j++){
            cout<<(int)zeroData[i*column + j];
            cout<<" ";
        }
        cout<<endl;
    }
    zeroMatrix(row , column);
}


void::BMP::zeroMatrix(int width , int height){
    
    width = (2 * width ) + 1 ;
    height = (2 * height ) + 1 ;
   
    tempMatrix = new BYTE[ width * height];
    BYTE  * iterator = zeroData;


    for(int i = 0 ; i <  height; i++){
        for(int j = 0 ; j < width ; j++){

            if( (i % 2) == 0 || (j % 2) == 0){
                tempMatrix[i * width + j] = 0;
            }
            else{
                tempMatrix[i * width + j] = *iterator;
                iterator++;
            }
        }
    }

    for(int i = 0 ; i < height ; i++){
        for (int j = 0 ; j < width ; j++){
            cout<<(int)tempMatrix[i*width+ j];
            cout<<" ";
        }
        cout<<endl;
    }

    zoomImage(width , height);
}

void BMP::zoomImage(int width , int height){

    int sum = 0;
    int iter = 1;

    float convolutionMatrix [9] = { 0.25 , 0.50 , 0.25 , 0.50 , 1 , 0.50 , 0.25 , 0.50 , 0.25 } ;
    

    zoomData = new BYTE [width * height];

    for(int i = 0 ; i < height ; i++){
        for (int j = 0 ; j < width ; j++){
            /* cout<<"."; */
            cout<<(int)tempMatrix[i*width+ j];
            cout<<" ";
        }
        cout<<endl;
    }
    
                    //matrisi sıfırlamamız gerek
    for(int i = 0 ; i < height ; i++){
        for (int j = 0 ; j < width ; j++){
            zoomData[i * width + j] = 0;
        }
    }

    for(int i = 0 ; i <= height - 3 ; i++){
        for(int j = 0 ; j <= width - 3 ; j++){
            for(int k = 0 ; k < 3 ; k++){
                for(int m = 0 ; m < 3 ; m++){
                
                    sum += tempMatrix[(width * k + m) + j + (i*width)] * convolutionMatrix[3 * k + m];
                    
                    // (width * k + m) --> move convolution matrix
                    // j --> move convolution matrix x axis
                    // (i*width) --> move convolution matrix y axis  
                }
            }
            zoomData[(i + 1) * width + (j + 1)] = sum;
            sum = 0;
        }
    }


    for(int i = 0 ; i < height ; i++){
        for (int j = 0 ; j < width ; j++){
            cout<<(int)zoomData[i*width+ j];
            cout<<" ";
        }
        cout<<endl;
    }
}

