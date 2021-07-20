//Sleep함수를 위한 헤더
#include "Node.h"

int main() {
    char ID[20];
    char video[20];
    head = (struct Node*)malloc(sizeof(struct Node));//Node의 첫 노드를 생성
    vhead = (struct Video*)malloc(sizeof(struct Video));//Video의 첫 노드를 생성
    strcpy(vhead->video, "\0");//head에는 모두 \0를 삽입
    strcpy(head->ID, "\0");
    strcpy(head->video, "\0");
    head->next = NULL;//head노드 다음은 NULL
    vhead->next = NULL;//vhead노드 다음은 NULL
    openfile(); //파일을 먼저 읽음
    openfileV();

    while (1)
    {
        printf("어떠한 기능을 이용하시겠습니까\n");
        printf("1.검색 2.대여 3.입고 4.입고품 삭제 5 가입자추가 6.전체출력 7.프로그램 종료\n");
        int select;
        scanf("%d", &select);
        while (getchar() != '\n');   // 예외처리문
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
            printf("삭제 할 비디오의 이름을 입력해주세요:\n");
            scanf("%s", &video);//Rental()에서 Del을 적용하므로 케이스에서 video를 받음
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
        default://예외처리문
            printf("선택란에 있지 않습니다.\n");
        }
    }
    return 0;
}