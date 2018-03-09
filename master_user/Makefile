
CC=arm-linux-gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-g -Wall

EXEC_FILE=master_listen

all: $(EXEC_FILE)

$(EXEC_FILE): master_listen.o 
	$(CC) $^ -o $@

clean: 
	rm  -rf $(EXEC_FILE)  *.o *~ 
