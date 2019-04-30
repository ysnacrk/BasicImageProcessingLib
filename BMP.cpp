#include "BMP.h"

BMP::BMP(){
    imageinfo = new ImageInfo;
    fileinfo = new FileInfo;
}


void BMP::init(){
    //Initialize functions

    BYTE * tempData = new BYTE[imageinfo->getWidth() * imageinfo->getHeight() ];
    BYTE * label = new BYTE[imageinfo->getWidth() * imageinfo->getHeight() ];

    
    tempData = kMeans(grayData, imageinfo->getWidth()  , imageinfo->getHeight() , 2);    
    saveGrayScale(tempData , "images/3kmeans");

    tempData = erosion(tempData , imageinfo->getWidth() , imageinfo->getHeight() );
    tempData = erosion(tempData , imageinfo->getWidth() , imageinfo->getHeight() );
    tempData = erosion(tempData , imageinfo->getWidth() , imageinfo->getHeight() );


    //saveGrayScale(tempData , "images/erosion");

    tempData = dilation(tempData , imageinfo->getWidth() , imageinfo->getHeight() );
    tempData = dilation(tempData , imageinfo->getWidth() , imageinfo->getHeight() );

    //saveGrayScale(tempData , "images/dilation");

    label = labeling(tempData, imageinfo->getWidth() , imageinfo->getHeight());
    objectCout(label , imageinfo->getWidth() , imageinfo->getHeight());
    drawRect(label ,tempData, imageinfo->getWidth()  , imageinfo->getHeight());

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

void BMP::cropImage(BYTE * buffer , int width , int height , int x1, int y1 , int x2 , int y2 , int indice){

    int cheight = abs(y2 - y1);
    int cwidth =  abs(x2 - x1);
    int iter = 0;

    BYTE * croppedData = new BYTE[cwidth * cheight];


    for (int i = y1; i < y2; i++){
        for (int j = x1; j < x2; j++){
				croppedData[iter] = buffer[i * width + j]; 
                iter++;
		}
    }
    
    for(int i = 0 ; i < cheight * cwidth ; i++){

        if(croppedData[i] == indice) croppedData[i] = 0;
        else croppedData[i] = 255;
    }
    string path = "images/cropped" + std::to_string(indice);


 /*    int width2 = (2 * cwidth ) + 1;
    int height2 = (2 * cheight ) + 1 ;
    
    BYTE * tempMatrix = new BYTE[ width2 * height2];

    for(int i = 0 ; i <  height2; i++){
        for(int j = 0 ; j < width2 ; j++){

            if( (i % 2) == 0 || (j % 2) == 0){
                tempMatrix[i * width2 + j] = 0;
            }
            else{
                tempMatrix[i * width2 + j] = zeroData[iter];
                iter++;
            }
        }
    }
    float zoomMatrix [9] = { 0.25 , 0.50, 0.25 , 0.50 , 1 , 0.50 , 0.25 , 0.50 , 0.25 } ; */

    BMP * bmp = new BMP;

    int newPadding = 0;
    
    while((cwidth *3+newPadding)%4 != 0) newPadding++;

    DWORD imageSize = cwidth * cheight * 3 + 54;

    bmp -> fileinfo->setFileHeader((char*)fileinfo->getAllHeader());
    bmp -> fileinfo->setSize(imageSize);
    bmp -> padding = newPadding;
    bmp -> imageinfo -> setImageHeader((char *)imageinfo->getAllInfo());
    bmp -> imageinfo -> setHeight(cheight);
    bmp -> imageinfo -> setWidth(cwidth);
    bmp -> imageinfo -> setSize(imageSize - 54);

    bmp -> saveGrayScale(croppedData , path);
}


BYTE * BMP::convolutionMatrix(BYTE * buffer ,int width , int height , float *  convolutionMatrix, int row , int column){
   
    BYTE * temp = new BYTE[ width * height];

    //add zero first , last column and row then add masked

    for(int i = 0 ; i < height ; i++){
        for (int j = 0 ; j < width ; j++){
            temp[i * width + j] = 255;
        }
    }

    int sum = 0;

    for(int i = 0 ; i < height - 2 ; i++){
        for(int j = 0 ; j < width - 2 ; j++){
            for(int k = 0 ; k < row ; k++){
                for(int m = 0 ; m < column ; m++){
                
                    sum +=(buffer[(width * k) + m + j + (i * width)] * convolutionMatrix[3 * k + m]);
                    
                    // (width * k + m) --> move convolution matrix
                    // j --> move convolution matrix x axis
                    // (i*width) --> move convolution matrix y axis  
                }
            }

            
                
            temp[(i + ((row - 1)/2)) * width + (j + ((row - 1)/2))] = sum;
            sum = 0;
        }
    }

    BMP * bmp = new BMP;

    int newPadding = 0;
    
    while((width*3+newPadding)%4 != 0) newPadding++;

    DWORD imageSize = width * height * 3 + 54;

    bmp -> fileinfo->setFileHeader((char*)fileinfo->getAllHeader());
    bmp -> fileinfo->setSize(imageSize);
    bmp -> padding = newPadding;
    bmp -> imageinfo -> setImageHeader((char *)imageinfo->getAllInfo());
    bmp -> imageinfo -> setHeight(height);
    bmp -> imageinfo -> setWidth(width);
    bmp -> imageinfo -> setSize(imageSize - 54);

    bmp -> saveGrayScale(temp , "masked");

    return buffer;
}

BYTE * BMP :: kMeans(BYTE * data , int width  , int height , int K){
        
        int histogram[256];
        
        for(int i = 0; i < 256 ; i++)histogram[i] = 0;

        for(int i = 0; i < height * width ; i++)histogram[data[i]]++;


        int * label = new int [256];    //tag matrix
        int * T = new int[K];           //T1 T2 T3 vs.

        int * mean = new int [K];       
        int * meanSum = new int[K];

/*         srand(time(0));

        for(int i = 0 ; i < K  ; i++){
            T[i] = rand() % 256;
        } */
        T[0] = 10;
        T[1] = 150;
        int min = 0;
        double sum = 0;

        while(true){

            for(int i = 0; i<256 ; i++){
                min = 0;
                for(int j = 1 ; j < K ; j++){
                    if(abs(T[j] - i) < abs(T[min] - i))  //Euclidian distance
                        min = j;

                }
                label[i] = T[min];
            }

            for(int i = 0 ; i < K ; i++){               //set zero mean
                mean[i] = 0;
                meanSum[i] = 0;
            }

            for(int i = 0; i<256 ; i ++){
                for(int j = 0 ; j < K ; j++){
                    if(label[i] == T[j]){
                        mean[j] += histogram[i] * i;    
                        meanSum[j] += histogram[i];     
                    }
                }
            }
 
            int * tempT = new int[K];
            int counterFlag = 0;

            for(int i = 0 ; i < K ; i++){               //find new T values or finish   
                tempT[i] = T[i];

                if(meanSum[i] == 0) meanSum[i] = 1;   
                T[i] = mean[i] / meanSum[i];
                if(T[i] == tempT[i]) counterFlag ++;
            }

            if(counterFlag == K) break;
            
        }
        //int katsayi = 256 / K;                          //color range

        for(int i = 0 ; i < 256 ; i++){                 
            for(int j= 0; j<K ; j++ ){
                if(label[i] == T[j]){
                    if(j % 2 == 0){
                        label[i] = 255;
                    }
                    else{
                        label[i] = 0;
                    }
                };
            }
        }
        for(int i = 0 ; i < width * height ; i++) data[i] = label[data[i]];
        //saveGrayScale(data , "images/mean");
        return data;
}

BYTE * BMP :: erosion(BYTE* image, int width, int height) {

	BYTE * data = new BYTE[width*height];

	for (int i = 0; i < width*height; i++)data[i] = image[i];

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (image[i + j * width] == 0) {
				if		(i > 0			&& image[i - 1 +	width * j]		 == 255)	data[i + width * j] = 255;
				else if (j > 0			&& image[i +		width * (j - 1)] == 255)	data[i + width * j] = 255;
				else if (i + 1 < width	&& image[i + 1 +	width * j]		 == 255)	data[i + width * j] = 255;
				else if (j + 1 < height && image[i +		width * (j + 1)] == 255)	data[i + width * j] = 255;
			}
		}
	}
    
	return data;
}

BYTE * BMP :: dilation(BYTE * image, int width, int height)
{
	BYTE* data = new BYTE[width*height];
	for (int i = 0; i < width*height; i++)data[i] = image[i];
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if(image[i + j * width] == 0) {
				if		(i > 0			&& image[i - 1 + width * j]		  == 0)				data[(i - 1)+ width * j] = 0;
				if (j > 0			&& image[i	   + width * (j - 1)] == 0)				data[i + width * (j-1)] = 0;
				if (i + 1 < width	&& image[i + 1 + width * j]		  == 0)				data[(i + 1) + width * j] = 0;
				if (j + 1 < height && image[i	   + width * (j + 1)] == 0)				data[i + width * (j+1)] = 0;

			}
		}
	}
	return data;
}

BYTE * BMP :: labeling (BYTE * buffer , int width , int height){

    BYTE * label = new BYTE [width  * height];
    
    for(long i = 0 ; i < width  * height ; i++) label[i] = 0;
    
    int labelNumber = 1; //number of objects
    int min = 0;
    int firstValue = 0 , lastValue = 0;
    bool collision = true;

    
    while(collision){

        collision = false;
        
        for (int i = 1; i < height -1; i++){
            for (int j = 1; j < width -1; j++){
                if(buffer[j + i * width] == 0){
                        
                    firstValue = label[j + i * width];
                    min = 0;
                    for(int k = -1 ; k<= 1 ; k++){
                        for(int l = -1 ; l<= 1 ; l++){
                            if(label[j + l +(i + k) * width] != 0 && min == 0){
                                min = label[j + l +(i + k) * width] ;
                            }
                            else if(label[j + l +(i + k) * width] < min && label[j + l +(i + k) * width] != 0 && min != 0){
                                min = label[j + l +(i + k) * width];
                            
                            }

                        }
                    }
                    if(min != 0)  label[j + i * width ] = min;
                    else if(min == 0) {
                        label[j + i * width] = labelNumber;
                        labelNumber ++;
                    }
                    lastValue = label[ j + i * width];

                    if(firstValue != lastValue) collision = true;
                }
            }
        }
    }

    return label;
}

void BMP :: objectCout(BYTE * label , int width , int height){
    
    BYTE temp = label[0];
    BYTE * tempMatrix = new BYTE [width * height];
    for(long  i = 0 ; i < width*height -1 ; i++) tempMatrix[i] = label[i];
    int counter = 0;

    std::sort(tempMatrix, tempMatrix + (width * height - 1));

    for(int i = 0 ; i < width * height -1 ; i++ ){
        
        if(tempMatrix[i] != 0){
            if(tempMatrix[i] != temp){
                counter++;
                cout<<counter;
                temp = tempMatrix[i];
            }
        }
    }
    numberOffield = counter;
    cout<<"Obje sayısı :"<<counter<<endl;
    objectLabelNumbers = new int[counter];
    int index = 0;
    
    for(int i = 0 ; i < width * height -1 ; i++ ){
        
        if(tempMatrix[i] != 0){
            if(tempMatrix[i] != temp){
                objectLabelNumbers[index] = tempMatrix[i];
                temp = tempMatrix[i];
                index++;
            }
        }
    }
    for(int i = 0 ; i < counter ; i++) cout<<objectLabelNumbers[i]<<endl;
}

//drawLine on bmp file

void BMP::drawLine(BYTE * image , int width , int height , int x1 , int y1 , int x2 , int y2){


	int subX = (x1 - x2);
	int subY = (y1 - y2);

    int y = 0;

		//vertical
	if (subX == 0){
		for (int i = y1; i < y2; i++)
			image[i * width + x1] = 128;
    }
		//horizontal
	if (subY == 0) {
		for (int i = x1; i < x2; i++)
			image[y1 * width + i] = 128;
    }
 
} 


void BMP::drawRect(BYTE * label , BYTE  * image , int width , int height){

    cout<<"Obje sayısı :"<<numberOffield;
    for(int x = 0 ; x < numberOffield ; x++){

        int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
        bool flag = false;
		
        for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				if ( (int)label[i + j * width] == objectLabelNumbers[x]  && y1 == -1)
				{	
                    y1=j;
				}
				else if ((int)label[width-i + (height-1-j) * width] == objectLabelNumbers[x] && y2 == -1)
				{
					y2 = height - j;
				}

			}
		}
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				if ((int)label[i + j * width] == objectLabelNumbers[x] && x1 == -1)
				{
					x1 = i;
				}
				else if ((int)label[(width-i) + (height-1-j) * width] == objectLabelNumbers[x] && x2 == -1)
				{
					x2 = width - i;
				}

			}
		}
        cout<<endl;
        cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2;
        cropImage(label , width , height ,x1 , y1 , x2 , y2 , objectLabelNumbers[x]);
        
        drawLine(image , width, height, x1 , y1 , x1 , y2); //sol
        drawLine(image , width, height, x1 , y1 , x2 , y1); //üst
        drawLine(image , width, height, x2 , y1 , x2 , y2); //sağ       
        drawLine(image , width, height, x1 , y2 , x2 , y2); //alt

    }
    saveGrayScale(image , "images/4rect");
}



int * BMP::histogram(){
    int * histogram = new int[256]();
    for(int i = 0; i < (imageinfo->getWidth() * imageinfo->getHeight()) ; i++)histogram[grayData[i]]++;
    return histogram;
}



