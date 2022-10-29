CC      = g++
CFLAGS  = -Wall -O2 -g
CFLAGS_O  = -Wall -O
OBJECTS = prodcon.o qcontrol.o iocontrol.o tands.o
OBJECTS_O = prodcono.o qcontrolo.o iocontrolo.o tandso.o

prodcon: $(OBJECTS)
	$(CC) -pthread -o prodcon $(OBJECTS)
prodcon.o: prodcon.cpp
	$(CC) $(CFLAGS) -c prodcon.cpp -o prodcon.o
qcontrol.o: qcontrol.cpp
	$(CC) $(CFLAGS) -c qcontrol.cpp -o qcontrol.o
iocontrol.o: iocontrol.cpp
	$(CC) $(CFLAGS) -c iocontrol.cpp -o iocontrol.o
tands.o: tands.c
	gcc $(CFLAGS) -c tands.c -o tands.o

prodcono: $(OBJECTS_O)
	$(CC) -pthread -o prodcon $(OBJECTS_O)
prodcono.o: prodcon.cpp
	$(CC) $(CFLAGS_O) -c prodcon.cpp -o prodcono.o
qcontrolo.o: qcontrol.cpp
	$(CC) $(CFLAGS_O) -c qcontrol.cpp -o qcontrolo.o
iocontrolo.o: iocontrol.cpp
	$(CC) $(CFLAGS_O) -c iocontrol.cpp -o iocontrolo.o
tandso.o: tands.c
	gcc $(CFLAGS_O) -c tands.c -o tandso.o
clean:
	rm *.o prodcon