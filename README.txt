Socket�� SSL �׸��� Mutex�� �̿��Ͽ� ��ź����� ��ȭ�� ä�� ���α׷�


1. ������
gcc -g -Wall -pthread -c common.c -o common.o
gcc -g -Wall -pthread -c server.c -o server.o
gcc -g -Wall -pthread -c client.c -o client.o
gcc -g -Wall -pthread -o server server.o common.o -lssl -lcrypto
gcc -g -Wall -pthread -o client client.o common.o -lssl -lcrypto

2. SSL key
./server : prkey
./cilent pskey