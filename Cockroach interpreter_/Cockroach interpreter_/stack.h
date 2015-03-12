#define STACK_MAX 256

struct {
	int	data[STACK_MAX];
    int size;
}Stack;

void Stack_Init();

void Push(int d);

int Pop();