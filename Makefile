NASM = nasm
# -g flag - show bebug info
ASMFLAGS += -g -f elf32

CFLAGS ?= -O2 -g

CFLAGS += -std=gnu99

CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal -Wabsolute-value

CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error

CC = gcc
CC += -m32 -no-pie -fno-pie

LDLIBS = -lm

.DELETE_ON_ERROR:

.PHONY: all clean test

all: integral
# $@ - name of .o file, target
# $< - name of .c/.cpp file, source
# $^ - requirements args 
integral: integral.c funcs.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

funcs.o: funcs.asm
	$(NASM) $(ASMFLAGS) $< -o $@

test: all
	./integral --test-root 1:2:-10.0:10.0:0.0001:5.09838
	./integral --test-root 3:4:-10.0:10.0:0.0001:3.847760
	./integral --test-root 5:6:-10.0:10.0:0.0001:4.549509
	./integral --test-integral 4:1.0:5.0:0.0001:16.0000
	./integral --test-integral 5:1.0:5.0:0.0001:8.0000
	./integral --test-integral 6:1.0:5.0:0.0001:-8.0471895


clean:
	rm -rf *.o
