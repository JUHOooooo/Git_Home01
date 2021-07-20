#include "hard.h"
unsigned int bnum = 0;//pbn�� ���� ������Ű������ �������� ó��
unsigned int snum = 0;//psn�� ���� ������Ű������ �������� ó��
//����ڰ� �Է��� MB������ �Ҵ� �޴´�. MB�� byte�� ȯ���Ͽ� block�� sector�� ������ �ľ��ϰ� array�� �����.
void init(int megabyte) {
    unsigned int byte = 1024 * 1024 * megabyte;// megabyte�� byte�� ȯ��
    unsigned int pblock = byte / 16384; //��� �� ���ϱ�
    unsigned int psector = byte / 512;  //���� �� ���ϱ�
    printf("������ ��� ������ %d �� �Դϴ�.\n", pblock);
    printf("������ ���� ������ %d �� �Դϴ�.\n", psector);
    for (int i = 0; i < pblock; i++)//��� �� ��ŭ �ݺ��ؼ� addblock()���� ��� �߰�
    {
        addblock();
    }
    savefile();// ��� ���, ����, pbn, psn�� ������ ���� "�ϵ����.txt"�� ����
}

//Node�� �߰��ϴ� �Լ�, Block������ �߰��ϸ�, Block�� ����Ű�� Sector�� �����Ͽ� �����Ҵ��� �Ѵ�.
void addblock() {
    struct Block* bNode;
    struct Block* newNode;
    bNode = head;
    while (bNode->next)
    {
        bNode = bNode->next; //Block��� NULL������ ������ ����
    }
    newNode = (struct Block*)malloc(sizeof(struct Block));//���ο� ��� �����Ҵ�
    newNode->pbn = bnum;//����� pbn���� ����
    bnum++;// ���� addblock()�Լ��� ����Ǹ�, 1�����ϵ��� ����
    for (int i = 0; i < 32; i++)
    {
        newNode->sector[i] = (struct Sector*)malloc(sizeof(struct Sector));//newNode�� sector�� ���� �Ҵ�
        newNode->sector[i]->psn = snum;//newNode�� sector�� psn�� ���� addblock()�Լ� ����, for������ 32�� �����Ҷ����� psn�� 1�� ����
        snum++;
        strcpy(newNode->sector[i]->data, "\0");
    }
    newNode->next = NULL;//newNode�� ������ �������� NULL�� ����
    bNode->next = newNode;//���� ������ ��忴�� bNode�� ������ ���� �߰��� newNode�� ����
}

//�ش� psn�� �ִ� data�� �д� �Լ�
void Flash_read(unsigned int psn) {
    struct Block* bNode = head->next;
    while (bNode)
    {
        for (int i = 0; i < 32; i++)// bNode���� sector�� 32���� �� �ѷ���
        {
            if (bNode->sector[i]->psn == psn) {// sector�� psn�� �̿��ڰ� ���ϴ� psn�� �����ϸ� �Ʒ� �ڵ� ����
                printf("psn: %d\ndata: %s\n", bNode->sector[i]->psn, bNode->sector[i]->data);
                return;
            }
        }
        bNode = bNode->next;//sector���� �� ����, ���� ���� ����
    }
}

//������ psn�� data�� �Է��ϴ� �Լ�
void Flash_write(unsigned int psn, char* data) {
    struct Block* bNode = head->next;
    while (bNode)
    {
        for (int i = 0; i < 32; i++)
        {
            if (bNode->sector[i]->psn == psn)// sector�� psn�� �̿��ڰ� ���ϴ� psn�� �����ϸ� �Ʒ� �ڵ� ����
            {
                printf("psn�߰�\n");
                if (strlen(bNode->sector[i]->data) > 0) { //�ش� bNode�� sector�� �����Ͱ� �����ϸ� �Ʒ� �ڵ� ����
                    printf("�ش� ���Ϳ� �����Ͱ� �����մϴ�.\n");
                    return;
                }
                    strcpy(bNode->sector[i]->data, data);//�ش� bNode�� sector�� �����Ͱ� �������� ������ �Ʒ� �ڵ� ����
                    printf("psn: %d\tdata: %s\n", bNode->sector[i]->psn, bNode->sector[i]->data);//������ psn�� data�� ������
                    savefile();// "�ϵ����.txt"�� �ٽ� ����
                    return;
            }
        }
        bNode = bNode->next;//sector���� �� ����, ���� ���� ����
    }
}
//������ psn�� �Ҵ������Ͽ� ���ִ� �Լ�.
void Flash_erase(unsigned int pbn){
    struct Block* bNode;
    bNode = head->next;
    while (bNode)
    {
        if (bNode->pbn == pbn) {// �̿��ڰ� ����� ���ϴ� pbn�� ������ �ִ� bNode�� ã���� �Ʒ� �ڵ� ����
            for (int i = 0; i < 32; i++)
            {
                strcpy(bNode->sector[i]->data, "\0");//�ش� bNode�� sector��ü�� data�� "\0"�� ����
            }
            savefile();// �����Ұ��� "�ϵ����.txt"�� �ٽ� ����
        }
        bNode = bNode->next;
    }
    
}
//��� pbn,psn�� �����ִ� �Լ�
void show() {
    struct Block* bNode;
    bNode = head->next;
    while (bNode)
    {
        printf("pbn: %d\n", bNode->pbn);
        for (int i = 0; i < 32; i++)
        {
            printf("psn: %d\t", bNode->sector[i]->psn);
            printf("data: %s\t", bNode->sector[i]->data);
        }
        printf("\n");
        bNode = bNode->next;
    }
}
//Block��忡 ����Ǿ��ִ� ��� �����͸� "�ϵ����.txt"�� ���� �Լ�.
int savefile() {
    FILE* fp = fopen("�ϵ����.txt", "w");//"�ϵ����.txt"������ ���� ���� ����
    struct Block* bNode = head->next;//bNode�� head�������� ����.
    if (fp == NULL) {//fopen�Լ��� �����߻��� NULL�� �����ϹǷ� �����߻���"���� ����" ����Ʈ
        printf("���� ����.\n");
        return 1;
    }
    while (bNode)
    {
        for (int i = 0; i < 32; i++)
        {
            fprintf(fp, "%d %s", bNode->sector[i]->psn, bNode->sector[i]->data);
            fputc('\n', fp);
        }
        bNode = bNode->next;
    }
    fclose(fp);
}
#include "hard.h"
unsigned int bnum = 0;//pbn�� ���� ������Ű������ �������� ó��
unsigned int snum = 0;//psn�� ���� ������Ű������ �������� ó��
//����ڰ� �Է��� MB������ �Ҵ� �޴´�. MB�� byte�� ȯ���Ͽ� block�� sector�� ������ �ľ��ϰ� array�� �����.
void init(int megabyte) {
    unsigned int byte = 1024 * 1024 * megabyte;// megabyte�� byte�� ȯ��
    unsigned int pblock = byte / 16384; //��� �� ���ϱ�
    unsigned int psector = byte / 512;  //���� �� ���ϱ�
    printf("������ ��� ������ %d �� �Դϴ�.\n", pblock);
    printf("������ ���� ������ %d �� �Դϴ�.\n", psector);
    for (int i = 0; i < pblock; i++)//��� �� ��ŭ �ݺ��ؼ� addblock()���� ��� �߰�
    {
        addblock();
    }
    savefile();// ��� ���, ����, pbn, psn�� ������ ���� "�ϵ����.txt"�� ����
}

//Node�� �߰��ϴ� �Լ�, Block������ �߰��ϸ�, Block�� ����Ű�� Sector�� �����Ͽ� �����Ҵ��� �Ѵ�.
void addblock() {
    struct Block* bNode;
    struct Block* newNode;
    bNode = head;
    while (bNode->next)
    {
        bNode = bNode->next; //Block��� NULL������ ������ ����
    }
    newNode = (struct Block*)malloc(sizeof(struct Block));//���ο� ��� �����Ҵ�
    newNode->pbn = bnum;//����� pbn���� ����
    bnum++;// ���� addblock()�Լ��� ����Ǹ�, 1�����ϵ��� ����
    for (int i = 0; i < 32; i++)
    {
        newNode->sector[i] = (struct Sector*)malloc(sizeof(struct Sector));//newNode�� sector�� ���� �Ҵ�
        newNode->sector[i]->psn = snum;//newNode�� sector�� psn�� ���� addblock()�Լ� ����, for������ 32�� �����Ҷ����� psn�� 1�� ����
        snum++;
        strcpy(newNode->sector[i]->data, "\0");
    }
    newNode->next = NULL;//newNode�� ������ �������� NULL�� ����
    bNode->next = newNode;//���� ������ ��忴�� bNode�� ������ ���� �߰��� newNode�� ����
}

//�ش� psn�� �ִ� data�� �д� �Լ�
void Flash_read(unsigned int psn) {
    struct Block* bNode = head->next;
    while (bNode)
    {
        for (int i = 0; i < 32; i++)// bNode���� sector�� 32���� �� �ѷ���
        {
            if (bNode->sector[i]->psn == psn) {// sector�� psn�� �̿��ڰ� ���ϴ� psn�� �����ϸ� �Ʒ� �ڵ� ����
                printf("psn: %d\ndata: %s\n", bNode->sector[i]->psn, bNode->sector[i]->data);
                return;
            }
        }
        bNode = bNode->next;//sector���� �� ����, ���� ���� ����
    }
}

//������ psn�� data�� �Է��ϴ� �Լ�
void Flash_write(unsigned int psn, char* data) {
    struct Block* bNode = head->next;
    while (bNode)
    {
        for (int i = 0; i < 32; i++)
        {
            if (bNode->sector[i]->psn == psn)// sector�� psn�� �̿��ڰ� ���ϴ� psn�� �����ϸ� �Ʒ� �ڵ� ����
            {
                printf("psn�߰�\n");
                if (strlen(bNode->sector[i]->data) > 0) { //�ش� bNode�� sector�� �����Ͱ� �����ϸ� �Ʒ� �ڵ� ����
                    printf("�ش� ���Ϳ� �����Ͱ� �����մϴ�.\n");
                    return;
                }
                strcpy(bNode->sector[i]->data, data);//�ش� bNode�� sector�� �����Ͱ� �������� ������ �Ʒ� �ڵ� ����
                printf("psn: %d\tdata: %s\n", bNode->sector[i]->psn, bNode->sector[i]->data);//������ psn�� data�� ������
                savefile();// "�ϵ����.txt"�� �ٽ� ����
                return;
            }
        }
        bNode = bNode->next;//sector���� �� ����, ���� ���� ����
    }
}
//������ psn�� �Ҵ������Ͽ� ���ִ� �Լ�.
void Flash_erase(unsigned int pbn) {
    struct Block* bNode;
    bNode = head->next;
    while (bNode)
    {
        if (bNode->pbn == pbn) {// �̿��ڰ� ����� ���ϴ� pbn�� ������ �ִ� bNode�� ã���� �Ʒ� �ڵ� ����
            for (int i = 0; i < 32; i++)
            {
                strcpy(bNode->sector[i]->data, "\0");//�ش� bNode�� sector��ü�� data�� "\0"�� ����
            }
            savefile();// �����Ұ��� "�ϵ����.txt"�� �ٽ� ����
        }
        bNode = bNode->next;
    }

}
//��� pbn,psn�� �����ִ� �Լ�
void show() {
    struct Block* bNode;
    bNode = head->next;
    while (bNode)
    {
        printf("pbn: %d\n", bNode->pbn);
        for (int i = 0; i < 32; i++)
        {
            printf("psn: %d\t", bNode->sector[i]->psn);
            printf("data: %s\t", bNode->sector[i]->data);
        }
        printf("\n");
        bNode = bNode->next;
    }
}
//Block��忡 ����Ǿ��ִ� ��� �����͸� "�ϵ����.txt"�� ���� �Լ�.
int savefile() {
    FILE* fp = fopen("�ϵ����.txt", "w");//"�ϵ����.txt"������ ���� ���� ����
    struct Block* bNode = head->next;//bNode�� head�������� ����.
    if (fp == NULL) {//fopen�Լ��� �����߻��� NULL�� �����ϹǷ� �����߻���"���� ����" ����Ʈ
        printf("���� ����.\n");
        return 1;
    }
    while (bNode)
    {
        for (int i = 0; i < 32; i++)
        {
            fprintf(fp, "%d %s", bNode->sector[i]->psn, bNode->sector[i]->data);
            fputc('\n', fp);
        }
        bNode = bNode->next;
    }
    fclose(fp);
}
