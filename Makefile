all:
	gcc -o dijkstra main.c list.c binheap.c state.c hashmap.c dstarlite.c -lm -O3

