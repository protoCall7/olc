all: calculator.cgi

calculator.o: calculator.c
	cc -ggdb -I/usr/local/include -c -o calculator.o calculator.c

calculator.cgi: calculator.o
	cc -ggdb -static -L/usr/local/lib -o calculator.cgi calculator.o -lkcgi -lkcgijson -lz

install: calculator.cgi
	install -o www -g www -m 0500 calculator.cgi /var/www/cgi-bin

clean:
	rm -rf *.o *.cgi *.core

.PHONY: all install clean
