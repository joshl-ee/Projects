CC = g++
CPPFLAGS = -g -Wall

rsa: rsa.o
	$(CC) $(CPPFLAGS) $^ -o ./rsa

rsa.o: rsa.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

MinHeap.o: MinHeap.h
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	-@rm -rf *.o
