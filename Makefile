PROGRAM   = kockica
CC        = g++
CFLAGS    = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS   = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm


$(PROGRAM): kockica.o image.o
	$(CC) $(LDFLAGS) -o $(PROGRAM) image.o kockica.o $(LDLIBS) 
	

.PHONY: clean dist

clean:
	-rm -f *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)




run:
	./$(PROGRAM)
