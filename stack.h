typedef struct Stack Stack;
struct Stack {
    Stack *top;
};

void stack_init(void *stack);
void stack_push(void *stack, void *entry);
Stack *stack_pop(void *stack);
int stack_empty(void *stack);