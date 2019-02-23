#include<iostream>

#include <cstdint>
#include <cstring>

using namespace std;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef uint32_t DWORD;

class FileInfo{
    public:
        FileInfo(){
            cout<<"file a girdi"<<endl;
        }

        FileInfo(char * );

        //get functions
        WORD    getType();
        DWORD   getSize();
        DWORD   getOffSet();
        BYTE * getAllHeader();
        
        //set functions

        void setFileHeader(char *);
        void setSize(DWORD);
    
    private:
        
        BYTE allHeader[14];

};