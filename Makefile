CC = gcc
CFLAGS = -Werror 
LDFLAGS =  -lcrypto
test_my_program: main.c
	$(CC) $(CFLAGS) main.c -o TEST $(LDFLAGS) 

.PHONY: clean
clean:
	rm -f TEST



