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
    FILE *fp = fopen("�ϵ����.txt", "at");//���� ����
    fclose(fp);
    printf("�� megabyte�� �Ҵ� �Ͻðڽ��ϱ�?\n");
    scanf("%d", &megabyte);
    init(megabyte);

    while (1)
    {
        printf("����� �����ϼ���\n1:read\t2:write\t3:erase\t4:show\t5:����\n");
        scanf("%d", &select);
        while (getchar() != '\n');   // char���� �ԷµǴ� �Ϳ� ���� ����ó����
        switch (select)
        {
        case 1:
            printf("�о�� psn�� �Է����ּ���.");
            scanf("%d", &psn);
            while (getchar() != '\n');   // char���� �ԷµǴ� �Ϳ� ���� ����ó����
            Flash_read(psn);
            break;
        case 2:
            printf("psn�� �Է����ּ���.");
            scanf("%d", &psn);
            while (getchar() != '\n');   // char���� �ԷµǴ� �Ϳ� ���� ����ó����
            printf("�Է��� data: ");
            scanf("%s", data);
            Flash_write(psn, data);
            break;
        case 3:
            printf("���� pbn�� �Է��ϼ���.");
            scanf("%d", &psn);
            while (getchar() != '\n');   // char���� �ԷµǴ� �Ϳ� ���� ����ó����
            Flash_erase(psn);
            break;
        case 4:
            show();
            break;
        case 5:
            printf("�������");
            savefile();
            return 0;
        default:
            break;

        }
    }
}