CC=g++
CFLAGS=`sdl-config --cflags`
CLIBS=`sdl-config --libs`
DBFLAG= 		# '-g' to insert debugging information, otherwise ''
FNAME=SDLMemory

CFLAGS:=$(CFLAGS) $(DBFLAG)

$(FNAME): SDLMemory.o accs.o
	$(CC) -o $(FNAME) SDLMemory.o accs.o $(CLIBS) -lSDL -lSDL_image

SDLMemory.o: SDLMemory.cpp
	$(CC) $(CFLAGS) -c SDLMemory.cpp

accs.o: accs.cpp
	$(CC) $(CFLAGS) -c accs.cpp

clean:
	rm -f *.o $(FNAME)
