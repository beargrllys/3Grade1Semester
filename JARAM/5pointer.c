/*
손은 눈보다 빠르다. 타짜의 세상에서는 0.01초의 순간에도 패를 뒤바꿀 수 있어야지. 고니야, 실력 좀 보여줘
고니는 자신이 가진 11점짜리 카드를 50점짜리 골드카드로 순식간에 패를 뒤바꾸려고 한다. 
순식간에 두개의 int형 변수를 포인터를 이용해 뒤바꾸는 Sakura 함수를 완성해라.
main함수 건들면 손모가지 날라가는 거시여. 쫄리면 뒤지시든가.

출력: 
후달리냐?ㅋㅋㅋㅋ
고니의 카드: 11점

(눈 깜짝할새)

사쿠라여?
고니의 카드: 50점
*/

#include <stdio.h>

void Sakura()
{
}

/*****************************************************Main함수를 건드리지 마세요***********************************************************/
int main()
{
    int Goni_sCard = 11;
    int goldCard = 50;

    printf("후달리냐?ㅋㅋㅋㅋ\n");
    printf("고니의 카드: %d 점\n\n", Goni_sCard);

    printf("(눈 깜짝할새)\n\n");
    Swap(&Goni_sCard, &goldCard);

    printf("사쿠라여?\n");
    printf("고니의 카드: %d 점\n", Goni_sCard);

    return 0;
}
/*****************************************************Main함수를 건드리지 마세요***********************************************************/