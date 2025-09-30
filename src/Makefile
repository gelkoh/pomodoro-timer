CC ?= gcc

PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk4)
LIBS = $(shell $(PKGCONFIG) --libs gtk4)

SRC = main.c
OBJS = $(SRC:.c=.o)

all: pomodoro-timer

%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

pomodoro-timer: $(OBJS)
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f pomodoro-timer
