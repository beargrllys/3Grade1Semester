#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100

typedef char element;
typedef struct{
	element data[MAX_STACK_SIZE];
	int top;
} StackType;

//스택 초기화 함수
void init_stack(StackType *s)
{
	s->top = -1;
}

//공백 상태 검출 함수
int is_empty(StackType *s)
{
	return(s->top == -1);
}

//포화 상태 검출 함수
int is_full(StackType *s)
{
	return(s->top == (MAX_STACK_SIZE - 1));
}

//삽입 함수
void push(StackType *s, element item)
{
	if(is_full(s)){
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item;
}

//삭제 함수
element pop(StackType *s)
{
	if(is_empty(s)){
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}

//피크 함수
element peek(StackType *s)
{
	if(is_empty(s)){
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[s->top];
}

//연산자의 우선순위를 반환
int prec(char op)
{
	switch(op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}

//중위 표기 수식 -> 후위 표기 수식
void infix_to_postfix(char exp[])
{
	int i =0;
	char ch, top_op;
	int len = strlen(exp);
	StackType s;

	init_stack(&s);		//스택 초기화
	for (i = 0; i<len; i++) {
		ch = exp[i];
		switch (ch) {
		case '+': case '-': case '*': case '/':		//연산자
			//스택에 있는 연산자의 우선순위가 더 크거나 같으면 출력
			while (!is_empty(&s) && (prec(ch) <= prec(peek(&s))))
				printf("%c", pop(&s));
			push(&s, ch);
			break;
		case '(':
			push(&s, ch);
			break;
		case ')':
			top_op = pop(&s);
			while (top_op != '(') {
				printf("%c", top_op);
				top_op = pop(&s);
			}	
			break;
		default:	//피연산자
			printf("%c", ch);
			break;
		}
	}
	while (!is_empty(&s))	//스택에 저장된 연산자들 출력
		printf("%c", pop(&s));
}

//후위 표기 수식 계산 함수
int eval(char exp[])
{
	int op1, op2, value, i = 0;
	int len = strlen(exp);
	char ch, top_op;
	StackType s;

	init_stack(&s);
	for (i = 0; i<len; i++) {
		ch = exp[i];
		if (ch != '+' && ch != '-' && ch != '*' && ch != '/') {
			value = ch - '0';	//입력이 피연산자이면
			push(&s, value);
		}
		else {	//연산자이면 피연산자를 스택에서 제거
			op2 = pop(&s);
			op1 = pop(&s);
			switch (ch) {	//연산을 수행하고 스택에 저장
			case '+': push(&s, op1 + op2); break;
			case '-': push(&s, op1 - op2); break;
			case '*': push(&s, op1 * op2); break;
			case '/': push(&s, op1 / op2); break;
			}
		}
	}
	return pop(&s);
}

int main(void)
{
	int result;
	char *s = "(2+3)*4+9";
	printf("중위표시수식 %s \n", s);
	printf("후위표시수식 ");
	infix_to_postfix(s);
	printf("\n");
	result = eval("infix_to_postfix(s)");
	printf("결과값: %d\n", result);
	return 0;
}
