SHELL = /bin/sh
CC = gcc
LIBS =
FLAGS = -Wall -Wextra

OBJECTS = main.o rbtree.o
TARGET = rbtree.out

all: $(TARGET)

debug: FLAGS += -DDEBUG -g
debug: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)

shared:
	echo "Unimplemented"

main.o : rbtree.h log.h
rbtree.o : rbtree.h	log.h

%.o : %.c
	$(CC) -c $(FLAGS) $<

.PHONY : clean
clean :
	rm -rf $(TARGET) $(OBJECTS) 