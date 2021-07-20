#include "Node.h"
struct Node* insertID(char* buffer) {

    struct Node* hNode;//임의의 노드
    struct Node* newNode;//새로생긴 노드
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

    struct Node* hNode;//임의의 노드
    struct Node* newNode;//새로생긴 노드
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

    struct Video* hNode;//임의의 노드
    struct Video* newNode;//새로생긴 노드
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
int openfile() {//파일로부터 데이터를 불러오는 함수
    FILE* fp = fopen("대여목록.txt", "rt"); //rt모드로 대여목록.txt 오픈

    struct Node* hNode = NULL;

    if (fp == NULL) {
        printf("대여목록.txt 파일이 없습니다. 프로그램을 종료하여 생성하세요.\n");
        return 1;
    }

    printf("Running the open file method..\n");
    char buffer[20] = "\0";
    int num = -1;
    //파일에 저장된 데이터를 구조체 배열에 저장
    while (1) {   // ID
        fscanf(fp, "%d", &num);   // 번호
        fscanf(fp, "%s", buffer); // 데이터
        if (feof(fp) != 0)//파일의 끝에 도달하면 while문 탈출
            break;

        if (strlen(buffer) > 0) {//ID // 데이터가 존재하면
            hNode = insertID(buffer); // buffer를 가진 Node를 생성
            printf("ID : %s\n", hNode->ID);
        }

        for (int i = 0; i < num; i++) {// Video    //num만큼
            fscanf(fp, "%s ", buffer);             //video데이터 받고 
            hNode = insertV(buffer);               //video데이터를 가진 Node생성
            printf("Video : %s\n", hNode->video);

            if (feof(fp) != 0)//파일의 끝에 도달하면 while문 탈출
                break;
        }
    }
    fclose(fp);
}
int openfileV() {
    FILE* vfp = fopen("대여가능목록.txt", "rt");
    struct Video* vNode = NULL;
    if (vfp == NULL) {
        printf("대여가능목록.txt 파일이 없습니다. 프로그램을 종료하여 생성하세요.\n");
        return 1;
    }
    char buffer[20] = "\0";
    while (1)
    {
        fscanf(vfp, "%s", buffer);
        if (feof(vfp) != 0)//파일의 끝에 도달하면 while문 탈출
            break;
        vNode = insertVV(buffer);
    }
    fclose(vfp);
}

int savefile() {//데이터를 파일에 저장하는 함수
    FILE* fp = fopen("대여목록.txt", "wt");
    struct Node* hNode;
    hNode = head;   // 전역
    if (fp == NULL) {//fopen함수는 오류발생시 NULL을 리턴하므로 오류발생시"파일 에러" 프린트
        printf("파일 에러.\n");
        return 1;
    }
    int cnt = 0;
    while (hNode)
    {
        struct Node* a = hNode;
        int num = 0;
        int idNum = 0;

        while (a) {// Node* a로 한번 Node돌기

            if (strlen(a->ID) > 0 && idNum == 0) idNum++;// 한 행을 식별하기 위한 장치
            else if (strlen(a->ID) > 0 && idNum != 0) {// 한 행을 식별하기 위한 장치
                break;
            }
            else if (strcmp(a->ID, "\0") == 0 && strlen(a->video) > 0) {//video 데이터 개수만큼 num증가
                num++;
            }
            a = a->next;
        }   // count the number of Video

        if (strlen(hNode->ID) > 0 && cnt == 0) {
            fprintf(fp, "%d %s ", num, hNode->ID);//위에 video식별한 num을 출력하고 ID출력
            cnt++;// 다음 행 식별하기 위한 장치
        }
        else  if (strlen(hNode->ID) > 0 && cnt > 0) {// 첫 행을 제외한 나머지 행은 ID가 올때 개행문자 삽입
            fputc('\n', fp);//줄바꿈으로 구분하여 저장
            fprintf(fp, "%d %s ", num, hNode->ID);//위에 video식별한 num을 출력하고 ID출력
        }

        if (strcmp(hNode->ID, "\0") == 0 && strlen(hNode->video) > 0)
        {
            fprintf(fp, "%s ", hNode->video);// video데이터 식별하여 출력
        }
        hNode = hNode->next;
    }

    fclose(fp);
    return 0;
}
int savefileV() {
    FILE* vfp = fopen("대여가능목록.txt", "wt");
    struct Video* vNode;
    vNode = vhead;   // 전역
    if (vfp == NULL) {//fopen함수는 오류발생시 NULL을 리턴하므로 오류발생시"파일 에러" 프린트
        printf("파일 에러.\n");
        return 1;
    }
    while (vNode)
    {
        fprintf(vfp, "%s ", vNode->video);
        vNode = vNode->next;
    }
    fclose(vfp);
}

void Search() {//검색
    char ID[20];
    struct Node* hNode;
    hNode = head;
    system("cls");
    printf("가입자 이름을 입력해주세요.\n");
    scanf("%s", &ID);
    while (hNode)
    {
        if (!strcmp(hNode->ID, ID)) {//ID발견
            printf("대여품목:");
            hNode = hNode->next;
            if ((hNode == NULL) || (strlen(hNode->ID) > 0)) {//예외처리문
                printf("대여하신 비디오가 없습니다.\n");
                Sleep(500);
                system("cls");
                return;
            }
            while (!strcmp(hNode->ID, "\0"))//video데이터라면
            {
                printf("%s ", hNode->video);//프린트
                hNode = hNode->next;//다음 노드로 이동
                if (hNode == NULL)//노드가 NULL을 만나거나
                {
                    while (getchar() != '\n');
                    printf("\n============Enter를 입력하시면 메뉴로 넘어갑니다.============\n");
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
                if (strlen(hNode->ID) > 0)//노드가 ID데이터를 만나면 return;
                {
                    while (getchar() != '\n');
                    printf("\n============Enter를 입력하시면 메뉴로 넘어갑니다.============\n");
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
        hNode = hNode->next;//다음 노드로
    }
    printf("ID가 존재하지 않습니다.\n");// hNode가 NULL까지 갔는데 없으면 출력
    Sleep(1000);
    system("cls");
    return;
}
void Rentalshow() {
    struct Node* hNode;
    hNode = head;
    system("cls");
    printf("=============================================================\n");
    printf("                    ID: 비디오\n");
    while (hNode)
    {
        if (strlen(hNode->ID) > 0) {//ID데이터 출력
            printf("\n%s:", hNode->ID);
        }
        else if (strlen(hNode->ID) > 0 && strlen(hNode->next->ID) > 0) {//예외처리 적용
            printf("\n%s: 대여비디오 없음\n", hNode->ID);
        }
        else if (strlen(hNode->ID) > 0 && hNode->next == NULL) {//예외처리 적용
            printf("\n%s: 대여비디오 없음\n", hNode->ID);
        }
        if (strcmp(hNode->ID, "\0") == 0 && strlen(hNode->video) > 0)//video데이터 출력
        {
            printf("%s ", hNode->video);
        }

        hNode = hNode->next;//다음 노드로
    }
    Receivingshow();
    printf("============Enter를 입력하시면 메뉴로 넘어갑니다.============\n");
    while (1) {
        if (getchar() == '\n') {
            system("cls");
            return;
        }
        else if (getchar() != '\n')
            continue;
    }

}
void Rental() {//대여
    char ID[20];
    char video[20];
    struct Node* hNode;
    struct Video* vNode;
    hNode = head;
    vNode = vhead;
    system("cls");
    printf("사용자 ID를 입력해주세요\n");
    scanf("%s", &ID);
    while (hNode)
    {
        if (strcmp(hNode->ID, ID) == 0) {// 입력한 ID가 Node에서 발견되면
            printf("로그인 성공!\n");
            Sleep(500);
            system("cls");
            printf("다음은 대여 가능 비디오입니다.\n");
            Receivingshow();            // 대여 가능한 video들을 보여주고
            printf("\n어느 비디오를 빌리시겠습니까?\n");
            scanf("%s", &video);        //video 선택
            while (vNode)               //입고가능한 video가 들어있는 vNode를 한번 돈다
            {
                if (!strcmp(vNode->video, video)) {// 원하는 video가 있다면
                    Rentalsub(hNode, ID, video);   // 로그인했던 ID뒤에 video 삽입
                    Del(video);                    // Video노드에서 선택한 노드 삭제
                    system("cls");
                    return;
                }
                else if (vNode->next == NULL) {//예외처리
                    printf("대여 목록에 없는 비디오입니다.");
                    Sleep(500);
                    system("cls");
                    return;
                }
                vNode = vNode->next;
            }
        }
        else if (hNode->next == NULL) {//예외처리문
            printf("로그인 실패!\n");
            Sleep(500);
            system("cls");
            return;
        }
        hNode = hNode->next;
    }

}
void Receivingshow() {//대여가능 비디오
    printf("\n=============================================================\n");
    printf("                    대여가능 비디오\n");
    struct Video* vNode;
    vNode = vhead;

    while (vNode)
    {
        printf("%s  ", vNode->video);// Video를 쭉 돌며 video 출력
        vNode = vNode->next;
    }
    printf("\n");
    printf("=============================================================\n");
}

void Rentalsub(struct Node* hNode, char* ID, char* video) {//
    struct Node* after = hNode->next;   //after는 hNode의 다음으로 설정
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));// 새로 만들 노드 생성
    strcpy(newNode->ID, "\0");          //ID에는 "\0"입력
    strcpy(newNode->video, video);      //video에는 원하는 video입력
    hNode->next = newNode;              //hNode다음은 newNode
    newNode->next = after;              //newNode 다음은 after
}
void Receiving() {//입고
    system("cls");
    char video[20];
    int num = 0;
    printf("비디오 몇개를 입고하시겠습니까?\n");
    scanf("%d", &num);  //한번에 원하는 갯수만큼 입고할 수 있게한다.
    while (getchar() != '\n');// 예외처리문

    for (int i = 0; i < num; i++) {
        printf("입고 할 비디오 이름을 입력해주세요.\n");
        scanf("%s", &video);
        Addvideo(video);//원하는 만큼 video 추가
    }
    system("cls");
}
void Del(char* video) {
    system("cls");
    struct Video* vNode;
    struct Video* temp;
    struct Video* rmNode;
    vNode = vhead;
    rmNode = vhead;     //rmNode는 처음부터
    vNode = vNode->next;//vNode는 다음 Node로 이동
    if (vNode == NULL) {
        printf("입고된 비디오가 없습니다.");//예외처리
        return;
    }
    while (!(vNode == NULL))
    {
        if (!strcmp(vNode->video, video)) {//원하는 video를 vNode에서 찾으면
            temp = vNode;                  //vNode의 현위치에 temp노드를 옮김
            rmNode->next = vNode->next;    //전노드랑 vNode의 다음노드를 연결
            vNode = rmNode;                //vNode를 전노드로 옮김
            free(temp);                    //원하는 video가 들어있는 위치의 노드(temp)를 삭제
            return;
        }
        else if (vNode->next == NULL) {//예외처리
            printf("입력하신 비디오는 입고하지 않았습니다.\n");
            return;
        }
        rmNode = vNode;         //전노드를 vNode위치로 옮기고
        vNode = vNode->next;    //vNode는 다음으로 옮김으로써 다시 rmNode는 전노드가 됨
    }
    system("cls");
}

void Addvideo(char* video) {//입고추가
    struct Video* vNode;
    struct Video* newNode;
    vNode = vhead;   // first
    while (vNode->next) {       //vNode를 NULL까지 옮김
        vNode = vNode->next;
    };
    newNode = (struct Video*)malloc(sizeof(struct Video));//newNode를 생성

    strcpy(newNode->video, video);//newNode에 원하는video데이터 삽입

    newNode->next = NULL;         //newNode다음은 NULL로 설정
    vNode->next = newNode;        //vNode다음은 newNode로 설정
}

void AddID() {
    system("cls");
    char ID[20];
    printf("추가하실 ID를 입력해주세요.\n");
    scanf("%s", ID);

    struct Node* hNode;//임의의 노드
    struct Node* sNode;
    hNode = head;
    sNode = head;
    while (sNode) {
        if (!strcmp(sNode->ID, ID)) {// 예외처리문
            printf("중복되는 아이디가 있습니다. 변경해주세요.\n");
            scanf("%s", &ID);
            sNode = head;
        }
        sNode = sNode->next;
    }
    struct Node* newNode;//새로생긴 노드
    while (hNode->next) {
        hNode = hNode->next;    //hNode를 NULL까지 옮김
    }
    newNode = (struct Node*)malloc(sizeof(struct Node));//newNode생성

    strcpy(newNode->ID, ID);        //newNode의 ID에 ID삽입
    strcpy(newNode->video, "\0");   //newNode의 video에 vdieo삽입
    newNode->next = NULL;           //newNode 다음은 NULL
    hNode->next = newNode;          //hNode의 다음은 newNode
    system("cls");
}