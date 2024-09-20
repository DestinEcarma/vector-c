#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

typedef struct {
	size_t length;
	size_t capacity;
	size_t typeSize;
	unsigned char data[];
} VectorHeader;

VectorHeader *_vector_header(Vector vector) {
	return (VectorHeader *)vector - 1;
}

VectorHeader *_vector_realloc(VectorHeader *header) {
	size_t newCapacity = header->capacity == 0 ? 1 : header->capacity * 2;
	size_t size = sizeof(VectorHeader) + newCapacity * header->typeSize;

	VectorHeader *newHeader = realloc(header, size);

	if (newHeader != NULL) {
		newHeader->capacity = newCapacity;
	}

	return newHeader;
}

Vector new_vector(size_t typeSize) {
	VectorHeader *header = malloc(sizeof(VectorHeader));

	header->length = 0;
	header->capacity = 0;
	header->typeSize = typeSize;

	return &header->data;
}

void vector_free(Vector vector) {
	free(_vector_header(vector));
}

size_t vector_length(Vector vector) {
	return _vector_header(vector)->length;
}

bool _vector_push(Vector *vector, void *element) {
	VectorHeader *header = _vector_header(*vector);

	if (header->length == header->capacity) {
		VectorHeader *newHeader = _vector_realloc(header);

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

void *_vector_pop(Vector *vector) {
	VectorHeader *header = _vector_header(*vector);

	if (header->length == 0) {
		return NULL;
	}

	header->length--;
	return header->data + header->length * header->typeSize;
}
