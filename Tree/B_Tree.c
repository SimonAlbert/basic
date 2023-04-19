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

void insert(pTreeNode, key_type*);

void exchange(key_type* a, key_type* b)
{
    key_type tmp = *a;
    *a = *b;
    *b = tmp;
}

// 数组插入
int array_insert(key_type* arr, int num, key_type *v)
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
    for (int i = num; i >= position; i--) {
        arr[i] = arr[i-1];
    }
    arr[position] = *v;
    return position;
}

// 分裂节点
void split(pTreeNode t)
{
    // 分裂后不再是叶节点
    t->is_leaf = 0;
    
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
    if(t->children == NULL){
        array_insert(t->keys, t->size, v);
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