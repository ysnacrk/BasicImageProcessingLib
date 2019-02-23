#include "BMP.h"

BMP::BMP(){
    imageinfo = new ImageInfo;
    fileinfo = new FileInfo;
}
void BMP::readImage(string filename){
    

    cout<<"deneme";
    ifstream file(filename , ios::binary);

    char filetemp[14];

    file.read(filetemp , sizeof(filetemp));
    fileinfo->setFileHeader(filetemp);
    char imagetemp[40];
    file.read(imagetemp , sizeof(imagetemp));

    imageinfo->setImageHeader(imagetemp);


    file.seekg(fileinfo -> getOffSet());
    
    padding = 0;
    while((imageinfo->getWidth()*3+padding)%4 != 0) padding++;

    data = new BYTE[imageinfo -> getBiSize()];
    BYTE * pointerOfData = data;
    BYTE buffer[imageinfo->getWidth()*3];

    for(int i=0;i<(int)(imageinfo->getBiSize()/sizeof(buffer));i++){
        
        file.read((char*)buffer,sizeof(buffer));
        memcpy(pointerOfData,buffer,sizeof(buffer));
        pointerOfData += sizeof(buffer);
        file.read((char*)buffer,padding); 
    }

    file.close();
}

void BMP::saveGrayScale(string filename){
	int padding = 0;
	int scanlinebytes = imageinfo->getWidth() * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// create new buffer
	BYTE* newbuf = new BYTE[imageinfo->getWidth()*imageinfo->getHeight()];

/*     BYTE * buffer = new BYTE[imageinfo->getWidth() * imageinfo->getHeight()];
 */    

    long bufpos = 0;
	long newpos = 0;
    
    for(int i=0 ; i<(int)(imageinfo -> getHeight());i++){
        for(int j = 0 ; j < (int)(imageinfo -> getWidth()) ; j++){
		      newpos = i * imageinfo -> getWidth() + j;
		      bufpos = (imageinfo -> getHeight() - i - 1) * psw + j * 3;
		      newbuf[newpos] = BYTE(0.11*data[bufpos + 2] + 0.59*data[bufpos + 1] + 0.3*data[bufpos + 0]);
        }    
    }

    ofstream file(filename + ".bmp" , ios::binary);
    file.write((char *)fileinfo->getAllHeader() , 14);
    file.write((char *)imageinfo->getAllInfo() , 40);

    BYTE* pointerOfData = newbuf;
    
    unsigned int pixelNumber = 0; // for padding

    for(int i=0;i<(int)imageinfo->getBiSize();i++){
        file.write((char*)pointerOfData,1);
        file.write((char*)pointerOfData,1);
        file.write((char*)pointerOfData++,1);
        pixelNumber++;

        if(pixelNumber>=imageinfo->getWidth()*3){
            BYTE pad = 0;
            for(int i=0;i<padding;i++) file.write((char*)&pad,1);
            pixelNumber = 0; i+=padding;
        }
    }

    file.close();
}

void BMP::saveImage(string filename){

    ofstream file(filename + ".bmp" , ios::binary);
    file.write((char *)fileinfo->getAllHeader() , 14);
    file.write((char *)imageinfo->getAllInfo() , 40);

    BYTE* pointerOfData = data;
    unsigned int pixelNumber = 0; // for padding
    for(int i=0;i<(int)imageinfo->getBiSize();i++){
        file.write((char*)pointerOfData++,1);
        pixelNumber++;
        if(pixelNumber>=imageinfo->getWidth()*3){
            BYTE pad = 0;
            for(int i=0;i<padding;i++) file.write((char*)&pad,1);
            pixelNumber = 0; i+=padding;
        }
    }

    file.close();
}        