#include "stack.h"

void Stack_Init()
{
	Stack.size=0;
}

void Push(int d)
{
        Stack.data[Stack.size++] = d;
}

int Pop()
{
        Stack.size--;
		return Stack.data[Stack.size];
}