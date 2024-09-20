#include <stdio.h>

#include "vector.h"

typedef struct {
	int x;
	int y;
} Point;

int main() {
	// * Creating vectors

	// Vector of integers
	int *int_vec = new_vector(sizeof(int));

	// Vector of array of integers with size 10
	int(*int_arr_vec)[10] = new_vector(sizeof(int[10]));

	// Vector of structures
	Point *point_vec = new_vector(sizeof(Point));

	// Vector of vector of integers
	int **int_vec_vec = new_vector(sizeof(int *));
	// This means that we can also store vector of vector ... of vector of
	// structs.

	// * Pushing elements

	// The vector does not work for literals, so you need to create a variable
	int number = 5;
	int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	Point point = {1, 2};

	// There is no need to pass the address of `int_vec` and `number`,
	// converting them into addresses is already being handled by the macro
	vector_push(int_vec, number);

	vector_push(int_arr_vec, arr);
	vector_push(point_vec, point);
	vector_push(int_vec_vec, int_vec);

	// * Accessing elements

	printf("int_vec[0] = %d\n", int_vec[0]);
	printf("int_arr_vec[0][0] = %d\n", int_arr_vec[0][0]);
	printf("point_vec[0].x = %d, point_vec[0].y = %d\n", point_vec[0].x,
		   point_vec[0].y);
	printf("int_vec_vec[0][0] = %d\n", int_vec_vec[0][0]);

	// * Popping elements

	int popped_number = *(int *)vector_pop(int_vec);

	// Since we are storing an array of integers,
	// we have to use a pointer to store the popped array
	int(*popped_arr)[10] = vector_pop(int_arr_vec);

	Point popped_point = *(Point *)vector_pop(point_vec);

	// Note: This is a vector of vector of integers,
	// so we have to consider freeing the inner vector
	// once we are done with it.
	int *popped_int_vec = *(int **)vector_pop(int_vec_vec);

	printf("Popped number: %d\n", popped_number);
	printf("Popped arr[0]: %d\n", (*popped_arr)[0]);
	printf("Popped point.x: %d, Popped point.y: %d\n", popped_point.x,
		   popped_point.y);
	printf("Popped int_vec[0]: %d\n", popped_int_vec[0]);

	// * Freeing vectors

	vector_free(int_vec);
	vector_free(int_arr_vec);
	vector_free(point_vec);

	// Since we are storing a vector of vector of integers,
	// we have to interate over the inner vector and free them

	for (int i = 0; i < (int)vector_length(int_vec_vec); i++) {
		vector_free(int_vec_vec[i]);
	}

	vector_free(int_vec_vec);

	return 0;
}
