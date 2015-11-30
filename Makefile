# Make file for program 5

# Create main executable.
p5: cs2123p5Driver.o cs2123p5DannyN.o cs2123p5Quote.o
	gcc -g -o p5 cs2123p5Driver.o cs2123p5DannyN.o cs2123p5Quote.o

# Create compile of Driver
cs2123p5Driver.o: cs2123p5Driver.c
	gcc -g -c -o cs2123p5Driver.o cs2123p5Driver.c

# Create compile of DannyN extra file
cs2123p5DannyN.o: cs2123p5DannyN.c
	gcc -g -c -o cs2123p5DannyN.o cs2123p5DannyN.c

# Create compile of Quote file
	gcc -g -c -o cs2123p5Quote.o cs2123p5Quote.c

# Clean out all files
clean: 
	rm *.o
