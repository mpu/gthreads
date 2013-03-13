CFLAGS = -g -Wall

gttest: gthr.o gtswtch.o
	$(CC) -o $@ $^

.S.o:
	as -o $@ $^

.PHONY: clean
clean:
	rm -f *.o gttest
