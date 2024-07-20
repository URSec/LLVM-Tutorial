# LLVM Tutorial for Security Practitioners

This tutorial intends to teach the audience several core concepts of LLVM, using
an example pass that creates and inserts a function call to a library function into
the target program.

## Tutorial Video

[Video](https://www.youtube.com/watch?v=JMXhmMQ0bNw)

**Note**: The tutorial lecture was based on LLVM 8 and the old pass manager,
while the code used in the Docker image is based on LLVM 18 and the new pass manager.
You will therefore find inconsistencies in the pass's interface between the lecture
and the code in the Docker image. However, the core APIs for writing the pass
remain the same.

## Docker Image

We have created a Linux-AMD64-based Docker image containing a ready-to-use
development environment for writing LLVM IR passes, as you'll need for the
tutorial. It contains an installation of LLVM 18.1.8 (the latest stable version
as of this writing), along with the source code. Use the following command to
download the docker image

```shell
docker pull jzhou41/llvm-tutorial
```

The docker image has been tested in an M-chip MacOS and an x86-64 Ubuntu. Please
raise an issue if it does not work on your host machine.

## How to Use

### Start the docker

Run `run-container` to start a Docker container.

### Write and compile your own pass

Open `/llvm-tutorial/llvm-src/llvm/lib/Transforms/Utils/LLVMTutorial.cpp` to
write the pass under the skeleton code. After you finish the pass, go to
`/llvm/tutorial/llvm-src/build/` and compile the new compiler with

```shell
ninja clang opt
```

After that, you should be able to use the new pass to transform test programs.

#### Editor
The Docker image was preinstalled with `vim` and `nano`. However, feel free to
install your favorite editor using `apt` or `brew`.

### Compile test programs to human-readable LLVM IR files (.ll)

All test programs are located in `/llvm-tutorial/tests`.

Use the `Makefile` to compile the test program to `.ll` files:

```Makefile
make
```

### Run your pass (named "llvm-tutorial") on a selected test program

```shell
./run-passes a_program_in_tests
```

This will create an instrumented human-readable LLVM IR file.

### Compile test programs to executables

```Makefile
make bin
```

This will compile all test programs to binaries instrumented by your LLVM pass.
You can run the binaries to see how the pass changes the target program.