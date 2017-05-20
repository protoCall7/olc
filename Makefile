P=calculator
OBJECTS=
CFLAGS=-g -Wall -O3 -I/usr/local/include
LDFLAGS=-static -L/usr/local/lib
LDLIBS=-lkcgi -lkcgijson -lz
CC=cc

$(P): $(OBJECTS)

install: $(P)
	install -o www -g www -m 0500 $(P) /var/www/cgi-bin

clean:
	rm -rf $(P) $(OBJECTS) *.core
