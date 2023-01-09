CC = gcc

# To link all header files in include folder
INC = ./include

# Flags of GCC compiler
FLAGS = -O3 -g  -maes
# Path of all source code files in src folder
REF_QS_FILES = src/ref_aes_ni.c src/ref_padding.c src/ref_qs_permutation.c src/ref_quicksand.c

ref_quicksand: clean
	$(CC) $(REF_QS_FILES) src/ref_quicksand_main.c  -o ref_quicksand $(FLAGS) -I$(INC)


clean:
	rm -f ref_quicksand
