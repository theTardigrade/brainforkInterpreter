CC = gcc
CFLAGS = -ansi
CC_CFLAGS = $(CC) $(CFLAGS)
CFLAGS_DEV = -Wall -pedantic -Wextra -Werror \
			 -g -ggdb3
CFLAGS_PRO = -DNDEBUG

INPUT = main.c err.c load.c run.c opts.c
TARGET = bfi
INPUT_OUTPUT = -o $(TARGET) $(INPUT)

all: pro

pro:
	$(CC_CFLAGS) $(CFLAGS_PRO) $(INPUT_OUTPUT)

dev:
	$(CC_CFLAGS) $(CFLAGS_DEV) $(INPUT_OUTPUT)

syn:
	$(CC_CFLAGS) $(CFLAGS_DEV) -fsyntax-only $(INPUT)

clean:
	$(RM) -v $(TARGET)
