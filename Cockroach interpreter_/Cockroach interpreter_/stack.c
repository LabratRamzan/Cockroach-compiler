#include "stack.h"

void Push(double d, Stack* st)
{
        st->data[st->size++] = d;
}

int Pop(Stack* st)
{
        st->size--;
		return st->data[st->size];
}