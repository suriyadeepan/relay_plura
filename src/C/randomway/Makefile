main:  main.o node.o
	gcc main.o node.o -o main.bin -lm

main.o:  main.c node.c
	gcc -c node.c main.c 

clean:
	rm -f *.o *~ *.bin
