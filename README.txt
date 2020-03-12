Socket과 SSL 그리고 Mutex를 이용하여 통신보안을 강화한 채팅 프로그램


1. 컴파일
gcc -g -Wall -pthread -c common.c -o common.o
gcc -g -Wall -pthread -c server.c -o server.o
gcc -g -Wall -pthread -c client.c -o client.o
gcc -g -Wall -pthread -o server server.o common.o -lssl -lcrypto
gcc -g -Wall -pthread -o client client.o common.o -lssl -lcrypto

2. SSL key
./server : prkey
./cilent pskey