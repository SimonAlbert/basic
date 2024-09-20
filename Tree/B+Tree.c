//
// Created by Simon on 2023/4/17.
//
/*
 * B+tree 数组实现
 * 如何理解B+树
 * 首先想象一个链表, 链表的每个节点可以存储 4 个元素
 * [1, 2, 3, 4] -> [5, 6, 7, 8] -> [9, 10, 11, 12]
 * 用链表每个节点的最大值构造B树
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


// 关键字类型
typedef int key_type;
// 阶数
#define M 5
// 最大关键字数
const int MAX_KEY_COUNT = M - 1;
// 最小关键字数
int MIN_KEY_COUNT = MAX_KEY_COUNT + 1 / 2;
// 除根结点外的其它结点的最小关键字数为MIN_KEY_COUNT, 最大关键字数为MAX_KEY_COUNT; 最小子节点数为MIN_KEY_COUNT, 最大子节点数为MAX_KEY_COUNT
// 数据节点
typedef struct DataNode {
    key_type key;
    //
} *pDataNode, DataNode;
// TreeNode 树节点
typedef struct TreeNode {
    int is_root;
    int is_leaf;
    int size;
    key_type *keys;
    union {
        struct TreeNode **children;
        struct DataNode **data;
    };
    struct TreeNode *pNext;
    struct TreeNode *pPrev;
} *pTreeNode, TreeNode;

typedef struct BPTree{
    pTreeNode root;
    pTreeNode head;
}*pBPTree, BPTree;

// 创建叶节点
pTreeNode createLeafNode();
// 创建非叶节点
pTreeNode createNotLeafNode();
// 创建B+树
pBPTree createBPTree();
// 1.插入数据
int insertTree(pBPTree pTree, pDataNode pData);
int insert(pTreeNode node, pDataNode pData);
// --节点已满, 分裂
void split(pTreeNode parent, int index);
// 2.删除数据
int delete(pBPTree pTree, key_type key);
// --节点过小, 合并
void merge(pTreeNode parent, int index);
// 3.查找数据
DataNode findDataNode(pBPTree tree, key_type key);
pTreeNode findTreeNode(pBPTree tree, key_type key);
// 4.修改
void update(pBPTree tree, key_type key, pDataNode newData);

pTreeNode createLeafNode() {
    pTreeNode pNode = (pTreeNode) malloc(sizeof(TreeNode));
    pNode->is_root = 0;
    pNode->is_leaf = 1;
    pNode->size = 0;
    pNode->keys = (key_type*) calloc(M, sizeof(key_type ));
    pNode->data = (DataNode**) calloc(M, sizeof(pDataNode));
    pNode->pNext = NULL;
    pNode->pPrev = NULL;
    return pNode;
}
pTreeNode createNotLeafNode() {
    pTreeNode pNode = (pTreeNode) malloc(sizeof(TreeNode));
    pNode->is_root = 0;
    pNode->is_leaf = 1;
    pNode->size = 0;
    pNode->keys = (key_type*) calloc(M, sizeof(key_type ));
    pNode->children = (TreeNode **) calloc(M, sizeof(pTreeNode));
    pNode->pNext = NULL;
    pNode->pPrev = NULL;
    return pNode;
}
pBPTree createBPTree() {
    pBPTree pTree = (pBPTree) malloc(sizeof(BPTree));
    pTreeNode pNode = createLeafNode();
    pNode->is_root = 1;
    pTree->root = pNode;
    pTree->head = pNode;
    return pTree;
}
int insertTree(pBPTree pTree, pDataNode pData) {
    pTreeNode root = pTree->root;
    
}
int insert(pTreeNode node, pDataNode pData) {
    if(node->is_leaf) {

    } else {
        // 定位
        for (int i = 0; i < node->size; ++i) {

        }
        // 递归扩展

        // 拆分
    }
    return node->size >= M;
}
int main(){

    return 0;
}