mymake2: mymake2.o graph.o makereader.o utils.o
	gcc -Wall -g mymake2.o graph.o makereader.o utils.o -o mymake2

mymake2.o: mymake2.c utils.h graph.h makereader.h
	gcc -Wall -c mymake2.c -o mymake2.o

graph.o: graph.c utils.h graph.h
	gcc -Wall -c graph.c -o graph.o

makereader.o: makereader.c utils.h makereader.h
	gcc -Wall -c makereader.c -o makereader.o

utils.o: utils.c utils.h
	gcc -Wall -c utils.c -o utils.o

clean:
	rm -f mymake2 *.o
