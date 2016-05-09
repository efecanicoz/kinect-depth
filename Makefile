
#-I/usr/local/include/libfreenect
#-L/usr/lib/x86_64-linux-gnu/

CFLAGS=-O3 -g
LDFLAGS= -I/usr/include/libusb-1.0 -I/usr/include/libpng12  
LDLIBS=-L/usr/local/lib -L/usr/lib/x86_64-linux-gnu/ -L$(shell pwd)/lib/ -lpng12 -lm -lGL -lGLU -lglut -lpthread -lfreenect -lusb-1.0

all: clean measure-depth measure-depth-example

measure-depth-example: measure-depth-example.o
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	
measure-depth-example.o:
	gcc $(CFLAGS) -c measure-depth-example.c $(LDFLAGS) $(LDLIBS)
	

measure-depth: measure-depth.o
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	
measure-depth.o:
	gcc $(CFLAGS) -c measure-depth.c $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o measure-depth measure-depth-example
