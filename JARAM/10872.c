#include <stdio.h>

int fac(int n);

int main(void)
{
	int n = 0;
	scanf("%d", &n);

	printf("%d", fac(n));

	return 0;
}

int fac(int n)
{
	if (n > 1)
	{
		return fac(n - 1) * n;
	}
	else if (n == 1)
	{
		return 1;
	}
	else
		return 1;
}