SRC_DIR = src
BUILD_DIR = build

CC = gcc
CFLAGS = -O3 -std=c23 -D_XOPEN_SOURCE=500 -g
TARGET = build/fish_editor

LDFLAGS = -O3

SRCS = $(shell find src -type f -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) 

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $< -o $@ $(CFLAGS) -c

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
