objects = main.o input.o calcu.o 
main: $(objects)
	gcc -o main $(objects)
%.o : %.c 
	gcc -c $<

clean:
	rm *%.o
	rm main
