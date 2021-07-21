#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define _CRT_SECURE_NO_WARNINGS;
int savefile();
void init(int megabyte);
void addsector();
void addLBN();
struct Sector* Flash_read(unsigned int psn);
void Flash_write(unsigned int psn, char* data);
void Flash_erase(unsigned int psn);
void show();

void FTL_Bwrite(unsigned int psn, char* data);
void FTL_Bread(unsigned int psn);
void Btable(unsigned int psn);
int Flash_Blockfind(unsigned int psn);
int Flash_find();
void Flash_switch(unsigned int newpbn, unsigned int erasepbn);
void Flash_copy(unsigned int newpbn, unsigned int erasepbn, int nmg, char* data);
void COUNT();
//����ü Sector �迭 32�� ����
struct Sector {
    unsigned int psn;
    char data[504];
    struct Sector* next;
};
//Sector �迭 32���� ����Ű�� Block ��带 ����
struct Sector* head;
struct LBN {
    int lbn;
    struct LBN* next;
};
struct LBN* lhead;
