#include "common.h"

#define CERTFILE "server.pem"

int cnt = 0;
SSL	*clients[10];
pthread_mutex_t mutex;
THREAD_TYPE *tids;

void init_openSSL()
{
        SSL_load_error_strings();
        SSL_library_init();
        OpenSSL_add_all_algorithms();
}
SSL_CTX *setup_server_ctx(void){
        SSL_CTX *ctx;

        ctx = SSL_CTX_new(SSLv23_method( ));

        if(SSL_CTX_use_certificate_file(ctx, CERTFILE, SSL_FILETYPE_PEM) != 1)
                int_error("Error loading certificate from file");
        if(SSL_CTX_use_PrivateKey_file(ctx, CERTFILE, SSL_FILETYPE_PEM)  != 1)
                int_error("Error loading private key from file");
        return ctx;
}
void *ServerFunc(void *arg)
{
	SSL	*ssl = (SSL*)arg;
        char 	buf[1024];
	int	len;
	int 	read;

        while(1)
	{
		len = SSL_read(ssl, buf, sizeof(buf)-1);
		buf[len] = '\0';
		if(strcmp(buf, "") != 0)
		{
			pthread_mutex_lock(&mutex);
			for(int i=0; i<cnt; i++)
				if(pthread_self() == tids[i])
					read = i;
			printf("Server read from Player %d : %s\n\n", read+1, buf);
			for(int i=0; i<cnt; i++)
				if(i != read)
					SSL_write(clients[i], buf, strlen(buf));
			pthread_mutex_unlock(&mutex);
		}
	}
}
int main(int argc, char *argv[])
{
	BIO     *server;
        BIO     *client;
        SSL     *ssl;
        SSL_CTX *ctx;

        init_openSSL( );

        ctx = setup_server_ctx( );

	if(!(ssl = SSL_new(ctx)))
                int_error("Error creating an SSL context");

	if((server = BIO_new_ssl(ctx, 0)) == NULL)
                int_error("Error matching bio with ssl");

	if((server = BIO_new_accept(PORT)) == 0)
                int_error("Error creating server socket");

        if (BIO_do_accept(server) <= 0) //server connect
                int_error("Error binding server socket");
	
	printf("Success Connecting Server\n\n");
	
	tids = (THREAD_TYPE*)malloc(sizeof(THREAD_TYPE)*10);

    	while(1)
    	{
        	if (BIO_do_accept(server) <= 0) // accept client
                	        int_error("Error acceptiong connection");
		
		client = BIO_pop(server);

		SSL_set_bio(ssl, client, client);

		if(SSL_accept(ssl) <= 0)
                	int_error("Error accepting SSL connection");
		
		printf("Player%d SSL Connection opened\n\n", cnt+1);

        	pthread_mutex_lock(&mutex);
        	clients[cnt++] = ssl;
		pthread_mutex_unlock(&mutex);
 
        	pthread_create(&(tids[cnt-1]), NULL, ServerFunc, ssl);

		if(!(ssl = SSL_new(ctx)))
                	int_error("Error creating an SSL context");
    	}

	BIO_free(server);
	BIO_free(client);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	free(tids);
	return 0;
}
