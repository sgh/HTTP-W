// HTTPS SERVER
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "http_server.h"
#include "http_ssl.h"

#define PORT 8080


// openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 10000 -nodes

int main(int argc, char const *argv[])
{
    SSL_CTX *ctx;
    init_openssl();
    ctx = create_context();
    configure_context(ctx);

    int client;
    int server_fd = createSocket(PORT);

    for (;;)
    {
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        int valread;
        char buffer[1024] = {0};

        SSL *ssl;

        if ((client = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accpet");
            exit(EXIT_FAILURE);
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        ERR_clear_error();
        auto ret = SSL_accept(ssl);
//        fprintf(stderr, "SSL_accept: %d   %d\n", ret, SSL_get_error(ssl, ret));
        if(ret <= 0){
            ERR_print_errors_fp(stderr);
        }
        else{
//            showCerts(ssl);
            valread = SSL_read(ssl, buffer, sizeof(buffer));

            if (valread > 0)
            {
                std::string response = getFile(buffer, address);
                SSL_write(ssl, response.c_str(), response.size());
            }
        }
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client);
    }
    close(server_fd);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}