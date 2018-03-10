//
// Created by kreska on 11.03.17.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "address_book.h"

void addToList(List *list, char **data, int flag);

void addToBTree(BTree *bTree, char **data, int flag);

AddressBook *createAddressBook(int flag) {
    AddressBook *addressBook = malloc(sizeof(AddressBook));
    addressBook->flag = flag;

    if ((flag & 4) == 4) {
        addressBook->bTree = malloc(sizeof(BTree));
        addressBook->bTree->root = NULL;
        addressBook->list = NULL;
    } else {
        addressBook->list = malloc(sizeof(List));
        addressBook->bTree = NULL;
        ListNode *head = malloc(sizeof(ListNode));
        ListNode *tail = malloc(sizeof(ListNode));
        head->prev = tail->next = NULL;
        head->next = tail;
        tail->prev = head;
        addressBook->list->head = head;
        addressBook->list->tail = tail;
    }
    return addressBook;
}

void addPerson(AddressBook *addressBook, char **data) {
    if ((addressBook->flag & 4) == 0) {
        addToList(addressBook->list, data, addressBook->flag);
    } else
        addToBTree(addressBook->bTree, data, addressBook->flag & 3);
    return;
}

void printBTree(BTreeNode *root) {
    if (!root)
        return;
    printBTree(root->left);
    for (int i = 0; i < 6; ++i) {
        printf("%s ", root->data[i]);
    }
    printf("\n");
    printBTree(root->right);
}

void printList(AddressBook *addressBook) {
    ListNode *i = addressBook->list->head;
    while (i->next != addressBook->list->tail) {
        i = i->next;
        for (int j = 0; j < 6; ++j) {
            printf("%s ", i->data[j]);
        }
        printf("\n");
    }
}

void printBook(AddressBook *addressBook) {
    if ((addressBook->flag & 4) == 4)
        printBTree(addressBook->bTree->root);
    else printList(addressBook);
}


void addToList(List *list, char **data, int key) {
    ListNode *node = malloc(sizeof(ListNode));
    node->data = data;

    ListNode *i = list->head;
    while (i->next != list->tail && strcmp(i->next->data[key], node->data[key]) < 0)
        i = i->next;
    i->next->prev = node;
    node->next = i->next;
    i->next = node;
    node->prev = i;
}

void quickSort(ListNode *head, ListNode *tail, int key) {
    if (head->next == tail || head->next->next == tail)
        return;
    ListNode *p = head->next;
    ListNode *i = p->next;

    if (i != tail) {
        do {
            ListNode *j = i;
            i = i->next;
            if (strcmp(j->data[key], p->data[key]) < 0) {
                j->prev->next = j->next;
                j->next->prev = j->prev;
                j->next = p;
                j->prev = p->prev;
                p->prev = j;
                j->prev->next = j;
            }
        } while (i != tail);
        if (head->next != p) quickSort(head, p, key);
        if (p->next != tail) quickSort(p, tail, key);
    }
}

ListNode *searchInList(List *list, char *word, int key) {
    ListNode *i = list->head->next;
    while (i != list->tail && strcmp(i->data[key], word) != 0)
        i = i->next;
    if (i != list->tail)
        return i;
    return NULL;
}

void popList(ListNode *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;

    for (int i = 0; i < 6; ++i)
        free(node->data[i]);
    free(node->data);
    free(node);
}

void deleteList(List *list) {
    ListNode *i = list->head->next;
    while (i != list->tail) {
        ListNode *p = i;
        i = i->next;
        popList(p);
    }
    free(list->head);
    free(list->tail);
    free(list);
}

//BTREE

void insertToBTree(BTree *bTree, BTreeNode *node, int key) {
    BTreeNode *root = bTree->root;
    node->left = node->right = node->parent = NULL;

    if (root == NULL) {
        bTree->root = node;
        return;
    }

    BTreeNode *p = root;
    BTreeNode *q = NULL;

    while (p != NULL) {
        if (strcmp(node->data[key], p->data[key]) >= 0) {
            q = p;
            p = p->right;
        } else {
            q = p;
            p = p->left;
        }
    }

    if (strcmp(node->data[key], q->data[key]) >= 0)
        q->right = node;
    else
        q->left = node;

    node->parent = q;
}

void addToBTree(BTree *bTree, char **data, int key) {
    BTreeNode *node = malloc(sizeof(BTreeNode));
    node->data = data;

    insertToBTree(bTree, node, key);
}

BTreeNode *searchInBTree(BTreeNode *root, char *word, int key) {
    if (root == NULL) {
        return NULL;
    }
    int tmp = strcmp(word, root->data[key]);
    if (tmp == 0) {
        return root;
    }
    if (tmp < 0)
        return searchInBTree(root->left, word, key);
    else
        return searchInBTree(root->right, word, key);
}

BTreeNode *minBTree(BTreeNode *node) {
    if (node != NULL)
        while (node->left != NULL)
            node = node->left;
    return node;
}

BTreeNode *successor(BTreeNode *p) {
    BTreeNode *r;
    if (p != NULL) {
        if (p->right != NULL)
            return minBTree(p->right);
        else {
            r = p->parent;
            while (r != NULL && (p == r->right)) {
                p = r;
                r = r->parent;
            }
            return r;
        }
    }
    return p;
}


void removeFromBTree(BTree *bTree, BTreeNode *node) {
    if (node == NULL)
        return;
    BTreeNode *p, *q;

    if (node->left == NULL || node->right == NULL)
        p = node;
    else
        p = successor(node);

    if (p->left != NULL)
        q = p->left;
    else
        q = p->right;

    if (q != NULL)
        q->parent = p->parent;

    if (p->parent == NULL)
        bTree->root = q;
    else if (p == p->parent->left) p->parent->left = q;
    else p->parent->right = q;

    for (int i = 0; i < 6; ++i) {
        if (node->data[i] != NULL)
            free(node->data[i]);
    }
    free(node->data);
    free(node);
}

void rewriteBTree(BTreeNode *oldRoot, BTree *bTree, int key) {
    if (oldRoot == NULL)
        return;
    rewriteBTree(oldRoot->left, bTree, key);
    rewriteBTree(oldRoot->right, bTree, key);
    insertToBTree(bTree, oldRoot, key);
}

BTree *rearrangeBTree(BTreeNode *root, int key) {
    BTree *neue = malloc(sizeof(BTree));
    neue->root = NULL;
    rewriteBTree(root, neue, key);
    return neue;
}

void sortByKey(AddressBook *addressBook, int key) {
    if ((addressBook->flag & 4) == 4) {
        BTree *p = rearrangeBTree(addressBook->bTree->root, key);
        free(addressBook->bTree);
        addressBook->bTree = p;
    } else {
        quickSort(addressBook->list->head, addressBook->list->tail, key);
    }

    addressBook->flag &= 4;
    addressBook->flag |= key;
}

void removeEl(AddressBook *addressBook, char *word) {
    if ((addressBook->flag & 4) == 4) {
        BTreeNode *p = searchInBTree(addressBook->bTree->root, word, addressBook->flag);
        removeFromBTree(addressBook->bTree, p);
    } else {
        ListNode *p = searchInList(addressBook->list, word, addressBook->flag);
        if (p != NULL) popList(p);
    }
}

char **searchEl(AddressBook *addressBook, char *word) {
    if ((addressBook->flag & 4) == 4) {
        BTreeNode *p = searchInBTree(addressBook->bTree->root, word, addressBook->flag & 3);
        return p->data;
    }
    ListNode *p = searchInList(addressBook->list, word, addressBook->flag);
    return p->data;
}

void deleteBTree(BTreeNode *p) {
    if (p == NULL) return;
    deleteBTree(p->left);
    deleteBTree(p->right);

    for (int i = 0; i < 6; ++i) {
        if (p->data[i] != NULL)
            free(p->data[i]);
    }
    free(p->data);
    free(p);
}

void deleteBook(AddressBook *addressBook) {
    if ((addressBook->flag & 4) == 4) {
        deleteBTree(addressBook->bTree->root);
        free(addressBook->bTree);
    } else {
        deleteList(addressBook->list);
    }
    free(addressBook);
}

char **createData(string name, string surname, string birth, string email, string phone, string address) {
    char **result = malloc(sizeof(char *) * 6);
    result[0] = malloc(sizeof(char) * 20);
    result[1] = malloc(sizeof(char) * 10);
    result[2] = malloc(sizeof(char) * 60);
    result[3] = malloc(sizeof(char) * 20);
    result[4] = malloc(sizeof(char) * 20);
    result[5] = malloc(sizeof(char) * 60);
    strcpy(result[0], surname);
    strcpy(result[1], birth);
    strcpy(result[2], email);
    strcpy(result[3], phone);
    strcpy(result[4], name);
    strcpy(result[5], address);
    return result;
}

BTreeNode *returnRoot(AddressBook *addressBook) {
    if ((addressBook->flag & 4) == 4)
        return addressBook->bTree->root;
    return NULL;
}



