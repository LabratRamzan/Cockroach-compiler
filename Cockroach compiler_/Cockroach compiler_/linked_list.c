#include "linked_list.h"

void add_token(node *pointer, Token data)
{
        while(pointer->next!=NULL)
        {
                pointer = pointer -> next;
        }
        pointer->next = (node *)malloc(sizeof(node));
        pointer = pointer->next;
        pointer->data = data;
        pointer->next = NULL;
}

void print_token(node *pointer)
{
        if(pointer==NULL)
        {
                return;
        }
		printf("type: %d ; from: %d ; to: %d\n",pointer->data.Type,pointer->data.From, (pointer->data).To);
        printf("%d\n",pointer->next);
}

int list_len(node* pointer)
{
	int cnt;
	if ( &(pointer->data)==0) return 0;
	cnt=1;
	while (pointer->next!=0)
	{
		cnt++;
		pointer=pointer->next;
	}
	return cnt;
}

