ROOT_DIR := $(shell realpath ./)
BUILD_DIR := $(ROOT_DIR)/llvm-src/build

CC := $(BUILD_DIR)/bin/clang
OPT := $(BUILD_DIR)/bin/opt

CFLAGS := -c -emit-llvm -S

TEST_FILES := $(wildcard tests/*.c)
LL_FILES := $(patsubst tests/%.c, tests/%.ll, $(TEST_FILES))
BC_FILE := $(LL_FILES:.ll=.bc)
BIN_FILE := $(BC_FILE:.bc=)

#-----------------------------------------------------------------------------#
# Compile source files to human-readable LLVM IR code.
#-----------------------------------------------------------------------------#
all: all-tests

tests/%.ll: tests/%.c
	$(CC) $(CFLAGS) -o $@ $<

all-tests: $(LL_FILES)

#-----------------------------------------------------------------------------#
# Compile the tests to executables with the memory checking facility.
#-----------------------------------------------------------------------------#

lib/mem_checker.o: lib/mem_checker.c
	$(CC) $(INCLUDE) -c $< -o $@

memcheck_lib: lib/mem_checker.o

tests/%.bc: tests/%.ll
	$(OPT) -passes=llvm-tutorial $< -o $@

tests/%: tests/%.bc memcheck_lib
	$(CC) lib/mem_checker.o $< -o $@

bin: $(BIN_FILE)

clean:
	rm -f tests/*.ll tests/*.o tests/*.bc $(BIN_FILE)

.PHONY: all clean
