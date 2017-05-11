p1.x: p1.c
	gcc -D PROB1 -o p1.x p1.c
p2.x: p1.c
	gcc -D -o p2.x p1.c
p3.x: p3.c
	gcc -o p3.x p3.c

clean:
	rm *.x


