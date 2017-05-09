all:
	gcc -o dijkstra list.c main.c list.c graph.c vertice.c edge.c fibheap.c binheap.c dijkstra.c state.c hashmap.c dstarlite.c -lm -O3

