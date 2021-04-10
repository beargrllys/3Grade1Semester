CC = gcc

httpServe : httpServe.o
	gcc httpServe.o -o httpServe

httpServe.o : httpServe.c
	gcc -c httpServe.c  

clean : 
	rm -rf *.o httpServe