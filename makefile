SOURCE_FILES = math.c my_io.c
HEADER_FILES = math.h my_io.h
CC = gcc
CFLAGS = -Wall -g

all:
	$(CC) $(CFLAGS) $(SOURCE_FILES) -o math

edit:
	vim -u vimrc -p $(SOURCE_FILES) $(HEADER_FILES)

clean: 
	rm -f math stats.txt

push:
	git push https://github.com/ewscott9/Math
