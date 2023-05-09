//
// Created by Simon on 2023/4/17.
//
#include <stdio.h>
#include <stdlib.h>
/*
 * B-tree 数组实现
 * M B树的阶
 * size 节点关键字数
 * keys 关键字数组
 * children 子节点数组
 *    2-3-4树为例
 *
 *          root
 *        /      \
 *      A|B      C
 *     / | \    / \
 *    E  F G   H   I
 */
typedef int key_type;
#define M 4 // 阶数(最大子节点数)
int maxsize = M - 1; // 最大关键字数
int minsize = M / 2; // 最小关键字数
// 节点定义
typedef struct TreeNode {
    int size;
    key_type* keys;
    struct TreeNode** children;
    int is_leaf;
} *pTreeNode, TreeNode;
// 创建节点
pTreeNode createNode(){
    pTreeNode pNode = (pTreeNode) malloc(sizeof(TreeNode));
    pNode->size = 0;
    pNode->keys = (key_type*) malloc(sizeof(key_type) * M);
    for (int i = 0; i < M; ++i) {
        pNode->keys[i] = 0;
    }
    // 指向子节点的指针数组, 阶数不会很大, 空间浪费小
    pNode->children = (pTreeNode*) malloc(sizeof(pTreeNode) * M);
    pNode->is_leaf = 1;
    return pNode;
}

// 关键字换值
void exchange(key_type *a, key_type *b) {
    key_type tmp = *a;
    *a = *b;
    *b = tmp;
}

// 有序数组中插入
int key_array_insert(key_type *arr, int* psize, key_type *v) {

    *psize++;
    int size = *psize;
    //*psize++;
    // 节点满
    if (size == M) return -1;
    // 节点空
    if (size == 0)
    {
        arr[0] = *v;
        return 0;
    }
    // 遍历找到目标位置(下标)
    int position = 0;
//    for (int i = 0; i <= size && arr[i] < *v; ++i) {
//        position = i;
//    }
    for (position = 0; position <= size && arr[position] < *v; position++) {
//        position = i;
    }
    // 从后向前遍历到新元素, 每个后移一位, 减少exchange带来的内存复制
    for (int i = size; i > position; i--) {
        arr[i] = arr[i-1];
    }
    arr[position] = *v;
    return position;
}

// 有序数组插入到指定点
int key_directed_insert(key_type *arr, int num, key_type *v, int dis_index) {
    if(num >= M){
        return -1;
    }
    for (int i = num; i > dis_index; --i) {
        arr[i] = arr[i-1];
    }
    arr[dis_index] = *v;
    return 0;
}
// 有序数组插入到指定点
int node_directed_insert(pTreeNode *arr, int num, pTreeNode v, int dis_index) {
    if(num >= M){
        return -1;
    }
    for (int i = num; i > dis_index; --i) {
        arr[i] = arr[i-1];
    }
    arr[dis_index] = v;
    return 0;
}
// 分裂节点 由父节点进行操作 将超限的子节点一分为二
void split(pTreeNode t, int index)
{
    t->is_leaf = 0;
    int mid = (M + 1) / 2;// 中值定位
    // 第index个子节点超限, 将该节点的中间关键字提取到父节点
    key_type pick = t->children[index]->keys[mid]; // 中间关键字
    key_directed_insert(t->keys, t->size, &pick, index); // 插入父节点
    // 分裂子节点并建立联系
    // 子节点1
//    pTreeNode child1 = (pTreeNode) malloc(sizeof(TreeNode));
    pTreeNode child1 = createNode();
    // 建立连接
    node_directed_insert(t->children, t->size, child1, index);
    // 关键字填充
    for (int i = 0; i < mid; ++i) {
        child1->keys[i] = t->keys[i];
    }
    // 子节点2
//    pTreeNode child2 = (pTreeNode) malloc(sizeof(TreeNode));
    pTreeNode child2 = createNode();
    // 建立连接
    t->children[index + 1] = child2;
    // 关键字填充
    for (int i = M-1; i > mid; --i) {
        child2->keys[i] = t->keys[i];
    }
    t->size++;
}

pTreeNode find(pTreeNode t, key_type *v) {
    // 空树

    // 找到新key的位置
    key_type tmp;
    for (int i = 0; i < t->size; ++i) {
        tmp = t->keys[i];
        if (tmp == *v) {
            return t;
        } else if (tmp < *v && t->keys[i + 1] > *v) {
            return find(t->children[i], v);
        }
    }
    return find(t->children[t->size - 1], v);
}

/* 只能插入叶子节点
 * 插入成功: 返回null
 * 插入超限: 返回中间值
 */
key_type* insert(pTreeNode t, key_type *v) {
    key_type* ret = NULL;
    // 叶节点
    if (t->is_leaf) {
        key_array_insert(t->keys, &(t->size), v);
        t->size++;
    } else {
        for (int i = 0; i <= t->size; ++i) {
            // 找到插入的子节点
            if (t->keys[i] < *v && t->keys[i + 1] > *v) {
                // 插入 并根据返回值决定是否要进行上溢
                key_type* child_insert = insert(t->children[i], v);
                if(child_insert)
                {
                    split(t, i);
                }
            }
        }
    }
    if(t->size == M- 1){
        ret = t->keys + t->size / 2;
    }
    return ret;
}

void print_tree(pTreeNode t){
    if(t->is_leaf){
        for (int i = 0; i < t->size; ++i) {
            printf("%d,", t->keys[i]);
        }
        return;
    }
    for (int i = 0; i <= t->size; ++i) {
        print_tree(t->children[i]);
    }
    for (int i = 0; i < t->size; ++i) {
        printf("%d,", t->keys[i]);
    }
    printf("\n");
}

int main() {
    int keys[] = { 4, 7, 9, 5};
    pTreeNode tree = createNode();
    insert(tree, keys + 0);
    insert(tree, keys + 1);
    insert(tree, keys + 2);
    insert(tree, keys + 3);
    print_tree(tree);
    system("pause");
}