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
int maxsize = M - 1; // 最大关键字数
int minsize = M / 2; // 最小关键字数
// 节点定义
typedef struct TreeNode {
    int size;
    value_type* values;
    struct TreeNode** children;
    int is_leaf;
} *pTreeNode, TreeNode;
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
 * v 要插入的值
 * return -1: 超限
 *        数字: 插入的下标
 */
int key_array_insert(value_type *arr, const int count, const value_type *v) {
    // 节点满
    if (count == M) return -1;
    // 找到目标位置(下标)
    int position = 0;
    while(position < count && *v > arr[position]){
        position++;
    }
    // 从后向前遍历到position, 每个后移一位, 减少exchange带来的内存复制
    for (int i = count; i > position; i--) {
        arr[i] = arr[i-1];
    }
    // 插入新值
    arr[position] = *v;
    // 返回下标
    return position;
}

/* 有序数组插入到指定下标
 * arr 数组
 * num 数组长度
 * v 要插入的值
 * dis_index 目标下标
 * return -1: 超限 0: 成功
 */
int key_directed_insert(value_type *arr, int num, const value_type *v, int dis_index) {
    // 节点关键字数组已满
    if(num >= maxsize){
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
// 分裂节点 由父节点进行操作 将超限的子节点一分为二
void split(pTreeNode current_node, int index)
{
    current_node->is_leaf = 0;
    int mid = (M + 1) / 2;// 中值定位
    // 第index个子节点超限, 将该节点的中间关键字提取到父节点
    value_type pick = current_node->children[index]->values[mid]; // 中间关键字
    key_directed_insert(current_node->values, current_node->size, &pick, index); // 插入父节点
    // 分裂子节点并建立联系
    // 子节点1
//    pTreeNode child1 = (pTreeNode) malloc(sizeof(TreeNode));
    pTreeNode child1 = createNode();
    // 建立连接
    node_directed_insert(current_node->children, current_node->size, child1, index);
    // 关键字填充
    for (int i = 0; i < mid; ++i) {
        child1->values[i] = current_node->values[i];
    }
    // 子节点2
//    pTreeNode child2 = (pTreeNode) malloc(sizeof(TreeNode));
    pTreeNode child2 = createNode();
    // 建立连接
    current_node->children[index + 1] = child2;
    // 关键字填充
    for (int i = M-1; i > mid; --i) {
        child2->values[i] = current_node->values[i];
    }
    current_node->size++;
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
 * 插入成功: 返回null
 * 插入超限: 分裂并返回父节点指针
 */
value_type* insert(pTreeNode current_node, value_type *v) {
    value_type* ret = NULL;
    if (current_node->is_leaf) { // 叶节点
        int insert = key_array_insert(current_node->values, current_node->size, v);
        if (insert == -1) { // 超限
            ret = current_node->values + current_node->size / 2;
            // 分裂当前节点

        }else{
            current_node->size++;
        }
    } else { // 非叶节点
        // 找到要插入的子节点
        int position = 0;
        while(current_node->values[position] < *v) {
            position++;
        }
        // 将新值插入position指向的子节点
        ret = insert(current_node->children[position], v);
        if(ret)
        {
            // 如果子节点超限, 则分裂
            split(current_node, position);
        }
    }
    return ret;
}

void print_tree(pTreeNode t){
    if(t->is_leaf){
        for (int i = 0; i < t->size; ++i) {
            printf("%d,", t->values[i]);
        }
        return;
    }
    for (int i = 0; i <= t->size; ++i) {
        print_tree(t->children[i]);
    }
    for (int i = 0; i < t->size; ++i) {
        printf("%d,", t->values[i]);
    }
    printf("\n");
}

int main() {
    int values[] = { 4, 7, 9, 5};
    pTreeNode tree = createNode();
    insert(tree, values + 0);
    insert(tree, values + 1);
    insert(tree, values + 2);
    insert(tree, values + 3);
    print_tree(tree);
    system("pause");
}