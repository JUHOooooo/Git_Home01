//Sleep�Լ��� ���� ���
#include "Node.h"

int main() {
    char ID[20];
    char video[20];
    head = (struct Node*)malloc(sizeof(struct Node));//Node�� ù ��带 ����
    vhead = (struct Video*)malloc(sizeof(struct Video));//Video�� ù ��带 ����
    strcpy(vhead->video, "\0");//head���� ��� \0�� ����
    strcpy(head->ID, "\0");
    strcpy(head->video, "\0");
    head->next = NULL;//head��� ������ NULL
    vhead->next = NULL;//vhead��� ������ NULL
    openfile(); //������ ���� ����
    openfileV();

    while (1)
    {
        printf("��� ����� �̿��Ͻðڽ��ϱ�\n");
        printf("1.�˻� 2.�뿩 3.�԰� 4.�԰�ǰ ���� 5 �������߰� 6.��ü��� 7.���α׷� ����\n");
        int select;
        scanf("%d", &select);
        while (getchar() != '\n');   // ����ó����
        switch (select)
        {
        case 1:
            Search();
            break;
        case 2:
            Rental();
            break;
        case 3:
            Receiving();
            break;
        case 4:
            printf("���� �� ������ �̸��� �Է����ּ���:\n");
            scanf("%s", &video);//Rental()���� Del�� �����ϹǷ� ���̽����� video�� ����
            Del(video);
            break;
        case 5:
            AddID();
            break;
        case 6:
            Rentalshow();
            break;
        case 7:
            savefile();
            savefileV();
            printf("End\n");
            return 0;
        default://����ó����
            printf("���ö��� ���� �ʽ��ϴ�.\n");
        }
    }
    return 0;
}