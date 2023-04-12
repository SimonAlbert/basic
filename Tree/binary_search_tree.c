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
    struct node *left;
    struct node *right;
} *node_ptr, node;
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
        printf("        ");
    }
    printf("%d\n", t->val);

    linkedlist_format_output(lc(t), deepth+1);
}
// 插入
node_ptr insert(node_ptr t, T *v) {
    if (end(t)) {
        node_ptr p = (node_ptr)malloc(sizeof(node));
        p->val = *v;
        p->count = 1;
        p->left = NULL;
        p->right = NULL;
        return p;
    }
    if (*v == t->val) {
        t->count++;
    } else if (*v < t->val) {
        set_lc(t, insert(lc(t), v));
    } else if (*v > t->val) {
        set_rc(t, insert(rc(t), v));
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
node_ptr delete_val(node_ptr t, T *v);

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
                node_ptr _t = find_max(lc(t));
                set_rc(_t, t->right);
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
        set_lc(t, delete_val(lc(t), v));
    } else {
        set_rc(t, delete_val(rc(t), v));
    }
    return t;
}

// 左右璇 在链表的实现中难以避免发生节点复制
/* 节点A右旋
 * 需要操作三个节点: A, A的左子树B, B的右子树E
 *               R                    R
 *               |                    |
 *               A                    B
 *             /  \                 /  \
 *            B    C     ===>      D    A
 *          /  \  /  \                 / \
 *         D   E F    G               E   C
 *                                       /  \
 *                                      F    G
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
    node_ptr root = NULL;
    T in = 1;
    while(in != '0'){
        scanf("%d", &in);
        if (!in) { break; }
        root = insert(root, &in);
        print(root);
    }
    T d = 3;
//    root = delete_val(root, &d);
    rotate_right(root);
    print(root);
    rotate_left(rc(rc(root)));
    print(root);
    return 0;
}