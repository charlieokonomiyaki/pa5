all: random
random: random.c
	gcc -Wall -Werror -fsanitize=address -std=c11 random.c -o random
clean:
	rm -rf random