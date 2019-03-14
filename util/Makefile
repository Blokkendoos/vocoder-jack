CC=g++
CFLAGS= -I/opt/local/include -L/opt/local/lib -ljack -lsndfile

all:	play_loop

play_loop:
	$(CC) $(CFLAGS) -o play_loop play_loop.cc

clean:
	rm -f *~ *.o play_loop
