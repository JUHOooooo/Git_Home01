#include "hard.h"
unsigned int bnum = 0;//pbn을 점점 증가시키기위해 전역변수 처리
unsigned int snum = 0;//psn을 점점 증가시키기위해 전역변수 처리
//사용자가 입력한 MB단위로 할당 받는다. MB를 byte로 환산하여 block과 sector의 개수를 파악하고 array로 만든다.
void init(int megabyte) {
    unsigned int byte = 1024 * 1024 * megabyte;// megabyte를 byte로 환산
    unsigned int pblock = byte / 16384; //블록 수 구하기
    unsigned int psector = byte / 512;  //섹터 수 구하기
    printf("실질적 블록 개수는 %d 개 입니다.\n", pblock);
    printf("실질적 섹터 개수는 %d 개 입니다.\n", psector);
    for (int i = 0; i < pblock; i++)//블록 수 만큼 반복해서 addblock()으로 블록 추가
    {
        addblock();
    }
    savefile();// 모든 블록, 섹터, pbn, psn을 생성한 것을 "하드웨어.txt"에 쓰기
}

//Node를 추가하는 함수, Block단위로 추가하며, Block이 가르키는 Sector도 생성하여 동적할당을 한다.
void addblock() {
    struct Block* bNode;
    struct Block* newNode;
    bNode = head;
    while (bNode->next)
    {
        bNode = bNode->next; //Block노드 NULL만나기 전까지 진행
    }
    newNode = (struct Block*)malloc(sizeof(struct Block));//새로운 노드 동적할당
    newNode->pbn = bnum;//노드의 pbn값을 선언
    bnum++;// 다음 addblock()함수가 실행되면, 1증가하도록 설정
    for (int i = 0; i < 32; i++)
    {
        newNode->sector[i] = (struct Sector*)malloc(sizeof(struct Sector));//newNode의 sector를 동적 할당
        newNode->sector[i]->psn = snum;//newNode의 sector의 psn을 다음 addblock()함수 실행, for문으로 32번 실행할때마다 psn을 1씩 증가
        snum++;
        strcpy(newNode->sector[i]->data, "\0");
    }
    newNode->next = NULL;//newNode의 다음을 마지막인 NULL로 설정
    bNode->next = newNode;//현재 마지막 노드였던 bNode의 다음을 새로 추가한 newNode로 설정
}

//해당 psn에 있는 data를 읽는 함수
void Flash_read(unsigned int psn) {
    struct Block* bNode = head->next;
    while (bNode)
    {
        for (int i = 0; i < 32; i++)// bNode안의 sector의 32개를 다 둘러봄
        {
            if (bNode->sector[i]->psn == psn) {// sector의 psn과 이용자가 원하는 psn이 동일하면 아래 코드 실행
                printf("psn: %d\ndata: %s\n", bNode->sector[i]->psn, bNode->sector[i]->data);
                return;
            }
        }
        bNode = bNode->next;//sector까지 다 돌고, 다음 노드로 진행
    }
}

//선택한 psn에 data를 입력하는 함수
void Flash_write(unsigned int psn, char* data) {
    struct Block* bNode = head->next;
    while (bNode)
    {
        for (int i = 0; i < 32; i++)
        {
            if (bNode->sector[i]->psn == psn)// sector의 psn과 이용자가 원하는 psn이 동일하면 아래 코드 실행
            {
                printf("psn발견\n");
                if (strlen(bNode->sector[i]->data) > 0) { //해당 bNode의 sector에 데이터가 존재하면 아래 코드 실행
                    printf("해당 섹터에 데이터가 존재합니다.\n");
                    return;
                }
                    strcpy(bNode->sector[i]->data, data);//해당 bNode의 sector에 데이터가 존재하지 않으면 아래 코드 실행
                    printf("psn: %d\tdata: %s\n", bNode->sector[i]->psn, bNode->sector[i]->data);//변경한 psn과 data를 보여줌
                    savefile();// "하드웨어.txt"에 다시 쓰기
                    return;
            }
        }
        bNode = bNode->next;//sector까지 다 돌고, 다음 노드로 진행
    }
}
//선택한 psn을 할당해제하여 없애는 함수.
void Flash_erase(unsigned int pbn){
    struct Block* bNode;
    bNode = head->next;
    while (bNode)
    {
        if (bNode->pbn == pbn) {// 이용자가 지우길 원하는 pbn을 가지고 있는 bNode를 찾으면 아래 코드 실행
            for (int i = 0; i < 32; i++)
            {
                strcpy(bNode->sector[i]->data, "\0");//해당 bNode의 sector전체의 data에 "\0"로 설정
            }
            savefile();// 변경할것을 "하드웨어.txt"에 다시 쓰기
        }
        bNode = bNode->next;
    }
    
}
//모든 pbn,psn을 보여주는 함수
void show() {
    struct Block* bNode;
    bNode = head->next;
    while (bNode)
    {
        printf("pbn: %d\n", bNode->pbn);
        for (int i = 0; i < 32; i++)
        {
            printf("psn: %d\t", bNode->sector[i]->psn);
            printf("data: %s\t", bNode->sector[i]->data);
        }
        printf("\n");
        bNode = bNode->next;
    }
}
//Block노드에 저장되어있는 모든 데이터를 "하드웨어.txt"에 쓰는 함수.
int savefile() {
    FILE* fp = fopen("하드웨어.txt", "w");//"하드웨어.txt"파일을 쓰는 모드로 오픈
    struct Block* bNode = head->next;//bNode는 head다음부터 시작.
    if (fp == NULL) {//fopen함수는 오류발생시 NULL을 리턴하므로 오류발생시"파일 에러" 프린트
        printf("파일 에러.\n");
        return 1;
    }
    while (bNode)
    {
        for (int i = 0; i < 32; i++)
        {
            fprintf(fp, "%d %s", bNode->sector[i]->psn, bNode->sector[i]->data);
            fputc('\n', fp);
        }
        bNode = bNode->next;
    }
    fclose(fp);
}
#include "hard.h"
unsigned int bnum = 0;//pbn을 점점 증가시키기위해 전역변수 처리
unsigned int snum = 0;//psn을 점점 증가시키기위해 전역변수 처리
//사용자가 입력한 MB단위로 할당 받는다. MB를 byte로 환산하여 block과 sector의 개수를 파악하고 array로 만든다.
void init(int megabyte) {
    unsigned int byte = 1024 * 1024 * megabyte;// megabyte를 byte로 환산
    unsigned int pblock = byte / 16384; //블록 수 구하기
    unsigned int psector = byte / 512;  //섹터 수 구하기
    printf("실질적 블록 개수는 %d 개 입니다.\n", pblock);
    printf("실질적 섹터 개수는 %d 개 입니다.\n", psector);
    for (int i = 0; i < pblock; i++)//블록 수 만큼 반복해서 addblock()으로 블록 추가
    {
        addblock();
    }
    savefile();// 모든 블록, 섹터, pbn, psn을 생성한 것을 "하드웨어.txt"에 쓰기
}

//Node를 추가하는 함수, Block단위로 추가하며, Block이 가르키는 Sector도 생성하여 동적할당을 한다.
void addblock() {
    struct Block* bNode;
    struct Block* newNode;
    bNode = head;
    while (bNode->next)
    {
        bNode = bNode->next; //Block노드 NULL만나기 전까지 진행
    }
    newNode = (struct Block*)malloc(sizeof(struct Block));//새로운 노드 동적할당
    newNode->pbn = bnum;//노드의 pbn값을 선언
    bnum++;// 다음 addblock()함수가 실행되면, 1증가하도록 설정
    for (int i = 0; i < 32; i++)
    {
        newNode->sector[i] = (struct Sector*)malloc(sizeof(struct Sector));//newNode의 sector를 동적 할당
        newNode->sector[i]->psn = snum;//newNode의 sector의 psn을 다음 addblock()함수 실행, for문으로 32번 실행할때마다 psn을 1씩 증가
        snum++;
        strcpy(newNode->sector[i]->data, "\0");
    }
    newNode->next = NULL;//newNode의 다음을 마지막인 NULL로 설정
    bNode->next = newNode;//현재 마지막 노드였던 bNode의 다음을 새로 추가한 newNode로 설정
}

//해당 psn에 있는 data를 읽는 함수
void Flash_read(unsigned int psn) {
    struct Block* bNode = head->next;
    while (bNode)
    {
        for (int i = 0; i < 32; i++)// bNode안의 sector의 32개를 다 둘러봄
        {
            if (bNode->sector[i]->psn == psn) {// sector의 psn과 이용자가 원하는 psn이 동일하면 아래 코드 실행
                printf("psn: %d\ndata: %s\n", bNode->sector[i]->psn, bNode->sector[i]->data);
                return;
            }
        }
        bNode = bNode->next;//sector까지 다 돌고, 다음 노드로 진행
    }
}

//선택한 psn에 data를 입력하는 함수
void Flash_write(unsigned int psn, char* data) {
    struct Block* bNode = head->next;
    while (bNode)
    {
        for (int i = 0; i < 32; i++)
        {
            if (bNode->sector[i]->psn == psn)// sector의 psn과 이용자가 원하는 psn이 동일하면 아래 코드 실행
            {
                printf("psn발견\n");
                if (strlen(bNode->sector[i]->data) > 0) { //해당 bNode의 sector에 데이터가 존재하면 아래 코드 실행
                    printf("해당 섹터에 데이터가 존재합니다.\n");
                    return;
                }
                strcpy(bNode->sector[i]->data, data);//해당 bNode의 sector에 데이터가 존재하지 않으면 아래 코드 실행
                printf("psn: %d\tdata: %s\n", bNode->sector[i]->psn, bNode->sector[i]->data);//변경한 psn과 data를 보여줌
                savefile();// "하드웨어.txt"에 다시 쓰기
                return;
            }
        }
        bNode = bNode->next;//sector까지 다 돌고, 다음 노드로 진행
    }
}
//선택한 psn을 할당해제하여 없애는 함수.
void Flash_erase(unsigned int pbn) {
    struct Block* bNode;
    bNode = head->next;
    while (bNode)
    {
        if (bNode->pbn == pbn) {// 이용자가 지우길 원하는 pbn을 가지고 있는 bNode를 찾으면 아래 코드 실행
            for (int i = 0; i < 32; i++)
            {
                strcpy(bNode->sector[i]->data, "\0");//해당 bNode의 sector전체의 data에 "\0"로 설정
            }
            savefile();// 변경할것을 "하드웨어.txt"에 다시 쓰기
        }
        bNode = bNode->next;
    }

}
//모든 pbn,psn을 보여주는 함수
void show() {
    struct Block* bNode;
    bNode = head->next;
    while (bNode)
    {
        printf("pbn: %d\n", bNode->pbn);
        for (int i = 0; i < 32; i++)
        {
            printf("psn: %d\t", bNode->sector[i]->psn);
            printf("data: %s\t", bNode->sector[i]->data);
        }
        printf("\n");
        bNode = bNode->next;
    }
}
//Block노드에 저장되어있는 모든 데이터를 "하드웨어.txt"에 쓰는 함수.
int savefile() {
    FILE* fp = fopen("하드웨어.txt", "w");//"하드웨어.txt"파일을 쓰는 모드로 오픈
    struct Block* bNode = head->next;//bNode는 head다음부터 시작.
    if (fp == NULL) {//fopen함수는 오류발생시 NULL을 리턴하므로 오류발생시"파일 에러" 프린트
        printf("파일 에러.\n");
        return 1;
    }
    while (bNode)
    {
        for (int i = 0; i < 32; i++)
        {
            fprintf(fp, "%d %s", bNode->sector[i]->psn, bNode->sector[i]->data);
            fputc('\n', fp);
        }
        bNode = bNode->next;
    }
    fclose(fp);
}
