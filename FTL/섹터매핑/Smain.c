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
            printf("�о�� lsn�� �Է����ּ���.");
            scanf("%d", &lsn);
            while (getchar() != '\n');   // char���� �ԷµǴ� �Ϳ� ���� ����ó����
            FTL_Sread(lsn);
            COUNT();// �а� ���� ���� ī��Ʈ�� ������
            savefile();// �����Ұ��� "�ϵ����.txt"�� �ٽ� ����
            break;
        case 2:
            printf("lsn�� �Է����ּ���.");
            scanf("%d", &lsn);
            while (getchar() != '\n');   // char���� �ԷµǴ� �Ϳ� ���� ����ó����
            printf("�Է��� data: ");
            scanf("%s", data);
            FTL_Swrite(lsn, data);
            COUNT();// �а� ���� ���� ī��Ʈ�� ������
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