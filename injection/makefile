# File Variables
PROG1_BIN = prog1
PROG2_BIN = prog2
PROG1_C = PROG1_BIN.c
PROG2_C = PROG2_BIN.c
LINKER_C = injection.c

# Arrays of binaries to generate
PROG1_BIN_ARRAY = ssl/PROG1_BIN
PROG2_BIN_ARRAY = base_program/PROG2_BIN

# Final executable name
EXEC = $(PROG1_BIN).exe

# Compiler and options
CC = gcc
CFLAGS = -Wall -fPIC -I/usr/include/openssl

# Link to OpenSSL libraries
LIBS = -lssl -lcrypto

# Folder containing PEM files
CERT_DIR = certs

# Certificate and key files
CERT_FILE = $(CERT_DIR)/server.crt
KEY_FILE = $(CERT_DIR)/server.key

# Compile C binary files from the binaries (xxd)
$(PROG1_C): $(PROG1_BIN)
	xxd -i $< > $@

$(PROG2_C): $(PROG2_BIN)
	xxd -i $< > $@

# Compile object files and final program
$(EXEC): $(LINKER_C) $(PROG1_C) $(PROG2_C)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile PROG1 program
PROG1: ssl/PROG1.c
	$(CC) $(CFLAGS) -o $(PROG1_BIN) ssl/PROG1.c $(LIBS)

# Target to run the linker program
run: $(EXEC)
	sudo ./$(EXEC)

# Target to clean generated files
clean:
	rm -f $(PROG1_C) $(PROG2_C)
	rm -rf .config

# Default target: do everything (generate binaries and compile)
all: $(EXEC)

.PHONY: all clean run
