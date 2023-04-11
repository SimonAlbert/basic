#include <stdio.h>
#include <stdlib.h>

void print_array(int arr[], int arSize) {
    for (int i = 0; i < arSize; i++) {
        printf("%3d ", arr[i]);
    }
    putchar('\n');
}

int max_of(int arr[], int arSize, int *_max) {
    int max = 0, maxPos = 0;
    for (int i = 0; i < arSize; i++) {
        if (max < arr[i]) {
            max = arr[i];
            maxPos = i;
        }
    }

    *_max = max;
    return maxPos;
}

int *selection_sort(int arr[], int arSize) {
    int *newArray = (int *) malloc(arSize * sizeof(int));
    for (int i = 0; i < arSize; i++) {
        int max, maxPos;
        maxPos = max_of(arr, arSize, &max);
        arr[maxPos] = -1;
        newArray[i] = max;

        print_array(arr, arSize);
        print_array(newArray, arSize);

        getchar();
    }
    return newArray;
}

int main() {
    int a[10];
    for (int i = 0; i < 10; i++)
        scanf("%d", &a[i]);

    getchar();

    selection_sort(a, 10);
    return 0;
}
