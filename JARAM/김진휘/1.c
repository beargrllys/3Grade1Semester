#include <stdio.h>
#include <string.h>
#define MAX_INDEX 100

typedef struct _champ
{
    char name[20];
    int HP;
    int MANA;
    char Q_skill[20];
    char W_skill[20];
    char E_skill[20];
    char R_skill[20];
} champ;

int main()
{

    champ teemo = {
        .name = "티모",
        .HP = 528,
        .MANA = 334,
        .Q_skill = "실명다트",
        .W_skill = "신속한이동",
        .E_skill = "맹독다트",
        .R_skill = "유독성함정"};

    char skillInput[MAX_INDEX] = {
        0,
    };
    printf("어떤 스킬을 사용하시겠습니까? : ");
    scanf("%s", skillInput);
    printf("%s 발동!!!\n", skillInput);
    int mpPoint = 0;
    int mpResult;

    if (strcmp(skillInput, teemo.Q_skill) == 0 || strcmp(skillInput, teemo.W_skill) == 0 || strcmp(skillInput, teemo.E_skill) == 0 || strcmp(skillInput, teemo.R_skill) == 0)
    {
        for (int i = 0; i < MAX_INDEX; i++)
        {
            if (skillInput[i] != 0)
            {
                mpPoint++;
            }
            else if (skillInput[i] == 0)
            {
                mpResult = teemo.MANA - (mpPoint / 2);
                printf("마나 소모 (%d -> %d, 마나 %d 사용!)", teemo.MANA, mpResult, mpPoint / 2); //한글은 영어와 달리 2바이트라서 배열의 길이가 2배로 늘어남 따라서 2로 나눔
                break;
            }
        }
    }
    else
    {
        printf("스킬을 잘못 입력했습니다!");
    }
    return 0;
}