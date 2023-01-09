
CC = gcc

# Flags of GCC compiler
# FLAGS = -O3 -g -msse2 -msse -march=native -maes 
#FLAGS_aes_ni = -O3 -g  -maes
#FLAGS_aes = -O3 -g

INC = ./include

# Path of all source code files in src folder
Skein_FILES =  skein.c skein_block.c SHA3api_ref.c


skein: clean
	$(CC) $(Skein_FILES) example.c -o skein -O3 -I$(INC)

clean:
	rm -f skein *.dSYM 
