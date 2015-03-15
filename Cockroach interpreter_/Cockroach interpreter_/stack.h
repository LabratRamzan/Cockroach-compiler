#define STACK_MAX 256

typedef struct {
	int	data[STACK_MAX];
    unsigned long size;
}Stack;

void Push(double d, Stack* st);

int Pop(Stack* st);