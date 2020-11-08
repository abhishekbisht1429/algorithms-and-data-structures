#include<stdio.h>
#include<limits.h>
#include "../../../data-structures/c/graph/graph_using_matrix.c"
#define INF INT_MAX

static inline int min(int a, int b) {
    return a<b?a:b;
}
void single_source_sp(GRAPH_AM *graph, int *parent, int start) {
    int n = graph->n;
    int dist[n];
    int visited[n];

    for(int i = 0;i<n; ++i) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[start] = 0;
    visited[start] = 1;
    parent[start] = -1;

    int current = start;
    while(current != -1) {
        // printf("current %d\n", current);
        int minDist = INF;
        int next = -1;
        for(int i=0;i<n;++i) {
            // printf("i : %d ", i);
            if(isAdjacent(graph, current, i) && !visited[i]) {
                int newDist = dist[current]+ getAMEdge(graph, current, i);
                if(newDist < dist[i]) {
                    dist[i] = newDist;
                    parent[i] = current;
                    // printf("update %d ", dist[i]);
                }
            }
            
            if(!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                next = i;
                // printf("updatetwo %d", minDist);
            }
            // printf("\n");
        }

        if(next != -1) {
            visited[next] = 1;
        }
        current = next;
    }
}
int main() {
    GRAPH_AM *graph = newAMGraph(7);
    int parent[graph->n];

    addAMEdgeUndirected(graph, 0, 1, 2);
    addAMEdgeUndirected(graph, 0, 2, 6);
    addAMEdgeUndirected(graph, 1, 3, 5);
    addAMEdgeUndirected(graph, 2, 3, 8);
    addAMEdgeUndirected(graph, 3, 5, 15);
    addAMEdgeUndirected(graph, 3, 4, 10);
    addAMEdgeUndirected(graph, 4, 5, 6);
    addAMEdgeUndirected(graph, 4, 6, 2);
    addAMEdgeUndirected(graph, 5, 6, 6);

    displayAM(graph);
    printf("\n");

    single_source_sp(graph, parent, 0);
    
    for(int i=0;i<graph->n;++i) {
        printf("%d -> %d\n", parent[i], i);
    }
}