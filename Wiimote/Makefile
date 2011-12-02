#run:
#	runghc Run.hs

all: compile

compile: copilot.o main.o
	gcc -o Main $^ -lcwiid -lm

copilot.c: Compile.hs Wiimote.hs
	runghc Compile.hs

test:
	./Main

clean:
	rm -f copilot.c copilot.h Main
	rm -f *~ *.o *.hi

.PHONY: all clean test
