//
// Created by Simon on 2023/4/17.
//
/*
 * B-tree
 * M B树的阶
 * size 节点关键字数
 * keys 关键字数组
 * children 子节点指针数组
 *      A
 *    /
 *   /
 *  B--C--D--E
 *
 */

#define M 4 // M=4, 2-3-4 Tree
int maxsize = M-1;
int minsize = M/2;
typedef struct TreeNode{
    int size;
    int keys[M-1];
    struct TreeNode** children;
} *pTreeNode, TreeNode;