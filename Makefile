#
# Simple Makefile
# Mike Lam, James Madison University, August 2016
# Edited for Josh Makela, 2024
#
# This makefile builds a simple application that contains a main module
# (specified by the EXE variable) and a predefined list of additional modules
# (specified by the MODS variable).
# Extenal library dependencies (e.g., the math library, "-lm") are listed in the LIBS variable.
# If there are any precompiled object files, list them in the OBJS variable.
#
# By default, this makefile will build the project with debugging symbols and
# without optimization. To change this, edit or remove the "-g" and "-O0"
# options in CFLAGS and LDFLAGS accordingly.



# application-specific settings and run target

EXE=spreadsheet
MODS=expression.o cell.o grid.o runtime.o token.o lexer.o parser.o interface.o main.o
OBJS=
LIBS=
MODEL=expression.o cell.o grid.o runtime.o
VIEW=token.o lexer.o parser.o


default: build $(EXE)

build:
	mkdir build

test: $(EXE)
	make -C tests test

# compiler/linker settings

CC=g++ #C++ Compiler (maybe use clang++? IDK)
CFLAGS=-g -O0 -Wall --std=c++20 -pedantic
LDFLAGS=-g -O0


# build targets
# make -C tests clean //Formally cleaned the "test" folder in a cs361 project

BUILD=$(addprefix build/, $(MODS))

$(EXE): build/main.o $(BUILD) $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXE) $^ $(LIBS) -lncurses

build/%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf $(EXE) build $(MODS)

clean_model:
	cd build && rm -rf $(MODEL)

clean_view:
	cd build && rm -rf $(VIEW)

.PHONY: default clean

