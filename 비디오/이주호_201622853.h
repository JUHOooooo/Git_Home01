#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>//Sleep함수를 위한 헤더


struct Node {
    char ID[20];
    char video[20];
    struct Node* next;
};

struct Video {
    char video[20];
    struct Video* next;
};

struct Node* head;
struct Video* vhead;
int openfile();
int openfileV();
int savefile();
int savefileV();
void Rentalshow();
void AddID();
void Addvideo(char* video);
void Receiving();
void Receivingshow();
void Rental();
void Rentalsub(struct Node* hNode, char* ID, char* video);
void Search();
void Del(char* video);