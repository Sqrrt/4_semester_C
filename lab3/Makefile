Lab3 : lab3_main.o lab3_memfunc.o lab3.o
	gcc lab3_main.o lab3_memfunc.o lab3.o -o Lab3
lab3_main.o: lab3_main.c
	gcc -c -g -pthread lab3_main.c -o lab3_main.o
lab3_memfunc.o: lab3_memfunc.c
	gcc -c -g -pthread lab3_memfunc.c -o lab3_memfunc.o
lab3.o: lab3.c
	gcc -c -g -pthread lab3.c -o lab3.o
clean:
	rm Lab3 *.o
