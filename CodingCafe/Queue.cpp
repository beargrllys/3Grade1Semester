#include <queue>
#include <iostream>

using namespace std;

int main()
{
    queue<int> que; //큐 변수 생성, <>사이에는 큐를 구성할 자료형을 지정합니다.

    //큐에 데이터 추가하기
    que.push(3);
    que.push(2);
    que.push(1);

    //큐의 가장 앞쪽에 있는 데이터 조회하기
    cout << "가장 입구쪽에 있는 데이터는 " << que.front() << "입니다." << endl;
    //큐의 가장 뒷쪽에 있는 데이터 조회하기
    cout << "가장 출구쪽에 있는 데이터는 " << que.back() << "입니다." << endl;

    //큐의 출구에서 원소하나를 빼내기
    que.pop();

    cout << "다음번에 큐에서 나올 데이터는 " << que.front() << "입니다." << endl;

    cout << "스택의 크기는 " << que.size() << "입니다.\n";

    //스택 비우기
    que.pop();
    que.pop();

    cout << "스택이 비어있나요? " << (que.empty() ? "네" : "아니오") << '\n';

    return 0;
}