ifeq ($(shell uname), Darwin)
	APPLE_CCFLAGS = -m64
	APPLE_ASFLAGS = -arch x86_64
endif

CFLAGS = $(APPLE_CCFLAGS) -g -Wall

gttest: gthr.o gtswtch.o
	$(CC) $(APPLE_CCFLAGS) -o $@ $^

.S.o:
	as $(APPLE_ASFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o gttest
