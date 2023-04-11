#include <stdio.h>
#include <stdlib.h>

void print_array(int array[], int arSize) {
    for (int i = 0; i < arSize; i++)
        printf("%4d ", array[i]);
    putchar('\n');
}

void insertion_sort(int array[], int arSize) {
    if (arSize == 1)
        return;
    else {
        for (int i = 0; i < arSize; i++) {

            for (int j = 0; j < i; j++) {
                if (array[i] < array[j]) {
                    int temp = array[i];
                    for (int k = i; k > j; k--) {
                        array[k] = array[k - 1];
                    }
                    array[j] = temp;
                    break;
                }
            }

            print_array(array, arSize);
            getchar();
        }
    }
}

int main() {
    int a[10];
    for (int i = 0; i < 10; i++)
        scanf("%d", &a[i]);

    getchar();
    insertion_sort(a, 10);
    return 0;
}

