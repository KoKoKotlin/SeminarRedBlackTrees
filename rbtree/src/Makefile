SHELL = /bin/sh
CC = gcc
LIBS = -lm
FLAGS = -Wall -Wextra -Iinclude/

OBJECTS = main.o stack.o visualize.o binary.o 
TARGET = rbtree.out

# for compiling the recursive version of the code
ifeq ($(strip $(REC)), )
OBJECTS += rbtree_s.o
else
OBJECTS += rbtree_h.o
endif

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

main.o : include/rbtree.h include/log.h
rbtree_s.o : include/rbtree.h	include/log.h
rbtree_h.o : include/rbtree.h	include/log.h
stack.o : include/log.h
visualize.o: include/visualize.h

%.o : %.c
	$(CC) -c $(FLAGS) $<

.PHONY : clean
clean :
	rm -rf $(TARGET) $(OBJECTS) log rbtree_h.o rbtree_s.o