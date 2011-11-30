run:
	runghc Run.hs

copilot.c: Compile.hs Fibonacci.hs
	runghc Compile.hs

compile: copilot.o main.o
	gcc -o Fibonacci $^

clean:
	rm -f copilot.c copilot.h Fibonacci
	rm -f *~ *.o *.hi

.PHONY: run clean
