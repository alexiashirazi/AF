#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Profiler.h"

/*      NUME: SHIRAZI ALEXIA
*       Grupa 30225 calculatoare romana, seria A
* 
    Complexitatea spatiului este O(1) deoarece hash table ul are dimensiune fixa(la perf)

    In demo am citit o dimensiune a tabelei, iar dupa citit
    cate elemente vreau sa adaug pentru fiecare element voi 
    introduce o cheie si un nume, iar dupa ce introduc elementele 

    voi citi o cheie pe care o voi cauta in tabela

    Cautarea in cel mai rau caz va fi O(n)

    In functia factor calculez eforturile si exista un parametru 
    alfa pe care il apelez cu ce valoare am nevoie pentru factorul de umplere
    
    
    
    */
int N = 9973; //dimensiunea fixa pentru perf
int c1 = 1; //constantele pentru functia de hashing
int c2 = 2;

typedef struct cell
{
    int key;
    char name[30]="gol";
} Entry;

int h(int key, int i, int n) { //functia de hashing patratica
    return (key+ c1 * i + c2 * i * i) % n; //
}


void hash_insert(Entry** hash, int k, char nume[], int n)
{
    int i = 0;
    do
    {
        int j = h(k, i, n); //imi hash-uiesc k
        //daca locul e gol aloc dinamic si pun pe indexul j cheia k
        //daca nu cresc un i si il pun practic pe urmatorul loc posibil
        //adica se mai face odata hashing cu noul i
        if (hash[j] == NULL || hash[j]->key == 0)
        {
            if (hash[j] == NULL) {
                hash[j] = (Entry*)malloc(sizeof(Entry));
            }
            hash[j]->key = k;
            strcpy_s(hash[j]->name, sizeof(hash[j]->name), nume);
            return;
        }
        else
            i++;
    } while (i < n);
}




int hash_search(Entry** hash, int k, int n)
{
    int i = 0;
    int j;
    do
    {
        j = h(k, i, n); //gasesc indexul cu functia de hashing
        if (hash[j] != NULL && hash[j]->key == k)
        {
            return 1; //daca am gasit cheia pe pozitia asta returnez 1 altfel cresc un 
            //i ca la inserare pentru a verifica daca nu e pe urmatorul loc posibil
        }
        else
            i++;
    } while (i!= n);
    //daca nu o gasesc deloc returez -1
    return -1;
}

int hash_search_perf(Entry** hash, int k, int n, int* operatii)
{
    int i = 0;
    int j;
    *operatii = 0;  //de cate ori se acceseaza celula

    do
    {
        j = h(k, i, n);
        (*operatii)++;  //incrementam de fiecare data cand gasim index

        if (hash[j] != NULL && hash[j]->key == k)
        {
            return 1;  // daca gasim chia pe indexul j retunam 1 
            //daca nu crestem i si cautam in urmatorul loc posibil
        }
        else
            i++;
    } while (hash[j] != NULL && i != n);

    return -1;  //daca nu am gasit nicunde returnam -1
}
void shuffleArray(int v[]) {

    
    int rand;

    //in aceasta functie permut random elementele intre ele
    for (int i = 2999; i >= 0; i--) {
        FillRandomArray(&rand, 1, 10000, 500000, 1, 0);
        int j = rand % (i + 1);

        int aux = v[i];
        v[i] = v[j];
        v[j] = aux;
    }
}

void generate_array(int v[], int copy[]) {
    
    int vec2[1500];
   //practic copia de aici este vectorul de 1500 elemente luate random din tabela
    FillRandomArray(vec2, 1500, 15001, 30000, 1, 0); //generam 1500 de elemente care sigur nu se afla
    //in tabela (elementele au range maxim de 15000, iar aici incep cu range ul de la 15001)
    for (int i = 0; i < 1500; i++)
        v[i] = copy[i];
    for (int i = 1500; i < 3000; i++)
        v[i] = vec2[i-1500];
    //lipesc ambii vectori in vectorul final si dupa dau shuffle
    shuffleArray(v);

}


void printHash(Entry** hash, int n) {
    for (int i = 0; i < n; i++) {
        if (hash[i] != NULL) {
            printf("Index %d: Key = %d, Name = %s\n", i, hash[i]->key, hash[i]->name);
        }
    }
}

void factor(Entry** hash,double alfa, double *efort_mediu_gasite, double *efort_mediu_negasite, double *maxim_gasite, double *maxim_negasite)
{

    char str[] = "test";
    int vec[10000];
    int vector_cautare[3001];
    alfa = 0.95;
    alfa = alfa * N;
    int integer_alfa = int(alfa);

    int copy[10000];
    int numar = 0;
    int efort_total_gasit = 0;
    int efort_total_negasite = 0;
     *maxim_gasite = -1;
     *maxim_negasite = -1;
    int ki = 0;
    for (int j = 0; j < 5; j++) { ///mediu statsitic
        for (int i = 0; i < N; i++)
        {
            hash[i] = NULL; //initializare

        }

        for (int i = 0; i < integer_alfa; i++)
            vec[i] = 0;
        //generam elemente random in vec pentru a putea sa le inseram
        FillRandomArray(vec, integer_alfa, 10, 15000, 1, 0);

        CopyArray(copy, vec, integer_alfa);
        for (int i = 0; i < integer_alfa; i++)

        {
            hash_insert(hash, vec[i], str, N);

        }

        shuffleArray(copy); //dam shuffle la elemente pentru a nu lua primele 1500 de elemente
        generate_array(vector_cautare, copy); //si generam vectorul de cautare


        for (int i = 0; i < 3000; i++) {
            int operatii = 0;
            int key = vector_cautare[i];

            int find = hash_search_perf(hash, key, N, &operatii);

            if (find == -1) {
                if (operatii > *maxim_negasite)
                    *maxim_negasite = operatii;
                efort_total_negasite += operatii;
            }


            if (find == 1) {
                if (operatii > *maxim_gasite)
                    *maxim_gasite = operatii;
                efort_total_gasit += operatii;
            }

            // printf("%d \n", operatii);
        }

    }
    // printf("%d", ki);
    * efort_mediu_gasite = (double)efort_total_gasit / 7500;
    *efort_mediu_negasite = (double)efort_total_negasite / 7500;
}

int main()
{
    int choice;
    printf("For demo, press 1; for performance, press 2\n");
    scanf_s("%d", &choice);

    if (choice == 1)
    {
        int n;
        printf("Enter the size\n");
        scanf_s("%d", &n);

        Entry** hash = (Entry**)calloc(n , sizeof(Entry*));
        for (int i = 0; i < n; i++)
        {
            hash[i] = NULL;
        }

        int nr_elem;
        printf("\nHow many elements do you want to insert?\n");
        scanf_s("%d\n", &nr_elem);
        char string[30];
        int x;

        for (int i = 0; i < nr_elem; i++)
        {
            
            printf("Enter the key of the element and the name\n");
            scanf_s("%d\n", &x);
            
            fgets(string, sizeof(string), stdin);
            
         
            hash_insert(hash, x, string, n);
        }

        int search_key;
        printf("\nEnter the key you want to search\n");
        scanf_s("%d", &search_key);

        int gasit= hash_search(hash, search_key, n);
        if (gasit == 1)
            printf("cheia a fost gasita\n");
        else
            printf("cheia nu a fost gasita\n");

        printf("hash ul este:\n");
        printHash(hash, n);
       

        for (int i = 0; i < n; i++)
        {
            free(hash[i]);
        }
        free(hash);
    }
    else
    {
        int N = 9973;
        Entry** hash = (Entry**)malloc(N *sizeof(Entry*));
        for (int i = 0; i < N; i++)
        {
            hash[i]= NULL;
        }

        double efort_mediu_gasit_80 = 0.0;
        double efort_mediu_negasit_80 = 0.0;
        double efort_maxim_gasit_80 = 0.0;
        double efort_maxim_negasit_80 = 0.0;

        double efort_mediu_gasit_85 = 0.0;
        double efort_mediu_negasit_85 = 0.0;
        double efort_maxim_gasit_85 = 0.0;
        double efort_maxim_negasit_85 = 0.0;


        double efort_mediu_gasit_90 = 0.0;
        double efort_mediu_negasit_90 = 0.0;
        double efort_maxim_gasit_90 = 0.0;
        double efort_maxim_negasit_90 = 0.0;


        double efort_mediu_gasit_95 = 0.0;
        double efort_mediu_negasit_95 = 0.0;
        double efort_maxim_gasit_95 = 0.0;
        double efort_maxim_negasit_95 = 0.0;

        double efort_mediu_gasit_99 = 0.0;
        double efort_mediu_negasit_99 = 0.0;
        double efort_maxim_gasit_99 = 0.0;
        double efort_maxim_negasit_99 = 0.0;

        factor(hash, 0.80, &efort_mediu_gasit_80, &efort_mediu_negasit_80, &efort_maxim_gasit_80, &efort_maxim_negasit_80);
        factor(hash, 0.85, &efort_mediu_gasit_85, &efort_mediu_negasit_85, &efort_maxim_gasit_85, &efort_maxim_negasit_85);
        factor(hash, 0.90, &efort_mediu_gasit_90, &efort_mediu_negasit_90, &efort_maxim_gasit_90, &efort_maxim_negasit_90);
        factor(hash, 0.95, &efort_mediu_gasit_95, &efort_mediu_negasit_95, &efort_maxim_gasit_95, &efort_maxim_negasit_95);
        factor(hash, 0.99, &efort_mediu_gasit_99, &efort_mediu_negasit_99, &efort_maxim_gasit_99, &efort_maxim_negasit_99);

      
        printf("Factor de umplere | Efort mediu gasite | Efort mediu negasite | Efort maxim gasite | Efort maxim negasite\n");
        printf("-------------------|---------------------|----------------------|---------------------|------------------------\n");
        printf("        0.80       | %-19lf | %-20lf | %-19lf | %-21lf\n", efort_mediu_gasit_80, efort_mediu_negasit_80, efort_maxim_gasit_80, efort_maxim_negasit_80);
        printf("        0.85       | %-19lf | %-20lf | %-19lf | %-21lf\n", efort_mediu_gasit_85, efort_mediu_negasit_85, efort_maxim_gasit_85, efort_maxim_negasit_85);
        printf("        0.90       | %-19lf | %-20lf | %-19lf | %-21lf\n", efort_mediu_gasit_90, efort_mediu_negasit_90, efort_maxim_gasit_90, efort_maxim_negasit_90);
        printf("        0.95       | %-19lf | %-20lf | %-19lf | %-21lf\n", efort_mediu_gasit_95, efort_mediu_negasit_95, efort_maxim_gasit_95, efort_maxim_negasit_95);
        printf("        0.99       | %-19lf | %-20lf | %-19lf | %-21lf\n", efort_mediu_gasit_99, efort_mediu_negasit_99, efort_maxim_gasit_99, efort_maxim_negasit_99);


        for (int i = 0; i < N; i++)
        {
            free(hash[i]);
        }
        free(hash);

    }

    return 0;
}
