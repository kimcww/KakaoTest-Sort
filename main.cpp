#include <iostream>
#include <stdio.h>
#include <list>
#include <queue>
#include <string.h>
#define NUMBER0     48
#define NUMBER9     57

using namespace std;

typedef struct TFILENAME {
    char *header;
    unsigned long long number;
    char *tail;
} FILENAME , *PFILENAME;

queue<string> InputList;                                        //입력 String 을 ',' 기준으로 정렬
queue<string> outputList;
list<PFILENAME> CompareList;                                    //실제로 비교 할 LIST

int nInputCount = 0;

void sort(list<PFILENAME> sortData)
{
    list<PFILENAME>::iterator iter_1;
    list<PFILENAME>::iterator iter_2;
    for(iter_1 = sortData.begin(); iter_1 != sortData.end(); iter_1++){

    }
}

void split(string data)
{
    char *SplitArray = new char[1000];

    if( data.size() > 1000){
        printf("size error");                                   // File 입력 1000 글자 이하 예외처리
        return;
    } else {
        strcpy(SplitArray, data.c_str());
    }

    char *SplitPoint = strtok(SplitArray,",");
    while( SplitPoint != nullptr){
        InputList.push(string(SplitPoint));
        SplitPoint = strtok(nullptr,",");
        nInputCount++;
    }
    return;
}

void makeCompareList(string data)
{
    unsigned long long NumberStartCount = 0;
    unsigned long long NumberFinishCount = 100;
    unsigned long long bufferSize = data.size() + 1;

    cout<<"bufferSize : " <<bufferSize<<endl;
    PFILENAME pfileName = new FILENAME;

    char *buffer = new char[bufferSize];
    strcpy(buffer, data.c_str());                               // string 을 char buffer 로 복사

    for(unsigned long long i = 0; i < bufferSize; i++){         // header , number ,tail 구분 Start

        if(buffer[i] == '['){                                   // '[' 입력 = 삭제 후 한칸씩 땡기기
            for(unsigned long long i = 0 ; i < bufferSize-1; i++){
                buffer[i] = buffer[i+1];
            }
            buffer[bufferSize] = ' ';
        }

        if( i == (NumberFinishCount+1)){                                       //tail 파트 ( 마무리 )
            unsigned long long nTailSize = bufferSize - NumberFinishCount;
            pfileName->tail= new char[nTailSize];
            for(unsigned long long i = 0; i < nTailSize ; i++){
                if(buffer[NumberFinishCount + 1 + i] == ']'){                                   // ']' 입력 = 삭제
                    buffer[NumberFinishCount + 1 + i] = ' ';
                }
                pfileName->tail[i] = buffer[NumberFinishCount + 1 + i];
            }
            CompareList.push_back(pfileName);
            return;
        }
        if((NUMBER0 <= buffer[i] && buffer[i] <= NUMBER9) && !(NUMBER0 <= buffer[i-1] && buffer[i-1] <= NUMBER9)){      //header
            unsigned long long nHeaderSize = i-1;
            pfileName->header = new char[nHeaderSize];
            for( unsigned long long j = 0; j < i ; j++){
                pfileName->header[j] = buffer[j];
            }
            NumberStartCount = i;
        }

        if((NUMBER0 <= buffer[i] && buffer[i] <= NUMBER9) && !(NUMBER0 <= buffer[i+1] && buffer[i+1] <= NUMBER9 )){     // number
            NumberFinishCount = i;
            unsigned long long nNumberSize = NumberFinishCount - NumberStartCount + 1;
            char *NumberBuffer = new char[nNumberSize];
            for(unsigned long long i = 0 ; i < nNumberSize ; i++){
                NumberBuffer[i] = buffer[NumberStartCount + i];
            }
            pfileName->number = (unsigned long long)atoi(NumberBuffer);
        }
    }
    return;
}

int main()
{
    string InputArray;
    std::cin>>InputArray;
    split(InputArray);
    for( int i = 0 ; i < nInputCount ; i++){
        std::cout<<InputList.front()<<std::endl;
        makeCompareList(InputList.front());
        InputList.pop();
    }
    list<PFILENAME>::iterator iter;
    for(iter = CompareList.begin(); iter != CompareList.end() ; iter++){
        PFILENAME A = *iter;
        cout<<"header : " <<A->header<<endl;
        cout<<"number : " <<A->number<<endl;
        cout<<"tail : " << A->tail<<endl;
    }
    return 0;
}
