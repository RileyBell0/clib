#include "../stack.h"

stack_t new_stack(unsigned int elementSize)
{
    return new_list(elementSize);
}

void stack_push(stack_t* stack, void *data)
{
    list_append((list_t*)stack, data);
}

void* stack_pop(stack_t* stack)
{
    if (stack == NULL || stack->size == 0)
    {
        return NULL;
    }
    return list_remove_last((list_t*)stack);
}

void* stack_top(stack_t* stack)
{
    if (stack == NULL || stack->last_node == NULL)
    {
        return NULL;
    }
    return stack->last_node->data;
}
