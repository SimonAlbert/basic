//
// Created by Simon on 2023/4/17.
//
#include <stdio.h>
#include <stdlib.h>
/*
 * B-tree 数组实现
 * M B树的阶
 * size 节点关键字数
 * values 关键字数组
 * children 子节点数组
 *    2-3-4树为例
 *
 *          root
 *        /      \
 *      A|B      C
 *     / | \    / \
 *    E  F G   H   I
 */
typedef int value_type;
#define M 4 // 阶数(最大子节点数)
int MAX_VALUE_COUNT = M - 1; // 最大关键字数
int MIN_VALUE_COUNT = M / 2; // 最小关键字数
// 除根结点外的其它结点的最小关键字数为MIN_VALUE_COUNT, 最大关键字数为MAX_VALUE_COUNT; 最小子节点数为MIN_VALUE_COUNT+1, 最大子节点数为MAX_VALUE_COUNT+1
// 节点定义
typedef struct TreeNode {
    int size;
    value_type* values;
    struct TreeNode** children;
    struct TreeNode* parent;
    int is_leaf;
} *pTreeNode, TreeNode;

pTreeNode createNode();
void exchange(value_type *a, value_type *b);
int key_array_insert(value_type *arr, const int count, const value_type *v);
int key_directed_insert(value_type *arr, int num, const value_type *v, int dis_index);
int node_directed_insert(pTreeNode *arr, int num, pTreeNode v, int dis_index);
void split(pTreeNode current_node, int index);
pTreeNode find(pTreeNode t, value_type *v);
int insert(pTreeNode current_node, value_type *v);
void insert_tree(pTreeNode root, value_type *v);

// 创建节点
pTreeNode createNode(){
    pTreeNode pNode = (pTreeNode) malloc(sizeof(TreeNode));
    pNode->size = 0;
    pNode->values = (value_type*) malloc(sizeof(value_type) * M);
    for (int i = 0; i < M; ++i) {
        pNode->values[i] = 0;
    }
    // 指向子节点的指针数组, 阶数不会很大, 空间浪费小
    pNode->children = (pTreeNode*) malloc(sizeof(pTreeNode) * M);
    pNode->is_leaf = 1;
    return pNode;
}

// 关键字换值
void exchange(value_type *a, value_type *b) {
    value_type tmp = *a;
    *a = *b;
    *b = tmp;
}

/* 有序数组中插入
 * arr 数组
 * count 数组长度
 * v 要插入的关键字
 * return 0成功 1失败
 */
int key_array_insert(value_type *arr, const int count, const value_type *v) {
    // 节点满
    if (count == MAX_VALUE_COUNT) return 1;
    // 找到目标位置(下标)
    int position = 0;
    while(position < count && *v > arr[position]){
        position++;
    }
    // 从后向前遍历到position, 每个后移一位, 减少exchange带来的内存复制
    for (int i = count; i > position; i--) {
        arr[i] = arr[i-1];
    }
    // 插入新关键字
    arr[position] = *v;
    return 0;
}

/* 有序数组插入到指定下标
 * arr 数组
 * num 数组长度
 * v 要插入的关键字
 * dis_index 目标下标
 * return -1: 超限 0: 成功
 */
int key_directed_insert(value_type *arr, int num, const value_type *v, int dis_index) {
    // 节点关键字数组已满
    if(num >= MAX_VALUE_COUNT){
        return -1;
    }
    for (int i = num; i > dis_index; --i) {
        arr[i] = arr[i-1];
    }
    arr[dis_index] = *v;
    return 0;
}
/* 新的子节点插入到指定下标
 * arr 节点数组
 * num 数组长度
 * v 要插入的节点
 * dis_index 目标下标
 * return -1: 超限 0: 成功
 */
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

/*
 * 第一种拆分方法, 不使用父节点指针
 * 由父节点进行操作, 对父节点的第i个子节点进行拆分
 */
void split(pTreeNode current_node, int index)
{
    // 先创建两个新节点
    pTreeNode child1 = createNode();
    pTreeNode child2 = createNode();
    // 仅有一个叶节点时, 一分为二, 根节点只保留一个value
//    if(current_node->is_leaf){
//        // 关键字填充
//        child1->size = MIN_VALUE_COUNT;
//        for (int i = 0; i < MIN_VALUE_COUNT; ++i) {
//            child1->values[i] = current_node->values[i];
//        }
//        child2->size = M - MIN_VALUE_COUNT - 1;
//        for (int i = MIN_VALUE_COUNT + 1; i < M; ++i) {
//            child2->values[i] = current_node->values[i];
//        }
//        current_node->values[0] = current_node->values[MIN_VALUE_COUNT];
//        current_node->size = 1;
//        current_node->is_leaf = 0;
//        // 建立连接
//        current_node->children[0] = child1;
//        current_node->children[1] = child2;
//        return;
//    }else {
        // 非叶子节点
        // 取待分裂节点
        pTreeNode split_node = current_node->children[index];
        // 新节点size
        child1->size = MIN_VALUE_COUNT;
        child2->size = MAX_VALUE_COUNT - MIN_VALUE_COUNT -1;
        child1->is_leaf = split_node->is_leaf;
        child2->is_leaf = split_node->is_leaf;
        // 关键字拆分
        // split_node->values的[0, MIN_VALUE_COUNT-1]放入child1
        for (int i = 0; i < MIN_VALUE_COUNT; ++i) {
            child1->values[i] = split_node->values[i];
        }
        // split_node->values的[MIN_VALUE_COUNT+1, MAX_VALUE_COUNT-1]放入child2
        for (int i = MIN_VALUE_COUNT; i < MIN_VALUE_COUNT; ++i) {
            child2->values[i] = split_node->values[i];
        }
        // 子节点连接到新节点
        // split_node->children的[0, MIN_VALUE_COUNT]放入child1
        for (int i = 0; i <= MIN_VALUE_COUNT; ++i) {
            child1->children[i] = split_node->children[i];
        }
        // split_node->children的[MIN_VALUE_COUNT+1, MAX_VALUE_COUNT]放入child2
        for (int i = MIN_VALUE_COUNT; i <= MAX_VALUE_COUNT; ++i) {
            child1->children[i] = split_node->children[i];
        }
        // split_node->values[MIN_VALUE_COUNT]插入到当前节点的values[index]
        key_directed_insert(current_node->values, current_node->size, split_node->values + MIN_VALUE_COUNT, index);
        // child1替换当前节点的children[index], child2插入到当前节点的children[index+1]
        current_node->children[index] = child1;
        node_directed_insert(current_node->children, current_node->size, child2, index + 1);
        // 当前节点size++
        current_node->size++;
//    }
}

pTreeNode find(pTreeNode t, value_type *v) {
    // 空树

    // 找到新key的位置
    value_type tmp;
    for (int i = 0; i < t->size; ++i) {
        tmp = t->values[i];
        if (tmp == *v) {
            return t;
        } else if (tmp < *v && t->values[i + 1] > *v) {
            return find(t->children[i], v);
        }
    }
    return find(t->children[t->size - 1], v);
}

/* 只能插入叶子节点
 * return 0: 成功 1: 超限
 */
int insert(pTreeNode current_node, value_type *v) {
    printf("%d <- %d\n", current_node->size, *v);
    if (current_node->is_leaf) { // 叶节点
        int insert = key_array_insert(current_node->values, current_node->size, v);
        if(insert){
            return 1;
        }
        current_node->size++;
    } else { // 非叶节点
        // 找到要插入的关键字下标position
        int position = 0;
        while(current_node->values[position] < *v) {
            position++;
        }
        // 将新关键字插入position指向的子节点
        int ret = insert(current_node->children[position], v);
        if(ret){// 如果子节点超限, 则分裂
            split(current_node, position);
        }
    }
    return current_node->size >= MAX_VALUE_COUNT;
}

// 处理根节点场景
void insert_tree(pTreeNode root, value_type *v) {
    if(insert(root, v)){
        // 创建新的根节点
        pTreeNode new_root = createNode();
        TreeNode old_root = *root;
        new_root->is_leaf = 0;
        new_root->children[0] = &old_root;
        split(new_root, 0);
        *root = *new_root;
    }
}

int main() {
    int values[] = { 4, 7, 9, 5, 11};
    pTreeNode tree = createNode();
    insert_tree(tree, values + 0);
    insert_tree(tree, values + 1);
    insert_tree(tree, values + 2);
    insert_tree(tree, values + 3);
    insert_tree(tree, values + 4);
    system("pause");
}