//
// Created by kreska on 11.03.17.
//

#ifndef SYSOPY01_01_ADDRESS_BOOK_H
#define SYSOPY01_01_ADDRESS_BOOK_H

/**
 *  @structures
 */

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    char **data;      // 0-last name 1-birth date 2-email 3-phone, 4-first name 5-address
} ListNode;

typedef struct List {
    ListNode *head;
    ListNode *tail;
} List;

typedef struct BTreeNode {
    struct BTreeNode *parent;
    struct BTreeNode *left;
    struct BTreeNode *right;
    char **data;
} BTreeNode;

typedef struct BTree {
    BTreeNode *root;
} BTree;

typedef struct AddressBook {
    List *list;
    BTree *bTree;
    int flag;
} AddressBook;

/**
 *  @functions
 */

AddressBook *createAddressBook(int flag);

/**
 *  Creates an Address Book
 *
 *  @param flag is a Bit Map as a character
 *      Two first bits define key order
 *          00 - order by last name
 *          01 - order by birth date
 *          10 - order by email
 *          11 - order by phone number
 *      Third(higher) bit define type of structure
 *          0 - list
 *          1 - binary tree
 *      For example if you want to use binary tree
 *      ordered by birth date(1 01) you put 5 into
 *      function
 *  @return pointer to AddressBook structure
 */

void addPerson(AddressBook *addressBook, char **data);

void sortByKey(AddressBook *addressBook, int key);

void printBook(AddressBook *addressBook);

void removeEl(AddressBook *addressBook, char *word);

char **searchEl(AddressBook *addressBook, char *word);

void deleteBook(AddressBook *addressBook);

typedef const char *string;

char **createData(string name, string surname, string birth, string email, string phone, string address);

BTreeNode *returnRoot(AddressBook *addressBook);

#endif //SYSOPY01_01_ADDRESS_BOOK_H
