#include "SFTL.h"
#define _CRT_SECURE_NO_WARNINGS;
unsigned int psector = 0;//findsector에서 쓰이기때문에 전역변수 처리
unsigned int snum = -1;//psn을 점점 증가시키기위해 전역변수 처리
unsigned int rcount = 0;//읽는 함수 실행횟수
unsigned int wcount = 0;//쓰는 함수 실행횟수
unsigned int ecount = 0;//지우는 함수 실행횟수
//사용자가 입력한 MB단위로 할당 받는다. MB를 byte로 환산하여 block과 sector의 개수를 파악하고 array로 만든다.
void init(int megabyte) {
	unsigned int byte = 1024 * 1024 * megabyte;// megabyte를 byte로 환산
	unsigned int pblock = byte / 16384; // 테이블크기를 뺀 블록 수 구하기
	psector = byte / 512;  //테이블크기를 뺀 섹터 수 구하기
	unsigned int tablesize = 2 * 4 * pblock * 32;//table크기
	printf("테이블의 크기는 %d byte입니다.\n", tablesize);
	printf("실질적 블록 개수는 %d 개 입니다.\n", pblock);
	printf("실질적 섹터 개수는 %d 개 입니다.\n", psector);
	for (int i = 0; i < psector; i++)//블록 수 만큼 반복해서 addblock()으로 블록 추가 278,528
	{
		addsector();
	}
}
//Node를 추가하는 함수, Block단위로 추가하며, Block이 가르키는 Sector도 생성하여 동적할당을 한다.
void addsector() {
	struct Sector* bNode = head;
	struct Sector* newNode;
	while (bNode->next)
	{
		bNode = bNode->next; //Block노드 NULL만나기 전까지 진행
	}
	newNode = (struct Sector*)malloc(sizeof(struct Sector));//새로운 노드 동적할당
	snum++;
	newNode->psn = snum;
	strcpy(newNode->data, "\0");
	Stable(newNode->psn);
	newNode->next = NULL;//newNode의 다음을 마지막인 NULL로 설정
	bNode->next = newNode;//현재 마지막 노드였던 bNode의 다음을 새로 추가한 newNode로 설정
}

//해당 psn에 있는 data를 읽는 함수
struct Sector* Flash_read(unsigned int psn) {
	struct Sector* bNode = head->next;
	while (bNode)
	{
		if (bNode->psn == psn) {// sector의 psn과 이용자가 원하는 psn이 동일하면 아래 코드 실행
			rcount++;//읽기 카운트 1증가
			return bNode;// Sector노드를 반환
		}
		bNode = bNode->next;//sector까지 다 돌고, 다음 노드로 진행
	}
}
//선택한 psn에 data를 입력하는 함수
void Flash_write(unsigned int psn, char* data) {
	struct Sector* bNode = head->next;
	bNode = Flash_read(psn);//해당 psn의 노드를 추출
	strcpy(bNode->data, data);//추출한 Node에 data 삽입
	wcount++;//읽기 카운트 1증가
	return;
}
//선택한 psn을 할당해제하여 없애는 함수.
void Flash_erase(unsigned int pbn) {
	ecount++;//지우기 카운트 증가
	struct Sector* bNode = head->next;
	for (int i = pbn; i < pbn + 32; i++)//pbn은 0,32,64,....
	{
		bNode = head->next;//bNode를 처음부터 다시
		while (bNode) {
			if (bNode->psn == i)//bNode를 돌리며 i를 발견하면 "\0"삽입
			{
				strcpy(bNode->data, "\0");
			}
			bNode = bNode->next;
		}
	}

}
//모든 pbn,psn을 보여주는 함수
void show() {
	struct Sector* bNode;
	bNode = head->next;
	while (bNode)
	{
		printf("psn: %d\t", bNode->psn);
		printf("data: %s\t", bNode->data);
		printf("\n");
		bNode = bNode->next;
	}
}
//Block노드에 저장되어있는 모든 데이터를 "하드웨어.txt"에 쓰는 함수.
int savefile() {
	FILE* fp = fopen("하드웨어.txt", "w");//"하드웨어.txt"파일을 쓰는 모드로 오픈
	struct Sector* bNode = head->next;//bNode는 head다음부터 시작.
	if (fp == NULL) {//fopen함수는 오류발생시 NULL을 리턴하므로 오류발생시"파일 에러" 프린트
		printf("파일 에러.\n");
		return 1;
	}
	while (bNode)
	{
		fprintf(fp, "%d %s", bNode->psn, bNode->data);
		fputc('\n', fp);
		bNode = bNode->next;
	}
	fclose(fp);
}
void Stable(unsigned int psn) {
	struct LSN* lNode = lhead;
	while (lNode->next)
	{
		lNode = lNode->next; //LSN노드 NULL만나기 전까지 진행
	}
	struct LSN* newNode = (struct LSN*)malloc(sizeof(struct LSN));
	newNode->lsn = psn;
	newNode->next = NULL;//newNode의 다음을 마지막인 NULL로 설정
	lNode->next = newNode;//현재 마지막 노드였던 bNode의 다음을 새로 추가한 newNode로 설정
}
void FTL_Sread(unsigned int lsn) {
	struct LSN* lNode = lhead->next;
	struct Sector* bNode = head;
	for (int i = 0; i < lsn; i++)
	{
		lNode = lNode->next;//이용자가 원하는 lsn만큼 진행
	}
	bNode = Flash_read(lNode->lsn);//이용자가 원한는 LSN노드의 lsn을 읽어서 bNode에 집어 넣음
	printf("psn: %d\tdata: %s\n", bNode->psn, bNode->data);//해당 bNode의 psn,data를 보여줌
}
void FTL_Swrite(unsigned int lsn, char* data) {
	struct LSN* lNode = lhead->next;
	struct Sector* bNode = head->next;
	struct Sector* fNode = head->next;
	for (int i = 0; i < lsn; i++)
	{
		lNode = lNode->next;
	}
	bNode = Flash_read(lNode->lsn);
	if (strlen(bNode->data) > 0)// 해당 lsn이 가르키는 bNode의 data가 존재한다면
	{
		fNode = findsector(data);//data가 빈 섹터를 찾아 이용자가 원하는 data를 삽입하고 fNode에 삽입
		switchsector(fNode->psn, bNode->psn);//빈섹터의 psn,data가 존재하던 섹터의 psn을 가르키던 LSN의 lsn을 바꿔줌
		printf("완료\n");
	}
	else if (strcmp(bNode->data, "\0") == 0) {//해당 lsn이 가르키는 bNode의 data가 존재하지 않는다면,
		Flash_write(bNode->psn, data);//해당 섹터에 data삽입
	}
}
struct Sector* findsector(char* data) {
	struct Sector* bNode = head->next;
	for (int i = 0; i < psector; i++)//i=0부터 섹터 마지막psn까지
	{
		bNode = Flash_read(i);//i인 psn을 집어넣어 bNode를 추출
		if (strcmp(bNode->data, "\0") == 0)//그 bNode의 data가 없다면 아래 코드 실행
		{
			Flash_write(bNode->psn, data);//그 data가 빈 bNode에 data삽입
			return bNode;//bNode 반환
		}
	}
}
void switchsector(unsigned int findpsn, unsigned int usedpsn) {//빈섹터의 psn, data가 존재한 섹터의 psn
	struct LSN* lNode = lhead->next;
	while (lNode)
	{
		if (lNode->lsn == findpsn)//lNode의 lsn이 빈 섹터의 psn과 같다면 
		{
			lNode->lsn = usedpsn;//lNode의 lsn을 data가 존재한 섹터의 psn으로 바꿔줌
		}
		else if (lNode->lsn == usedpsn)//lNode의 lsn을 data가 존재한 섹터의 psn이랑 같다면,
		{
			lNode->lsn = findpsn;//lNode의 lsn이 빈 섹터의 psn으로 바꿔줌
		}
		lNode = lNode->next;
	}
}
void COUNT() {
	printf("read:%d번\twrite:%d번\terase:%d번\n", rcount, wcount, ecount);
	rcount = 0;
	wcount = 0;
	ecount = 0;
}