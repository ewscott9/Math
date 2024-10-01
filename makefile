CFLAGS=-Wall -g

all:
	make math

clean: 
	rm -f math stats.txt

push:
	git push https://github.com/ewscott9/Math
