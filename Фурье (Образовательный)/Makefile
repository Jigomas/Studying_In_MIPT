all: ft pngfft ft2d_test

ft2d_test: ft.o ft2d_test.o
	$(CC) -o $@ $^ -lm

ft: ft.o ft_test.o
	$(CC) -o $@ $^ -lm

pngfft: ft.o pngfft.o
	$(CC) -o $@ $^ -lm

%.o:%.c
	$(CC) -c -O3 -o $@ $<
