#run:
#	runghc Run.hs

compile: copilot.o main.o
	gcc -o Main $^ -lcwiid

copilot.c: Compile.hs Wiimote.hs
	runghc Compile.hs

clean:
	rm -f copilot.c copilot.h Main
	rm -f *~ *.o *.hi

.PHONY: run clean
