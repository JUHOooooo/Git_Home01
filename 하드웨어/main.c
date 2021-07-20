#include "hard.h"

int main(void) {
    int megabyte;
    int select;
    unsigned int psn;
    char data[508];
    head = (struct Block*)malloc(sizeof(struct Block));
    head->pbn = NULL;
    for (int i = 0; i < 32; i++)
    {
        head->sector[i] = (struct Sector*)malloc(sizeof(struct Sector));
        head->sector[i]->psn = NULL;
        strcpy(head->sector[i]->data, "\0");
    }
    head->next = NULL;
    FILE *fp = fopen("하드웨어.txt", "at");//파일 생성
    fclose(fp);
    printf("몇 megabyte를 할당 하시겠습니까?\n");
    scanf("%d", &megabyte);
    init(megabyte);

    while (1)
    {
        printf("기능을 선택하세요\n1:read\t2:write\t3:erase\t4:show\t5:종료\n");
        scanf("%d", &select);
        while (getchar() != '\n');   // char형이 입력되는 것에 대한 예외처리문
        switch (select)
        {
        case 1:
            printf("읽어올 psn을 입력해주세요.");
            scanf("%d", &psn);
            while (getchar() != '\n');   // char형이 입력되는 것에 대한 예외처리문
            Flash_read(psn);
            break;
        case 2:
            printf("psn을 입력해주세요.");
            scanf("%d", &psn);
            while (getchar() != '\n');   // char형이 입력되는 것에 대한 예외처리문
            printf("입력할 data: ");
            scanf("%s", data);
            Flash_write(psn, data);
            break;
        case 3:
            printf("지울 pbn을 입력하세요.");
            scanf("%d", &psn);
            while (getchar() != '\n');   // char형이 입력되는 것에 대한 예외처리문
            Flash_erase(psn);
            break;
        case 4:
            show();
            break;
        case 5:
            printf("사용종료");
            savefile();
            return 0;
        default:
            break;

        }
    }
}