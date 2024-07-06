CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c warp.c peek.c pastevents.c exec.c seek.c prompt.c proclore.c IOredirec.c signal.c BF.c neonate.c iMan.c
OBJS = $(SRCS:.c=.o)

TARGET = a.out

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
