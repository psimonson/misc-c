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

/* ------------------------ Finish of Vector ------------------------- */

int main(void)
{
	int *v = NULL;

	vector_push_back(v, 1);
	vector_push_back(v, 2);
	vector_push_back(v, 3);
	vector_push_back(v, 4);
	vector_push_back(v, 5);
	vector_push_back(v, 6);
	printf("Printing with array subscript...\n");
	for(unsigned int i = 0; i < vector_size(v); i++)
		printf("Test %u\n", v[i]);
	printf("Printing with pointer...\n");
	for(int *p = vector_begin(v); p != vector_end(v); p++)
		printf("Test %d\n", *p);
	vector_free(v);
	return 0;
}
