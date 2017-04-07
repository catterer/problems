all: braces gen_braces reverse remove_duplicates_o2 tree_hor median deps

braces: braces.cc
	g++ braces.cc -std=c++1y -Wall -o braces
gen_braces: gen_braces.cc
	g++ gen_braces.cc -std=c++1y -Wall -o gen_braces
tree_hor: tree_hor.cc
	g++ tree_hor.cc -std=c++14 -Wall -o tree_hor
reverse: reverse.c
	gcc reverse.c -Wall -o reverse
remove_duplicates_o2: remove_duplicates_o2.c
	gcc remove_duplicates_o2.c -Wall -o remove_duplicates_o2
median: median.cc
	g++ median.cc -Wall -o median
deps: deps.cc
	g++ deps.cc -Wall -o deps

clean:
	rm -rf braces gen_braces reverse remove_duplicates_o2 tree_hor median deps

