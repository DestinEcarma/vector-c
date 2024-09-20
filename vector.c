#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

// Private types

typedef struct {
	size_t length;
	size_t capacity;
	size_t typeSize;
	unsigned char data[];
} VectorHeader;

// Public functions

Vector newVector(size_t typeSize) {
	VectorHeader *header = malloc(sizeof(VectorHeader));

	header->length = 0;
	header->capacity = 0;
	header->typeSize = typeSize;

	return &header->data;
}

// Private functions

VectorHeader *vectorHeader(Vector vector) {
	return (VectorHeader *)vector - 1;
}

VectorHeader *vectorRealloc(VectorHeader *header) {
	size_t newCapacity = header->capacity == 0 ? 1 : header->capacity * 2;
	size_t size = sizeof(VectorHeader) + newCapacity * header->typeSize;

	VectorHeader *newHeader = realloc(header, size);

	if (newHeader != NULL) {
		newHeader->capacity = newCapacity;
	}

	return newHeader;
}

size_t _vectorLength(Vector *vector) {
	return vectorHeader(*vector)->length;
}

bool _vectorPush(Vector *vector, void *element) {
	VectorHeader *header = vectorHeader(*vector);

	if (header->length == header->capacity) {
		VectorHeader *newHeader = vectorRealloc(header);

		if (newHeader == NULL) {
			return false;
		}

		*vector = newHeader->data;
		header = newHeader;
	}

	void *dest = header->data + header->length * header->typeSize;
	memcpy(dest, element, header->typeSize);
	header->length++;

	return true;
}

void *_vectorPop(Vector *vector) {
	VectorHeader *header = vectorHeader(*vector);

	if (header->length == 0) {
		return NULL;
	}

	header->length--;
	return header->data + header->length * header->typeSize;
}
