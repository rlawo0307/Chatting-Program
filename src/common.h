#ifndef COMMON_H
#define COMMON_H

#include <string.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include<stdio.h>
#include<stdlib.h>

#ifndef WIN32
#include <pthread.h>
#define THREAD_CC
#define THREAD_TYPE pthread_t
#define THREAD_CREATE(tid, entry, arg) pthread_create(&(tid), NULL, (entry), (arg))

#else
#define THREAD_CC __cdecl
#define THREAD_TYPE DWORD

#endif

#define PORT "16001"
#define SERVER "localhost"
#define CLIENT "localhost"

#define int_error(msg) handle_error(__FILE__, __LINE__, msg)
void handle_error(const char *file, int lineno, const char *msg);

#endif
