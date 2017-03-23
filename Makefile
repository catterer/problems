all: braces gen_braces

braces: braces.cc
	g++ braces.cc -std=c++1y -Wall -o braces
gen_braces: gen_braces.cc
	g++ gen_braces.cc -std=c++1y -Wall -o gen_braces

clean:
	rm -rf braces gen_braces

