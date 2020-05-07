PROGRAM   = zadatak
CC        = g++
CFLAGS    = -g -Wall
LDFLAGS   = -lGL -lGLU -lglut -lm

$(PROGRAM): skelet.o
	$(CC) -o $(PROGRAM) skelet.o $(LDFLAGS)
	

.PHONY: clean dist

clean:
	-rm -f *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)




run:
	./$(PROGRAM)
