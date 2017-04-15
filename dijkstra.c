#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "dijkstra.h"
#include "edge.h"
#include "vertice.h"
#include "fibheap.h"
#include "binheap.h"

//DIJKSTRA COM HEAP BINÁRIA
ans dijkstra_bin(graph g,int source){

bin_heap* heap = create_heap_bin(g.nv);

//4 vetores auxiliares
int* a = (int*)malloc(sizeof(int)*g.nv); //Guarda qual é o anterior para pegar o menor caminho
int* finished = (int*)malloc(sizeof(int)*g.nv); //Marca os que já estão com caminho ótimo
double* d = (double*)malloc(sizeof(double)*g.nv); //Guarda distância mínima até o vértice no momento
bin_node** tracker = (bin_node**)malloc(sizeof(bin_node*)*g.nv); //Para que possamos pegar um nodo que ainda está na heap

//Guarda o número de vértices ainda não visitados
int missing = g.nv;

//Variável contadora
int i;

for(i=0;i<g.nv;i++){
	a[i] = i;
	finished[i] = 0;

	bin_node* new = create_node_bin(g.vertices[i]->id);
	tracker[i] = new;
	insert_bin(heap,new);
	
	d[i] = DBL_MAX;
}

//Fazemos a source ter 0 como key
decreace_key_bin(heap,source,0);
d[source] = 0;
while(missing > 0){
	//printf("%d\n",missing);

	bin_node* node = extract_min_bin(heap);
	finished[node->id] = 1;
	
	edge_list* temp_list = g.vertices[node->id]->edges;
	while(temp_list != NULL){

		//Pegamos o id do outro vértices
		int id_des;
		if(temp_list->edge->v1 == node->id) id_des = temp_list->edge->v2;
		else id_des = temp_list->edge->v1;		

		//Se o outro vértice já tiver achado o mínimo, não fazemos nada		
		if(finished[id_des] != 1){
			bin_node* des = tracker[id_des];
	
			//Se necessário atualizamos o valor para chegar ao nodo
			if(des->key > (temp_list->edge->weight + node->key)){
				if((temp_list->edge->weight + node->key) >= 0){

					decreace_key_bin(heap,des->index,(temp_list->edge->weight + node->key));
					a[id_des] = node->id;
					d[id_des] = temp_list->edge->weight + node->key;

				}
			}
		}

		temp_list = temp_list->next;
	}//Fim do while temp != NULL
	missing--;
}//Fim do while missing > 0

//for(i=g.nv-1;i > -1;i--) printf("|%lf|\n",d[i]);

//free nos vetores auxiliares
//free(a);
free(finished);
//free(d);
for(i=0;i<g.nv;i++) free(tracker[i]);
free(tracker);

//Retornamos para impressão as distâncias e os antecessores
ans ans;
ans.d = d;
ans.a = a;
return ans;
}


//DIJKSTRA USANDO HEAP DE FIBONACCI
ans dijkstra_fib(graph g,int soucr){


}

ans print_results(){


ans ans;
return ans;
}
