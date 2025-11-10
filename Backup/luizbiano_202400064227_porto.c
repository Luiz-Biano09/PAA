#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  char cnpj[20];
  char codigo[20];
  uint32_t peso;
  uint32_t senha;
} conteiners;

// Funções
void mergesort(int32_t *A, int32_t *aux, int32_t i, int32_t j);
void intercalar(int32_t *A, int32_t *aux, int32_t i, int32_t m, int32_t j);
void print_trecho(const char *msg, int32_t *A, int32_t ini, int32_t fim);

// Mergesort recursivo
void mergesort(int32_t *A, int32_t *aux, int32_t i, int32_t j) {
    if (i >= j) return; // caso base
    int32_t m = i + (j - i) / 2;
    mergesort(A, aux, i, m);
    mergesort(A, aux, m + 1, j);
    intercalar(A, aux, i, m, j);
}

// Função auxiliar para imprimir um trecho do vetor
void print_trecho(const char *msg, int32_t *A, int32_t ini, int32_t fim) {
    printf("%s [", msg);
    for (int k = ini; k <= fim; k++) {
        printf("%d", A[k]);
        if (k < fim) printf(" ");
    }
    printf("]\n");
}

// Intercala dois subvetores ordenados A[i..m] e A[m+1..j]
void intercalar(int32_t *A, int32_t *aux, int32_t i, int32_t m, int32_t j) {
    printf("\n=== Intercalando de %d a %d (meio=%d) ===\n", i, j, m);
    print_trecho("Subvetor esquerdo:", A, i, m);
    print_trecho("Subvetor direito :", A, m + 1, j);

    int32_t p1 = i;
    int32_t p2 = m + 1;
    int32_t k  = i;

    // mescla os elementos
    while (p1 <= m && p2 <= j) {
        if (A[p1] <= A[p2]) {
            aux[k++] = A[p1++];
        } else {
            aux[k++] = A[p2++];
        }
        print_trecho("  -> aux parcial :", aux, i, k - 1);
    }

    // copia o que restou da esquerda
    while (p1 <= m) {
        aux[k++] = A[p1++];
        print_trecho("  -> resto Esq :", aux, i, k - 1);
    }

    // copia o que restou da direita
    while (p2 <= j) {
        aux[k++] = A[p2++];
        print_trecho("  -> resto Dir :", aux, i, k - 1);
    }

    // copia de volta para A
    for (int32_t t = i; t <= j; t++) {
        A[t] = aux[t];
    }

    print_trecho("Resultado final :", A, i, j);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <entrada> <saida>\n", argv[0]);
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    if (!input || !output) {
        perror("Erro ao abrir arquivos");
        return 1;
    }
    uint32_t quantidade;
    fscanf(input,"%d\n",&quantidade);
    printf("%d",quantidade);
    conteiners *lista;
    lista = (conteiners*) malloc(sizeof(conteiners) * quantidade);
    for(uint32_t i = 0; i < quantidade; i++){
         fscanf(input,"%s %s %d",lista[i].codigo,lista[i].cnpj, &lista[i].peso,lista[i].senha = i);
    }
    uint32_t *aux;
   
    for(uint32_t x = 0; x< quantidade; x++){
fprintf(output,"%s,  %s,   %d  %d \n",lista[x].codigo,lista[x].cnpj, lista[x].peso, lista[x].senha);

    }
     uint32_t quantidade2;
    fscanf(input,"%d\n",&quantidade2);
        printf("%d",quantidade2);
}

