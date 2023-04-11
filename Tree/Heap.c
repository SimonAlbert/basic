//
// Created by Simon on 2023/4/5.
//
/*
 * 堆
 */
# include "stdio.h"


# define HEAP_SIZE 1024
# define ELEMENT char

ELEMENT heap[HEAP_SIZE];

// 左子节点
int lc(int cur) {
    return cur * 2;
}

// 右子节点
int rc(int cur) {
    return cur * 2 + 1;
}

// 节点对比
bool compare(ELEMENT x, ELEMENT y) {
    return x < y;
}

void swap(int x, int y) {
    ELEMENT e = heap[x];
    heap[x] = heap[y];
    heap[y] = e;
}

// 小根堆, 小的向上
void up(int x) {
    // 到顶点下一层, 或者上面比自己小
    while (x > 1 && compare(heap[x], heap[x / 2])) {
        swap(x, x / 2);
        x /= 2;
    }
}

void down(int x) {
    // 到倒数第二层
    int t = x * 2;
    while (t < HEAP_SIZE) {
        t += compare(heap[t], heap[t + 1]);// 选出最大的
        if (compare(heap[t], heap[x])) {
            swap[t, x];
        }
        t *= 2;
    }
}
void build(){
    for (int i = 1; i <= HEAP_SIZE; ++i) {
        up(i);
    }
}
void insert(ELEMENT e) {

}

ELEMENT queryTop() {

}

void deleteTop() {

}

void mergeHeap(ELEMENT *heap1, ELEMENT *heap2) {

}

void changeNode(ELEMENT *h, int n, ELEMENT ch_val) {

}


