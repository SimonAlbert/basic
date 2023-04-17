//
// Created by Simon on 2023/4/17.
//
#include <stdio.h>
#include <stdlib.h>
/*
 * B-tree
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
    key_type keys[M-1];
    struct TreeNode* children[M];
    int is_leaf;
} *pTreeNode, TreeNode;

void insert(pTreeNode, key_type*);

pTreeNode find(pTreeNode t, key_type* v)
{
    // 空树

    // 搜索
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