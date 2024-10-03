SOURCE_FILES = math.c my_io.c
CC = gcc
CFLAGS = -Wall -g

all:
	$(CC) $(CFLAGS) $(SOURCE_FILES) -o math

clean: 
	rm -f math stats.txt

push:
	git push https://github.com/ewscott9/Math
