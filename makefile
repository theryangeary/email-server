CC = g++
CFLAGS = -g -lsqlite3
SRC = email.cpp
OBJS = $(SRC:.cpp=.o)
all: email

email: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

#test: $(OBJS) test.o
#	$(CC) -o $@ $^
#	./test
#	echo "YOUR TESTS PASSED :)"

clean:
	rm email *.o
