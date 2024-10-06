//
// Created by DUMOND on 27/09/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "lists.h"

DList* createList(){
    DList* list = (DList*)calloc(1, sizeof(DList));
    if (!list) return NULL;
    list->sentinel = (DListNode*)calloc(1, sizeof(DListNode));
    list->sentinel->next = list->sentinel;
    list->sentinel->previous = list->sentinel;
	list->total = 0;
    return list;
}

DListNode* createNode(void* value) {
    DListNode *node = (DListNode *) calloc(1, sizeof(DListNode));
    if (!node) return NULL;
    node->value = value;
    return node;
}

void print(DList* list, void (*printData)(void*)){
    //printf("(size=%d) : ", list->size);
    DListNode* current = list->sentinel->next;
    while (current!=list->sentinel){
        printData(current->value);
        current = current->next;

    }
    //printf("[FIN]\n");
}

void insertFirst(DList* list, DListNode* node){
    node->previous = list->sentinel;
    node->next = list->sentinel->next;
    node->previous->next = node;
    node->next->previous = node;
    list->size++;
}

void insertLast(DList* list, DListNode* node){
    node->previous = list->sentinel->previous;
    node->next = list->sentinel;
    node->previous->next = node;
    node->next->previous = node;
    list->size++;
}

void* pop(DList* list){
    if (!list->size) return NULL;
    DListNode* popNode = list->sentinel->next;
    popNode->next->previous =  list->sentinel;
    list->sentinel->next = popNode->next;
    void * value = popNode->value;
    free(popNode);
    list->size --;
    return value;
}

DListNode* pop2(DList* list){
    if (!list->size) return NULL;
    DListNode* popNode = list->sentinel->next;
    popNode->next->previous =  list->sentinel;
    list->sentinel->next = popNode->next;
    list->size --;
    return popNode;
}

DListNode* firstOf(DList* list, void* value, int (*compareData)(void*, void*)){
    if(!list->size) return NULL;
    DListNode* current = list->sentinel->next;
    while (current!=list->sentinel){
        if(!compareData(current->value,value)){
            return current;
        }
        current = current->next;

    }
    return NULL;
}

DListNode* lastOf(DList* list, void* value, int (*compareData)(void*, void*)){
    if(!list->size) return NULL;
    DListNode* current = list->sentinel->previous;
    while (current!=list->sentinel){
        if(!compareData(current->value,value)){
            return current;
        }
        current = current->previous;

    }
    return NULL;
}

DList* insertTable(DList* list, int table[], int reverse, int size){
    if (!list) return NULL;
    if (reverse>0) {
        DListNode* current = list->sentinel;
        for (int i = 0; i < size; i ++) {
            DListNode* node = createNode(table + i);
            insertFirst(list, node);
        }
        return list;
    } else {
        DListNode* current = list->sentinel;
        for (int i = 0; i < size; i ++) {
            DListNode* node = createNode(table+i);
            insertLast(list, node);
        }
        return list;
    }
}


void insertSorted(DList* list, DListNode* node, int reverse, int (*compareData)(void*, void*)){
    if (reverse>0) {
        if(!list->size) return insertFirst(list, node);
        DListNode* current = list->sentinel->next;
        while (current!=list->sentinel){
            if(compareData(current->value,node->value)<0){
                current->previous->next = node;
                node->previous = current->previous;
                node->next = current;
                current->previous = node;
                list->size++;
                return;
            }
            current = current->next;
        }
        if(current==list->sentinel){
            node->previous = current->previous;
            node->next = current;
            current->previous->next = node;
            current->previous = node;
            list->size ++;
            return;
        }
    } else {
        if(!list->size) return insertFirst(list, node);
        DListNode* current = list->sentinel->next;
        while (current!=list->sentinel){
            if(compareData(current->value,node->value)>0){
                current->previous->next = node;
                node->previous = current->previous;
                node->next = current;
                current->previous = node;
                list->size++;
                return;
            }
            current = current->next;
        }
        if(current==list->sentinel){
            node->previous = current->previous;
            node->next = current;
            current->previous->next = node;
            current->previous = node;
            list->size ++;
            return;
        }
    }
}

DListNode* get(DList* list, int index)
{
    if (index > list->size-1 || -index > list->size) return NULL;
    if (index < 0){
        DListNode* current = list->sentinel->previous;
        for (int i = -1; i > index; i--){
            current=current->previous;
        }
        return current;
    }
    DListNode* current = list->sentinel->next;
    for (int i = 0; i < index; i++){
        current=current->next;
    }
    return current;
}

void insertBefore(DList* list, DListNode* node, int index){
    DListNode* nextNode = get(list, index);
    nextNode->previous->next = node;
    node->previous = nextNode->previous;
    node->next = nextNode;
    nextNode->previous = node;
    list->size++;
}

void insertAfter(DList* list, DListNode* node, int index){
    DListNode* previousNode = get(list, index);
    previousNode->next->previous = node;
    node->previous = previousNode;
    node->next = previousNode->next;
    previousNode->next = node;
    list->size++;
}

void* removeAt(DList* list, int index){
    if (!list->size || index > list->size || index < -(list->size)) return NULL;
    DListNode* popNode = get(list, index);
    popNode->next->previous =  popNode->previous;
    popNode->previous->next = popNode->next;
    void * value = popNode->value;
    free(popNode);
    list->size --;
    return value;
}

DList* slice(DList* list, int i, int j){
    DListNode * firstPart = get(list, i), * lastPart = get(list, j);
    DList* newList = createList();
    int ib = 0;
    int jb = 0;
    if (i < 0) {
        i += list->size;
    }
    if (j < 0) {
        j += list->size;
    }
    firstPart->previous->next = lastPart->next;
    lastPart->next->previous = firstPart->previous;

    firstPart->previous = newList->sentinel;
    lastPart->next = newList->sentinel;

    newList->sentinel->next = firstPart;
    newList->sentinel->previous = lastPart;
    newList->size = j-i+1;
    list->size -= newList->size;

    return newList;
}


DList* fusion(DList* list1, DList* list2, int (*compareData)(void*, void*))
{
    DList *fusionList = createList();
    DListNode *first1 = pop2(list1), *first2 = pop2(list2);
    while (first1 && first2){
        if (compareData(first1->value, first2->value) > 0){
            insertLast(fusionList, first2);
            first2 = pop2(list2);
        } else {
            insertLast(fusionList, first1);
            first1 = pop2(list1);
        }
    }
    while (first1){
        insertLast(fusionList, first1);
        first1 = pop2(list1);
    }
    while (first2){
        insertLast(fusionList, first2);
        first2 = pop2(list2);
    }
    return fusionList;
}

DList* sort(DList* list, int (*compareData)(void*, void*))
{
    if (list->size <= 1) return list;
    DList *listSlice = slice(list, 0, list->size/2-1);
    listSlice = sort(listSlice, compareData);
    list = sort(list, compareData);

    return fusion(listSlice, list, compareData);
}