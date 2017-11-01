#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
struct meta_data
{
size_t size;
struct meta_data *next;
int free;
int debug_data;
};
#define SIZE_OF_META sizeof(struct meta_data)
void *base = NULL;
struct meta_data *find_free_chunk(struct meta_data **tail, size_t size)
{
struct meta_data *current = base;
while (current && !(current->free && current->size >= size))
{
*tail = current;
current = current->next;
}
return current;
}
struct meta_data *allocate_chunk(struct meta_data* tail, size_t size)
{
struct meta_data *space;
space = sbrk(0);
void *allocate = sbrk(size + SIZE_OF_META);
assert((void* )space == allocate);
if (allocate == (void*) -1)
return NULL;
if (tail)
tail->next = space;
space->size = size;
space->next = NULL;

//CMPE 220 Fall 2016 Date: 17th November 2016

//12

space->free = 0;
space->debug_data = 0x12345678;
return space;
}
void *mymalloc(size_t size)
{
struct meta_data *space;
if (size <= 0)
return NULL;
if (!base)
{
space = allocate_chunk(NULL, size);
if (!space)
return NULL;
base = space;
}
else
{
struct meta_data *tail = base;
space = find_free_chunk(&tail, size);
if (!space)
{
space = allocate_chunk(tail, size);
if (!space)
return NULL;

}
else
{
space->free = 0;
space->debug_data = 0x99999999;
}
}
return (space + 1);
}
void * malloc(size_t size)
{
size_t * ret = mymalloc(sizeof(size_t) + size);
*ret = size;
return &ret[1];
}
size_t allocated_size(void * ptr)
{
return ((size_t*) ptr)[-1];
}


struct meta_data *get_space_ptr(void *ptr)
{
return (struct meta_data*) ptr - 1;
}
void myfree(void *ptr)
{
if (!ptr)
return;
struct meta_data* space_ptr = get_space_ptr(ptr);
assert(space_ptr->free == 0);
assert(space_ptr->debug_data == 0x99999999 || space_ptr->debug_data ==
0x12345678);
space_ptr->free = 1;
space_ptr->debug_data = 0x55555555;
}
void free(void * ptr)
{
memset(ptr, 0, (sizeof(*ptr)));
myfree((size_t*) ptr - 1);
}
size_t check_overflow(size_t a, size_t b)
{
size_t size = a * b;
assert(a && size / a == b);
return size;
}
void *calloc(size_t nelem, size_t elsize)
{
size_t size = check_overflow(nelem, elsize);
void *ptr = malloc(size);
memset(ptr, 0, size);
return ptr;
}
int main()
{
int *i, j, *k, *l;
int *arr;
i = (int *) malloc(sizeof(int) * 4); //Allocating space for i
if (i != NULL) //Checking if space for i is allocated or not

//CMPE 220 Fall 2016 Date: 17th November 2016//

//14
printf("i:\tValue = %d\t\tAddress = %p\t\tsize = %d\n", *i, i, (int)

allocated_size(i)); //Printing details of i
else
printf("Space for i is not allocated\n");

printf("j:\tValue = %d\t\tAddress = %p\tsize = %d\n", j, &j, (int) sizeof(j));

k = (int *) malloc(11); //Allocating space for i
if (k != NULL) //Checking if space for k is allocated or not
printf("k:\tValue = %d\t\tAddress = %p\t\tsize = %d\n", *k, k, (int)

allocated_size(k)); //Printing details of k
else
printf("Space for k is not allocated\n");

free(i); //freeing space allocated for i

l = (int *) malloc(5); //Allocating space for i
if (l != NULL) //Checking if space for l is allocated or not
printf("l:\tValue = %d\t\tAddress = %p\t\tsize = %d\n", *l, l, (int)

allocated_size(l)); //Printing details of l
else
printf("Space for l is not allocated\n");
if (arr != NULL) //Checking if space for arr is allocated or not
printf("arr:\tValue = %d\t\tAddress = %p\t\tsize = %d\n", *arr, arr,

(int) allocated_size(arr)); //Printing details of arr
else
printf("Space for arr is not allocated\n");
arr = (int *) calloc(5, sizeof(int)); //allocating space for array of integers
if (arr != NULL) //Checking if space for arr is allocated or not
printf("\n\narr:\tValue = %d\t\tAddress = %p\t\tsize = %d\n", *arr, arr,

(int) allocated_size(arr)); //Printing details of arr
else
printf("Space for arr is not allocated\n");

//CMPE 220 Fall 2016 Date: 17th November 2016
											
//15
printf("\nElements of Arr are as follows\n\n");//printing elements of array
if (arr != NULL) //Checking if space for arr is allocated or not
{
printf("arr[0]:\tValue = %d\t\tAddress = %p\t\tsize = %d\n", *arr, arr,

(int) sizeof(*arr)); //Printing details of arr

printf("arr[1]:\tValue = %d\t\tAddress = %p\t\tsize = %d\n", *(arr+1),

(arr+1), (int) sizeof(*(arr+1))); //Printing details of arr

printf("arr[2]:\tValue = %d\t\tAddress = %p\t\tsize = %d\n", *(arr+2),

(arr+2), (int) sizeof(*(arr+2))); //Printing details of arr

printf("arr[3]:\tValue = %d\t\tAddress = %p\t\tsize = %d\n", *(arr+3),

(arr+3), (int) sizeof(*(arr+3))); //Printing details of arr

printf("arr[4]:\tValue = %d\t\tAddress = %p\t\tsize = %d\n", *(arr+4),

(arr+4), (int) sizeof(*(arr+4))); //Printing details of arr
}
return 0;
}
