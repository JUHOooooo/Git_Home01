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
    FILE* fp = fopen("�ϵ����.txt", "at");//���� ����
    fclose(fp);
    printf("�� megabyte�� �Ҵ� �Ͻðڽ��ϱ�?\n");
    scanf("%d", &megabyte);
    init(megabyte);
    savefile();
    while (1)
    {

        printf("����� �����ϼ���\n1:Sector read\t2:Sector write\t3:show\t4:����\n");
        scanf("%d", &select);
        while (getchar() != '\n');   // char���� �ԷµǴ� �Ϳ� ���� ����ó����
        switch (select)
        {
        case 1:
            printf("�о�� psn�� �Է����ּ���.");
            scanf("%d", &psn);
            while (getchar() != '\n');   // char���� �ԷµǴ� �Ϳ� ���� ����ó����
            FTL_Bread(psn);
            COUNT();
            savefile();// �����Ұ��� "�ϵ����.txt"�� �ٽ� ����
            break;
        case 2:
            printf("psn�� �Է����ּ���.\n");
            scanf("%d", &psn);
            while (getchar() != '\n');
            printf("data�� �Է����ּ���.\n");
            scanf("%s", data);
            FTL_Bwrite(psn, data);
            COUNT();
            savefile();// �����Ұ��� "�ϵ����.txt"�� �ٽ� ����
            break;
        case 3:
            show();
            break;
        case 4:
            printf("�������");
            savefile();

            return 0;
        default:
            break;

        }
    }
}