/* vector.c -- New C vector for any data type, using preprocessor
 * macros in pure C.
 * by Philip R. Simonson
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* ------------------------ Start of Vector -------------------------- */

#define vector_set_capacity(vec, size) ((size_t*)(vec))[-1] = (size)
#define vector_set_size(vec, size) ((size_t*)(vec))[-2] = (size)

/* Get the total capacity of the vector. */
#define vector_capacity(vec) ((vec) ? ((size_t*)(vec))[-1] : (size_t)0)
/* Get the size of the vector. */
#define vector_size(vec) ((vec) ? ((size_t*)(vec))[-2] : (size_t)0)
/* Get the whether the vector is empty. */
#define vector_empty(vec) (vector_size(vec) == 0)

/* Grow the vector in capacity.
 */
#define vector_grow(vec, count) do { \
	if(!(vec)) { \
		size_t *_p; \
		_p = malloc((count)*sizeof(*(vec))+(sizeof(size_t)*2)); \
		assert(_p); \
		(vec) = (void*)(&_p[2]); \
		vector_set_capacity((vec), (count)); \
		vector_set_size((vec), 0); \
	} else { \
		size_t *_p1, *_p2; \
		_p1 = &((size_t*)(vec))[-2]; \
		_p2 = \
		realloc(_p1, (count)*sizeof(*(vec))+(sizeof(size_t)*2)); \
		assert(_p2); \
		(vec) = (void*)(&_p2[2]); \
		vector_set_capacity((vec), (count)); \
	} \
} while(0)

#define vector_pop_back(vec) (vector_set_capacity((vec), vector_size(vec)-1))
#define vector_erase(vec, i) do { \
	if(vec) { \
		const size_t __sz = vector_size(vec); \
		if((i) < __sz) { \
			size_t __x; \
			vector_set_size((vec), __sz-1); \
			for(__x = (i); __x < (__sz-1); ++__x) { \
				(vec)[__x] = (vec)[__x+1]; \
			} \
		} \
	} \
} while(0)

#define vector_free(vec) do { \
	if(vec) { \
		size_t *p1 = &((size_t*)(vec))[-2]; \
		free(p1); \
	} \
} while(0)

#define vector_begin(vec) ((vec) ? &(vec)[0] : NULL)
#define vector_end(vec) ((vec) ? &(vec)[vector_capacity(vec)] : NULL)

#ifdef LOGARITHMIC_GROWTH
#define vector_push_back(vec, value) do { \
	size_t __cap = vector_capacity(vec); \
	if(__cap <= vector_size(vec)) { \
		vector_grow((vec), !__cap ? __cap+1 : __cap*2); \
	} \
	vec[vector_size(vec)] = (value); \
	vector_set_size((vec), vector_size(vec)+1); \
} while(0)
#else
#define vector_push_back(vec, value) do { \
	size_t __cap = vector_capacity(vec); \
	if(__cap <= vector_size(vec)) { \
		vector_grow((vec), __cap+1); \
	} \
	vec[vector_size(vec)] = (value); \
	vector_set_size((vec), vector_size(vec)+1); \
} while(0)
#endif

/* Copy vector (from) to vector (to). */
#define vector_copy(from, to) do { \
	size_t __i, __cur_size = vector_size(from); \
	for(__i = 0; __i < __cur_size; __i++) { \
		vector_push_back(to, from[__i]); \
	} \
} while(0)

/* ------------------------ Finish of Vector ------------------------- */

int main(void)
{
#define PRNT_VAR(var, message) printf( #var ": " message "\n");
	int *v1 = NULL, *v2 = NULL;

	/* Copy values to vector (v1) */
	vector_push_back(v1, 1);
	vector_push_back(v1, 2);
	vector_push_back(v1, 3);
	vector_push_back(v1, 4);
	vector_push_back(v1, 5);
	vector_push_back(v1, 6);

	/* Vector one (v1) */
	PRNT_VAR(v1, "Printing with array subscript...");
	for(unsigned int i = 0; i < vector_capacity(v1); i++)
		printf("Test %u\n", v1[i]);
	PRNT_VAR(v1, "Printing with pointer...");
	for(int *p = vector_begin(v1); p != vector_end(v1); p++)
		printf("Test %d\n", *p);

	/* Copy vector (v1) to new (v2) */
	vector_copy(v1, v2);

	/* Vector two (v2) */
	PRNT_VAR(v2, "Printing with array subscript...");
	for(unsigned int i = 0; i < vector_capacity(v2); i++)
		printf("Test %u\n", v2[i]);
	PRNT_VAR(v2, "Printing with pointer...");
	for(int *p = vector_begin(v2); p != vector_end(v2); p++)
		printf("Test %d\n", *p);

	/* Cleanup vectors */
	vector_free(v1);
	vector_free(v2);

	return 0;
}
