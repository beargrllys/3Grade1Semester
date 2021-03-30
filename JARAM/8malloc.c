/*
대한민국 해군은 조국 해양수호를 위해 24시간 356일 우리의 바다를 지키고 있다. 한국의 대형 이지스함 세종대왕함은 서해 28구역 순찰 중 북한의 잠수함 3대를 발견했다.
즉시 전투배치를 완료한 세종대왕함의 승조원들은 미사일 발사를 위해 SonarDetector()함수를 실행하여 소나 탐지정보를 열람하고 A,B,C로 표시된 적잠수함의 정확한 좌표를 입력하고자한다. 
동적할당을 이용해 적 잠수함의 둘레를 X로 둘러 싸도록 설정하고 
나머지 구역은 O표시를 해서 아군과 어선을 보호하라 

출력:
적잠수함 A 파괴!
적잠수함 B 파괴!
적잠수함 C 파괴!
전투배치 해제, 적 잠수함 침몰 확인

*/
#include <stdio.h>
#include <stdlib.h>

/*****************************************************이 함수들은 건드리지 마세요***********************************************************/
void SonarDetector()
{
    printf("실전! 총원 전투배치! 실전!\n\n");
    printf("0  12345678901234567890\n");
    printf("1  OOOOOOOOOOOOOOOOOOOO\n");
    printf("2  OOOOOOAOOOOOOOOOOOOO\n");
    printf("3  OOOOOOOOOOOOOOOOOOOO\n");
    printf("4  OOOOOOOOOOOOOOOOOOOO\n");
    printf("5  OOOOOOOOOOOOOOOOOOOO\n");
    printf("6  OOOOOOOOOOOOOOOOOOOO\n");
    printf("7  OOOOOOOOOOOOOOOOOOOO\n");
    printf("8  OOOOOOOOOOOOOOOOOOOO\n");
    printf("9  OOOBOOOOOOOOOOOOOOOO\n");
    printf("10 OOOOOOOOOOOOOOOOOOOO\n");
    printf("11 OOOOOOOOOOOOOOOOOOOO\n");
    printf("12 OOOOOOOOOOOOOOOOOOOO\n");
    printf("13 OOOOOOOOOOOOOOOOOOOO\n");
    printf("14 OOOOOOOOOOOOOOOOCOOO\n");
    printf("15 OOOOOOOOOOOOOOOOOOOO\n");
}

void printWarResult(char **Submarine_pos)
{
    int count = 0;
    if (Submarine_pos[2][6] == 'A' && Submarine_pos[2][7] == 'A' && Submarine_pos[2][8] == 'A' && Submarine_pos[1][6] == 'A' && Submarine_pos[1][7] == 'A' && Submarine_pos[1][8] == 'A' && Submarine_pos[3][6] == 'A' && Submarine_pos[3][7] == 'A' && Submarine_pos[3][8] == 'A')
    {
        printf("적잠수함 A 파괴!\n");
        count++;
    }
    if (Submarine_pos[9][2] == 'A' && Submarine_pos[9][3] == 'A' && Submarine_pos[9][4] == 'A' && Submarine_pos[8][2] == 'A' && Submarine_pos[8][3] == 'A' && Submarine_pos[8][4] == 'A' && Submarine_pos[10][2] == 'A' && Submarine_pos[10][3] == 'A' && Submarine_pos[10][4] == 'A')
    {
        printf("적잠수함 B 파괴!\n");
        count++;
    }
    if (Submarine_pos[14][16] == 'A' && Submarine_pos[14][17] == 'A' && Submarine_pos[14][18] == 'A' && Submarine_pos[13][16] == 'A' && Submarine_pos[13][17] == 'A' && Submarine_pos[13][18] == 'A' && Submarine_pos[15][16] == 'A' && Submarine_pos[15][17] == 'A' && Submarine_pos[15][18] == 'A')
    {
        printf("적잠수함 C 파괴!\n");
        count++;
    }
    if (count == 3)
    {
        printf("전투배치 해제, 적 잠수함 침몰 확인\n");
    }
    else
    {
        printf("선저 타기실 어뢰로 인한 파공발생! 전 승무원 비상이함 하라.. 실ㅈㅓ..\n");
    }
}

/*****************************************************이 함수들은 건드리지 마세요***********************************************************/

int main()
{
    SonarDetector();
    /*전투 메뉴얼*/
    //1. char형 15 X 20 크기 이중 배열을 동적 할당하라
    char **Submarine_pos;

    //2. 적 잠수함 주변을 X 문자로 바꾸어라
    printWarResult(Submarine_pos);
    //3. 잠수함 파괴 여부를 확인하라
}