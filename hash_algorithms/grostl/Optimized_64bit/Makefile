
CC = gcc

# Flags of GCC compiler
# FLAGS = -O3 -g -msse2 -msse -march=native -maes 
#FLAGS_aes_ni = -O3 -g  -maes
#FLAGS_aes = -O3 -g

INC = ./include

# Path of all source code files in src folder
Groestl_FILES = Groestl-opt.c


groestl: clean
	$(CC) $(Groestl_FILES) example.c -o groestl -I$(INC)


clean:
	rm -f groestl *.dSYM 
