CC = g++
CFLAGS = -g -std=c++11 -I/usr/include/python2.7 -I/usr/include/x86_64-linux-gnu/python2.7  -fno-strict-aliasing -Wdate-time -D_FORTIFY_SOURCE=2 -g -fdebug-prefix-map=/build/python2.7-3hk45v/python2.7-2.7.15~rc1=. -fstack-protector-strong -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O2  
LFLAGS = -lsqlite3 -lgcrypt -L/usr/lib/python2.7/config-x86_64-linux-gnu -L/usr/lib -lpython2.7 -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions
OFLAGS =
SRC = email.cpp
OBJS = $(SRC:.cpp=.o)
all: email

email: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

clean:
	rm email *.o
