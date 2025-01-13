# Variables
KEY_FILE = server.key              # Private key file
CSR_FILE = server.csr              # Certificate Signing Request file
CRT_FILE = server.crt              # Server certificate file
CRT_FILE_CLIENT = client.crt       # Client certificate file
CERT_HEX_FILE = cert_hex.c         # Certificate in hex format
KEY_HEX_FILE = key_hex.c           # Key in hex format
PASSPHRASE_FILE = passphrase.txt   # File for storing passphrase
SRC_FILE = server.c                # Server source file
OBJ_FILE = server.o                # Object file for the server
EXE_FILE = server                  # Final executable file
LIBS = -lssl -lcrypto              # OpenSSL libraries

# Compiler and linker
CC = gcc
CFLAGS = -Wall -g                  # Compiler flags for warnings and debugging

# Command to generate the private key with a random passphrase
generate_key:
	@openssl rand -base64 20 > $(PASSPHRASE_FILE)
	@echo "Passphrase generated and stored in $(PASSPHRASE_FILE)"
	openssl genpkey -algorithm RSA -out $(KEY_FILE) -aes256 -pass pass:$$(cat $(PASSPHRASE_FILE))
	openssl rsa -in $(KEY_FILE) -outform DER -out $(KEY_FILE) -passin pass:$$(cat $(PASSPHRASE_FILE))

# Command to generate the CSR (Certificate Signing Request)
generate_csr: $(KEY_FILE)
	openssl req -new -key $(KEY_FILE) -out $(CSR_FILE) -passin pass:$$(cat $(PASSPHRASE_FILE))

# Command to generate the self-signed certificate
generate_crt: $(CSR_FILE)
	openssl x509 -req -in $(CSR_FILE) -signkey $(KEY_FILE) -out $(CRT_FILE_CLIENT) -days 365 -passin pass:$$(cat $(PASSPHRASE_FILE))
	openssl x509 -in $(CRT_FILE_CLIENT) -outform DER -out $(CRT_FILE)

# Command to convert the certificate to hexadecimal format
generate_cert_hex: $(CRT_FILE)
	xxd -i $(CRT_FILE) > $(CERT_HEX_FILE)

# Command to convert the key to hexadecimal format
generate_key_hex: $(KEY_FILE)
	xxd -i $(KEY_FILE) > $(KEY_HEX_FILE)

# Compile the server
compile_server: $(SRC_FILE) $(CERT_HEX_FILE) $(KEY_HEX_FILE)
	$(CC) $(CFLAGS) -o $(EXE_FILE) $^ $(LIBS)

# Clean up the generated files
clean:
	rm -f $(KEY_FILE) $(CSR_FILE) $(CRT_FILE) $(CERT_HEX_FILE) $(KEY_HEX_FILE) $(OBJ_FILE) $(PASSPHRASE_FILE)

# Default rule: generate everything (key, certificate, then compile)
all: generate_key generate_csr generate_crt generate_cert_hex generate_key_hex compile_server

.PHONY: generate_key generate_csr generate_crt generate_cert_hex generate_key_hex compile_server clean all
