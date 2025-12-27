#include "stack.h"

typedef struct stack {
    void **data;
    size_t size;     // number of elements
    size_t capacity; // allocated slots in data[]
} stack;

enum { MIN_CAPACITY = 4 };

static void grow_stack(stack *s)
{
    size_t new_cap = (s->capacity == 0) ? MIN_CAPACITY : s->capacity * 2;

    void **new_data = realloc(s->data, new_cap * sizeof(*new_data));
    if (new_data == NULL) {
        fprintf(stderr, "stack: out of memory (grow_stack).\n");
        exit(EXIT_FAILURE);
    }

    s->data = new_data;
    s->capacity = new_cap;
}

static void shrink_stack(stack *s)
{
    if (s->capacity <= MIN_CAPACITY) return;

    size_t new_cap = s->capacity / 2;
    if (new_cap < MIN_CAPACITY) new_cap = MIN_CAPACITY;
    if (new_cap < s->size) return;

    void **new_data = realloc(s->data, new_cap * sizeof(*new_data));
    if (new_data == NULL) {
        // keep old buffer (safe)
        return;
    }

    s->data = new_data;
    s->capacity = new_cap;
}

stack *create_stack(void)
{
    stack *s = malloc(sizeof(*s));
    if (s == NULL) {
        fprintf(stderr, "stack: out of memory (create_stack).\n");
        exit(EXIT_FAILURE);
    }

    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
    return s;
}

void delete_stack(stack *s)
{
    if (s == NULL) return;
    free(s->data);
    free(s);
}

bool isempty_stack(stack *s)
{
    return (s == NULL) || (s->size == 0);
}

void push(void *val, stack *s)
{
    if (s == NULL) return;

    if (s->size == s->capacity) {
        grow_stack(s);
    }

    s->data[s->size++] = val;
}

void *pop(stack *s)
{
    if (s == NULL || s->size == 0) {
        fprintf(stderr, "stack: pop() on empty stack.\n");
        exit(EXIT_FAILURE);
    }

    void *val = s->data[s->size - 1];
    s->size--;

    if (s->capacity > MIN_CAPACITY && s->size <= s->capacity / 4) {
        shrink_stack(s);
    }

    return val;
}