#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define OVERFLOW 1 

int default_size = 4;

typedef struct {
    int a;
} Elem;

int compare(Elem* a,Elem* b){
    return a->a==b->a;
}

typedef struct {
    Elem* arr;
    int max_size;
    int length;
}Sqlist;

// 初始化空表
void InitList(Sqlist* list){
    // 初始化数组
    list->arr = (Elem*)calloc(default_size, sizeof(Elem));//calloc 元素个数 元素size
    if (!list->arr) {
        exit(OVERFLOW);
    }
    // 最大长度
    list->max_size = default_size;
    // 已用长度
    list->length = 0;
    // 复习 `指针`->属性;`变量`.属性
}

// 销毁数组
void DestoryList(Sqlist* list){
    free(list->arr);
    free(list);
}

// 清空数组
void ClearList(Sqlist* list){
    // 数组 先释放旧的,再申请新的(这样操作好像比较抵低效)
    free(list->arr);
    list->arr = calloc(default_size, sizeof(Elem));
    if (!list->arr) {
        exit(OVERFLOW);
    }
    // 最大长度
    list->max_size = default_size;
    // 已用长度
    list->length = 0;
}

int ListEmpty(Sqlist* list){
    return list->length==0;
} 

int ListLength(Sqlist* list){
    return list->length;
}

void GetElem(Sqlist* list, int i, Elem* elem){
    elem = list->arr + i*sizeof(Elem);
}

int Locate(Sqlist* list, Elem* e, int (* pcompare)(Elem*, Elem*)){
    for (int i = 0; i < list->length; i++) {
        if (pcompare((list->arr + i*sizeof(Elem)), e)) {
            return i;
        }
    }
    return 0;
}

void Prior(Sqlist* list, Elem* cur_e, Elem* pre_e, int (* pcompare)(Elem*, Elem*)){
    for (int i = 1; i < list->length; i++) {
        if (pcompare((list->arr + i*sizeof(Elem)), cur_e)) {
            pre_e = list->arr + (i-1)*sizeof(Elem);
        }
    }
}


void Next(Sqlist* list, Elem* cur_e, Elem* pre_e, int (* pcompare)(Elem*, Elem*)){
    for (int i = 0; i < list->length-1; i++) {
        if (pcompare((list->arr + i*sizeof(Elem)), cur_e)) {
            pre_e = list->arr + (i+1)*sizeof(Elem);
        }
    }
}

// 设置元素
void ListSet(Sqlist* list,int i, Elem* e){
    list->arr[list->length] = *e;
}

// 添加元素
void ListInsert(Sqlist* list, Elem* e){
    if (list->length < list->max_size) {
        list->arr[list->length] = *e;
        list->length++;
    }else{
        list->arr = realloc(list->arr,list->max_size*2);
        if (list->arr) {
            list->arr[list->length] = *e;
            list->length++;
            list->max_size*=2;
        }else{
            exit(OVERFLOW);
        }
    }
}

// 输出数组
void ListPrint(Sqlist* list){
    for (int i = 0; i < list->length; i++) {
        printf("%d\n", list->arr[i].a);
    }
}
int main(){
    Sqlist list;
    InitList(&list);
    Elem e;
    e.a = 1;
    ListInsert(&list, &e);
    e.a = 2;
    ListInsert(&list, &e);
    e.a = 3;
    ListInsert(&list, &e);
    e.a = 4;
    ListInsert(&list, &e);
    e.a = 5;
    ListInsert(&list, &e);
    ListPrint(&list);
    printf("%d\n", list.max_size);
    return 0;
}
