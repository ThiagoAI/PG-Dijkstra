all:
	gcc -o dijkstra list.c main.c list.c graph.c vertice.c edge.c fibheap.c binheap.c dijkstra.c -lm -O3

