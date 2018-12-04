CC = g++
CFLAGS = -g -std=c++11 
LFLAGS = -lsqlite3
OFLAGS = 
SRC = email.cpp
OBJS = $(SRC:.cpp=.o)
all: email

email: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

clean:
	rm email *.o
