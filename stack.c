#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

void stack_init(void *stack) {
    ((Stack*)stack)->top = NULL;
}

void stack_push(void *stack, void *entry) {
    ((Stack*)entry)->top = ((Stack*)stack)->top;
    ((Stack*)stack)->top = (Stack*)entry;
};

Stack *stack_pop(void *stack) {
    Stack *top = ((Stack*)stack)->top;
    ((Stack*)stack)->top = top->top;
    return top;
};


int stack_empty(void *stack) {
    return ((Stack*)stack)->top == NULL;
}


typedef struct MyEntry {
    Stack stack;
    int data;
} MyEntry;


int main(int argc, char **argv) {
    MyEntry root;
    stack_init(&root);

    for (int i = 0; i < 10; ++i) {
        MyEntry *item = malloc(sizeof(MyEntry));
        item->data = i;
        stack_push(&root, item);
    }

    while (!stack_empty(&root)) {
        Stack *entry = stack_pop(&root);
        printf("%d", ((MyEntry*)entry)->data);
        free(entry);
    }

    return 0;
}