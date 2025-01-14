#include <openssl/ssl.h>
#include <openssl/err.h>

void init_openssl();
void cleanup_openssl();
SSL_CTX *create_context();
void configure_context(SSL_CTX *ctx);
void showCerts(SSL *ssl);
void loadCertificates(SSL_CTX *ctx, char *CertFile, char *KeyFile);
