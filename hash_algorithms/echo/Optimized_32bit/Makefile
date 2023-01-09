
CC = gcc

# Flags of GCC compiler
# FLAGS = -O3 -g -msse2 -msse -march=native -maes 
#FLAGS_aes_ni = -O3 -g  -maes
#FLAGS_aes = -O3 -g

INC = ./include

echo: clean
	$(CC) echo_32.c example.c -o echo -O3 -I$(INC)

clean:
	rm -f echo *.dSYM 
