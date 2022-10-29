CC      = g++
CFLAGS  = -Wall -O2
OBJECTS = prodcon.o qcontrol.o iocontrol.o tands.o

prodcon: $(OBJECTS)
	$(CC) -pthread -o  prodcon $(OBJECTS)
prodcon.o: prodcon.cpp
	$(CC) $(CFLAGS) -c prodcon.cpp -o prodcon.o -g
qcontrol.o: qcontrol.cpp
	$(CC) $(CFLAGS) -c qcontrol.cpp -o qcontrol.o -g
iocontrol.o: iocontrol.cpp
	$(CC) $(CFLAGS) -c iocontrol.cpp -o iocontrol.o -g
tands.o: tands.c
	gcc $(CFLAGS) -c tands.c -o tands.o -g
clean:
	rm *.o prodcon