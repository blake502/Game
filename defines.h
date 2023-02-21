#pragma once

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

typedef _Bool b8;
//typedef int b32;

/*
typedef struct range {
    u64 offset;
    u64 size;
}range;
*/

#define true 1;
#define false 0;
#define null NULL;

_Static_assert(sizeof(u8) == 1, "Type u8 must have size of 1 byte");
_Static_assert(sizeof(u16) == 2, "Type u16 must have size of 2 byte");
_Static_assert(sizeof(u32) == 4, "Type u32 must have size of 4 byte");
_Static_assert(sizeof(u64) == 8, "Type u64 must have size of 8 byte");
_Static_assert(sizeof(i8) == 1, "Type i8 must have size of 1 byte");
_Static_assert(sizeof(i16) == 2, "Type i16 must have size of 2 byte");
_Static_assert(sizeof(i32) == 4, "Type i32 must have size of 4 byte");
_Static_assert(sizeof(i64) == 8, "Type i64 must have size of 8 byte");
_Static_assert(sizeof(f32) == 4, "Type f32 must have size of 4 byte");
_Static_assert(sizeof(f64) == 8, "Type f64 must have size of 8 byte");
_Static_assert(sizeof(b8) == 1, "Type b8 must have size of 1 byte");

//#define S_MIN(x, y) (x < y ? x : y)
//#define S_MAX(x, y) (x > y ? x : y)
//#define S_CLAMP(value, min, max) (KMAX(min, KMIN(value, max)))

//TODO: Platform detection