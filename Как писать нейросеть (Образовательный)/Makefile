CC=gcc
LD=gcc
LIBS:=-lm
TEST_LIBS:=-lm -lcunit

nn: nn.o matrix.o tensor.o optimizer.o
	$(LD) -o $@ $^ $(LIBS)

simplenn: simplenn.o matrix.o tensor.o optimizer.o
	$(LD)  -o $@ $^ $(LIBS)

tests: test_matrix.o matrix.o tensor.o optimizer.o
	$(LD)  -o $@ $^ $(TEST_LIBS)

clean:
	rm *.o

.PHONY: clean

%.o:%.c
	$(CC) -O3 -ffast-math -march=native -c -o $@ $^
