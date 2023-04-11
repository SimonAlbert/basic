//
// Created by Simon on 2023/4/11.
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

void print_node(node_ptr t) {
    printf("%d", t->val);
}

// 根左右
void pre_order_print(node_ptr t) {
    if (!t) return;
    print_node(t);
    pre_order_print(lc(t));
    pre_order_print(rc(t));
}

// 左根右
void in_order_print(node_ptr t) {
    if (!t) return;
    in_order_print(lc(t));
    print_node(t);
    in_order_print(rc(t));
}

// 左右根
void post_order_print(node_ptr t) {
    if (!t) return;
    post_order_print(lc(t));
    post_order_print(rc(t));
    print_node(t);
}

// 插入
void insert(T *v, node_ptr t) {
    if (!t) {
        t = (node_ptr) malloc(sizeof(node));
        t->val = *v;
        t->count = 1;
        t->left = NULL;
        t->right = NULL;
        return;
    }
    if (*v == t->val) {
        t->count++;
    } else if (*v < t->val) {
        insert(v, lc(t));
    } else if (*v > t->val) {
        insert(v, rc(t));
    }
}
node_ptr find_min(node_ptr t){
    if(!lc(t)) return t;
    return find_min(lc(t));
}
node_ptr find_max(node_ptr t){
    if(!rc(t)) return t;
    return find_max(rc(t));
}
//
void delete(T *v, node_ptr t){

}