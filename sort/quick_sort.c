#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "math.h"

void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}


//快速排序-递归
void quick_sort(int *p, int n) {
    // 递归结束条件
    if (n < 2) {
        return;
    }

    int i = 0, l = 0, r = n-1, pivot = p[rand() % n];
    printf("pivot: %d\n", pivot);
    while (i < r) {
        for(int x=0;x<n;x++){
            if(x == i){
                printf("{%d}", p[x]);
            } else if(x==l){
                printf("<%d>", p[x]);
            }else if(x==r){
                printf("[%d]", p[x]);
            } else{
                printf(" %d ", p[x]);
            }
        }
        printf("\n");
        // l到i之间的元素一定小于等于pivot,也就是小于
        if (p[i] < pivot) {
            if (i != l) {
                swap(p + i, p + l);
            }
            i++;
            l++;
        } else if (p[i] > pivot) {
            swap(p + i, p + r);
            r--;
        } else {
            i++;
        }
    }
    // l和i中间的数都等于pivot
    quick_sort(p, l);
    quick_sort(p + i, n - r);
}

int main() {
    int n, i;
    int *p = NULL;
    printf("how many nums : ");
    scanf("%d", &n);
    p = (int *) malloc(n * sizeof(int));
    printf("input array : ");
    for (i = 0; i < n; i++) {
        scanf("%d", p + i);
    }
    getchar();
    quick_sort(p, n);
    printf("sorted\n");
    for (i = 0; i < n; i++)
        printf("%d ", p[i]);
    printf("\n");
    return 0;
}