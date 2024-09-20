# Vector C

A simple generic vector implementation in C. The vector is a dynamic array that can store any type of data. The implementation is simple and easy to use, and it is inspired by the `Vec` in `Rust` and `std::vector` in `C++`. Some of the implementation is inspired by Mashpoe's [C Vector](https://github.com/Mashpoe/c-vector/).

## Usage

To use the vector, include the `vector.h` header file in your code. The vector can store any type of data, so you can use it to store integers, floats, strings, or any other type of data. Here is an example of how to use the vector:

Storing different types of data in the vector:

```c
// Vector of integers
int *int_vec = new_vector(sizeof(int));

// Vector of array of integers with size 10
int(*int_arr_vec)[10] = new_vector(sizeof(int[10]));

// Vector of structures
struct Point { int x; int y; };
struct Point *point_vec = new_vector(sizeof(struct Point));

// Vector of vector of integers
int **int_vec_vec = new_vector(sizeof(int *));
// This means that we can also store vector of vector ... of vector of structs.
```

Adding elements to the vector:

```c
// The vector does not work for literals, so you need to create a variable
int number = 5;
int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
struct Point point = {1, 2};

// There is no need to pass the address of `int_vec` and `number`,
// converting them into addresses is already being handled by the macro
vector_push(int_vec, number);

vector_push(int_arr_vec, arr);
vector_push(point_vec, point);
vector_push(int_vec_vec, int_vec);
```

Retrieving elements from the vector:

```c
printf("int_vec[0] = %d\n", int_vec[0]);
printf("int_arr_vec[0][0] = %d\n", int_arr_vec[0][0]);
printf("point_vec[0].x = %d, point_vec[0].y = %d\n", point_vec[0].x, point_vec[0].y);
printf("int_vec_vec[0][0] = %d\n", int_vec_vec[0][0]);
```

You will notice that we simply used the `[]` operator to access the elements of the vector. This will be explained at [How It Works](#how-it-works).

Removing elements from the vector:

```c
int popped_number = *(int *)vector_pop(int_vec);

// Since we are storing an array of integers,
// we have to use a pointer to store the popped array
int(*popped_arr)[10] = vector_pop(int_arr_vec);

Point popped_point = *(Point *)vector_pop(point_vec);

// Note: This is a vector of vector of integers,
// so we have to consider freeing the inner vector
// once we are done with it.
int *popped_int_vec = *(int **)vector_pop(int_vec_vec);
```

Freeing the vector:

```c
vector_free(int_vec);
vector_free(int_arr_vec);
vector_free(point_vec);

// Since we are storing a vector of vector of integers,
// we have to interate over the inner vector and free them

for (int i = 0; i < (int)vector_length(int_vec_vec); i++) {
    vector_free(int_vec_vec[i]);
}

vector_free(int_vec_vec);
```

Even though we can access the elements of the vector using the `[]` operator, that doesn't mean that we can treat the vector as a regular array. This means we can't store any element in the vector that is out of bounds. We also can't use `realloc` the same way we do with the regular array.

## How It Works

These vectors can be manipulated in a similar manner in `Rust` using `Vec` and in `C++` using `std::vector`. This means that we can access the elements of the vector using the `[]` operator, and we can iterate over the elements using a loop.

The reason for this operation to work is that the vector is a pointer to the data, and the data is stored directly with a special header that contains the size and capacity of the vector.

```

+--------+-------------+
| Header | Vector data |
+--------+-------------+
         |
         `-> Pointer returned to the user.

```

This design was inspired by anitrez's [Simple Dynamic Strings](https://github.com/antirez/sds/).
