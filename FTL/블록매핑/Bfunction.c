#include "BFTL.h"
#define _CRT_SECURE_NO_WARNINGS;
unsigned int psector = 0;
unsigned int snum = -1;//psn을 점점 증가시키기위해 전역변수 처리 #define _CRT_SECURE_NO_WARNINGS;
unsigned int rcount = 0;
unsigned int wcount = 0;
unsigned int ecount = 0;
//사용자가 입력한 MB단위로 할당 받는다. MB를 byte로 환산하여 block과 sector의 개수를 파악하고 array로 만든다.
void init(int megabyte) {
	unsigned int byte = 1024 * 1024 * megabyte;// megabyte를 byte로 환산
	unsigned int pblock = byte / 16384; // 테이블크기를 뺀 블록 수 구하기
	psector = byte / 512;  //테이블크기를 뺀 섹터 수 구하기
	unsigned int tablesize = 2 * 4 * pblock;//table크기
	printf("테이블의 크기는 %d byte입니다.\n", tablesize);
	printf("실질적 블록 개수는 %d 개 입니다.\n", pblock);
	printf("실질적 섹터 개수는 %d 개 입니다.\n", psector);
	int firstB = 0;
	for (int i = 0; i < psector; i++)//블록 수 만큼 반복해서 addblock()으로 블록 추가 278,528
	{
		if (i==firstB)//블록의 헤드psn
		{
			addLBN();
			firstB = firstB + 32;//다음 블록의 헤드psn
		}
		else
		{
			addsector();
		}
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
	newNode->next = NULL;//newNode의 다음을 마지막인 NULL로 설정
	bNode->next = newNode;//현재 마지막 노드였던 bNode의 다음을 새로 추가한 newNode로 설정
}
void addLBN() {
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
	Btable(newNode->psn);// addsector()함수와의 차이점. Btable()로 LBN노드를 생성한다.
	newNode->next = NULL;//newNode의 다음을 마지막인 NULL로 설정
	bNode->next = newNode;//현재 마지막 노드였던 bNode의 다음을 새로 추가한 newNode로 설정
}

//해당 psn에 있는 data를 읽는 함수
struct Sector* Flash_read(unsigned int psn) {
	struct Sector* bNode = head->next;
	while (bNode)
	{
		if (bNode->psn == psn) {// sector의 psn과 이용자가 원하는 psn이 동일하면 아래 코드 실행
			rcount++;
			return bNode;
		}
		bNode = bNode->next;//sector까지 다 돌고, 다음 노드로 진행
	}
}
//선택한 psn에 data를 입력하는 함수
void Flash_write(unsigned int psn, char* data) {
	struct Sector* bNode = head->next;
	bNode = Flash_read(psn);
	strcpy(bNode->data, data);
	wcount++;
	return;
}
//선택한 psn을 할당해제하여 없애는 함수.
void Flash_erase(unsigned int psn) {
	ecount++;
	struct Sector* bNode = head->next;
	for (int i = psn; i < psn + 32; i++)
	{
		bNode = head->next;
		while (bNode) {
			if (bNode->psn == i)
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
void Btable(unsigned int psn) {
	struct LBN* lNode = lhead;
	while (lNode->next)
	{
		lNode = lNode->next; //LSN노드 NULL만나기 전까지 진행
	}
	struct LBN* newNode = (struct LBN*)malloc(sizeof(struct LBN));
	newNode->lbn = psn;
	newNode->next = NULL;//newNode의 다음을 마지막인 NULL로 설정
	lNode->next = newNode;//현재 마지막 노드였던 bNode의 다음을 새로 추가한 newNode로 설정
}
void FTL_Bread(unsigned int psn) {
	struct LBN* lNode = lhead->next;
	struct Sector* bNode = head;
	int lbn = psn / 32;// 입력한 psn을 32로 나눠서 몇번째 블록으로 갈 것인지 계산한다.
	int offset = psn % 32;// lbn으로 계산하고 나머지 즉 offset
	for (int i = 0; i < lbn; i++)//lbn만큼 노드진행
	{
		lNode = lNode->next;
	}
	bNode = Flash_read((lNode->lbn)+offset);//즉 입력한 psn을 읽어서 해당 Sector노드를 추출
	printf("psn: %d\tdata: %s\n", bNode->psn, bNode->data);//Sector노드의 psn,data를 보여줌
}

void FTL_Bwrite(unsigned int psn,char* data) {
	struct LBN* lNode = lhead->next;
	struct Sector* bNode = head->next;
	int lbn = psn / 32;
	int offset = psn % 32;
	for (int i = 0; i < lbn; i++)
	{
		lNode = lNode->next;
	}
	bNode = Flash_read((lNode->lbn)+offset);
	if (strlen(bNode->data) > 0)//지울 pbn
	{
		int erasepbn = Flash_Blockfind(bNode->psn);//지금 업데이트 할 Node의 헤드psn을 찾아서 erasepbn에 넣어줌
		int newpbn = Flash_find();// 블록안에 data가 존재하지 않는 블록을 찾아서 블록의 헤드psn을 newpbn에 넣어줌.
		Flash_copy(newpbn, erasepbn, offset, data);// newpbn블록에 erasepbn블록의 데이터를 모두 복사하고 이용자가 data를 넣을라고 했던 offset에 data를 넣어줌
		Flash_erase(erasepbn);//업데이트되고 erasepbn의 블록을 다 지움
		Flash_switch(newpbn, erasepbn);// LSN에서 newpbn을 가르키던 LSN노드와 erasepbn을 가르키던 LSN노드를 서로 바꿔줌
		printf("완료\n");
	}
	else if (strcmp(bNode->data, "\0") == 0) {
		Flash_write(bNode->psn, data);
	}
}
void Flash_switch(unsigned int newpbn, unsigned int erasepbn) {
	struct LBN* lNode = lhead->next;
	while (lNode)
	{
		if (lNode->lbn==newpbn)//LSN노드의 lbn이 새로 생긴 블록의 psn헤드(newpbn)와 같으면 
		{
			lNode->lbn = erasepbn;//지워야 할 블록의 psn헤드(erasepbn)로 바꿔줌.
		}
		else if (lNode->lbn==erasepbn)
		{
			lNode->lbn = newpbn;
		}
		lNode = lNode->next;
	}
}
int Flash_Blockfind(unsigned int psn) {
	int bf = 0;//bf는 블록의 psn헤드와 같다.
	while (1)
	{
		for (int i = bf; i < bf + 32; i++)//한 블록씩 반복
		{
			if (i == psn)
			{
				return bf;//블록의 psn헤드인 bf를 반환
			}
		}
		bf = bf + 32;
		if (bf == snum)
		{
			printf("pbn을 찾지 못함\n");
			return;
		}
	}
}
int Flash_find() {
	struct Sector* bNode = head->next;
	int pbn = 0;
	while (1)
	{
		int cnt = 0;
		for (int i = pbn; i < pbn + 32; i++)//한 블록씩 반복
		{
			bNode = Flash_read(i);//한 블록의 psn을 하나씩 읽어듦임
			if (strcmp(bNode->data, "\0") == 0)// 그 psn을 가지고있는 노드의 데이터가 없다면,
			{
				cnt++;//cnt를 1증가시킴
			}
			if (cnt == 32)//한 블록(32개의 섹터)이 모두 data가 없다면 cnt는 32가 될거임 
			{
				return Flash_Blockfind(i);//data가 모두 비어있던 블록의 헤드psn을 추출하여 반환
			}
		}
		pbn = pbn + 32;//한 블록을 돌았지만 cnt를 32까지 못채우면 다음 블록으로 진행
	}
}
void Flash_copy(unsigned int newpbn, unsigned int erasepbn, int nmg, char* data) {
	struct Sector* nNode = head;
	struct Sector* eNode = head;
	int MAX_newpbn = newpbn + 32;//다음 블록으로 가기전까지
	int MAX_erasepbn = erasepbn + 32;//다음 블록으로 가기전까지
	int order = newpbn + nmg;// newpbn에 데이터를 적을때 이용자가 원했던 data위치에 적기위해 있는 변수

	while (1)
	{
		if ((newpbn == MAX_newpbn) && (erasepbn == MAX_erasepbn))//두 블록다 다음블록까지 돌면 함수종료
		{
			return;
		}
		eNode = Flash_read(erasepbn);//지울 블록의 data를 가지고 있는 Sector노드를 추출
		if (newpbn == order)//이용자가 원했던 위치에 도달하면
		{
			Flash_write(newpbn, data);//이용자가 원했던 data를 해당 위치에 적음
			newpbn++;//새로 적을 블록의 다음 섹터로 진행
			erasepbn++;//지울 블록의 다음 섹터로 진행
		}
		else
		{
			Flash_write(newpbn, eNode->data);//새로 적을 블록에 지울 블록의 data를 복사
			newpbn++;
			erasepbn++;
		}
	}
}
void COUNT() {
	printf("read:%d번\twrite:%d번\terase:%d번\n", rcount, wcount, ecount);
	rcount = 0;
	wcount = 0;
	ecount = 0;
}