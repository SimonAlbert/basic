//
// Created by Simon on 2023/4/11.
// search tree implemented by LinkedList
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef int T;

typedef struct node {
    T val;
    int count;
    int height;
    struct node *left;
    struct node *right;
} *node_ptr, node;
node_ptr root = NULL;
void rotate_right(node_ptr);
void rotate_left(node_ptr);
node_ptr lc(node_ptr t) {
    return t?t->left:NULL;
}
node_ptr rc(node_ptr t) {
    return t?t->right:NULL;
}

int height(node_ptr t){
    return t?t->height:0;
}
void updateHeight(node_ptr t){
    t->height = height(lc(t)) > height(rc(t))? height(lc(t)) + 1: height(rc(t)) + 1;
}
void set_lc(node_ptr t, node_ptr l){
    t->left = l;
    updateHeight(t);

}
void set_rc(node_ptr t, node_ptr r){
    t->right = r;
    updateHeight(t);
}
int end(node_ptr t) {
    return !t;
}

int balanceFactor(node_ptr t){
    return abs(height(lc(t)) - height(rc(t)));
}
void _situtation(node_ptr t, int deepth, int* lr_log){
    if(height(lc(t)) > height(rc(t))){
        if(deepth < 2){
            _situtation(lc(t), deepth + 1, lr_log);
        }
        *lr_log *= 2;
    }else if(height(lc(t)) < height(rc(t))){
        if(deepth < 2){
            _situtation(rc(t), deepth + 1, lr_log);
        }
        *lr_log = *lr_log * 2 + 1;
    }
}
int situtation(node_ptr t){
    if(height(t) < 2 || balanceFactor(t) < 1){
        return -1;
    }
    int deepth = 0,lr_log=0;
    _situtation(t, deepth, &lr_log);
    return lr_log;
}
void print_node(node_ptr t) {
    printf("{%d}", t->val);
}

// 根左右
void pre_order_print(node_ptr t) {
    if (end(t)) return;
    print_node(t);
    pre_order_print(lc(t));
    pre_order_print(rc(t));
}

// 左根右
void in_order_print(node_ptr t) {
    if (end(t)) return;
    in_order_print(lc(t));
    print_node(t);
    in_order_print(rc(t));
}

// 左右根
void post_order_print(node_ptr t) {
    if (end(t)) return;
    post_order_print(lc(t));
    post_order_print(rc(t));
    print_node(t);
}

void linkedlist_format_output(node_ptr t, int deepth){
    if (end(t)) {
        for (int i = 0; i < deepth; ++i) {
            printf("       ");
        }
        printf("N\n");
        return;
    }
    linkedlist_format_output(rc(t), deepth+1);

    for (int i = 0; i < deepth; ++i) {
        printf("       ");
    }
    printf("%d:%d\n", t->val, height(t));

    linkedlist_format_output(lc(t), deepth+1);
}

void print(node_ptr root_){
    printf("========format output========\n");
    linkedlist_format_output(root_, 0);
    printf("=============================\n");
}
// 插入
node_ptr insert(node_ptr t, T *v, int* lr_log) {
    if (end(t)) {
        node_ptr p = (node_ptr)malloc(sizeof(node));
        p->val = *v;
        p->count = 1;
        p->height = 1;
        p->left = NULL;
        p->right = NULL;
        return p;
    }
    if (*v == t->val) {
        t->count++;
        return t;
    } else if (*v < t->val) {
        set_lc(t, insert(lc(t), v, lr_log));
        *lr_log = *lr_log * 2;
    } else if (*v > t->val) {
        set_rc(t, insert(rc(t), v, lr_log));
        *lr_log = *lr_log * 2 + 1;
    }
    int flag = 0b11&*lr_log; // 00:LL 10:LR 01:RL 11:RR
    if(balanceFactor(t) > 1){
        // 偏左
        if(flag == 0b00){
            rotate_right(t);
        }else if (flag == 0b10){
            rotate_left(lc(t));
            rotate_right(t);
        }
        // 偏右
        else if(flag == 0b11){
            rotate_left(t);
        }else if (flag == 0b01){
            rotate_right(rc(t));
            rotate_left(t);
        }
    }
    return t;
}

node_ptr find_min(node_ptr t) {
    if (!lc(t)) return t;
    return find_min(lc(t));
}

node_ptr find_max(node_ptr t) {
    if (!rc(t)) return t;
    return find_max(rc(t));
}
node_ptr delete_max(node_ptr t){
    if(!rc(t)) {// 到达要删除的节点
        node_ptr  l = lc(t);
        free(t);
        return l;
    }
    set_rc(t, delete_max(rc(t)));
    return t;
}
node_ptr delete_min(node_ptr t){
    if(!lc(t)) {// 到达要删除的节点
        if(rc(t)){
            return rc(t);
        }
        return NULL;
    }
    set_lc(t, delete_min(lc(t)));
    return t;
}
node_ptr find_val(node_ptr t, T *v) {
    if (end(t)) return NULL;
    if (*v == t->val) {
        return t;
    } else if (*v < t->val) {
        return find_val(lc(t), v);
    } else {// *v > t->val
        return find_val(rc(t), v);
    }
}


// 使用链表实现
node_ptr delete_val(node_ptr t, T *v) {
    if (end(t)) return NULL;
    if (*v == t->val) {
        if (t->count > 1) {
            t->count--;
        } else {
            node_ptr l = lc(t);
            node_ptr r = rc(t);
            node_ptr child = l ? l : r;
            if (l && r) {
                /*       |                      |
                 *       t        ===>     predecessor
                 *      / \                    / \
                 *     l   r                  l   r
                 *      \                      \
                 *       lr                    lr
                 *      /  \                  /
                 *     lrl  predecessor     lrl
                 */
                node_ptr pre = find_max(l);
                *t = *pre;
                set_rc(t, r);
                set_lc(t, l);
                set_lc(t, delete_max(l));
                return t;
            } else if (!child) {
                free(t);
                return NULL;
            } else {
                free(t);
                return child;
            }
        }
    } else if (*v < t->val) {
        set_lc(t, delete_val(lc(t), v));
    } else {
        set_rc(t, delete_val(rc(t), v));
    }
    int flag = situtation(t);
    printf("%d situation:%d\n", t->val, flag);
    print(t);
    if(balanceFactor(t) > 1){
        // 偏左
        if(flag == 0b00){
            rotate_right(t);
        }else if (flag == 0b10){
            rotate_left(lc(t));
            rotate_right(t);
        }
            // 偏右
        else if(flag == 0b11){
            rotate_left(t);
        }else if (flag == 0b01){
            rotate_right(rc(t));
            rotate_left(t);
        }
    }
    return t;
}

// 左右璇 在链表的实现中难以避免发生节点复制
/* 节点A右旋
 * 需要操作三个节点: A, A的左子树B, B的右子树E
 *                R                    R
 *                |                    |
 *                A                    B
 *              /  \                  / \
 *             B    C     ===>       D   A
 *            / \  / \                  / \
 *           D  E F   G                E   C
 *                                        / \
 *                                       F   G
 */
void rotate_right(node_ptr newB_oldA){
    printf("before right rotate: %d\n", newB_oldA->val);
    print(root);
//    print(newB_oldA);
    // 不修改指针, 修改指针指向的值
    node_ptr oldB = lc(newB_oldA);
    node_ptr E = rc(oldB);
    node_ptr newA = (node_ptr) malloc(sizeof(node));
    *newA = *newB_oldA;
    *newB_oldA = *oldB;
    set_lc(newA, E);
    set_rc(newB_oldA, newA);
    printf("after right rotate\n");
//    print(newB_oldA);
    print(root);
}

/* 节点A左旋
 * 需要操作三个节点: A, A的右子树C, C的左子树F
 *               R                    R
 *               |                    |
 *               A                    C
 *             /   \                /  \
 *            B     C     ===>     A    G
 *          /  \  /  \           /  \
 *         D   E F    G         B    F
 *                            /  \
 *                           D    E
 */
void rotate_left(node_ptr newC_oldA){
    printf("before left rotate: %d\n", newC_oldA->val);
    print(root);
//    print(newC_oldA);
    node_ptr oldC = rc(newC_oldA);
    node_ptr F = lc(oldC);
    node_ptr newA = (node_ptr) malloc(sizeof(node));
    *newA = *newC_oldA;
    *newC_oldA = *oldC;
    set_rc(newA, F);
    set_lc(newC_oldA, newA);
    printf("after left rotate\n");
//    print(newC_oldA);
    print(root);
}

int main()
{
//    node_ptr root = NULL;
    T in = 1;
    int lr_log=0b00;
//    int x[16] = {5,3,6,7,4,2,1,-5,-3, -1, -7,-19, -99, -110, -111, -105};
    int x[6] = {5, 3, 7, 4, 2, 6};
    for (int i = 0; i < 6; ++i) {
        root = insert(root, x+i, &lr_log);
    }
//    while(in != '0'){
//        scanf("%d", &in);
////        system("cls");
//        if (!in) { break; }
//        printf("before insert\n");
//        print(root);
//        lr_log=0b00;
//        root = insert(root, &in, &lr_log);
//        printf("after insert\n");
//        print(root);
//    }
    while(in != '0'){
        scanf("%d", &in);
        if (!in) { break; }
        printf("before delete\n");
        print(root);
        root = delete_val(root, &in);
        printf("after delete\n");
        print(root);
    }
    return 0;
}