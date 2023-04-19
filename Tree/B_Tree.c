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
int maxsize = M-1;
int minsize = M/2;
typedef struct TreeNode{
    int size;
    key_type keys[M];
    struct TreeNode* children[M];
    int is_leaf;
} *pTreeNode, TreeNode;

void insert(pTreeNode, key_type*);

void exchange(key_type* a, key_type* b)
{
    key_type tmp = *a;
    *a = *b;
    *b = tmp;
}

// 数组插入
int key_into_node(key_type* arr, int num, key_type *v)
{
    // 节点满
    if(num == M) return -1;
    // 遍历找到目标位置(下标)
    int position;
    for (int i = 0; i < num; ++i) {
        if(arr[i] > *v){
            position = i;
            break;
        }
    }
    // 从后向前遍历到新元素, 每个后移一位, 减少exchange带来的内存复制
    for (int i = t->size; i >= position; ++i) {

    }
}

pTreeNode find(pTreeNode t, key_type* v)
{
    // 空树

    // 找到新key的位置
    key_type tmp;
    for (int i = 0; i < t->size; ++i) {
        tmp = t->keys[i];
        if(tmp == *v){
            return t;
        }else if(tmp < *v && t->keys[i+1] > *v){
            return find(t->children[i], v);
        }
    }
    return find(t->children[t->size-1], v);
}

void insert(pTreeNode t, key_type* v)
{
    // 叶节点
    if(t->is_leaf){

    }else{
        for (int i = 0; i < t->size; ++i) {
            if(t->keys[i] < *v && t->keys[i+1] > *v){
                insert(t->children[i], v);
            }
        }
        insert(t->children[t->size - 1], v);
    }
}
int main(){

}