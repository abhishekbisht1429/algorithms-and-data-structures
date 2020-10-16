#include<stdio.h>
#include<math.h>

void sieve(int *prime, int n) {
    for(int i=2;i<=n;++i) {
        prime[i] = 1;
    }

    for(int i=2; i*i<=n; ++i) {
        if(prime[i]) {
            int current;
            current = i*i;
            
            while(current<=n) {
                prime[current] = 0;
                current += i;
            }
        }
    }
}
void display(int *prime, int size) {
    for(int i=2;i<size;++i) {
        if(prime[i])
            printf("%d ", i);
    }
    printf("\n");
}
int main() {
    int n = 1000;
    int prime[n];
    sieve(prime, n);
    display(prime, n);

    return 0;
}