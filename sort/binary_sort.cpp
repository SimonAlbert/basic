#include<cstdio>

void sort(int *a, int n) {
    int t;
    if (n > 1) {
        for (int i = 0; i < n / 2; i++) {
            for (int j = n - 1; j >= n / 2; j--) {
                if (a[i] > a[j]) {
                    t = a[i];
                    a[i] = a[j];
                    a[j] = t;
                }
            }
        }
        if (n / 2) {
            sort(a, n / 2);
            sort(a + n / 2, (n + 1) / 2);
        }
    } else return;
}

int main() {
    int n;
    int a[200];
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%d", a+i);
    }
    sort(a, n);
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    return 0;
}
