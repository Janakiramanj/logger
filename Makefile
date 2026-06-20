CC = gcc
CFLAGS = -Wall -g -Iinclude

BUILD = build

APP = $(BUILD)/logger

all: $(APP)

$(APP): src/logger.c src/find.c  
	$(CC) $(CFLAGS) $^ -o $@


clean:
	rm -rf $(BUILD)/*
