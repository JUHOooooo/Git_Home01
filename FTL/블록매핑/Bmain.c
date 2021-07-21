#include "BFTL.h"
#define _CRT_SECURE_NO_WARNINGS;
int main(void) {
    int megabyte = 0;
    int select = 0;
    unsigned int psn=0;
    unsigned int lbn=0;
    char data[504];
    int bs = 0;
    head = (struct Sector*)malloc(sizeof(struct Sector));
    head->psn = NULL;
    strcpy(head->data, "\0");
    head->next = NULL;
    lhead = (struct LBN*)malloc(sizeof(struct LBN));
    lhead->lbn = NULL;
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
            printf("읽어올 psn을 입력해주세요.");
            scanf("%d", &psn);
            while (getchar() != '\n');   // char형이 입력되는 것에 대한 예외처리문
            FTL_Bread(psn);
            COUNT();
            savefile();// 변경할것을 "하드웨어.txt"에 다시 쓰기
            break;
        case 2:
            printf("psn을 입력해주세요.\n");
            scanf("%d", &psn);
            while (getchar() != '\n');
            printf("data를 입력해주세요.\n");
            scanf("%s", data);
            FTL_Bwrite(psn, data);
            COUNT();
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