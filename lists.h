//
// Created by DUMOND on 27/09/2022.
//

#pragma once

typedef struct sDListNode{
    void* value;
    struct sDListNode* previous;
    struct sDListNode* next;
} DListNode;

typedef struct {
    DListNode* sentinel;
    int size;
	int total;
	int isSoft;
} DList;

DListNode * createNode(void* value);
DList* createList();
void print(DList* list, void (*printData)(void*));
void insertFirst(DList* list, DListNode* node);
void insertLast(DList* list, DListNode* node);
void* pop(DList* list);
DListNode* pop2(DList* list);
DListNode* firstOf(DList* list, void* value, int (*compareData)(void*, void*));
DListNode* lastOf(DList* list, void* value, int (*compareData)(void*, void*));
DList* insertTable(DList* list, int table[], int reverse, int size);
void insertSorted(DList* list, DListNode* node, int reverse, int (*compareData)(void*, void*));
DListNode* get(DList* list, int index);
void insertBefore(DList* list, DListNode* node, int index);
void insertAfter(DList* list, DListNode* node, int index);
void* removeAt(DList* list, int index);
DList* slice(DList* list, int i, int j);
DList* fusion(DList* list1, DList* list2, int (*compareData)(void*, void*));
DList* sort(DList* list, int (*compareData)(void*, void*));