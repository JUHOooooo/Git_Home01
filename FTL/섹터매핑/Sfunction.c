#include "SFTL.h"
#define _CRT_SECURE_NO_WARNINGS;
unsigned int psector = 0;//findsector���� ���̱⶧���� �������� ó��
unsigned int snum = -1;//psn�� ���� ������Ű������ �������� ó��
unsigned int rcount = 0;//�д� �Լ� ����Ƚ��
unsigned int wcount = 0;//���� �Լ� ����Ƚ��
unsigned int ecount = 0;//����� �Լ� ����Ƚ��
//����ڰ� �Է��� MB������ �Ҵ� �޴´�. MB�� byte�� ȯ���Ͽ� block�� sector�� ������ �ľ��ϰ� array�� �����.
void init(int megabyte) {
	unsigned int byte = 1024 * 1024 * megabyte;// megabyte�� byte�� ȯ��
	unsigned int pblock = byte / 16384; // ���̺�ũ�⸦ �� ��� �� ���ϱ�
	psector = byte / 512;  //���̺�ũ�⸦ �� ���� �� ���ϱ�
	unsigned int tablesize = 2 * 4 * pblock * 32;//tableũ��
	printf("���̺��� ũ��� %d byte�Դϴ�.\n", tablesize);
	printf("������ ��� ������ %d �� �Դϴ�.\n", pblock);
	printf("������ ���� ������ %d �� �Դϴ�.\n", psector);
	for (int i = 0; i < psector; i++)//��� �� ��ŭ �ݺ��ؼ� addblock()���� ��� �߰� 278,528
	{
		addsector();
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
	Stable(newNode->psn);
	newNode->next = NULL;//newNode�� ������ �������� NULL�� ����
	bNode->next = newNode;//���� ������ ��忴�� bNode�� ������ ���� �߰��� newNode�� ����
}

//�ش� psn�� �ִ� data�� �д� �Լ�
struct Sector* Flash_read(unsigned int psn) {
	struct Sector* bNode = head->next;
	while (bNode)
	{
		if (bNode->psn == psn) {// sector�� psn�� �̿��ڰ� ���ϴ� psn�� �����ϸ� �Ʒ� �ڵ� ����
			rcount++;//�б� ī��Ʈ 1����
			return bNode;// Sector��带 ��ȯ
		}
		bNode = bNode->next;//sector���� �� ����, ���� ���� ����
	}
}
//������ psn�� data�� �Է��ϴ� �Լ�
void Flash_write(unsigned int psn, char* data) {
	struct Sector* bNode = head->next;
	bNode = Flash_read(psn);//�ش� psn�� ��带 ����
	strcpy(bNode->data, data);//������ Node�� data ����
	wcount++;//�б� ī��Ʈ 1����
	return;
}
//������ psn�� �Ҵ������Ͽ� ���ִ� �Լ�.
void Flash_erase(unsigned int pbn) {
	ecount++;//����� ī��Ʈ ����
	struct Sector* bNode = head->next;
	for (int i = pbn; i < pbn + 32; i++)//pbn�� 0,32,64,....
	{
		bNode = head->next;//bNode�� ó������ �ٽ�
		while (bNode) {
			if (bNode->psn == i)//bNode�� ������ i�� �߰��ϸ� "\0"����
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
void Stable(unsigned int psn) {
	struct LSN* lNode = lhead;
	while (lNode->next)
	{
		lNode = lNode->next; //LSN��� NULL������ ������ ����
	}
	struct LSN* newNode = (struct LSN*)malloc(sizeof(struct LSN));
	newNode->lsn = psn;
	newNode->next = NULL;//newNode�� ������ �������� NULL�� ����
	lNode->next = newNode;//���� ������ ��忴�� bNode�� ������ ���� �߰��� newNode�� ����
}
void FTL_Sread(unsigned int lsn) {
	struct LSN* lNode = lhead->next;
	struct Sector* bNode = head;
	for (int i = 0; i < lsn; i++)
	{
		lNode = lNode->next;//�̿��ڰ� ���ϴ� lsn��ŭ ����
	}
	bNode = Flash_read(lNode->lsn);//�̿��ڰ� ���Ѵ� LSN����� lsn�� �о bNode�� ���� ����
	printf("psn: %d\tdata: %s\n", bNode->psn, bNode->data);//�ش� bNode�� psn,data�� ������
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
	if (strlen(bNode->data) > 0)// �ش� lsn�� ����Ű�� bNode�� data�� �����Ѵٸ�
	{
		fNode = findsector(data);//data�� �� ���͸� ã�� �̿��ڰ� ���ϴ� data�� �����ϰ� fNode�� ����
		switchsector(fNode->psn, bNode->psn);//������ psn,data�� �����ϴ� ������ psn�� ����Ű�� LSN�� lsn�� �ٲ���
		printf("�Ϸ�\n");
	}
	else if (strcmp(bNode->data, "\0") == 0) {//�ش� lsn�� ����Ű�� bNode�� data�� �������� �ʴ´ٸ�,
		Flash_write(bNode->psn, data);//�ش� ���Ϳ� data����
	}
}
struct Sector* findsector(char* data) {
	struct Sector* bNode = head->next;
	for (int i = 0; i < psector; i++)//i=0���� ���� ������psn����
	{
		bNode = Flash_read(i);//i�� psn�� ����־� bNode�� ����
		if (strcmp(bNode->data, "\0") == 0)//�� bNode�� data�� ���ٸ� �Ʒ� �ڵ� ����
		{
			Flash_write(bNode->psn, data);//�� data�� �� bNode�� data����
			return bNode;//bNode ��ȯ
		}
	}
}
void switchsector(unsigned int findpsn, unsigned int usedpsn) {//������ psn, data�� ������ ������ psn
	struct LSN* lNode = lhead->next;
	while (lNode)
	{
		if (lNode->lsn == findpsn)//lNode�� lsn�� �� ������ psn�� ���ٸ� 
		{
			lNode->lsn = usedpsn;//lNode�� lsn�� data�� ������ ������ psn���� �ٲ���
		}
		else if (lNode->lsn == usedpsn)//lNode�� lsn�� data�� ������ ������ psn�̶� ���ٸ�,
		{
			lNode->lsn = findpsn;//lNode�� lsn�� �� ������ psn���� �ٲ���
		}
		lNode = lNode->next;
	}
}
void COUNT() {
	printf("read:%d��\twrite:%d��\terase:%d��\n", rcount, wcount, ecount);
	rcount = 0;
	wcount = 0;
	ecount = 0;
}