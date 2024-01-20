#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

Profiler p("tema9");
/*
NUME: Shirazi Alexia
grupa 30225

    Make Set
        Aceasta functie creeaza un set cu elementul cerut
        Complexitate timp O(1)


    Find Set 
        In aceasta functie cautam daca submultimea. Aceasta gaseste recursiv reprezetantul setului(Indexul)
        Complexitate timp O(logn)
        datorita submultimilor disjuncte

        Unify
        Aceasta este o functie ajutatoare pentru union si ajuta sa se faca merge intre doua seturi 
            si este optimizat cu ajutorul rancului
              -O(1)- datorita rangului
       

        Union-
            Practic este ajutata de Find set si unify. apeleaza find set pentru a gasi reprezentantul si dupa care 
            apeleaza unify pentru a se produce merge ul
            O(logn)-este mai scurt timpul datorita functiilor find set si unify
    Kruskal
        In aceasta functie gasit arborele minim de acoperire al unui graf dat
        sorteaza muchiile dupa ponderea fiecaruia si il adauga in P(arborele minim) daca muchia respectiva
        nu creaza un ciclu.
        O(nlogn)-n numarul de noduri
        iar complexitatea spatiala este O(n) datorita array ului de seturi si al alborelui minim de acoperire P

*/
typedef struct Node {
    int data;
    int rank;
    struct Node* parent;
} Node;

Node* Make_set(int x) {
    Node* p = (Node*)malloc(sizeof(Node));
    p->data = x;
    p->rank = 0;
    p->parent = NULL;
    return p;
}

Node* Make_set_perf(int x, Operation* op) {
    Node* p = (Node*)malloc(sizeof(Node));
    op->count(3);
    p->data = x;
    p->rank = 0;
    p->parent = NULL;
    return p;
}

Node* Find_set(Node* p) {
    if (p->parent != NULL) {
        p->parent = Find_set(p->parent);
        return p->parent;
    }
    else {
        return p;
    }
}

Node* Find_set_perf(Node* p, Operation* op) {
    if (p->parent != NULL) {
        op->count();
        p->parent = Find_set(p->parent);
        return p->parent;
    }
    else {
        return p;
    }
}

void unify(Node* a, Node* b) {
    if (b->rank > a->rank)
        a->parent = b;
    else {
        b->parent = a;
        if (b->rank == a->rank)
            a->rank++;
    }
}

void unify_perf(Node* a, Node* b, Operation* op) {
    if (b->rank > a->rank)
    {
        op->count();
        a->parent = b;
    }
    else {
        op->count();
        b->parent = a;
        op->count();
        if (b->rank == a->rank)
        {
            a->rank++;
            op->count();
        }
    }
}

void Union(Node* a, Node* b) {
    unify(Find_set(a), Find_set(b));
}

void Union_perf(Node* a, Node* b, Operation* op) {

    Node* x = Find_set_perf(a, op);
    Node* y = Find_set_perf(b, op);
    unify_perf(x, y, op);
}

void Free_set(Node* p) {
    free(p);
}

typedef struct Edge {
    int src;
    int dest;
    int pondere;
} Edge;

void insertionSort(Edge** arr, int n) {
    int i, j;
    Edge* key;

    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[0..i-1] that are greater than key to one position ahead of their current position
        while (j >= 0 && arr[j]->pondere > key->pondere) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

Edge** Kruskal(int n, int m, Edge** edges) {
    Edge** P = (Edge**)malloc((n - 1) * sizeof(Edge));
    Node** sets = (Node**)malloc(n * sizeof(Node*));

    for (int i = 0; i < n; i++) {
        sets[i] = Make_set(i);
    }

    insertionSort(edges, m);

    int ind = 0;
    for (int i = 0; i < m && ind < n - 1; i++) {
        Node* radacina = Find_set(sets[edges[i]->src]);
        Node* destinatie = Find_set(sets[edges[i]->dest]);

        if (radacina != destinatie) {
            P[ind++] = edges[i];
            Union(radacina, destinatie);
        }
    }

    return P;
}

Edge** Kruskal_perf(int n, int m, Edge** edges, Operation *op) {
    Edge** P = (Edge**)malloc((m - 1) * sizeof(Edge));
    Node** sets = (Node**)malloc(n * sizeof(Node*));

    for (int i = 0; i < n; i++) {
        sets[i] = Make_set_perf(i,op);
    }


    insertionSort(edges, m);


    int ind = 0;
    for (int i = 0; i < m && ind < m - 1; i++) {
        

        if (Find_set_perf(sets[edges[i]->src],op)!= Find_set_perf(sets[edges[i]->dest], op)) {
            P[ind++] = edges[i];
            Union_perf(sets[edges[i]->src], sets[edges[i]->dest],op);
        }
    }



    return P;
}

int main() {

    int choice = 1;

    if (choice == 1) {
        Node* sets[1000];

        int n;
        printf("introduceti numarul de seturi\n");
        scanf_s("%d", &n);
        int x;
        for (int i = 0; i < n; i++) {
            printf(" introduceti elementul %d\n", i);
            scanf_s("%d", &x);
            sets[i] = Make_set(x);
        }

        // Print initial sets
        for (int i = 0; i < n; i++) {
            printf("setul %d: { %d } cu rancul %d\n", i, sets[i]->data, sets[i]->rank);
        }

        int j;
        int y;
        printf("numarul de union \n");
        scanf_s("%d", &j);
        for (int i = 0; i < j; i++) {
            printf("index set union-1\n");
            scanf_s("%d", &x);
            printf("index set union-2\n");
            scanf_s("%d", &y);
            Union(sets[x], sets[y]);
        }

        // Print disjoint sets after unions
        for (int i = 0; i < n; i++) {
            Node* root = Find_set(sets[i]);
            if (root == sets[i]) {
                printf("{ %d", root->data);

                // Print elements in the same subset
                for (int j = i + 1; j < n; j++) {
                    if (Find_set(sets[j]) == root)
                        printf(", %d", sets[j]->data);
                }

                printf(" }\n");
            }
        }

        int numver;
        printf("introduceti numarul de varfuri:\n");
        scanf_s("%d", &numver);

        int numedge;
        printf("introduceti numarul de muchii\n");
        scanf_s("%d", &numedge);

        // Input edges and their weights
        Edge** edges = (Edge**)malloc(numedge * sizeof(Edge*));
        for (int j = 0; j < numedge; j++)
        {
            edges[j] = (Edge*)malloc(sizeof(Edge));
        }


        for (int i = 0; i < numedge; i++) {
            printf("introduceti datele pentru varful %d (sursa destinatie pondere): ", i);
            scanf_s("%d %d %d", &edges[i]->src, &edges[i]->dest, &edges[i]->pondere);
        }

        Node** set;

        // Run Kruskal's algorithm
        Edge** res = Kruskal(numedge, numver, edges);

        // Print the resulting minimum spanning tree
        for (int i = 0; i < numver - 1; i++) {
            printf("Muchia (%d, %d) - ponderea: %d\n", res[i]->src, res[i]->dest, res[i]->pondere);
        }

        free(res);

        for (int i = 0; i < n; i++) {
            Free_set(sets[i]);
        }
    }
   

    return 0;
}
