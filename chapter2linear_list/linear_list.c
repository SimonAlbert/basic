#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define OVERFLOW 1 

int default_size = 4;

typedef struct {
    int *a;
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

// 添加元素
void ListInsert(Sqlist* arr,int i, Elem e){
    arr->arr[arr->length] = e;
} 

int main(){
    Sqlist list;
    InitList(&list);
    printf("%d", list.max_size);
    return 0;
}
