#include "lab3_memfunc.h"
#include <stdio.h>
#include <pthread.h>

#define MEM_BLOCKS 3

pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

typedef struct memory_block{
	size_t size;
	int *ptr;
} memory_block;

typedef struct arguments{
	int number; // multiple of three
	memory_block *blocks;
} arguments;

//allocating
void *firstfunc(void *ar){
	arguments* arg = (arguments*) ar;
	memory_block *blocks = arg->blocks;
	int number = arg->number; 
	size_t sizes[MEM_BLOCKS] = {16, 1024, 1024*1024};
	for(int i = 0; i < MEM_BLOCKS; i++){
		blocks[i + MEM_BLOCKS*number].size = sizes[i];
		blocks[i + MEM_BLOCKS*number].ptr = my_malloc(sizes[i]);
	}
	return NULL;
}

//filling
void *secondfunc(void *ar){
	arguments* arg = (arguments*) ar;
	memory_block *blocks = arg->blocks;
	int number = arg->number;
	printf("this is %d\n", number);
	for(int i = 0; i < MEM_BLOCKS; i++){
		int* ptr = blocks[i].ptr;
		*ptr = number + 1;
		printf("this is %d\n", number+1);
	}
	return NULL;
}

//saving
void *trirdfunc(void *ar){
	arguments* arg = (arguments*) ar;
	memory_block *blocks = arg->blocks;
	int number;
	number = arg->number;
	FILE *fp = fopen("lab3.txt", "a");
	if(fp == NULL){
		printf("error of opening");
	}
	for(int i = 0; i < MEM_BLOCKS; i++){
		pthread_mutex_lock(&mutex2);
		fprintf(fp,"%d\n", *blocks[i].ptr);
		pthread_mutex_unlock(&mutex2);
		my_free(blocks[i].ptr);
	}
	fclose(fp);
	return 0;
}

int main(){
	int M, num, num2;
	printf("Type number of threads (devided into three)\n");
	scanf("%d", &num);
	if(num % 3 != 0){
		printf("Please type the correct number\n");
		scanf("%d", &num2);
		if(num2 % 3 != 0){
			printf("error");
			return -1;
		}
		else{
			num = num2;
		}
	}
	printf("Type number of iterations\n");
	scanf("%d", &M);
	num--;
	memory_block* blocks = (memory_block*) my_malloc(sizeof(memory_block) * num);
	printf("got malloced\n");
	arguments args[num];
	for(int i = 0; i < num; i++){
		args[i].blocks = blocks;
		args[i].number = i;
	}
	printf("opening thread\n");
	pthread_t threads[num];
	FILE *fp = fopen("lab3.txt", "w");
	if(fp == NULL){
		printf("error of opening");
	}
	fclose(fp);
	int result;
	for(int i = 0; i < M; i++){
		for(int j = 0; j < num/3; j++){
			result = pthread_create(&threads[j], NULL, (void*) firstfunc, (void*) &args[j]);
			printf("1");
			if(result != 0){
				printf("error %d\n", result);
				return -1;
			}
		}
		for (int j = 0; j < num/3; j++){
			result = pthread_join(threads[j], NULL);
			if(result != 0){
				printf("join error, %d", result);
				return -1;
			}
		}
		printf("first end\n");
		for(int j = num/3; j < (num/3)*2; j++){\
			result = pthread_create(&threads[j], NULL, (void*) secondfunc, (void*) &args[j]);
			printf("2");
			if(result != 0){
				printf("error %d\n", result);
				return -1;
			}
		}
		for (int j = num/3; j < (num/3)*2; j++){
			result = pthread_join(threads[j], NULL);
			if(result != 0){
				printf("join error, %d", result);
				return -1;
			}
		}
		printf("second end\n");
		for(int j = (num/3)*2; j < (num/3)*3; j++){
			result = pthread_create(&threads[j], NULL, (void*) trirdfunc, (void*) &args[j]);
			printf("3");
			if(result != 0){
				printf("error %d\n", result);
				return -1;
			}
		}
		for (int j = (num/3)*2; j < (num/3)*3; j++){
			result = pthread_join(threads[j], NULL);
			if(result != 0){
				printf("join error, %d", result);
				return -1;
			}
		}
		printf("third end\n");
	}
	my_free(blocks);
	return 0;
}
