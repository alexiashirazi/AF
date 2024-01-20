#include <stdio.h>
#include <stdlib.h>

/*
* NUME SI PRENUME: SHIRAZI ALEXIA
* CALCULATOARE ROMANA , SERIA A, GRUPA 30225
* PRETTY PRINT R1- 
*       complexitate timp -O(n) trece prin fiecare element o singura data
*       complexitatea spatiu O(h)- h este inaltimea arborelui
*                                -functia este recursiva si se duce in adancime in arbore
* 
* FUNCTIA T1-  transformarea din vector de tati in arbore multicai
*   complexitate timp -O(n) - trece o singura data prin fiecare element
*    din vectorul de parinti pentru a construi arborele
*      complexitatea spatiului- O(n) - am folosit un vector auxiliar de noduri pentru stocarea nodurilor
    PRINT PRETTY T1 - 
*       complexitate timp -O(n) trece prin fiecare element o singura data
*       complexitatea spatiu O(h)- h este inaltimea arborelui
*                                -functia este recursiva si se duce in adancime in arbore
* 
*   FUNCTIA T2-transformare din arbore multicai in arbore binar
*   complexitate timp -O(n) - trece o singura data prin arborele multicai
*   complexitate spatiu O(h) - h este inaltimea arborelui
* 
* În ceea ce privește spațiul, ambele algoritme T1 și T2 folosesc memorie auxiliară pentru a stoca nodurile.
Aceasta duce la o complexitate a spațiului de O(n) unde n este numărul total de noduri în arborele multicai și, respectiv, arborele binar.
*   

*/


///pretty print pentru vectorul parinte
void pretty_print_R1(int root, int spaces, int parent[], int size) {
    int i;
    for (i = 1; i <= size; i++) {
        if (parent[i] == root) {
            int j = 0;
            while (j < spaces) {
                printf(" ");
                j++;
            }
            printf("%d\n", i);
            pretty_print_R1(i, spaces + 1, parent, size);
        }
    }
}

//structura de arbore multi cai
typedef struct Node
{
    int key;
    int size;
    struct Node* children[1000];
}Node;

//transformare din vector parinte in arbore multicai
Node* T1(int v[], int n) {
    //am folosit memorie aditionala pentru a stoca nodurile
    Node* nod[1000];

    Node* root = NULL;

    // Initializam nodurile
    for (int i = 0; i <= n; i++) {
        nod[i] = (Node*)malloc(sizeof(Node));
        nod[i]->key = i;
        nod[i]->size = 0; 
    }

    //construim arborele multiway
    for (int j = 1; j <= n; j++) {
        if (v[j] != -1) {
           // Dacă v[j] nu este - 1, înseamnă că j este copilul lui v[j].
           // Îl adăugăm la lista de copii a părintelui său (v[j]).
            nod[v[j]]->children[nod[v[j]]->size++] = nod[j];
        }
        else {
            root = nod[j];
            // Dacă v[j] este -1, înseamnă că j este rădăcina arborelui.
            // Setăm rădăcina arborelui la nodul curent (j).
        }
    }
    return root;
}

//pretty print pt arbore multicai
void print_pretty_T1(Node* root, int space) {
    if (root == NULL)
        return;
    else {
        int j = 0;
        while (j < space) {
            printf(" ");
            j++;
        }
        printf("%d\n", root->key);
        for (int i = 0; i < root->size; i++)
            print_pretty_T1(root->children[i], space + 1);
    }
}
//structura pentru arbore binar
typedef struct NodeT {
    int key;
    struct NodeT* left;
    struct NodeT* right;
};

//transformare din arbore multicai in arbore binar
NodeT* T2(Node* root) 
{
    // Verificăm dacă arborele multiway este gol.

    if (root == NULL) {
        return NULL;
    }

    NodeT* p = (NodeT*)malloc(sizeof(NodeT));
    // Inițializăm valorile nodului arborelui binar cu cele ale nodului arborelui multiway.

    p->key = root->key;
    p->left = NULL;
    p->right = NULL;

    // Utilizăm un pointer auxiliar pentru a parcurge lista de copii dreapta în arborele binar.

    NodeT* current = p->left; 

    for (int j = 0; j < root->size; j++) {
        if (current == NULL) {
            // Dacă pointerul auxiliar este NULL, înseamnă că suntem pe primul copil
            p->left = T2(root->children[j]);
            current = p->left;
        }
        else {
            // Altfel, adăugăm copiii suplimentari în lista de copii dreapta.
            current->right = T2(root->children[j]);
            current = current->right;
        }
    }

    return p;
}

void pretty_print_binary(NodeT* k, int space) {
    if (k != NULL)
    {
        int j = 0;
        while (j < space) {
            printf(" ");
            j++;
        }
        printf("%d\n", k->key);
        pretty_print_binary(k->left, space + 1);
        pretty_print_binary(k->right, space);
    }
}
int main() {
    int parent[100000] = { 0 };
    int y;

    printf("introduceti numarul de noduri:\n");
    scanf_s("%d", &y);

    int cop_y = y;
    int max = -1;

    printf("\nintroduceti radacina:\n");
    int x;
    scanf_s("%d", &x);
    if (x > max)
        max = x;
    parent[x] = -1;
    y--;

    int a, b;
    for (int i = 0; i < y; i++) {
        printf("introduceti nodul dupa care parintele sau:\n");
        scanf_s("%d %d", &a, &b);
        if (a > max)
            max = a;
        parent[a] = b;
    }

    printf("\nVectorul de parinti generat:\n");
    for (int i = 1; i <= max; i++)
        printf("%d ", parent[i]);

    printf("\nPretty print1:\n");
    pretty_print_R1(-1, 0, parent, max);

    printf("Pretty print2:\n");
    Node* root = T1(parent, max);
    print_pretty_T1(root, 0);


    printf("Pretty print3:\n");
    NodeT* t = T2(root);
    pretty_print_binary(t, 0);

    return 0;
}