#include <stdio.h>

int power(int a,int n) {
	if(n==0)
		return 1;
	else if(n==1)
		return a;
	if(n%2==0)
		return power(a,n/2)*power(a,n/2);
	else
		return power(a,n/2)*power(a,n/2)*a;
}

int main() {
	int a = 2;
	int n = 11;
	printf("%d",power(a,n));
}
