ROOT_DIR := $(shell realpath ./)
BUILD_DIR := $(ROOT_DIR)/llvm-src/build

CC := $(BUILD_DIR)/bin/clang
OPT := $(BUILD_DIR)/bin/opt

CFLAGS := -c -emit-llvm -S

TEST_FILES := $(wildcard tests/*.c)
LL_FILES := $(TEST_FILES:.c=.ll)
BIN_FILES := $(TEST_FILES:.c=)

#-----------------------------------------------------------------------------#
# Compile source files to human-readable LLVM IR code.
#-----------------------------------------------------------------------------#
all: all-tests

tests/%.ll: tests/%.c
	@echo "Compiling src to human-readable LLVM IR"
	$(CC) $(CFLAGS) -o $@ $<

all-tests: $(LL_FILES)

#-----------------------------------------------------------------------------#
# Compile the tests to executables with the memory checking facility.
#-----------------------------------------------------------------------------#

lib/mem_checker.o: lib/mem_checker.c
	$(CC) -c -fPIC $< -o $@

lib/libmem_checker.so: lib/mem_checker.o
	@echo "Creating the mem_checker lib"
	$(CC) -shared -o $@ $<
	@rm -f $<

libmm_checker: lib/libmem_checker.so

tests/%.bc: tests/%.ll
	@echo "Instrumenting IR"
	$(OPT) -passes=llvm-tutorial $< -o $@

tests/%: tests/%.bc libmm_checker
	@echo "Compiling tests to executables"
	$(CC) -Llib -lmem_checker $< -o $@

.PHONY: bin
bin: $(BIN_FILES)

clean:
	rm -f tests/*.ll tests/*.o tests/*.bc $(BIN_FILES) lib/*.so lib/*.o

.PHONY: all clean
