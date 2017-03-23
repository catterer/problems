all: braces gen_braces reverse

braces: braces.cc
	g++ braces.cc -std=c++1y -Wall -o braces
gen_braces: gen_braces.cc
	g++ gen_braces.cc -std=c++1y -Wall -o gen_braces
reverse: reverse.c
	gcc reverse.c -Wall -o reverse

clean:
	rm -rf braces gen_braces

