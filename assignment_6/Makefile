CC = gcc
CFLAGS = -I.
DEPS = 20180284.h
OBJ = 20180284.o

%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)
20180284: $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS)
	rm -rf $^

.PHONY: clean
clean: 
	rm -f *.o 20180284
