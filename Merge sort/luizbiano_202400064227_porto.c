#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char cnpj[20];
  char codigo[20];
  int peso;
  uint32_t senha;  // índice original
} conteiners;



int binarySearch(conteiners *arr, uint32_t size, char *ENCONTRAR) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
       int mid = (left + right) / 2;

        if (strcmp(arr[mid].codigo,ENCONTRAR)== 0) {
            return mid;
        }

        if (strcmp(arr[mid].codigo, ENCONTRAR) < 0) {
            left = mid + 1; 
            
            
        } else {
           right = mid - 1;
        }
    }

    return -1;
}

/* Protótipos */
void mergesort_conteiner(conteiners *A, conteiners *aux, int i, int j);
void intercalar_conteiner(conteiners *A, conteiners *aux, int i, int m, int j);

void mergesort_conteiner(conteiners *A, conteiners *aux, int i, int j) {
    if (i >= j) return;
    int m = i + (j - i) / 2;
    mergesort_conteiner(A, aux, i, m);
    mergesort_conteiner(A, aux, m + 1, j);
    intercalar_conteiner(A, aux, i, m, j);
}

void intercalar_conteiner(conteiners *A, conteiners *aux, int i, int m, int j) {
    int p1 = i, p2 = m + 1, k = i;

    while (p1 <= m && p2 <= j) {
        if (strcmp(A[p1].codigo,A[p2].codigo)<= 0)  // estável
            aux[k++] = A[p1++];
        else
            aux[k++] = A[p2++];
    }
    while (p1 <= m) aux[k++] = A[p1++];
    while (p2 <= j) aux[k++] = A[p2++];

    for (int t = i; t <= j; t++) A[t] = aux[t];
}

void gerar_relatorio(FILE *output,
                     conteiners *lista, uint32_t tamanhoLista,
                     conteiners *vistoria, uint32_t tamanhoVistoria) {
    /* Coletamos os casos primeiro e depois imprimimos
       em ordem crescente da senha DO ITEM NA LISTA. */

    /* Vetores paralelos para guardar pares (idx_lista, idx_vistoria) */
    int *cnpj_L = malloc(sizeof(int) * tamanhoVistoria);
    int *cnpj_V = malloc(sizeof(int) * tamanhoVistoria);
    int *peso_L = malloc(sizeof(int) * tamanhoVistoria);
    int *peso_V = malloc(sizeof(int) * tamanhoVistoria);
    int qC = 0, qP = 0;

    /* 1) Coleta */
    for (uint32_t i = 0; i < tamanhoVistoria; i++) {
        int idx = binarySearch(lista, tamanhoLista, vistoria[i].codigo);
        if (idx == -1) continue;

        if (strcmp(lista[idx].cnpj, vistoria[i].cnpj) != 0) {
            cnpj_L[qC] = idx;
            cnpj_V[qC] = (int)i;
            qC++;
        } else {
            if (lista[idx].peso != vistoria[i].peso) {
                int base = lista[idx].peso;
                float dif_pct;
                if (base == 0) {
                    dif_pct = (vistoria[i].peso == 0) ? 0.0f : 100.0f;
                } else {
                    int dif_abs = abs(lista[idx].peso - vistoria[i].peso);
                    dif_pct = (dif_abs * 100.0f) / (float)base;
                }
                if (dif_pct > 10.0f) {
                    peso_L[qP] = idx;
                    peso_V[qP] = (int)i;
                    qP++;
                }
            }
        }
    }

    /* 2) Ordena cada grupo por ordem de chegada (senha da LISTA) */
    /* insertion sort estável nos dois grupos */
    for (int a = 1; a < qC; a++) {
        int il = cnpj_L[a], iv = cnpj_V[a];
        int s = lista[il].senha;
        int b = a - 1;
        while (b >= 0 && lista[cnpj_L[b]].senha > s) {
            cnpj_L[b + 1] = cnpj_L[b];
            cnpj_V[b + 1] = cnpj_V[b];
            b--;
        }
        cnpj_L[b + 1] = il;
        cnpj_V[b + 1] = iv;
    }
    for (int a = 1; a < qP; a++) {
        int il = peso_L[a], iv = peso_V[a];
        int s = lista[il].senha;
        int b = a - 1;
        while (b >= 0 && lista[peso_L[b]].senha > s) {
            peso_L[b + 1] = peso_L[b];
            peso_V[b + 1] = peso_V[b];
            b--;
        }
        peso_L[b + 1] = il;
        peso_V[b + 1] = iv;
    }

    /* 3) Imprime:
          - Primeiro todos com CNPJ diferente (sem peso)
          - Depois todos com peso diferente (>10%), com % */
    for (int k = 0; k < qC; k++) {
        int il = cnpj_L[k], iv = cnpj_V[k];
        fprintf(output, "%s:%s<->%s\n",
                vistoria[iv].codigo,
                lista[il].cnpj,
                vistoria[iv].cnpj);
    }

    for (int k = 0; k < qP; k++) {
        int il = peso_L[k], iv = peso_V[k];

        /* recomputa % só para imprimir arredondado */
        float dif_pct;
        if (lista[il].peso == 0) {
            dif_pct = (vistoria[iv].peso == 0) ? 0.0f : 100.0f;
        } else {
            int dif_abs = abs(lista[il].peso - vistoria[iv].peso);
            dif_pct = (dif_abs * 100.0f) / (float)lista[il].peso;
        }

        fprintf(output, "%s:%dkg(%.0f%%)\n",
                vistoria[iv].codigo,
                vistoria[iv].peso - lista[il].peso,
                dif_pct);
    }

    free(cnpj_L); free(cnpj_V);
    free(peso_L); free(peso_V);
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
        if (input) fclose(input);
        if (output) fclose(output);
        return 1;
    }

    uint32_t quantidade;
    if (fscanf(input, "%u", &quantidade) != 1) {
        fprintf(stderr, "Falha ao ler quantidade.\n");
        fclose(input); fclose(output);
        return 1;
    }

    conteiners *lista = malloc(sizeof *lista * quantidade);
    conteiners *aux   = malloc(sizeof *aux   * quantidade);
    if (!lista || !aux) {
        fprintf(stderr, "Falha de alocação.\n");
        free(lista); free(aux);
        fclose(input); fclose(output);
        return 1;
    }

    for (uint32_t i = 0; i < quantidade; i++) {
        if (fscanf(input, "%19s %19s %u",
                   lista[i].codigo,
                   lista[i].cnpj,
                   &lista[i].peso) != 3) {
            fprintf(stderr, "Falha ao ler registro %u.\n", i);
            free(lista); free(aux);
            fclose(input); fclose(output);
            return 1;
        }
        lista[i].senha = i; // guarda posição original
    }

    /* Ordena por ordem alfabetica */
    mergesort_conteiner(lista, aux, 0, (int)quantidade - 1);

   

     uint32_t quantidade2;
    if (fscanf(input, "%u", &quantidade2) != 1) {
        fprintf(stderr, "Falha ao ler quantidade.\n");
        fclose(input); fclose(output);
        return 1;
    }
   // printf("%u",quantidade2);
    conteiners *vistoria = malloc(sizeof * vistoria * quantidade2);
     
    
    for (uint32_t P = 0; P < quantidade2; P++) {
        if (fscanf(input, "%19s %19s %u",
                  vistoria[P].codigo,
                   vistoria[P].cnpj,
                   &vistoria[P].peso) != 3) {
            fprintf(stderr, "Falha ao ler registro %u.\n", P);
            free(lista); free(aux);
            fclose(input); fclose(output);
            return 1;
        }
        vistoria[P].senha = P; // guarda posição original
    }
    int vetor[quantidade2 ];
 
  gerar_relatorio(output, lista, quantidade, vistoria, quantidade2);


  
    free(aux);
    free(lista);
    fclose(input);
    fclose(output);
    return 0;
}
