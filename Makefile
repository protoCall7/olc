CC=cc
CFLAGS=-g -O3 -Wall -I/usr/local/include
LDFLAGS=-static -L/usr/local/lib
LDLIBS=-lkcgi -lkcgijson -lz

calculator: calculator.o http.o json.o process.o
	$(CC) $(LDFLAGS) -o calculator $(.ALLSRC) $(LDLIBS)

calculator.o: calculator.c calculator.h http.h json.h process.h
	$(CC) $(CFLAGS) -c calculator.c

http.o: http.c
	$(CC) $(CFLAGS) -c http.c

json.o: json.c calculator.h http.h
	$(CC) $(CFLAGS) -c json.c

process.o: process.c calculator.h http.h json.h
	$(CC) $(CFLAGS) -c process.c

install: calculator
	install -o www -g www -m 0500 calculator /var/www/cgi-bin

clean:
	rm -rf *.o *.core calculator

.PHONY: install clean
