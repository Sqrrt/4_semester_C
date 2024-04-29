#include "lab3.h"

t_block find_block(t_block *last, size_t s, void *first_block){
	t_block b = first_block;
	while(b && !(b->free && b->size >= s)){
		*last = b;
		b = b->next;
	}
	return b;
}
t_block extend_heap(t_block last, size_t s){
	t_block b;
	b = sbrk(0);
	if(sbrk(BLOCK_SIZE + s) == (void *)-1)
		return NULL;
	b->size = s;
	b->next = NULL;
	if(last)
		last->next = b;
	b->free = 0;
	return b;
}

void split_block(t_block b, size_t s){
	t_block new;
	new->data[1] = b->data[1] + s;
	new->size = b->size - s - BLOCK_SIZE;
	new->next = b->next;
	new->free = 1;
	b->size = s;
	b->next = new;
}

size_t align8(size_t s){
	if(s & 0x7 == 0){
		return s;
	}
	return ((s >> 3) + 1) << 3;
}

t_block get_block(void *p){
	char *tmp;
	tmp = p;
	return ( p = tmp -= BLOCK_SIZE);
}

int valid_address(void *p,void *first_block){
	if(first_block){
		if (p > first_block && p < sbrk(0)){
			return p == (get_block(p))->ptr;
		}
	}
	return 0;
}

t_block fusion(t_block b){
	if(b->next && b->next->free){
		b->size += BLOCK_SIZE + b->next->size;
		b->next = b->next->next;
		if(b->next){
			b->next->prev = b;
		}
	}
	return b;
}

void copy_block(t_block src, t_block dst){
	size_t *sdata, *ddata;
	sdata = src->ptr;
	ddata = dst->ptr;
	for(size_t i = 0; (i*8) < src->size && (i*8)< dst->size; i++){
		ddata[i] = sdata[i];
	}
}
