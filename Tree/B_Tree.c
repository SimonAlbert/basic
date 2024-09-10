//
// Created by Simon on 2023/4/17.
//
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
#include <stdio.h>
#include <stdlib.h>


// 关键字类型
typedef int value_type;
#define M 5 // 阶数(最大子节点数)
const int MAX_VALUE_COUNT = M - 1; // 最大关键字数
int MIN_VALUE_COUNT = MAX_VALUE_COUNT / 2; // 最小关键字数
// 除根结点外的其它结点的最小关键字数为MIN_VALUE_COUNT, 最大关键字数为MAX_VALUE_COUNT; 最小子节点数为MIN_VALUE_COUNT+1, 最大子节点数为MAX_VALUE_COUNT+1
// 节点定义
typedef struct TreeNode {
    int size;
    value_type* values;
    struct TreeNode** children;
    struct TreeNode* parent;
    int parent_index;
    int is_leaf;
    int is_root;
} *pTreeNode, TreeNode;

pTreeNode createNode();
void exchange(value_type *a, value_type *b);
int value_array_insert(value_type *arr, const int count, const value_type *v);
int value_directed_insert(value_type *arr, int num, const value_type *v, int dis_index);
int node_directed_insert(pTreeNode *arr, int num, pTreeNode v, int dis_index);
void split(pTreeNode parent_node, int index);
pTreeNode find(pTreeNode t, value_type *v);
int insert(pTreeNode current_node, value_type *v);
void insert_tree(pTreeNode root, value_type *v);

// 创建节点
pTreeNode createNode(){
    pTreeNode pNode = (pTreeNode) malloc(sizeof(TreeNode));
    pNode->size = 0;
    pNode->values = (value_type*) malloc(sizeof(value_type) * M);
    // 指向子节点的指针数组, 阶数不会很大, 空间浪费小
    pNode->children = (pTreeNode*) malloc(sizeof(pTreeNode) * M);
    for (int i = 0; i < M; ++i) {
        //设置默认值
        pNode->values[i] = 0;
        pNode->children[i] = NULL;
    }
    pNode->parent = NULL;
    pNode->is_leaf = 1;
    pNode->is_root = 0;
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
int value_array_insert(value_type *arr, const int count, const value_type *v) {
    // 节点满
    if (count > MAX_VALUE_COUNT) return 1;
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
int value_directed_insert(value_type *arr, int num, const value_type *v, int dis_index) {
    // 节点关键字数组已满
    if(num > MAX_VALUE_COUNT){
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
    if(num > M){
        return -1;
    }
    for (int i = num; i > dis_index; --i) {
        arr[i] = arr[i - 1];
    }
    arr[dis_index] = v;
    return 0;
}

/*
 * 第一种拆分方法, 不使用节点内部的parent指针
 * 由父节点进行操作, 对父节点的第i个子节点进行拆分
 */
void split(pTreeNode parent_node, int index)
{
    // 先创建两个新节点
    pTreeNode child1 = createNode();
    pTreeNode child2 = createNode();

    // 非叶子节点
    // 取待分裂节点
    pTreeNode split_node = parent_node->children[index];
    printf("分裂节点:%d\n", split_node);
    // 新节点size
    child1->size = MIN_VALUE_COUNT;
    child2->size = M - (MIN_VALUE_COUNT + 1);
    child1->is_leaf = split_node->is_leaf;
    child2->is_leaf = split_node->is_leaf;
    // M=5 min=2 max=4
    // 0, 1, 2, 3, 4
    // 关键字分配
    // split_node->values的[0, MIN_VALUE_COUNT - 1]放入child1
    // [0, 2)
    for (int i = 0; i < MIN_VALUE_COUNT; ++i) {
        child1->values[i] = split_node->values[i];
    }
    // split_node->values的[MIN_VALUE_COUNT + 1, MAX_VALUE_COUNT]放入child2
    // [3, 4]
    for (int i = MIN_VALUE_COUNT + 1, j = 0; i <= MAX_VALUE_COUNT; ++i, ++j) {
        child2->values[j] = split_node->values[i];
    }
    // 子节点分配
    // split_node->children的[0, MIN_VALUE_COUNT]放入child1
    // [0, 2]
    for (int i = 0; i <= MIN_VALUE_COUNT; ++i) {
        child1->children[i] = split_node->children[i];
    }
    // split_node->children的[MIN_VALUE_COUNT + 1, M]放入child2
    // [3, 4]
    for (int i = MIN_VALUE_COUNT + 1, j = 0; i <= M; ++i, ++j) {
        child2->children[j] = split_node->children[i];
    }

    // 上溢
    // split_node->values[MIN_VALUE_COUNT]插入到父节点的values[index]
    value_directed_insert(parent_node->values, parent_node->size, split_node->values + MIN_VALUE_COUNT, index);
    // 当前节点size++
    parent_node->size++;
    // child1替换当前节点的children[index], child2插入到当前节点的children[index+1]
    parent_node->children[index] = child1;
    node_directed_insert(parent_node->children, parent_node->size, child2, index + 1);
}
void print_spaces(int deep){
    for (int i = 0; i < deep; ++i) {
        printf("    ");
    }
}
void print(pTreeNode t, int deep, int num){
    print_spaces(deep);
    if(t->is_root) {
        printf("根节点%d: ", t);
    } else {
        printf("%d号子节点%d > ", num, t);
    }
    for (int i = 0; i < t->size; ++i) {
        printf("%d ", t->values[i]);
    }
    printf("\n");
    if(!t->is_leaf) {
        for (int i = 0; i <= t->size; ++i) {
            print(t->children[i], deep + 1, i);
        }
    }
}
// TODO 保留搜索路径
pTreeNode find(pTreeNode t, value_type *v) {
    if(t->is_leaf){
        value_type tmp;
        for (int i = 0; i < t->size; ++i) {
            tmp = t->values[i];
            if (tmp == *v) {
                printf("%d元素定位: %d节点下标为%d的元素\n", *v, t, i);
                return t;
            }
        }
        printf("没有查找到元素%d\n", *v);
        return NULL;
    }
    // 小于最小
    if(t->values[0] > *v){
        return find(t->children[0], v);
    }
    // 找到新value的位置
    value_type tmp;
    for (int i = 0; i < t->size - 1; ++i) {
        tmp = t->values[i];
        if (tmp == *v) {
            printf("%d元素定位: %d节点下标为%d的元素\n", *v, t, i);
            return t;
        } else if (tmp < *v && *v < t->values[i + 1]) {
            return find(t->children[i + 1], v);
        }
    }
    if(*v == t->values[t->size - 1]){
        printf("%d元素定位: %d节点下标为%d的元素\n", *v, t, t->size - 1);
        return t;
    }

    // 大于最大
    return find(t->children[t->size], v);
}
// 传入节点下属最大值所在节点
pTreeNode find_max(pTreeNode cur){
    if(cur->is_leaf){
        return cur;
    } else {
        return find_max(cur->children[cur->size - 1]);
    }
}
// 传入节点下属最小值所在节点
pTreeNode find_min(pTreeNode cur){
    if(cur->is_leaf){
        return cur;
    } else {
        return find_min(cur->children[0]);
    }
}
// 前驱节点, 比自己小的最大的 TODO 保留搜索路径
pTreeNode predecessor(pTreeNode t, value_type *v) {
    if(t->is_leaf){
        return t;
    } else {
        // 小于最小
        if(*v <= t->values[0]){
            return predecessor(t->children[0], v);
        }
        // 大于最大
        if(*v > t->values[t->size - 1]){
            return predecessor(t->children[t->size], v);
        }
        // 扫描定位
        for (int i = 0; i < t->size - 1; ++i) {
            if (t->values[i] < *v && *v <= t->values[i + 1]) {
                return predecessor(t->children[i + 1], v);
            }
        }
    }
    return NULL;
}
// 后继节点, 比自己大的最小的 TODO 保留搜索路径
pTreeNode successor(pTreeNode t, value_type *v) {
    if(t->is_leaf){
        return t;
    } else {
        // 小于最小
        if(*v < t->values[0]){
            return successor(t->children[0], v);
        }
        // 大于最大
        if(*v >= t->values[t->size - 1]){
            return successor(t->children[t->size], v);
        }
        // 扫描定位
        for (int i = 0; i < t->size - 1; ++i) {
            if (t->values[i] <= *v && *v < t->values[i + 1]) {
                return successor(t->children[i + 1], v);
            }
        }
    }
    return NULL;
}


// 节点合并
int merge(){

}

/** 删除值 还是递归吧
 * @return
 * 1 需要合并
 * 0 直接删除
 * 如果叶节点够, 直接删
 * 如果不够, 找左右兄弟要
 * 如果左右兄弟都不够, 删除后返回0
 */
int delete(pTreeNode root, value_type *v){
    if(root->is_leaf && root->is_root){
        // 只有根节点
        for (int i = 0; i < root->size; ++i) {
            // 匹配到值, 删除这个值, 缩短数组, 不做其他处理
            if(*v == root->values[i]){
                for (int j = i; j < root->size; ++j) {
                    root->values[j] = root->values[j + 1];
                }
                root->size -= 1;
                return 0;
            }
        }
        printf("未找到元素\n");
        return root->size < MIN_VALUE_COUNT;
    } else if ( !root->is_leaf ) {
        // 需要进行递归删除
        pTreeNode cur = root;
        // 执行删除操作的子节点
        pTreeNode dist_child = NULL;
        // 执行删除操作的子节点的下标
        int dist_child_index = -1;
        // 小于最小 扩展搜索最左
        if(*v < cur->values[0]){
            dist_child_index = 0;
        } else if(*v > cur->values[cur->size - 1]){
            // 大于最大 扩展搜索最右
            dist_child_index = cur->size;
        } else {
            // 扫描定位
            for (int i = 0; i < cur->size; ++i) {
                if (cur->values[i] == *v) {
                    // 匹配成功, 删除前驱节点(必然存在)最大值
                    // TODO 如果不考虑效率, 可以换值后继续递归, 如果考虑效率, 可以单独实现搜索删除前驱节点的方法
                    pTreeNode pre = find_max(cur->children[i]);
                }
                if (cur->values[i] < *v && *v < cur->values[i + 1]) {
                    dist_child_index = i;
                }
            }
        }

        dist_child = cur->children[dist_child_index];
        if(delete(dist_child, v)) {

        }
        return 0;
    } else if ( root->is_leaf ) {

    }
}

/*
 * 只能插入叶子节点
 * return 0: 成功 1: 超限
 */
int insert(pTreeNode current_node, value_type *v) {
    printf("%d插入节点%d\n", *v, current_node);
    if (current_node->is_leaf) { // 叶节点
        value_array_insert(current_node->values, current_node->size, v);
        current_node->size++;
    } else {
        // 非叶节点
        // 找到要插入的关键字下标position
        int position = 0;
        while(current_node->values[position] < *v && position < current_node->size) {
            position++;
        }
        // 将新关键字插入position指向的子节点
        int ret = insert(current_node->children[position], v);
        if(ret){
            // 如果子节点超限, 则分裂
            split(current_node, position);
        }
    }
    return current_node->size > MAX_VALUE_COUNT;
}

// 处理根节点
void insert_tree(pTreeNode root, value_type *v) {
    int leaf = 0;
    //只有根节点
    if(root->is_root && root->is_leaf && root->size <= MAX_VALUE_COUNT){
        // 插入值
        value_array_insert(root->values, root->size, v);
        root->size += 1;
        leaf = 1;
    } else {
        insert(root, v);
        leaf = 0;
    }
    if(root->size > MAX_VALUE_COUNT){
        printf("分裂根节点\n");
        // 先创建两个新节点
        pTreeNode child1 = createNode();
        pTreeNode child2 = createNode();
        child1->is_leaf = leaf;
        child2->is_leaf = leaf;
        // [0, 1, 2, 3, 4]
        // [0, 1] [2] [3, 4]
        // [0, 1]
        for (int i = 0; i < MIN_VALUE_COUNT; ++i) {
            child1->values[i] = root->values[i];
        }
        child1->size = MIN_VALUE_COUNT;
        // [3, 4]
        for (int i = MIN_VALUE_COUNT + 1, j = 0; i <= MAX_VALUE_COUNT; ++i, ++j) {
            child2->values[j] = root->values[i];
        }
        child2->size = M - MIN_VALUE_COUNT - 1;

        // 子节点分配
        // split_node->children的[0, MIN_VALUE_COUNT]放入child1
        // [0, 2]
        for (int i = 0; i <= MIN_VALUE_COUNT; ++i) {
            child1->children[i] = root->children[i];
        }
        // split_node->children的[MIN_VALUE_COUNT + 1, M]放入child2
        // [3, 4]
        for (int i = MIN_VALUE_COUNT + 1, j = 0; i <= M; ++i, ++j) {
            child2->children[j] = root->children[i];
        }

        // [2]
        root->values[0] = root->values[MIN_VALUE_COUNT];
        root->size = 1;
        root->is_leaf = 0;
        // 建立连接
        root->children[0] = child1;
        root->children[1] = child2;
    }
}

int main() {
    value_type values[] = { 4, 7, 9, 5, 11, 3, 2, 10, 100, 78, 66, 80,
                            88, 30, 35, 40, 60, 59, 110, 120, 130, 140,
                            150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250};
//    value_type values[] = { 1,2,3,4,5,6,7,8};
    pTreeNode root = createNode();
    root->is_root = 1;
    for (int i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        insert_tree(root, values + i);
        print(root, 0, 0);
    }
    int a = 78;
    pTreeNode position = find(root, &a);

    value_type find_value = 40;
    pTreeNode pre = predecessor(root, &find_value);
    printf("前驱节点: %d\n", pre);
    pTreeNode suc = successor(root, &find_value);
    printf("后继节点: %d\n", suc);
}