//////////////////////////////////////////////
// Implémentation des files par deux piles. //
//////////////////////////////////////////////

// Le fichier main.h définit la macro QUEUE_BY_TWO_STACKS.
// Vous pouvez la changer.

#include "main.h"


// Si cette macro est vraie, l'implémentation des files par deux piles est
// utilisée, et le fichier queue.c est "vidé" par le précompilateur. C'est ce
// fichier, queue_by_2_stacks.c, qui sera utilisé.

// Si la macro est fausse, l'implémentation des files par tableau circulaire est
// utilisée (fichier queue.c), et le contenu de ce fichier est "vidé".


#include "queue.h"
#include "stack.h"

///////////////////////////////////////////
// Définition du type, à ne pas modifier //
///////////////////////////////////////////

typedef struct queue {
    stack *left, *right;
} queue;

///////////////////////////////////////////////////////////////////////////
// Fonctions primitives à écrire, documentées dans le fichier d'en-tête. //
///////////////////////////////////////////////////////////////////////////

queue *create_queue(void) {
    queue *q = malloc(sizeof(queue));
    q->left = create_stack();
    q->right = create_stack();
    return q;
}

void delete_queue(queue *q) {
    delete_stack(q->left);
    delete_stack(q->right);
    free(q);
}

bool isempty_queue(const queue *q) {
    return isempty_stack(q->left)&& (isempty_stack(q->right));
}

void enqueue(void * val, queue *q) {
    push(val,q->left);
}

void *dequeue(queue *q) {
    if(isempty_stack(q->right)){
        if(isempty_stack(q->left)){
            return NULL;
        }
        while(!isempty_stack(q->left)){
            push(pop(q->left), q->right);
        }
    }
    return pop(q->right);
}




/////////////////////////////////////////
// Fonction d'affichage, déjà fournie. //
/////////////////////////////////////////
/*
static void print_queue_aux(queue *q) {
    if (isempty_queue(q)) {
        return;
    }

    int val;

    val = dequeue(q);
    print_queue_aux(q);
    printf("%4d", val);
    enqueue(val, q);
}

void print_queue(queue *q) {
    printf("File : ");
    if (isempty_queue(q)) {
        printf("(vide).\n");
        return;
    }
    print_queue_aux(q);
    printf("\n");
}
    */