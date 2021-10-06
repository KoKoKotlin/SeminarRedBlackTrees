SHELL = /bin/sh
CC = gcc
LIBS = -lm
FLAGS = -Wall -Wextra -Iinclude/

OBJECTS = main.o stack.o visualize.o binary.o 
TARGET = rbtree.out

TEST_OBJECTS = test.o stack.o visualize.o binary.o 
TEST_TARGET = test.out

# for compiling the recursive version of the code
ifeq ($(strip $(REC)), )
OBJECTS += rbtree_s.o
TEST_OBJECTS += rbtree_s.o
else
OBJECTS += rbtree_h.o
TEST_OBJECTS += rbtree_h.o
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

test: OBJECTS += test.o
test: $(TEST_TARGET)
test:
	./test.out

$(TEST_TARGET) : $(TEST_OBJECTS)
	$(CC) -o $(TEST_TARGET) $(TEST_OBJECTS) $(LIBS)

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)

main.o : include/rbtree.h include/log.h
rbtree_s.o : include/rbtree.h	include/log.h
rbtree_h.o : include/rbtree.h	include/log.h
stack.o : include/log.h
visualize.o: include/visualize.h
test.o: include/rbtree.h include/log.h

%.o : %.c
	$(CC) -c $(FLAGS) $<

.PHONY : clean
clean :
	rm -rf $(TARGET) $(TEST_TARGET) $(OBJECTS) log rbtree_h.o rbtree_s.o test.o