////////////////////////////
/// Primitives des files ///
////////////////////////////

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/////////////////////////
// Déclaration du type //
/////////////////////////

typedef struct queue queue;

///////////////////////////
// Fonctions primitives. //
///////////////////////////

// Crée une file vide.
queue *create_queue(void);

// Libère la mémoire allouée pour la file.
void delete_queue(queue *q);

// Renvoie vrai si la file est vide, faux sinon.
bool isempty_queue(const queue *q);

// Ajoute une valeur dans la file, à gauche de la valeur la plus à gauche.
void enqueue(void *val, queue *q);

// Renvoie la valeur la plus à droite de la file et la retire.
void *dequeue(queue *q);

#endif // QUEUE_H_
