APP_NAME = math
SOURCE_FILES = math.c my_io.c
HEADER_FILES = math.h my_io.h
CC = gcc
CFLAGS = -Wall -g

all:
	$(CC) $(CFLAGS) $(SOURCE_FILES) -o $(APP_NAME)

edit:
	vim -u vimrc -p $(SOURCE_FILES) $(HEADER_FILES)

clean: 
	rm -f $(APP_NAME)
