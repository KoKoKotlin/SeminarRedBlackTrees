SHELL = /bin/sh
CC = gcc
LIBS = -lm
FLAGS = -Wall -Wextra

OBJECTS = main.o rbtree.o stack.o
TARGET = rbtree.out

all: FLAGS += -O3
all: $(TARGET)

debug: FLAGS += -DDEBUG -g -ggdb
debug: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)

shared:
	echo "Unimplemented"

main.o : include/rbtree.h include/log.h
rbtree.o : include/rbtree.h	include/log.h
stack.o : include/log.h

%.o : %.c
	$(CC) -c $(FLAGS) $<

.PHONY : clean
clean :
	rm -rf $(TARGET) $(OBJECTS) 