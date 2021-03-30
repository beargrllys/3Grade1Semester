/*
배열을 선언하고 배열의 순서대로 값을 출력해주는 프로그램을 만드세요
알고리즘 정의: 배열을 사용하세요
입력: 4 5 2 5 3 8 2 5 3 7
출력: 4->5->2->5->3->8->2->5->3->7
*/

#include <stdio.h>

int main()
{
    int arr[11];

    for (int i = 0; i < 10; i++)
    {
        scanf("%d", &arr[i]);
    }

    for (int j = 0; j < 10; j++)
    {
        printf("%d", arr[j]);
        if (j == 9)
            break;
        printf("->");
    }

    return 0;
}