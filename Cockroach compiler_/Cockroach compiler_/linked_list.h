#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include "token.h"

typedef struct Node 
{
		Token data;
        struct Node *next;
}node;

int list_len(node *pointer);

void add_token(node *pointer, Token data);

void print_token(node *pointer);
#endif