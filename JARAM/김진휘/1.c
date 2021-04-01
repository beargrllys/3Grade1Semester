#include <stdio.h>

void printArray(int arr[], int count) // 배열의 포인터와 요소의 개수를 받음
{
    arr[1] = 0;
    arr[2] = 0;
    arr[3] = 0;
}

int main()
{
    int numArr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    printArray(numArr, sizeof(numArr) / sizeof(int)); // 배열과 요소의 개수를 넣음

    for (int i = 0; i < 10; i++)
    {
        printf("%d ", numArr[i]);
    }

    printf("\n");

    return 0;
}