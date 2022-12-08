detect: detect.o graph.o utils.o
	gcc -Wall -o detect -g -lm detect.o graph.o utils.o

detect.o: detect.c graph.h utils.h
	gcc -c detect.c -Wall -g 

graph.o: graph.c graph.h
	gcc -c graph.c -Wall -g
	
utils.o: utils.c utils.h
	gcc -c utils.c -Wall -g

make clean:
	rm -f detect *.o