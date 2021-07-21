#include "BFTL.h"
#define _CRT_SECURE_NO_WARNINGS;
unsigned int psector = 0;
unsigned int snum = -1;//psn�� ���� ������Ű������ �������� ó�� #define _CRT_SECURE_NO_WARNINGS;
unsigned int rcount = 0;
unsigned int wcount = 0;
unsigned int ecount = 0;
//����ڰ� �Է��� MB������ �Ҵ� �޴´�. MB�� byte�� ȯ���Ͽ� block�� sector�� ������ �ľ��ϰ� array�� �����.
void init(int megabyte) {
	unsigned int byte = 1024 * 1024 * megabyte;// megabyte�� byte�� ȯ��
	unsigned int pblock = byte / 16384; // ���̺�ũ�⸦ �� ��� �� ���ϱ�
	psector = byte / 512;  //���̺�ũ�⸦ �� ���� �� ���ϱ�
	unsigned int tablesize = 2 * 4 * pblock;//tableũ��
	printf("���̺��� ũ��� %d byte�Դϴ�.\n", tablesize);
	printf("������ ��� ������ %d �� �Դϴ�.\n", pblock);
	printf("������ ���� ������ %d �� �Դϴ�.\n", psector);
	int firstB = 0;
	for (int i = 0; i < psector; i++)//��� �� ��ŭ �ݺ��ؼ� addblock()���� ��� �߰� 278,528
	{
		if (i==firstB)//����� ���psn
		{
			addLBN();
			firstB = firstB + 32;//���� ����� ���psn
		}
		else
		{
			addsector();
		}
	}
}
//Node�� �߰��ϴ� �Լ�, Block������ �߰��ϸ�, Block�� ����Ű�� Sector�� �����Ͽ� �����Ҵ��� �Ѵ�.
void addsector() {
	struct Sector* bNode = head;
	struct Sector* newNode;
	while (bNode->next)
	{
		bNode = bNode->next; //Block��� NULL������ ������ ����
	}
	newNode = (struct Sector*)malloc(sizeof(struct Sector));//���ο� ��� �����Ҵ�
	snum++;
	newNode->psn = snum;
	strcpy(newNode->data, "\0");
	newNode->next = NULL;//newNode�� ������ �������� NULL�� ����
	bNode->next = newNode;//���� ������ ��忴�� bNode�� ������ ���� �߰��� newNode�� ����
}
void addLBN() {
	struct Sector* bNode = head;
	struct Sector* newNode;
	while (bNode->next)
	{
		bNode = bNode->next; //Block��� NULL������ ������ ����
	}
	newNode = (struct Sector*)malloc(sizeof(struct Sector));//���ο� ��� �����Ҵ�
	snum++;
	newNode->psn = snum;
	strcpy(newNode->data, "\0");
	Btable(newNode->psn);// addsector()�Լ����� ������. Btable()�� LBN��带 �����Ѵ�.
	newNode->next = NULL;//newNode�� ������ �������� NULL�� ����
	bNode->next = newNode;//���� ������ ��忴�� bNode�� ������ ���� �߰��� newNode�� ����
}

//�ش� psn�� �ִ� data�� �д� �Լ�
struct Sector* Flash_read(unsigned int psn) {
	struct Sector* bNode = head->next;
	while (bNode)
	{
		if (bNode->psn == psn) {// sector�� psn�� �̿��ڰ� ���ϴ� psn�� �����ϸ� �Ʒ� �ڵ� ����
			rcount++;
			return bNode;
		}
		bNode = bNode->next;//sector���� �� ����, ���� ���� ����
	}
}
//������ psn�� data�� �Է��ϴ� �Լ�
void Flash_write(unsigned int psn, char* data) {
	struct Sector* bNode = head->next;
	bNode = Flash_read(psn);
	strcpy(bNode->data, data);
	wcount++;
	return;
}
//������ psn�� �Ҵ������Ͽ� ���ִ� �Լ�.
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
//��� pbn,psn�� �����ִ� �Լ�
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
//Block��忡 ����Ǿ��ִ� ��� �����͸� "�ϵ����.txt"�� ���� �Լ�.
int savefile() {
	FILE* fp = fopen("�ϵ����.txt", "w");//"�ϵ����.txt"������ ���� ���� ����
	struct Sector* bNode = head->next;//bNode�� head�������� ����.
	if (fp == NULL) {//fopen�Լ��� �����߻��� NULL�� �����ϹǷ� �����߻���"���� ����" ����Ʈ
		printf("���� ����.\n");
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
		lNode = lNode->next; //LSN��� NULL������ ������ ����
	}
	struct LBN* newNode = (struct LBN*)malloc(sizeof(struct LBN));
	newNode->lbn = psn;
	newNode->next = NULL;//newNode�� ������ �������� NULL�� ����
	lNode->next = newNode;//���� ������ ��忴�� bNode�� ������ ���� �߰��� newNode�� ����
}
void FTL_Bread(unsigned int psn) {
	struct LBN* lNode = lhead->next;
	struct Sector* bNode = head;
	int lbn = psn / 32;// �Է��� psn�� 32�� ������ ���° ������� �� ������ ����Ѵ�.
	int offset = psn % 32;// lbn���� ����ϰ� ������ �� offset
	for (int i = 0; i < lbn; i++)//lbn��ŭ �������
	{
		lNode = lNode->next;
	}
	bNode = Flash_read((lNode->lbn)+offset);//�� �Է��� psn�� �о �ش� Sector��带 ����
	printf("psn: %d\tdata: %s\n", bNode->psn, bNode->data);//Sector����� psn,data�� ������
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
	if (strlen(bNode->data) > 0)//���� pbn
	{
		int erasepbn = Flash_Blockfind(bNode->psn);//���� ������Ʈ �� Node�� ���psn�� ã�Ƽ� erasepbn�� �־���
		int newpbn = Flash_find();// ��Ͼȿ� data�� �������� �ʴ� ����� ã�Ƽ� ����� ���psn�� newpbn�� �־���.
		Flash_copy(newpbn, erasepbn, offset, data);// newpbn��Ͽ� erasepbn����� �����͸� ��� �����ϰ� �̿��ڰ� data�� ������� �ߴ� offset�� data�� �־���
		Flash_erase(erasepbn);//������Ʈ�ǰ� erasepbn�� ����� �� ����
		Flash_switch(newpbn, erasepbn);// LSN���� newpbn�� ����Ű�� LSN���� erasepbn�� ����Ű�� LSN��带 ���� �ٲ���
		printf("�Ϸ�\n");
	}
	else if (strcmp(bNode->data, "\0") == 0) {
		Flash_write(bNode->psn, data);
	}
}
void Flash_switch(unsigned int newpbn, unsigned int erasepbn) {
	struct LBN* lNode = lhead->next;
	while (lNode)
	{
		if (lNode->lbn==newpbn)//LSN����� lbn�� ���� ���� ����� psn���(newpbn)�� ������ 
		{
			lNode->lbn = erasepbn;//������ �� ����� psn���(erasepbn)�� �ٲ���.
		}
		else if (lNode->lbn==erasepbn)
		{
			lNode->lbn = newpbn;
		}
		lNode = lNode->next;
	}
}
int Flash_Blockfind(unsigned int psn) {
	int bf = 0;//bf�� ����� psn���� ����.
	while (1)
	{
		for (int i = bf; i < bf + 32; i++)//�� ��Ͼ� �ݺ�
		{
			if (i == psn)
			{
				return bf;//����� psn����� bf�� ��ȯ
			}
		}
		bf = bf + 32;
		if (bf == snum)
		{
			printf("pbn�� ã�� ����\n");
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
		for (int i = pbn; i < pbn + 32; i++)//�� ��Ͼ� �ݺ�
		{
			bNode = Flash_read(i);//�� ����� psn�� �ϳ��� �о����
			if (strcmp(bNode->data, "\0") == 0)// �� psn�� �������ִ� ����� �����Ͱ� ���ٸ�,
			{
				cnt++;//cnt�� 1������Ŵ
			}
			if (cnt == 32)//�� ���(32���� ����)�� ��� data�� ���ٸ� cnt�� 32�� �ɰ��� 
			{
				return Flash_Blockfind(i);//data�� ��� ����ִ� ����� ���psn�� �����Ͽ� ��ȯ
			}
		}
		pbn = pbn + 32;//�� ����� �������� cnt�� 32���� ��ä��� ���� ������� ����
	}
}
void Flash_copy(unsigned int newpbn, unsigned int erasepbn, int nmg, char* data) {
	struct Sector* nNode = head;
	struct Sector* eNode = head;
	int MAX_newpbn = newpbn + 32;//���� ������� ����������
	int MAX_erasepbn = erasepbn + 32;//���� ������� ����������
	int order = newpbn + nmg;// newpbn�� �����͸� ������ �̿��ڰ� ���ߴ� data��ġ�� �������� �ִ� ����

	while (1)
	{
		if ((newpbn == MAX_newpbn) && (erasepbn == MAX_erasepbn))//�� ��ϴ� ������ϱ��� ���� �Լ�����
		{
			return;
		}
		eNode = Flash_read(erasepbn);//���� ����� data�� ������ �ִ� Sector��带 ����
		if (newpbn == order)//�̿��ڰ� ���ߴ� ��ġ�� �����ϸ�
		{
			Flash_write(newpbn, data);//�̿��ڰ� ���ߴ� data�� �ش� ��ġ�� ����
			newpbn++;//���� ���� ����� ���� ���ͷ� ����
			erasepbn++;//���� ����� ���� ���ͷ� ����
		}
		else
		{
			Flash_write(newpbn, eNode->data);//���� ���� ��Ͽ� ���� ����� data�� ����
			newpbn++;
			erasepbn++;
		}
	}
}
void COUNT() {
	printf("read:%d��\twrite:%d��\terase:%d��\n", rcount, wcount, ecount);
	rcount = 0;
	wcount = 0;
	ecount = 0;
}