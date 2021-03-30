/*
탈론의 의무관인 "모이라"는 상대의 피를 흡수하는 스킬을 지니고 있다. 그녀에게 자힐이 가능하고 탱탱한 탱커인 로드호그는 양껏 피를 빨 수 있는 숙주에 불과하다. 
현재 모이라의 흡혈량은 0이다. 모이라가 로드호그의 피를 빨 수 있도록 모이라 구조체의 suck_rate(흡혈량) 변수를 임의의 양수로 올리고 로드호그 구조체 HP변수를 2000 이하로 내리는 프로그램을 제작하라.
Main함수를 건들이지 말고 모이라 구조체의 suck_rate(흡혈량) 변수와 로드호그 구조체 HP변수를 내리는 change_suck_rate 함수를 완성하라. 


(흡혈량이 0 + 로드호드 HP 2000)
출력: 흡혈이 안되잖아.. 상태가 안 좋아...
(흡혈량이 0 + 로드호드 HP 2000 이하)
출력: 흡혈 기계가 고장났나?
(흡혈량이 0 이상 + 로드호드 HP 2000)
출력: 제프 카플란: 버그인가? 
(흡혈량이 0 이상 + 로드호드 HP 2000 이하)
출력: 내 연구를 막았다간 이렇게 되는 거다.
*/
#include <stdio.h>

typedef struct _Hero
{
    char name[20];
    int HP;
    int suck_rate;
} Hero;

void change_suck_rate(Hero *moira, Hero *rodhog)
{
}

/*****************************************************Main함수를 건드리지 마세요***********************************************************/
int main()
{

    Hero moira = {
        .name = "모이라",
        .HP = 250,
        .suck_rate = 0};

    Hero rodhog = {
        .name = "로드호그",
        .HP = 2000,
        .suck_rate = 0};

    change_suck_rate(&moira, &rodhog);

    printf("모이라가 %d만큼 흡혈한다! 로드호그의 HP는 %d이다.\n", moira.suck_rate, rodhog.HP);
    if (moira.suck_rate <= 0 && rodhog.HP >= 2000)
    {
        printf("흡혈이 안되잖아.. 상태가 안 좋아...\n");
    }
    else if (moira.suck_rate <= 0 && rodhog.HP <= 2000)
    {
        printf("흡혈 기계가 고장났나?\n");
    }
    else if (moira.suck_rate >= 0 && rodhog.HP <= 2000)
    {
        printf("제프 카플란: 버그인가? \n");
    }
    else if (moira.suck_rate >= 0 && rodhog.HP <= 2000)
    {
        printf("내 연구를 막았다간 이렇게 되는 거다.\n");
    }
}
/*****************************************************Main함수를 건드리지 마세요***********************************************************/