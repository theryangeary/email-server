CC = g++
CFLAGS = -g
SRC = email.cpp
OBJS = $(SRC:.cpp=.o)
all: email

email: $(OBJS)
	$(CC) -o $@ $^

#test: $(OBJS) test.o
#	$(CC) -o $@ $^
#	./test
#	echo "YOUR TESTS PASSED :)"

clean:
	rm email *.o
