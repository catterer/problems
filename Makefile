all: braces gen_braces reverse remove_duplicates_o2

braces: braces.cc
	g++ braces.cc -std=c++1y -Wall -o braces
gen_braces: gen_braces.cc
	g++ gen_braces.cc -std=c++1y -Wall -o gen_braces
reverse: reverse.c
	gcc reverse.c -Wall -o reverse
remove_duplicates_o2: remove_duplicates_o2.c
	gcc remove_duplicates_o2.c -Wall -o remove_duplicates_o2

clean:
	rm -rf braces gen_braces reverse remove_duplicates_o2

