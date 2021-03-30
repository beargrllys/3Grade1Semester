/*
당신은 50명의 이름과 ID, PW를 저장해야 합니다. 순서대로 나열된 이름,ID,PW 데이터가 제공되었을때
구조체를 활용해 이름은 1, ID는 2, PW는 3을 입력하고 1,2,3번 각각 선택된 항목을 기준으로 오름차순 정렬하는 
프로그램을 제작하세요  
예시) 
data = {윤지상, multitab, 0123},{뽀로로, apple, 7232},{일론머스크, tasla, 9374}
입력: 2
출력: ID를 기준으로 정렬합니다.
이름:뽀로로 / ID:apple / PW:7232
이름:윤지상 / ID:multitab / PW:0123
이름:일론머스크 / ID:tasla / PW:9374
*/
#include <stdio.h>

struct Person
{
    char name[15];
    char ID[15];
    int PW;
};

int main()
{

    char *name[15] = {"추신수", "최진우", "오영택", "김연두", "빌게이츠", "일론머스크", "부석종", "정경두", "손원일", "오영은", "박대기", "서욱", "김영환", "우하은", "이도형"};
    char *ID[15] = {"chu", "kane", "woowakgood", "rudbekia", "Microsoft", "SpaceX", "Navy", "Military", "NavyKing", "Kidmom", "wait", "Army", "bald", "greeting", "Computer"};
    int *PW[15] = {"3852", "9374", "0927", "3947", "5823", "9423", "2799", "4829", "9917", "2474", "2083", "2039", "2663", "7729", "0043"};
}
