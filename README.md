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
// This means that we can also store vector of vector ... of vector of
// structs.
```

Adding elements to the vector:

```c
// The vector does not work for literals; so you need to create a variable.
int number = 5;
int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
struct Point point = {1, 2};

// There is no need to pass the address of `int_vec` and `number`,
// converting them into addresses is already being handled by the macro.
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

// Since we are storing an array of integers, we have to use a
// pointer to store the popped array. We also have to consider
// copying the array to another array, since the popped array
// will be altered once we insert another array. This is because
// the function `vector_pop` returns a pointer that is part of
// the vector data. This means that if we insert another array,
// the `popped_arr` will be altered.
int(*popped_arr)[10] = vector_pop(int_arr_vec);

Point popped_point = *(Point *)vector_pop(point_vec);

// Note: This is a vector of vector of integers, so we have to
// consider freeing the inner vector once we are done with it.
int *popped_int_vec = *(int **)vector_pop(int_vec_vec);
```

Freeing the vector:

```c
vector_free(int_vec);
vector_free(int_arr_vec);
vector_free(point_vec);

// Since we are storing a vector of vector of integers,
// we have to iterate over the inner vector and free them.

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

To really understand how this works, let's take a look at the `Header`, which is defined in the [`vector.c`](vector.c) file:

```c
typedef struct {
	size_t length;
	size_t capacity;
	size_t size_type;
	unsigned char data[];
} VectorHeader;
```

So we can see that the `Header` contains the `length` of the vector, the `capacity` of the vector, the `size_type` of the vector, and the `data` of the vector.

The `size_t` is defined as `unsigned long`; in most cases, the byte size of this type is $8$ bytes. The `data` field is an array of `unsigned char` with an array size of $0$, which is a byte size of $0$. So the `Header` is $24$ bytes in total.

The most important thing here to recognize is the data type of `data`, which is an `unsigned char` of $0$ elements. Since we are returning the address of the `data` field, this allows us to find the address of the `Header` where the `data` field is located.

Illustration of the `Header`:

```
8 Bytes  8 Bytes    8 Bytes     0 Bytes
^        ^          ^           ^
|        |          |           |
+--------+----------+-----------+------+
| length | capacity | size_type | data |
+--------+----------+-----------+------+
                                |
                                `-> Pointer returned to the user.
```

This simple illustration shows that the `Header` is actually only available up to the field `size_type`. The `data` field allows us to access the `Header` of the returned pointer.

The function definition that allows us to access the `Header` is defined in the [`vector.c`](vector.c) file:

```c
VectorHeader *_vector_header(Vector vector) {
	return (VectorHeader *)vector - 1;
}
```

`vector` is the pointer returned to the user, and we change the type of the pointer to `VectorHeader` converting it to a byte size of $24$ bytes. We then subtract $1$ from the pointer, which means that we are moving the pointer to the `Header` of the vector.

This works because the `data` field is the last field in the `Header` and is also the last accessible address of the `Header`. This means that we can access the `Header` of the vector by subtracting $1$ from the pointer. You can even think of the `data` field as not part of the `Header`. This is why we can access the `Header` of the vector by subtracting $1$ from the pointer.
