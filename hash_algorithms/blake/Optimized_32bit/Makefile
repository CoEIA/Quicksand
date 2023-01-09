
CC = gcc

# Flags of GCC compiler
# FLAGS = -O3 -g -msse2 -msse -march=native -maes 
#FLAGS_aes_ni = -O3 -g  -maes
#FLAGS_aes = -O3 -g

INC = ./include

# Path of all source code files in src folder
blake_FILES = blake_opt32.c


blake: clean
	$(CC) $(blake_FILES) example.c -o blake -O3 -I$(INC)


clean:
	rm -f blake *.dSYM 
