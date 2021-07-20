#include "Node.h"
struct Node* insertID(char* buffer) {

    struct Node* hNode;//������ ���
    struct Node* newNode;//���λ��� ���
    hNode = head;
    while (hNode->next) {
        hNode = hNode->next;
    }
    newNode = (struct Node*)malloc(sizeof(struct Node));

    strcpy(newNode->ID, buffer);
    strcpy(newNode->video, "\0");
    newNode->next = NULL;
    hNode->next = newNode;
    return newNode;
}
struct Node* insertV(char* buffer) {

    struct Node* hNode;//������ ���
    struct Node* newNode;//���λ��� ���
    hNode = head;
    while (hNode->next) {
        hNode = hNode->next;
    }
    newNode = (struct Node*)malloc(sizeof(struct Node));

    strcpy(newNode->ID, "\0");
    strcpy(newNode->video, buffer);
    newNode->next = NULL;
    hNode->next = newNode;
    return newNode;
}

struct Video* insertVV(char* buffer) {

    struct Video* hNode;//������ ���
    struct Video* newNode;//���λ��� ���
    hNode = vhead;
    while (hNode->next) {
        hNode = hNode->next;
    }
    newNode = (struct Video*)malloc(sizeof(struct Video));
    strcpy(newNode->video, buffer);
    newNode->next = NULL;
    hNode->next = newNode;
    return newNode;
}
int openfile() {//���Ϸκ��� �����͸� �ҷ����� �Լ�
    FILE* fp = fopen("�뿩���.txt", "rt"); //rt���� �뿩���.txt ����

    struct Node* hNode = NULL;

    if (fp == NULL) {
        printf("�뿩���.txt ������ �����ϴ�. ���α׷��� �����Ͽ� �����ϼ���.\n");
        return 1;
    }

    printf("Running the open file method..\n");
    char buffer[20] = "\0";
    int num = -1;
    //���Ͽ� ����� �����͸� ����ü �迭�� ����
    while (1) {   // ID
        fscanf(fp, "%d", &num);   // ��ȣ
        fscanf(fp, "%s", buffer); // ������
        if (feof(fp) != 0)//������ ���� �����ϸ� while�� Ż��
            break;

        if (strlen(buffer) > 0) {//ID // �����Ͱ� �����ϸ�
            hNode = insertID(buffer); // buffer�� ���� Node�� ����
            printf("ID : %s\n", hNode->ID);
        }

        for (int i = 0; i < num; i++) {// Video    //num��ŭ
            fscanf(fp, "%s ", buffer);             //video������ �ް� 
            hNode = insertV(buffer);               //video�����͸� ���� Node����
            printf("Video : %s\n", hNode->video);

            if (feof(fp) != 0)//������ ���� �����ϸ� while�� Ż��
                break;
        }
    }
    fclose(fp);
}
int openfileV() {
    FILE* vfp = fopen("�뿩���ɸ��.txt", "rt");
    struct Video* vNode = NULL;
    if (vfp == NULL) {
        printf("�뿩���ɸ��.txt ������ �����ϴ�. ���α׷��� �����Ͽ� �����ϼ���.\n");
        return 1;
    }
    char buffer[20] = "\0";
    while (1)
    {
        fscanf(vfp, "%s", buffer);
        if (feof(vfp) != 0)//������ ���� �����ϸ� while�� Ż��
            break;
        vNode = insertVV(buffer);
    }
    fclose(vfp);
}

int savefile() {//�����͸� ���Ͽ� �����ϴ� �Լ�
    FILE* fp = fopen("�뿩���.txt", "wt");
    struct Node* hNode;
    hNode = head;   // ����
    if (fp == NULL) {//fopen�Լ��� �����߻��� NULL�� �����ϹǷ� �����߻���"���� ����" ����Ʈ
        printf("���� ����.\n");
        return 1;
    }
    int cnt = 0;
    while (hNode)
    {
        struct Node* a = hNode;
        int num = 0;
        int idNum = 0;

        while (a) {// Node* a�� �ѹ� Node����

            if (strlen(a->ID) > 0 && idNum == 0) idNum++;// �� ���� �ĺ��ϱ� ���� ��ġ
            else if (strlen(a->ID) > 0 && idNum != 0) {// �� ���� �ĺ��ϱ� ���� ��ġ
                break;
            }
            else if (strcmp(a->ID, "\0") == 0 && strlen(a->video) > 0) {//video ������ ������ŭ num����
                num++;
            }
            a = a->next;
        }   // count the number of Video

        if (strlen(hNode->ID) > 0 && cnt == 0) {
            fprintf(fp, "%d %s ", num, hNode->ID);//���� video�ĺ��� num�� ����ϰ� ID���
            cnt++;// ���� �� �ĺ��ϱ� ���� ��ġ
        }
        else  if (strlen(hNode->ID) > 0 && cnt > 0) {// ù ���� ������ ������ ���� ID�� �ö� ���๮�� ����
            fputc('\n', fp);//�ٹٲ����� �����Ͽ� ����
            fprintf(fp, "%d %s ", num, hNode->ID);//���� video�ĺ��� num�� ����ϰ� ID���
        }

        if (strcmp(hNode->ID, "\0") == 0 && strlen(hNode->video) > 0)
        {
            fprintf(fp, "%s ", hNode->video);// video������ �ĺ��Ͽ� ���
        }
        hNode = hNode->next;
    }

    fclose(fp);
    return 0;
}
int savefileV() {
    FILE* vfp = fopen("�뿩���ɸ��.txt", "wt");
    struct Video* vNode;
    vNode = vhead;   // ����
    if (vfp == NULL) {//fopen�Լ��� �����߻��� NULL�� �����ϹǷ� �����߻���"���� ����" ����Ʈ
        printf("���� ����.\n");
        return 1;
    }
    while (vNode)
    {
        fprintf(vfp, "%s ", vNode->video);
        vNode = vNode->next;
    }
    fclose(vfp);
}

void Search() {//�˻�
    char ID[20];
    struct Node* hNode;
    hNode = head;
    system("cls");
    printf("������ �̸��� �Է����ּ���.\n");
    scanf("%s", &ID);
    while (hNode)
    {
        if (!strcmp(hNode->ID, ID)) {//ID�߰�
            printf("�뿩ǰ��:");
            hNode = hNode->next;
            if ((hNode == NULL) || (strlen(hNode->ID) > 0)) {//����ó����
                printf("�뿩�Ͻ� ������ �����ϴ�.\n");
                Sleep(500);
                system("cls");
                return;
            }
            while (!strcmp(hNode->ID, "\0"))//video�����Ͷ��
            {
                printf("%s ", hNode->video);//����Ʈ
                hNode = hNode->next;//���� ���� �̵�
                if (hNode == NULL)//��尡 NULL�� �����ų�
                {
                    while (getchar() != '\n');
                    printf("\n============Enter�� �Է��Ͻø� �޴��� �Ѿ�ϴ�.============\n");
                    while (1) {
                        if (getchar() == '\n') {
                            system("cls");
                            return;
                        }
                        else if (getchar() != '\n')
                            continue;
                    }
                    return;
                }
                if (strlen(hNode->ID) > 0)//��尡 ID�����͸� ������ return;
                {
                    while (getchar() != '\n');
                    printf("\n============Enter�� �Է��Ͻø� �޴��� �Ѿ�ϴ�.============\n");
                    while (1) {
                        if (getchar() == '\n') {
                            system("cls");
                            return;
                        }
                        else if (getchar() != '\n')
                            continue;
                    }
                    return;
                }
            }
        }
        hNode = hNode->next;//���� ����
    }
    printf("ID�� �������� �ʽ��ϴ�.\n");// hNode�� NULL���� ���µ� ������ ���
    Sleep(1000);
    system("cls");
    return;
}
void Rentalshow() {
    struct Node* hNode;
    hNode = head;
    system("cls");
    printf("=============================================================\n");
    printf("                    ID: ����\n");
    while (hNode)
    {
        if (strlen(hNode->ID) > 0) {//ID������ ���
            printf("\n%s:", hNode->ID);
        }
        else if (strlen(hNode->ID) > 0 && strlen(hNode->next->ID) > 0) {//����ó�� ����
            printf("\n%s: �뿩���� ����\n", hNode->ID);
        }
        else if (strlen(hNode->ID) > 0 && hNode->next == NULL) {//����ó�� ����
            printf("\n%s: �뿩���� ����\n", hNode->ID);
        }
        if (strcmp(hNode->ID, "\0") == 0 && strlen(hNode->video) > 0)//video������ ���
        {
            printf("%s ", hNode->video);
        }

        hNode = hNode->next;//���� ����
    }
    Receivingshow();
    printf("============Enter�� �Է��Ͻø� �޴��� �Ѿ�ϴ�.============\n");
    while (1) {
        if (getchar() == '\n') {
            system("cls");
            return;
        }
        else if (getchar() != '\n')
            continue;
    }

}
void Rental() {//�뿩
    char ID[20];
    char video[20];
    struct Node* hNode;
    struct Video* vNode;
    hNode = head;
    vNode = vhead;
    system("cls");
    printf("����� ID�� �Է����ּ���\n");
    scanf("%s", &ID);
    while (hNode)
    {
        if (strcmp(hNode->ID, ID) == 0) {// �Է��� ID�� Node���� �߰ߵǸ�
            printf("�α��� ����!\n");
            Sleep(500);
            system("cls");
            printf("������ �뿩 ���� �����Դϴ�.\n");
            Receivingshow();            // �뿩 ������ video���� �����ְ�
            printf("\n��� ������ �����ðڽ��ϱ�?\n");
            scanf("%s", &video);        //video ����
            while (vNode)               //�԰����� video�� ����ִ� vNode�� �ѹ� ����
            {
                if (!strcmp(vNode->video, video)) {// ���ϴ� video�� �ִٸ�
                    Rentalsub(hNode, ID, video);   // �α����ߴ� ID�ڿ� video ����
                    Del(video);                    // Video��忡�� ������ ��� ����
                    system("cls");
                    return;
                }
                else if (vNode->next == NULL) {//����ó��
                    printf("�뿩 ��Ͽ� ���� �����Դϴ�.");
                    Sleep(500);
                    system("cls");
                    return;
                }
                vNode = vNode->next;
            }
        }
        else if (hNode->next == NULL) {//����ó����
            printf("�α��� ����!\n");
            Sleep(500);
            system("cls");
            return;
        }
        hNode = hNode->next;
    }

}
void Receivingshow() {//�뿩���� ����
    printf("\n=============================================================\n");
    printf("                    �뿩���� ����\n");
    struct Video* vNode;
    vNode = vhead;

    while (vNode)
    {
        printf("%s  ", vNode->video);// Video�� �� ���� video ���
        vNode = vNode->next;
    }
    printf("\n");
    printf("=============================================================\n");
}

void Rentalsub(struct Node* hNode, char* ID, char* video) {//
    struct Node* after = hNode->next;   //after�� hNode�� �������� ����
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));// ���� ���� ��� ����
    strcpy(newNode->ID, "\0");          //ID���� "\0"�Է�
    strcpy(newNode->video, video);      //video���� ���ϴ� video�Է�
    hNode->next = newNode;              //hNode������ newNode
    newNode->next = after;              //newNode ������ after
}
void Receiving() {//�԰�
    system("cls");
    char video[20];
    int num = 0;
    printf("���� ��� �԰��Ͻðڽ��ϱ�?\n");
    scanf("%d", &num);  //�ѹ��� ���ϴ� ������ŭ �԰��� �� �ְ��Ѵ�.
    while (getchar() != '\n');// ����ó����

    for (int i = 0; i < num; i++) {
        printf("�԰� �� ���� �̸��� �Է����ּ���.\n");
        scanf("%s", &video);
        Addvideo(video);//���ϴ� ��ŭ video �߰�
    }
    system("cls");
}
void Del(char* video) {
    system("cls");
    struct Video* vNode;
    struct Video* temp;
    struct Video* rmNode;
    vNode = vhead;
    rmNode = vhead;     //rmNode�� ó������
    vNode = vNode->next;//vNode�� ���� Node�� �̵�
    if (vNode == NULL) {
        printf("�԰�� ������ �����ϴ�.");//����ó��
        return;
    }
    while (!(vNode == NULL))
    {
        if (!strcmp(vNode->video, video)) {//���ϴ� video�� vNode���� ã����
            temp = vNode;                  //vNode�� ����ġ�� temp��带 �ű�
            rmNode->next = vNode->next;    //������ vNode�� ������带 ����
            vNode = rmNode;                //vNode�� ������ �ű�
            free(temp);                    //���ϴ� video�� ����ִ� ��ġ�� ���(temp)�� ����
            return;
        }
        else if (vNode->next == NULL) {//����ó��
            printf("�Է��Ͻ� ������ �԰����� �ʾҽ��ϴ�.\n");
            return;
        }
        rmNode = vNode;         //����带 vNode��ġ�� �ű��
        vNode = vNode->next;    //vNode�� �������� �ű����ν� �ٽ� rmNode�� ����尡 ��
    }
    system("cls");
}

void Addvideo(char* video) {//�԰��߰�
    struct Video* vNode;
    struct Video* newNode;
    vNode = vhead;   // first
    while (vNode->next) {       //vNode�� NULL���� �ű�
        vNode = vNode->next;
    };
    newNode = (struct Video*)malloc(sizeof(struct Video));//newNode�� ����

    strcpy(newNode->video, video);//newNode�� ���ϴ�video������ ����

    newNode->next = NULL;         //newNode������ NULL�� ����
    vNode->next = newNode;        //vNode������ newNode�� ����
}

void AddID() {
    system("cls");
    char ID[20];
    printf("�߰��Ͻ� ID�� �Է����ּ���.\n");
    scanf("%s", ID);

    struct Node* hNode;//������ ���
    struct Node* sNode;
    hNode = head;
    sNode = head;
    while (sNode) {
        if (!strcmp(sNode->ID, ID)) {// ����ó����
            printf("�ߺ��Ǵ� ���̵� �ֽ��ϴ�. �������ּ���.\n");
            scanf("%s", &ID);
            sNode = head;
        }
        sNode = sNode->next;
    }
    struct Node* newNode;//���λ��� ���
    while (hNode->next) {
        hNode = hNode->next;    //hNode�� NULL���� �ű�
    }
    newNode = (struct Node*)malloc(sizeof(struct Node));//newNode����

    strcpy(newNode->ID, ID);        //newNode�� ID�� ID����
    strcpy(newNode->video, "\0");   //newNode�� video�� vdieo����
    newNode->next = NULL;           //newNode ������ NULL
    hNode->next = newNode;          //hNode�� ������ newNode
    system("cls");
}