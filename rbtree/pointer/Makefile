SHELL = /bin/sh
CC = gcc
LIBS = -lm
FLAGS = -Wall -Wextra

OBJECTS = main.o stack.o visualize.o rbtree.o binary.o
TARGET = rbtree.out

all: FLAGS += -O3
all: $(TARGET)

debug: FLAGS += -g -ggdb
debug: $(TARGET)

debugOutput: FLAGS += -DDEBUG
debugOutput: debug

log: debug
log:
	./$(TARGET) > log

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)

shared:
	echo "Unimplemented"

main.o : include/rbtree.h include/log.h
rbtree.o : include/rbtree.h	include/log.h
stack.o : include/log.h
visualize.o: include/visualize.h

%.o : %.c
	$(CC) -c $(FLAGS) $<

.PHONY : clean
clean :
	rm -rf $(TARGET) $(OBJECTS) log