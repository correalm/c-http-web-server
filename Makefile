build: main.c
	gcc main.c -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o main

run:
	make build && ./main

format:
	clang-format -i main.c

test:
	./run_tests.sh $(suite)
