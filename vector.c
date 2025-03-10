#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

typedef struct {
	size_t length;
	size_t capacity;
	size_t size_type;
	unsigned char data[];
} VectorHeader;

VectorHeader *_vector_header(Vector vector) {
	return (VectorHeader *)vector - 1;
}

VectorHeader *_vector_realloc(VectorHeader *header, size_t newCapacity) {
	size_t size = sizeof(VectorHeader) + newCapacity * header->size_type;

	VectorHeader *newHeader = realloc(header, size);

	if (newHeader == NULL) {
		return header;
	}

	newHeader->capacity = newCapacity;

	return newHeader;
}

Vector new_vector(size_t size_type) {
	VectorHeader *header = malloc(sizeof(VectorHeader));

	if (header == NULL) {
		return NULL;
	}

	header->length = 0;
	header->capacity = 0;
	header->size_type = size_type;

	return &header->data;
}

void vector_free(Vector vector) {
	free(_vector_header(vector));
}

size_t vector_length(Vector vector) {
	return _vector_header(vector)->length;
}

bool _vector_reserve(Vector *vector, size_t capacity) {
	VectorHeader *header = _vector_header(*vector);

	if (header->capacity < capacity) {
		VectorHeader *newHeader = _vector_realloc(header, capacity);

		if (newHeader == NULL) {
			return false;
		}

		*vector = newHeader->data;
	}

	return true;
}

bool _vector_push(Vector *vector, void *element) {
	VectorHeader *header = _vector_header(*vector);

	if (header->length == header->capacity) {
		size_t newCapacity = header->capacity == 0 ? 1 : header->capacity * 2;

		VectorHeader *newHeader = _vector_realloc(header, newCapacity);

		if (newHeader == NULL) {
			return false;
		}

		*vector = newHeader->data;
		header = newHeader;
	}

	void *dest = header->data + header->length++ * header->size_type;
	memcpy(dest, element, header->size_type);

	return true;
}

void *_vector_pop(Vector *vector) {
	VectorHeader *header = _vector_header(*vector);

	if (header->length == 0) {
		return NULL;
	}

	return header->data + --header->length * header->size_type;
}
