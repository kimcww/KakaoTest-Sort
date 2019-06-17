#include <iostream>
#include <stdio.h>
#include <list>
#include <queue>
#include <string.h>
#define NUMBER0     48
#define NUMBER9     57
#define BIG_SMALL_ALPHABET   32

using namespace std;

typedef struct TFILENAME {
    char *header;
    unsigned long long number;
    char *tail;
} FILENAME , *PFILENAME;

queue<string> InputList;                                        //입력 String 을 ',' 기준으로 정렬
queue<string> outputList;
list<PFILENAME> CompareList;                                    //실제로 비교 할 LIST
PFILENAME pFileData;

int nInputCount = 0;
int nFileCount = 0;

void sort(PFILENAME pData)
{
    for(int i = 0 ; i < nFileCount -1 ; i++){
        for ( int j = 1; j < nFileCount - i ; j++){
            int nHeaderCheck;
            bool bNumberCheck = true;
            sizeof(pData[i].header) > sizeof (pData[j].header) ? nHeaderCheck = sizeof(pData[i].header) : nHeaderCheck = sizeof(pData[j].header);
            for( int k = 0; k < nHeaderCheck ; k++){                        // header 비교 정렬
                if( (pData[j-1].header[k] == pData[j].header[k]) || ((pData[j-1].header[k] - pData[j].header[k]) == BIG_SMALL_ALPHABET) ||((pData[j].header[k] - pData[j-1].header[k]) == BIG_SMALL_ALPHABET) ){
                    continue;                                               // k 값 skip
                } else if ( (pData[j-1].header[k] > pData[j].header[k]) &&((pData[j].header[k] -pData[j-1].header[k]) < 32) ){    // 이전 데이터가 다음데이터보다 클때, 대소문자 구분없음
                    PFILENAME pBuffer = new FILENAME;
                    *pBuffer = pData[j-1];                        // 버퍼를 생성해 주소 복사
                    pData[j-1] = pData[j];                                  // 이전 데이터에 다음 데이터 주소값 복사
                    pData[j] = *pBuffer;                                    // 다음 데이터에 버퍼에 저장되어있는 데이터 복사
                    bNumberCheck = false;
                    cout<<"Change Data"<<endl;
                    delete pBuffer;
                    break;                                                  // header 에서 순서 찾음  --> break 끝 , number 비교 x
                }
            }
            if( bNumberCheck == true ){
                if( pData[j-1].number > pData[j].number){
                    PFILENAME pBuffer = new FILENAME;                       // 버퍼를 생성해 주소 복사
                    *pBuffer = pData[j-1];
                    pData[j-1] = pData[j];                                  // 이전 데이터에 다음 데이터 주소값 복사
                    pData[j] = *pBuffer;                                    // 다음 데이터에 버퍼에 저장되어있는 데이터 복사
                    cout<<"Change Number"<<endl;
                    delete pBuffer;
                } else{
                    cout<<"NoChange"<<endl;
                }
            }

        }
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
    pFileData = new FILENAME[InputList.size()];
    cout<<"InputSize: "<<InputList.size()<<endl;
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
//            CompareList.push_back(pfileName);
            pFileData[nFileCount] = *pfileName;
            nFileCount++;
            return;
        }
        if((NUMBER0 <= buffer[i] && buffer[i] <= NUMBER9) && !(NUMBER0 <= buffer[i-1] && buffer[i-1] <= NUMBER9)){      //header
            unsigned long long nHeaderSize = i;
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

    sort(pFileData);


    cout<<"[";
    for(int i = 0; i <nFileCount ; i++){
        cout<<pFileData[i].header;
        cout<<pFileData[i].number;
        cout<<pFileData[i].tail;
        if( i != nFileCount-1){
            cout<<",";
        }
    }
    cout<<"]";
    return 0;
}
