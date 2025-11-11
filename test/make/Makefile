
objects:=hello.o main.o

.PHONY: clean

main: $(objects)
	gcc $(objects) -o main

main.o:main.c hello.h
	gcc -c main.c -o main.o

hello.o:hello.c hello.h
	gcc -c hello.c -o hello.o

clean:
	rm $(objects) main