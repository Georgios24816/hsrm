SRCS = $(shell ls *.cpp)
OBJS = $(addsuffix .o,$(basename $(SRCS)))

CC = g++
CFLAGS = -Wall -Wextra -std=c++17

Aufgabe_c: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -c -o $@

run: Aufgabe_c
	./Aufgabe_c

clean:
	rm $(OBJS)

