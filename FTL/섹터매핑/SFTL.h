#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define _CRT_SECURE_NO_WARNINGS;
int savefile();
void init(int megabyte);
void addsector();
struct Sector* Flash_read(unsigned int psn);
void Flash_write(unsigned int psn, char* data);
void Flash_erase(unsigned int psn);
void show();
void switchsector(unsigned int findpsn, unsigned int usedpsn);
struct Sector* findsector(char* data);
void FTL_Swrite(unsigned int lsn, char* data);
void FTL_Sread(unsigned int lsn);
void Stable(unsigned int psn);
void COUNT();

struct Sector {
    unsigned int psn;
    char data[504];
    struct Sector* next;
};
struct Sector* head;
struct LSN {
    int lsn;
    struct LSN* next;
};
struct LSN* lhead;
