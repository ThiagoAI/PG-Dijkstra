all:
	gcc -o dijkstra main.c list.c binheap.c state.c hashmap.c dstarlite.c astar.c -lm -lglut -lGL -lGLU -O3
