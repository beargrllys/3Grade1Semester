#include <stack>
#include <iostream>

using namespace std;

int main()
{
    stack<int> stk; //스택 변수 생성, <>사이에는 스택의 자료형을 지정합니다.

    //스택에 데이터 추가하기
    stk.push(3);
    stk.push(2);
    stk.push(1);

    //스택의 가장 위에 있는 데이터 조회하기
    cout << "가장 위에 있는 데이터는 " << stk.top() << "입니다." << endl;

    //스택에서 원소하나를 빼내기
    stk.pop();

    cout << "가장 위에 있는 데이터는 " << stk.top() << "입니다." << endl;

    cout << "스택의 크기는 " << stk.size() << "입니다.\n";

    //스택 비우기
    stk.pop();
    stk.pop();

    cout << "스택이 비어있나요? " << (stk.empty() ? "네" : "아니오") << '\n';

    return 0;
}