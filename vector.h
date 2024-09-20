#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

typedef void *Vector;

// Public functions

Vector newVector(size_t typeSize);

#define vectorLength(vector) (_vectorLength((Vector *)&vector))

#define vectorPush(vector, element) (_vectorPush((Vector *)&vector, &element))
#define vectorPop(vector) (_vectorPop((Vector *)&vector))

// Private functions

size_t _vectorLength(Vector *vector);

bool _vectorPush(Vector *vector, void *element);
void *_vectorPop(Vector *vector);

#endif
