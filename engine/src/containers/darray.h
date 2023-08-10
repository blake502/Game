#pragma once

#include "defines.h"

//Memory layout
//u64 capacity
//u64 length
//u64 stride
//void* elements

enum {
    DARRAY_CAPACITY,
    DARRAY_LENGTH,
    DARRAY_STRIDE,
    DARRAY_FIELD_LENGTH
};

S_API void* _darray_create(u64 length, u64 stride);
S_API void _darray_destroy(void* array);

S_API u64 _darray_field_get(void* array, u64 field);
S_API void _darray_field_set(void* array, u64 field, u64 value);

S_API void* _darray_resize(void* array);

S_API void* _darray_push(void* array, const void* value_ptr);
S_API void _darray_pop(void* array, void* dest);

S_API void* _darray_pop_at(void* array, u64 index, void* dest);
S_API void* _darray_insert_at(void* array, u64 index, void* value_ptr);

#define DARRAY_DEFAULT_CAPACITY 1
#define DARRAY_RESIZE_FACTOR 2

#define darray_create(type) \
    _darray_create(DARRAY_DEFAULT_CAPACITY, sizeof(type))

#define darray_reserve(type, capacity) \
    _darray_create(capacity, sizeof(type))

#define darray_destroy(array) _darray_destroy(array);

#define darray_push(array, value)          \
    {                                      \
        typeof(value) temp = value;        \
        array = _darray_push(array, &temp);\
    }

#define darray_pop(array, value_ptr)\
    _darray_pop(array, value_ptr)

#define darray_insert_at(array, index, value)         \
    {                                                 \
        typeof(value) temp = value;                   \
        array = _darray_insert_at(array, index, &temp)\
    }

#define darray_pop_at(array, index, value_ptr)\
    _darray_pop_at(array, index, value_ptr)

#define darray_clear(array)\
    _darray_field_set(array, DARRAY_LENGTH, 0)

#define darray_capacity(array)\
    _darray_field_get(array, DARRAY_CAPACITY)

#define darray_length(array)\
    _darray_field_get(array, DARRAY_LENGTH)

#define darray_stride(array)\
    _darray_field_get(array, DARRAY_STRIDE)

#define darray_length_set(array, value)\
    _darray_field_set(array, DARRAY_LENGTH, value)