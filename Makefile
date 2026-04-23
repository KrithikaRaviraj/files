CC      = gcc
CFLAGS  = -Wall -Wextra -g

ifeq ($(OS),Windows_NT)
TARGET  = slr_q7.exe
RM      = del /Q
RUN_CMD = .\$(TARGET)
else
TARGET  = slr_q7
RM      = rm -f
RUN_CMD = ./$(TARGET)
endif

SRCS    = main.c lexer.c slr_parser.c semantic.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	$(RUN_CMD)

clean:
	-$(RM) $(OBJS) $(TARGET)
