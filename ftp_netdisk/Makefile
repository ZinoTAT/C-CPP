SRC:=$(wildcard *.c)
process_pool_server:$(SRC)
	gcc $^ -o $@ -lpthread  -I /usr/include/mysql/ -lmysqlclient -lcrypto -lcrypt
clean:
	rm process_pool_server
