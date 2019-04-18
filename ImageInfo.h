#include<iostream>
#include <cstdint>
#include <cstring>

using namespace std;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef uint32_t DWORD;

using namespace std;

class ImageInfo{

    public:
        ImageInfo(){
            
        }


        ImageInfo(char *);

        DWORD getWidth();   //get image width
        DWORD getHeight();  //get image height
        WORD getBitCount(); //????
        BYTE * getAllInfo();
        DWORD getBiSize();
        
        void setImageHeader(char *);
        void setWidth(int);
        void setHeight(int);
        void setSize(DWORD );

    private:
        
        BYTE allInfo[40];
};