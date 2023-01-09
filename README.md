# **QuickSand**
QuickSand is a hash function dependent on the sponge constructor. It accepts any input of arbitrary length and generates a hash digest of 1024 bits. QuickSand provides superior security and efficiency in comparison to other hash algorithms.

The QuickSand implementation is based on the paper entitled "**Quicksand a Hash Function with 1024-bit digest**" which is referred to as the QuickSand document.



### **QuickSand implementation authors :**
* Saleh Almousa, s.almousa@coeia.edu.sa
* Ali Babtain, a.babtain@coeia.edu.sa
* Abdullah Almosallam, a.almosallam@coeia.edu.sa

***


## **Features:**
* Take any input length.
* Produce output with length 1024.
* Stronger Security as it is depend on sponge constructor.



## **Supported Output Length:**
>| **Output Length**     ||
>| ---     | ---
>| **1024** bits |
>|   |

***

# **Usage**
### **Note:**
When you run Quicksand on MacOS you need to modify the path of Openssl in the **Makfile**. Just edit **MAC_OPENSSL_INC**  and **MAC_OPENSSL_LIB**.<br> 
```makefile
# Modify this path to your openssl system path.
MAC_OPENSSL_INC = /usr/local/Cellar//openssl@1.1/1.1.1q/include
MAC_OPENSSL_LIB = /usr/local/Cellar//openssl@1.1/1.1.1q/lib
```


## **For Testing \*Optimized\* Code:**

```bash
cd quicksand/optimized
```
**Quicksand with \*`AES_NI`\*:**

Using make tool:
```bash
make quicksand
```
Then run:
```bash
./quicksand
```
**Quicksand with \*`AES`\*:**

Using make tool:
```bash
make quicksand_aes
```
Then run:
```bash
./quicksand_aes
```

## **For Testing \*Reference\* Code:**

```bash
cd quicksand/reference
```
**Quicksand with \*`AES_NI`\*:**

Using make tool:
```bash
make ref_quicksand
```
Then run:
```bash
./ref_quicksand
```

## **For Testing Both \*Optimization and Reference\*:**
```bash
cd quicksand
```

Using make tool:
```bash
make test
```
Then run:
```bash
./test
```
***


# **Documentation**

## **Functions in \*Optimization\*:**

**Initialization function:**

```c
QS_CTX qs_init();
```
This function will return `void pointer`. So you need to return the void pointer to `QS_CTX ctx`.


**Update (Absorb) function:**

```c
int qs_update(QS_CTX ctx, uint8_t *input, uint64_t input_len);
```

**Digest function:**

```c
int qs_digest(QS_CTX ctx, uint8_t *output);
```
return `digest` with size `128 Bytes`.

**Cleanup function:**

```c
void qs_cleanup(QS_CTX ctx);
```
Cleanup (`Close or Deconstruct`) function.


## **Functions in \*Reference\*:**

**Initialization function:**

```c
int ref_qs_init(void);
```

**Update (Absorb) function:**

```c
int ref_qs_update(uint8_t *input, uint64_t input_len);
```

**Digest function:**

```c
int ref_qs_digest(uint8_t *output);
```
return `digest` with size `128 Bytes`.

**Cleanup function:**

```c
void ref_qs_cleanup();
```
Cleanup (`Close or Deconstruct`) function.

***

# **Benchmark**
Benchmark will run the followin Hash algorithms:

 ### **Hash algorithms:**
* Quicksand_1024.
* Sha3_256.
* Sha3_512.
* Groestl_256.
* Groestl_512.
* JHcompression_256.
* JHcompression_512.
* Skein_256.
* Skein_512.
* Skein_1024.
* Echo_256.
* Echo_512.
* Blake_256.
* Blake_512.


 ### **Tested sizes for each Hash algorithms:**
* 1 KB, 4 KB, 8 KB, 16 KB, 1 MB, 4 MB, 8 MB, 16 MB, 1 GB


## **For Run \*Benchmark\*:**
```bash
cd quicksand
```

Using make tool:
```bash
make benchmark
```

Then run:

```bash
./benchmark
```



