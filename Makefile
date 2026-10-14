.PHONY: build compile run

build:
	docker build . -t ggc-openssl-image:v1

compile:
	#docker run --rm -it -v "$(realpath ./src/)":/usr/src/myapp -w /usr/src/myapp ggc-openssl-image:v1 gcc -Wall *.c -o myapp
	docker run --rm -it -v "$(CURDIR)/src/:/usr/src/myapp" -w /usr/src/myapp ggc-openssl-image:v1 gcc -Wall *.c -o ../t3c -lcrypto -lssl

run:
	./t3c

