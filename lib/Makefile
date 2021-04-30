SHELL = /bin/sh
CC = gcc
LIBS =
FLAGS = -Wall -Wextra

OBJECTS = main.o rbtree.o
TARGET = rbtree.out

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)

main.o : rbtree.h
rbtree.o : rbtree.h

%.o : %.c
	$(CC) -c $(FLAGS) $<

.PHONY : clean
clean :
	rm -rf $(TARGET) $(OBJECTS) 