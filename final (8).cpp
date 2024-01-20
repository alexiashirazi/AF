#include <stdio.h>
#include <stdlib.h>
#include "profiler.h"

Profiler p("dfs");

#define COLOR_WHITE 0
#define COLOR_GRAY 1
#define COLOR_BLACK 2

int ciclu = 0;//verificam daca exista cicluri

struct node {
    int vertex;
    struct node* parent;
};

struct Graph {
    int numar_varfuri;
    struct node** adjLists;
};

struct node* createNode(int v) {
    //cream nod
    struct node* p = (struct node*)malloc(sizeof(struct node));
    p->vertex = v;
    p->parent = NULL;
    return p;
}

struct Graph* createGraph(int vertices) {
    //crem graful
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numar_varfuri = vertices;
    graph->adjLists = (struct node**)malloc(vertices * sizeof(struct node*));

    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }

    return graph;
}

void addEdge(struct Graph* graph, int src, int dest) {
    //adaugam muchie
    struct node* p = createNode(dest);
    p->parent = graph->adjLists[src];
    graph->adjLists[src] = p;

   /* p = createNode(src);
    p->parent = graph->adjLists[dest];
    graph->adjLists[dest] = p;*/
}



void dfs_visit(struct Graph* graph, int vertex, int* time, int* color, int* discovery_time, int* finish_time, int* parent) {
    (*time)++;
    discovery_time[vertex] = *time;
    color[vertex] = COLOR_GRAY;

    printf("%d ", vertex);

    struct node* currentNode = graph->adjLists[vertex];

    while (currentNode != NULL) {
        int adjacentVertex = currentNode->vertex;

        if (color[adjacentVertex] == COLOR_WHITE /*&& parent[adjacentVertex] == -1*/) {
            parent[adjacentVertex] = vertex;
            dfs_visit(graph, adjacentVertex, time, color, discovery_time, finish_time, parent);
        }
        else if (color[adjacentVertex] == COLOR_GRAY && parent[vertex] != adjacentVertex) {
            // Back edge detected, indicating a cycle
            ciclu = 1;
        }
        currentNode = currentNode->parent;
    }

    color[vertex] = COLOR_BLACK;
    (*time)++;
    finish_time[vertex] = *time;
}

void dfs(struct Graph* graph, int f[]) {
    int* color = (int*)malloc(graph->numar_varfuri * sizeof(int));
    int* discovery_time = (int*)malloc(graph->numar_varfuri * sizeof(int));
    int* finish_time = (int*)malloc(graph->numar_varfuri * sizeof(int));
    int* parent = (int*)malloc(graph->numar_varfuri * sizeof(int));
    int time = 0;

    for (int i = 0; i < graph->numar_varfuri; i++) {
        color[i] = COLOR_WHITE;
        discovery_time[i] = 0;
        finish_time[i] = 0;
        parent[i] = -1;
    }

    for (int i = 0; i < graph->numar_varfuri; i++) {
        if (color[i] == COLOR_WHITE) {
            dfs_visit(graph, i, &time, color, discovery_time, finish_time, parent);
        }
    }

    // Assign the finish times to the array f
    for (int i = 0; i < graph->numar_varfuri; i++) {
        f[i] = finish_time[i];
    }

    free(color);
    free(discovery_time);
    free(finish_time);
    free(parent);
}


void freeGraph(struct Graph* graph) {
    for (int i = 0; i < graph->numar_varfuri; i++) {
        struct node* current = graph->adjLists[i];
        while (current != NULL) {
            struct node* temp = current;
            current = current->parent;
            free(temp);
        }
    }

    free(graph->adjLists);
    free(graph);
}

void printGraph(struct Graph* graph) {
    for (int v = 0; v < graph->numar_varfuri; v++) {
        struct node* temp = graph->adjLists[v];
        printf("Vertex %d: ", v);
        while (temp != NULL) {
            printf("%d -> ", temp->vertex);
            temp = temp->parent;
        }
        printf("NULL\n");
    }
}

void tarjan_dfs(struct Graph* graf, int varf, int* timp, int* culoare, int* timp_descoperire, int* timp_finalizare, int* parinte, int* minim, int* inStiva, int* stiva, int* indiceStiva, int* numarComponente) {
    (*timp)++; // incrementãm timpul curent
    timp_descoperire[varf] = *timp; // actualizãm timpul de descoperire
    minim[varf] = timp_descoperire[varf]; // valoarea minimã în timpul de descoperire
    culoare[varf] = COLOR_GRAY; // marcam vârful curent ca fiind gri
    stiva[*indiceStiva] = varf; // il adaugãm în stivã
    (*indiceStiva)++; // incrementãm indicele stivei
    inStiva[varf] = 1; // marcam vârful curent ca fiind în stivã

    struct node* p = graf->adjLists[varf];

    while (p != NULL) {
        int varfAdiacent = p->vertex;

        if (culoare[varfAdiacent] == COLOR_WHITE) { // verificãm dacã nodul vecin nu a fost vizitat
            parinte[varfAdiacent] = varf; // setãm pãrintele vârfului vecin
            tarjan_dfs(graf, varfAdiacent, timp, culoare, timp_descoperire, timp_finalizare, parinte, minim, inStiva, stiva, indiceStiva, numarComponente);
            if ((minim[varf] < minim[varfAdiacent]))
                minim[varf] = minim[varf];
            else
                minim[varfAdiacent];
        }
        else if (inStiva[varfAdiacent]) {
            if (minim[varf] < timp_descoperire[varfAdiacent])
                minim[varf] = minim[varf];
            else
                timp_descoperire[varfAdiacent];
        }

        p = p->parent;
    }

    if (timp_descoperire[varf] == minim[varf]) {
        printf("Componenta  Puternic Conexa %d: ", *numarComponente + 1);
        int v;
        do {
            v = stiva[--(*indiceStiva)];
            inStiva[v] = 0;
            printf("%d ", v);
        } while (v != varf);
        (*numarComponente)++;
        printf("\n");
    }
}

void tarjan(struct Graph* graf) {
    int* culoare = (int*)malloc(graf->numar_varfuri * sizeof(int));
    int* timp_descoperire = (int*)malloc(graf->numar_varfuri * sizeof(int));
    int* timp_finalizare = (int*)malloc(graf->numar_varfuri * sizeof(int));
    int* parinte = (int*)malloc(graf->numar_varfuri * sizeof(int));
    int* minim = (int*)malloc(graf->numar_varfuri * sizeof(int));
    int* inStiva = (int*)malloc(graf->numar_varfuri * sizeof(int));
    int* stiva = (int*)malloc(graf->numar_varfuri * sizeof(int));
    int indiceStiva = 0;
    int numarComponente = 0;
    int timp = 0;

    for (int i = 0; i < graf->numar_varfuri; i++) {
        culoare[i] = COLOR_WHITE;
        timp_descoperire[i] = 0;
        timp_finalizare[i] = 0;
        parinte[i] = -1;
        minim[i] = 0;
        inStiva[i] = 0;
    }

    for (int i = 0; i < graf->numar_varfuri; i++) {
        if (culoare[i] == COLOR_WHITE) {
            tarjan_dfs(graf, i, &timp, culoare, timp_descoperire, timp_finalizare, parinte, minim, inStiva, stiva, &indiceStiva, &numarComponente);
        }
    }

    free(culoare);
    free(timp_descoperire);
    free(timp_finalizare);
    free(parinte);
    free(minim);
    free(inStiva);
    free(stiva);
}




int main() {
    int numVertices;
    printf("Enter the number of vertices: ");
    scanf_s("%d", &numVertices);

    struct Graph* graph = createGraph(numVertices);

    int numEdges;
    printf("Enter the number of edges: ");
    scanf_s("%d", &numEdges);

    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        printf("Enter the vertices for edge %d: ", i + 1);
        scanf_s("%d %d", &src, &dest);
        addEdge(graph, src, dest);
    }

    printf("\nGraph:\n");
    printGraph(graph);

    printf("\n");
    int finish_time[1000];
    int f[10000];
    dfs(graph, f);

    if (ciclu == 1)
        printf("exista cicluri\n");
    else
        printf("nu exista cicluri\n");

    printf("\n");

    if (ciclu != 1) {
        int topological_order[1000];

        // Populate the topological_order array based on finish times
        for (int i = 0; i < numVertices; i++) {
            int max_finish_time = 0;
            int max_finish_time_vertex = -1;
            for (int j = 0; j < numVertices; j++) {
                if (f[j] > max_finish_time && f[j] != -1) {
                    max_finish_time = f[j];
                    max_finish_time_vertex = j;
                }
            }
            topological_order[i] = max_finish_time_vertex;
            f[max_finish_time_vertex] = -1; // Mark as visited
        }

        // Print the topological order
        printf("Topological Order: ");
        for (int i = numVertices - 1; i >= 0; i--) {
            printf("%d ", topological_order[i]);
        }
    }


    printf("tarjan \n");
    tarjan(graph);


    // Free allocated memory
    freeGraph(graph);

    return 0;
}
