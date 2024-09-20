#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

typedef void *Vector;

// Public functions

Vector new_vector(size_t typeSize);

void vector_free(Vector vector);
size_t vector_length(Vector vector);

#define vector_push(vector, element)                                           \
	(_vector_push((Vector *)&(vector), &element))
#define vector_pop(vector) (_vector_pop((Vector *)&vector))

// Private functions

bool _vector_push(Vector *vector, void *element);
void *_vector_pop(Vector *vector);

#endif
