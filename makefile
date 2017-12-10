all: client.o control.o
	gcc -o client client.o
	gcc -o control control.o

client.o: client.c
	gcc -c client.c

control.o: control.c
	gcc -c control.c

clean:
	./control -r
	rm -f story.txt
	rm -f *~
	rm -f *.o
	rm -f client
	rm -f control
