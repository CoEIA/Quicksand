CC = gcc

# To link all header files in include folder
INC = ./include

# Flags of GCC compiler
# FLAGS = -O3 -g -msse2 -msse -march=native -maes 
FLAGS_aes_ni = -O3 -g  -maes
FLAGS_aes = -O3 -g

# Path of all source code files in src folder
QS_FILES = src/aes.c src/aes_ni.c src/padding.c src/qs_permutation.c src/quicksand.c


quicksand: clean
	$(CC) $(QS_FILES) src/quicksand_main.c -o quicksand $(FLAGS_aes_ni) -I$(INC)

quicksand_aes: clean
	$(CC) $(QS_FILES) src/quicksand_main.c -o quicksand_aes $(FLAGS_aes) -I$(INC)


clean:
	rm -f quicksand_aes quicksand 
