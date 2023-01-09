CC = gcc

# To link all header files of optimized and reference in include folder
INC_QS = optimized/include
INC_REF = reference/include

INC_Groestl = hash_algorithms/grostl/Optimized_32bit/include
INC_JHcompression = hash_algorithms/JHcompression/Optimized_32bit/include
INC_Skein = hash_algorithms/skein/include
INC_Echo = hash_algorithms/echo/Optimized_32bit/include
INC_Blake = hash_algorithms/blake/Optimized_32bit/include

# Flags of GCC compiler (-Ofast -fomit-frame-pointer -fforce-addr)
FLAGS = -flto -w -O3 -Wall -ffast-math -ffinite-math-only -fno-signed-zeros -pthread
INTEL_FLAGS = -msse2 -msse -march=native -maes -DAESNI
ARM_FLAGS = -DAESARM
#FLAGS = -O3 -Wall -march=native -maes

# Path of all source code files in src folder
QS_FILES = optimized/src/aes.c optimized/src/aes_ni.c optimized/src/aes_arm.c optimized/src/padding.c optimized/src/qs_permutation.c optimized/src/quicksand.c
REF_QS_FILES = reference/src/ref_aes_ni.c reference/src/ref_padding.c reference/src/ref_qs_permutation.c reference/src/ref_quicksand.c

Groestl_FILES = hash_algorithms/grostl/Optimized_32bit/Groestl-opt.c
Skein_FILES = hash_algorithms/skein/skein.c hash_algorithms/skein/skein_block.c hash_algorithms/skein/SHA3api_ref.c
Echo_FILES = hash_algorithms/echo/Optimized_32bit/echo_32.c
Blake_FILES = hash_algorithms/blake/Optimized_32bit/blake_opt32.c

# Openssl Flags
OPENSSL_FLAGS = -lssl -lcrypto
# Openssl paths in MAC
# Modify this path to your openssl system path.
MAC_OPENSSL_INC = /usr/local/Cellar//openssl@1.1/1.1.1q/include
MAC_OPENSSL_LIB = /usr/local/Cellar//openssl@1.1/1.1.1q/lib

# Compile for Mac:
BENCHMARK_LINUX = $(CC)  benchmark.c -o benchmark $(QS_FILES)   $(Groestl_FILES) $(Skein_FILES) $(Echo_FILES) $(Blake_FILES) $(FLAGS) $(INTEL_FLAGS) -L/usr/lib/x86_64-linux-gnu -lssl -lcrypto -I$(INC_QS)   -I$(INC_Groestl)  -I$(INC_JHcompression) -I$(INC_Skein) -I$(INC_Echo) -I$(INC_Blake)
BENCHMARK_MAC_INTEL = $(CC) $(QS_FILES)   $(Groestl_FILES) $(Skein_FILES) $(Echo_FILES) $(Blake_FILES) $(FLAGS) $(OPENSSL_FLAGS) $(INTEL_FLAGS) -I$(MAC_OPENSSL_INC)  -L$(MAC_OPENSSL_LIB) -I$(INC_QS)   -I$(INC_Groestl) -I$(INC_JHcompression) -I$(INC_Skein) -I$(INC_Echo) -I$(INC_Blake) benchmark.c -o benchmark

LIBRARY_NAME = libquicksand


build: clean
	$(CC) -c $(FLAGS) $(INTEL_FLAGS) $(QS_FILES) -I$(INC_QS)
	ar rcs $(LIBRARY_NAME).a $(QS_FILES)


quicksand: clean
	$(CC) $(QS_FILES) optimized/src/quicksand_main.c -o quicksand $(FLAGS) $(INTEL_FLAGS) -I$(INC_QS)

ref_quicksand: clean
	$(CC) $(REF_QS_FILES) reference/src/ref_quicksand_main.c  -o ref_quicksand $(FLAGS) $(INTEL_FLAGS) -I$(INC_REF) 

benchmark: clean
	$(BENCHMARK_MAC_INTEL)
#	$(BENCHMARK_LINUX)

test: clean
	$(CC) $(QS_FILES) $(REF_QS_FILES)  main.c -o test $(FLAGS) -I$(INC_QS) -I$(INC_REF)

clean:
	rm -f -r quicksand ref_quicksand benchmark test *.dSYM quicksand_arm *.o *.a

	