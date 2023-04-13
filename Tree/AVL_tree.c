//
// Created by Simon on 2023/4/11.
// search tree implemented by LinkedList
//
#include <stdio.h>
#include <stdlib.h>

typedef int T;

typedef struct node {
    T val;
    int count;
//    int size;
//    int height;
    int bf;
    struct node *left;
    struct node *right;
} *node_ptr, node;
void rotate_right(node_ptr);
void rotate_left(node_ptr);
node_ptr lc(node_ptr t) {
    return t->left;
}
node_ptr rc(node_ptr t) {
    return t->right;
}

void set_lc(node_ptr t, node_ptr l){
    t->left = l;

}
void set_rc(node_ptr t, node_ptr r){
    t->right = r;
}
int end(node_ptr t) {
    return !t;
}

//void updateSize(node_ptr t){
//    t->size = lc(t)->size + rc(t)->size;
//}

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
    printf("%d\n", t->val);

    linkedlist_format_output(lc(t), deepth+1);
}
// 插入
node_ptr insert(node_ptr t, T *v, int* lr_log) {
    if (end(t)) {
        node_ptr p = (node_ptr)malloc(sizeof(node));
        p->val = *v;
        p->count = 1;
//        p->size = 1;
        p->bf = 0;
        p->left = NULL;
        p->right = NULL;
        return p;
    }
    if (*v == t->val) {
        t->count++;
    } else if (*v < t->val) {
        *lr_log = *lr_log * 2;
        t->bf --;
        set_lc(t, insert(lc(t), v, lr_log));
    } else if (*v > t->val) {
        *lr_log = *lr_log * 2 + 1;
        t->bf ++;
        set_rc(t, insert(rc(t), v, lr_log));
    }
    int flag = 0b11&*lr_log; // 00:LL 01:LR 10:RL 11:RR
    if(t->bf < -1){
        // 偏左
        if(flag == 0b00){
            rotate_right(t);
        }else if (flag == 0b01){
            rotate_left(lc(t));
            rotate_right(t);
        }
        t->bf += 2;
    }else if(t->bf > 1){
        // 偏右
        if(flag == 0b11){
            rotate_left(t);
        }else if (flag == 0b10){
            rotate_right(rc(t));
            rotate_left(t);
        }
        t->bf -= 2;
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
node_ptr delete_val(node_ptr t, T *v, int* lr_log);

node_ptr delete_val(node_ptr t, T *v, int* lr_log) {
    if (end(t)) return NULL;
    if (*v == t->val) {
        if (t->count > 1) {
            t->count--;
        } else {
            node_ptr l = lc(t);
            node_ptr r = rc(t);
            node_ptr child = l ? l : r;
            if (l && r) {
                node_ptr _t = find_max(lc(t));
                set_rc(_t, rc(t));
                free(t);
                return _t;
            } else if (!child) {
                free(t);
                return NULL;
            } else {
                free(t);
                return child;
            }
        }
    } else if (*v < t->val) {
        set_lc(t, delete_val(lc(t), v, lr_log));
        *lr_log = *lr_log * 2 + 1;
        t->bf ++;
    } else {
        set_rc(t, delete_val(rc(t), v, lr_log));
        *lr_log = *lr_log * 2;
        t->bf --;
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
    // 不修改指针, 修改指针指向的值
    node_ptr oldB = lc(newB_oldA);
    node_ptr E = rc(oldB);
    node_ptr newA = (node_ptr) malloc(sizeof(node));
    *newA = *newB_oldA;
    *newB_oldA = *oldB;
    set_lc(newA, E);
    set_rc(newB_oldA, newA);
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
    node_ptr oldC = rc(newC_oldA);
    node_ptr F = lc(oldC);
    node_ptr newA = (node_ptr) malloc(sizeof(node));
    *newA = *newC_oldA;
    *newC_oldA = *oldC;
    set_rc(newA, F);
    set_lc(newC_oldA, newA);
}

void print(node_ptr root){
    printf("========format output========\n");
    linkedlist_format_output(root, 0);
    printf("=============================\n");
}
int main()
{
    int _i = 0b0101;
    printf("%d\n", _i);
    node_ptr root = NULL;
    T in = 1;
    while(in != '0'){
        scanf("%d", &in);
        if (!in) { break; }
        int lr_log=0b00;
        root = insert(root, &in, &lr_log);
        print(root);
    }
//    T d = 3;
//    root = delete_val(root, &d, 0);
//    rotate_right(root);
//    print(root);
//    rotate_left(rc(rc(root)));
//    print(root);
    return 0;
}