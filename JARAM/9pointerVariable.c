/*
드디어 링크드 리스트의 문들 두드려 봅시다. main함수에는 링크드 리스트를 구성하는 기본적인 코드를 제작해놨습니다. 이 코드를 보고 printNodeList()함수의 메개변수부터 사용까지
링크드 리스트를 출력해 보세요

출력: 
7->3->5->4->9->0->1->8->2->6

*/
#include <stdio.h>
#include <stdlib.h>

typedef struct _Node
{
    int val;
    struct _Node *next_Node;
} Node;

void printNodeList()
{
    ;
}

/*****************************************************이 안쪽은 건들이지 마세요.***********************************************************/
int main()
{
    Node **phead;
    Node *head, *new_node;
    int list[10] = {7, 3, 5, 4, 9, 0, 1, 8, 2, 6};
    int i = 0;

    phead = &head;

    for (i = 0; i < 10; i++)
    {
        new_node = (Node *)malloc(sizeof(Node));
        new_node->val = list[i];
        new_node->next_Node = head;
        head = new_node;
    }
    /*****************************************************이 안쪽은 건들이지 마세요.***********************************************************/

    printNodeList();
}
