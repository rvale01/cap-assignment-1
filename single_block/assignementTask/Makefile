CC      = gcc
CFLAGS  = -g
RM      = rm -f

default: all

all: assignmentTask

assignmentTask: main.c
	$(CC) $(CFLAGS) -o main main.c -lssl -lcrypto

clean:
	$(RM) assignmentTask