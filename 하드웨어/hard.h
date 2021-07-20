#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int savefile();
void init(int megabyte);
void addblock();
void Flash_read(unsigned int psn);
void Flash_write(unsigned int psn, char* data);
void Flash_erase(unsigned int psn);
void show();

//����ü Sector �迭 32�� ����
struct Sector {
    unsigned int psn;
    char data[508];
}sector[32];
//Sector �迭 32���� ����Ű�� Block ��带 ����
struct Block {
    unsigned int pbn;
    struct Sector* sector[32];
    struct Block* next;
};
struct Block* head;