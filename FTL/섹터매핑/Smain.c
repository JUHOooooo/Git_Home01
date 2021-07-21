#include "SFTL.h"
#define _CRT_SECURE_NO_WARNINGS;
int main(void) {
    int megabyte;
    int select;
    unsigned int lsn;
    unsigned int lbn;
    char data[504];

    head = (struct Sector*)malloc(sizeof(struct Sector));
    head->psn = NULL;
    strcpy(head->data, "\0");
    head->next = NULL;
    lhead = (struct LSN*)malloc(sizeof(struct LSN));
    lhead->lsn = NULL;
    lhead->next = NULL;
    FILE* fp = fopen("하드웨어.txt", "at");//파일 생성
    fclose(fp);
    printf("몇 megabyte를 할당 하시겠습니까?\n");
    scanf("%d", &megabyte);
    init(megabyte);
    savefile();
    while (1)
    {

        printf("기능을 선택하세요\n1:Sector read\t2:Sector write\t3:show\t4:종료\n");
        scanf("%d", &select);
        while (getchar() != '\n');   // char형이 입력되는 것에 대한 예외처리문
        switch (select)
        {
        case 1:
            printf("읽어올 lsn을 입력해주세요.");
            scanf("%d", &lsn);
            while (getchar() != '\n');   // char형이 입력되는 것에 대한 예외처리문
            FTL_Sread(lsn);
            COUNT();// 읽고 쓰고 지운 카운트를 보여줌
            savefile();// 변경할것을 "하드웨어.txt"에 다시 쓰기
            break;
        case 2:
            printf("lsn을 입력해주세요.");
            scanf("%d", &lsn);
            while (getchar() != '\n');   // char형이 입력되는 것에 대한 예외처리문
            printf("입력할 data: ");
            scanf("%s", data);
            FTL_Swrite(lsn, data);
            COUNT();// 읽고 쓰고 지운 카운트를 보여줌
            savefile();// 변경할것을 "하드웨어.txt"에 다시 쓰기
            break;
        case 3:
            show();
            break;
        case 4:
            printf("사용종료");
            savefile();
            return 0;
        default:
            break;

        }
    }
}