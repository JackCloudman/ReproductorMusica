run: a.out
	./a.out
a.out: Song.o main.o Play_list.o
	gcc Song.o main.o Play_list.o -l vlc `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
main.o: main.c
	gcc -c `pkg-config --cflags gtk+-3.0` -o main.o main.c `pkg-config --libs gtk+-3.0` -Wall
Song.o: Song.c
	gcc -c Song.c -Wall
Play_list.o: Play_list.c
	gcc -c Play_list.c -Wall
remove: Song.o main.o Play_list.o a.out
	rm Song.o main.o Play_list.o a.out
