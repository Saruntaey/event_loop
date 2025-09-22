CC = gcc
CFLAGS = -Wall -Wextra -Werror
GLTHREAD_DIR = ../../glthread/
LDFLAGS = -L$(GLTHREAD_DIR)
IDFLAGS = -I$(GLTHREAD_DIR)
TARGET = exe
SRCS = $(shell find . -type f -name '*.c')
OBJS = $(patsubst ./%.c, %.o, $(SRCS))

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) -lglthread

%.o: %.c
	$(CC) -c $(CFLAGS) $(IDFLAGS) -o $@ $<

.PHONY: clean run

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
