CC=cc
CFLAGS=-g -O3 -Wall -I/usr/local/include
LDFLAGS=-static -L/usr/local/lib
LDLIBS=-lkcgi -lkcgijson -lz
HTDOCS=/var/www/htdocs
CGIBIN=/var/www/cgi-bin

olc: calculator.o http.o json.o process.o
	$(CC) $(LDFLAGS) -o $(.TARGET) $(.ALLSRC) $(LDLIBS)

calculator.o: calculator.c calculator.h http.h json.h process.h
	$(CC) $(CFLAGS) -c calculator.c

http.o: http.c
	$(CC) $(CFLAGS) -c http.c

json.o: json.c http.h process.h
	$(CC) $(CFLAGS) -c json.c

process.o: process.c calculator.h http.h process.h json.h
	$(CC) $(CFLAGS) -c process.c

depend:
	$(CC) -E -MM *.c > .depend

splint:
	splint -weak +unixlib *.c

install: olc
	install -o www -g www -m 0500 olc $(CGIBIN)
	install -o www -g www -m 0440 htdocs/robots.txt $(HTDOCS)
	install -o www -g www -m 0440 htdocs/sitemap.xml $(HTDOCS)

clean:
	rm -rf *.o *.core olc

.PHONY: install clean
