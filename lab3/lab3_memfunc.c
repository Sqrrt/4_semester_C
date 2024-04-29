#include "lab3_memfunc.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *first_block = NULL;

void *my_malloc(size_t size){
	t_block b, last;
	size_t s;

	pthread_mutex_lock(&mutex);

	s = align8(size);
	if(first_block){
		last = first_block;
		b = find_block(&last, s, first_block);
		if(b){
			if((b->size - s) >= (BLOCK_SIZE + 8)){
				split_block(b,s);
			}
			b->free = 0;
		}
		else{
			b = extend_heap(last,s);
			if(!b){
				pthread_mutex_unlock(&mutex);
				return NULL;
			}
		}
	}
	else{
		b = extend_heap(NULL, s);
		if(!b){
			pthread_mutex_unlock(&mutex);
			return NULL;
		}
		first_block = b;
	}
	pthread_mutex_unlock(&mutex);
	b->ptr = &(b->data);
	return b->data;
}

void *my_calloc(size_t number, size_t size){
	size_t *new;
	size_t i8, i;
	new = my_malloc(number * size);
	if(new){
		i8 = align8(number * size) >> 3;

		for(i = 0; i < i8; i++){
			new[i] = 0;
		}
	}
	return new;
}

void my_free(void *p) {
	t_block b;
	pthread_mutex_lock(&mutex);
	if (valid_address(p, first_block)){
		b = get_block(p);
		b->free = 1;
		if(b->prev && b->prev->free){
			b = fusion(b->prev);
		}
		if(b->next){
			fusion(b);
		}
		else{
			if(b->prev){
				b->prev->next = NULL;
			}
			else{
				first_block = NULL;
			}
			brk(b);
		}
	}
	pthread_mutex_unlock(&mutex);		
}
void *my_realloc(void *p, size_t size){
	size_t s;
	t_block b, new;
	void *newp;
	if(!p){
		return my_malloc(size);
	}
	if (valid_address(p, first_block)){
		s = align8(size);
		b = get_block(p);
		if(b->size >= s){
			if(b->size - s >= (BLOCK_SIZE + 8)){
				split_block(b,s);
			}
		}
		else{
			if(b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s){
				fusion(b);
				if(b->size - s >= (BLOCK_SIZE + 8)){
					split_block(b, s);
				}
			}
			else{
				newp = my_malloc(s);
				if(!newp){
					return NULL;
				}
				new = get_block(newp);
				copy_block(b, new);
				my_free(p);
				return (newp);
			}
		}
		return (p);
	}
	return NULL;
}
/*int main(){
	int* numbers = my_calloc(sizeof(int), 10);
	int count =10;
	for(int i = 0; i < count; i++){
		numbers[i] = i+1;
	}
	for(int i = 0; i < count; i++){
		printf("%d\n", numbers[i]);
	}
	my_free(numbers);
}
*/
