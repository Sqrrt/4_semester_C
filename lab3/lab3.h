#ifndef LAB3_H
#define LAB3_H

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define BLOCK_SIZE 24
typedef struct s_block *t_block;

struct s_block{
	size_t size;
	t_block next;
	t_block prev;
	int free;
	int padding;
	void* ptr;
	char data [1];
};

t_block find_block(t_block *last, size_t s,void *first_block);
t_block extend_heap(t_block last, size_t s);
void split_block(t_block b, size_t s);
size_t align8(size_t s);
t_block get_block(void *p);
int valid_address(void *p,void *first_block);
t_block fusion(t_block b);
void copy_block(t_block src, t_block dst);

#endif /*LAB3_H*/
