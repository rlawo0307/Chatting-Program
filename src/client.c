#include "common.h"

#define CERTFILE "client.pem"

void init_openSSL()
{
        SSL_load_error_strings();
        SSL_library_init();
        OpenSSL_add_all_algorithms();
}
SSL_CTX *setup_client_ctx(void){
        SSL_CTX *ctx;

        ctx = SSL_CTX_new(SSLv23_method());
        if(SSL_CTX_use_certificate_chain_file(ctx, CERTFILE) !=1 )
                int_error("Error loading certificate from file");
	if(SSL_CTX_use_PrivateKey_file(ctx, CERTFILE, SSL_FILETYPE_PEM) != 1)
                int_error("Error loading private key from file");
	return ctx;
}
void* Send(void* arg)
{
	SSL     *ssl = (SSL*)arg;
        char    buf[1024];
        while(1)
	{
		scanf("%s", buf);
                SSL_write(ssl, buf, strlen(buf));
	}
        return NULL;
}
void* Recv(void* arg)
{
        SSL     *ssl = (SSL*)arg;
        char    buf[1024];
        int len;

        while(1)
        {
		len = SSL_read(ssl, buf, sizeof(buf)-1);
                buf[len] = '\0';
		if(strcmp(buf, "")!=0)
			printf("Other Player : %s\n\n", buf);
        }
        return NULL;
} 
int main(int argc, char *argv[])
{
	BIO     *conn;
        SSL     *ssl;
        SSL_CTX *ctx;
	pthread_t send, recv;
	void	*thread_return;

        init_openSSL( );

        ctx = setup_client_ctx();

	if(!(ssl = SSL_new(ctx)))
                int_error("Error creating an SSL context");

	conn = BIO_new_connect(SERVER ":" PORT);
        
	SSL_set_bio(ssl, conn, conn);

	if(SSL_connect(ssl) <= 0)
                int_error("Error connecting SSL object");

	printf("SSL Connection opened\n\n");
        
	pthread_create(&send, NULL, Send, ssl);
	pthread_create(&recv, NULL, Recv, ssl);
	pthread_join(send, &thread_return);
	pthread_join(recv, &thread_return);

	BIO_free(conn);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 0;
}	
