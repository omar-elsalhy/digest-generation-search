.PHONY: build compile run

build:
	docker build . -t ggc-openssl-image:v1

compile:
	docker run --rm -it -v "$(PWD):/usr/src/myapp" -w /usr/src/myapp ggc-openssl-image:v1 gcc -Wall ./src/main.c ./src/binaryTree.c -o ./t3c -lcrypto

run:
	./t3c

clean:
	rm ./t3c


#Brouillon Makefile (si le Makefile faisait la compilation seulement)


# the compiler: gcc for C program, define as g++ for C++
#CC = gcc

# compiler flags:
# -g adds debugging information to the executable file
# -Wall turns on most, but not all, compiler warnings
#CFLAGS = -g -Wall

# the build target executable:
#TARGET = hello-universe
#all: $(TARGET)
#$(TARGET): $(TARGET).c
#	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
#clean:
#	$(RM) $(TARGET)