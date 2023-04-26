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
 * children 子节点指针数组
 *    2-3-4树为例
 *
 *          root
 *        /      \
 *      A|B      C
 *     / | \    / \
 *    E  F G   H   I
 */
typedef int key_type;
#define M 4 // M=4, 2-3-4 Tree
int maxsize = M - 1;
int minsize = M / 2;
typedef struct TreeNode {
    int size;
    key_type* keys;
    struct TreeNode** children;
    int is_leaf;
} *pTreeNode, TreeNode;

pTreeNode createNode(int degree){
    pTreeNode pNode = (pTreeNode) malloc(sizeof(TreeNode));
    pNode->size = 0;
    pNode->keys = (key_type*) malloc(sizeof(key_type) * degree);
    pNode->children = (TreeNode**) malloc(sizeof(TreeNode) * degree);
    pNode->is_leaf = 1;
    return pNode;
}


void exchange(key_type *a, key_type *b) {
    key_type tmp = *a;
    *a = *b;
    *b = tmp;
}

// 有序数组中插入
int array_insert(key_type *arr, int num, key_type *v) {
    // 节点满
    if (num == M) return -1;
    // 遍历找到目标位置(下标)
    int position;
    for (int i = 0; i < num; ++i) {
        if (arr[i] > *v) {
            position = i;
            break;
        }
    }
    // 从后向前遍历到新元素, 每个后移一位, 减少exchange带来的内存复制
    for (int i = num; i >= position; i--) {
        arr[i] = arr[i-1];
    }
    arr[position] = *v;
    return position;
}
// 有序数组插入到指定点
int array_distinct_insert(key_type *arr, int num, key_type *v, int dis_index) {
    if(num == M){
        return -1;
    }
    for (int i = num; i > dis_index; --i) {
        arr[i] = arr[i-1];
    }
    arr[dis_index] = *v;
    return 0;
}
// 有序数组插入到指定点
int node_distinct_insert(pTreeNode *arr, int num, pTreeNode *v, int dis_index) {
    if(num == M){
        return -1;
    }
    for (int i = num; i > dis_index; --i) {
        arr[i] = arr[i-1];
    }
    arr[dis_index] = *v;
    return 0;
}
// 分裂节点
void split(pTreeNode t, int index)
{
    // 分裂后不再是叶节点
    t->is_leaf = 0;
    array_distinct_insert(t->keys, t->size, t->keys + index, index);
    node_distinct_insert(t->children, t->size, t->children + index, index);
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
        array_insert(t->keys, t->size, v);
        if(t->size == M- 1){
            ret = t->keys + t->size / 2;
        }
        return ret;
    } else {
        for (int i = 0; i <= t->size; ++i) {
            // 找到插入的子节点
            if (t->keys[i] < *v && t->keys[i + 1] > *v) {
                return insert(t->children[i], v);
            }
        }
    }
}

int main() {

}