#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct {
  char cnpj[20];
  char codigo[20];
  int peso;
  int prcnt;
  uint32_t senha;  // índice original
} conteiners;


int arredonda_cima_manual(float v) {
    int inteiro = (int)v;
    int resultado;

    // Se a parte decimal for > 0.5, arredonda para cima
    if (v > inteiro + 0.5f) {
        resultado = inteiro + 1;
    } else {
        resultado = inteiro;  // mantém inteiro
    }

    // Regra especial: se chegou em 10, vira 11
    if (resultado == 10) {
        resultado = 11;
    }

    return resultado;
}



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
void intercalarPORCENTAGEM(conteiners *A, conteiners *aux, int i, int m, int j) {
    int p1 = i, p2 = m + 1, k = i;

    while (p1 <= m && p2 <= j) {
        if (A[p1].prcnt >= A[p2].prcnt)  // estável
            aux[k++] = A[p1++];
        else
            aux[k++] = A[p2++];
    }
    while (p1 <= m) aux[k++] = A[p1++];
    while (p2 <= j) aux[k++] = A[p2++];

    for (int t = i; t <= j; t++) A[t] = aux[t];
}

void mergesortPORCENTAGEM(conteiners *A, conteiners *aux, int i, int j) {
    if (i >= j) return;
    int m = i + (j - i) / 2;
    mergesortPORCENTAGEM(A, aux, i, m);
    mergesortPORCENTAGEM(A, aux, m + 1, j);
    intercalarPORCENTAGEM(A, aux, i, m, j);
}




void gerar_relatorio(FILE *output,
                     conteiners *lista, uint32_t tamanhoLista,
                     conteiners *vistoria, uint32_t tamanhoVistoria) {
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
            /* Divergência de CNPJ */
            cnpj_L[qC] = idx;
            cnpj_V[qC] = (int)i;
            qC++;
        } else {
            /* Mesma CNPJ -> podemos checar peso */
            if (lista[idx].peso != vistoria[i].peso) {
                int base = lista[idx].peso;
                float dif_pct;

                if (base == 0) {
                    dif_pct = (vistoria[i].peso == 0) ? 0.0f : 100.0f;
                } else {
                    int dif_abs = vistoria[i].peso - lista[idx].peso;
                    dif_pct = (abs(dif_abs) * 100.0f) / (float)base;  // usa |dif|
                }

                if (dif_pct >= 10.0f) {
                    peso_L[qP] = idx;
                    peso_V[qP] = (int)i;
                    qP++;
                }
            }
        }
    }

    /* 2) Ordena cada grupo por ordem de chegada (senha da LISTA) */
    /* Grupo CNPJ divergente */
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

    /* Grupo peso divergente (CNPJ igual) */
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

    /* 3) Impressão */

    /* --- 3.1 CNPJ divergente (e peso junto, se >10%) --- */
    for (int k = 0; k < qC; k++) {
        int il = cnpj_L[k], iv = cnpj_V[k];

        /* Linha de CNPJ divergente (como já estava) */
        fprintf(output, "%s:%s<->%s\n",
                vistoria[iv].codigo,
                lista[il].cnpj,
                vistoria[iv].cnpj);

        /* Se também tiver diferença de peso > 10%, imprime logo abaixo */
        if (lista[il].peso != vistoria[iv].peso) {
            int base = lista[il].peso;
            float dif_pct;
            int dif_abs = vistoria[iv].peso - lista[il].peso;

            if (base == 0) {
                dif_pct = (vistoria[iv].peso == 0) ? 0.0f : 100.0f;
            } else {
                dif_pct = (abs(dif_abs) * 100.0f) / (float)base;
            }

            if (dif_pct > 10.0f) {
                int dif_kg = abs(dif_abs);
                int prcnt = (int)roundf(dif_pct);

                fprintf(output, "%s:%dkg(%d%%)\n",
                        vistoria[iv].codigo,
                        dif_kg,
                        prcnt);
            }
        }
    }

    /* --- 3.2 Divergência apenas de peso (CNPJ igual) --- */
    int acc = 0;
    float valor;

    conteiners *imprimir = malloc(sizeof *imprimir * tamanhoVistoria);
    conteiners *aux1     = malloc(sizeof *aux1     * tamanhoVistoria);

    for (int k = 0; k < qP; k++) {
        int il = peso_L[k], iv = peso_V[k];

        int base = lista[il].peso;
        int dif_abs = vistoria[iv].peso - lista[il].peso;
        int dif_kg = abs(dif_abs);

        float dif_pct;
        if (base == 0) {
            dif_pct = (vistoria[iv].peso == 0) ? 0.0f : 100.0f;
        } else {
            dif_pct = (abs(dif_abs) * 100.0f) / (float)base;
        }

        /* Já sabemos que dif_pct > 10 aqui, por construção,
           mas recalculamos consistente com imprimir[]. */
        strcpy(imprimir[acc].codigo, lista[il].codigo);
        imprimir[acc].peso  = dif_kg;
        imprimir[acc].prcnt = (int)roundf(dif_pct);

        acc++;
    }

    if (acc > 0) {
        mergesortPORCENTAGEM(imprimir, aux1, 0, acc - 1);
    }

    for (int i = 0; i < acc; i++) {
       if(imprimir[i].prcnt >=11){
        fprintf(output, "%s:%dkg(%d%%)\n",
                imprimir[i].codigo,
                imprimir[i].peso,
                imprimir[i].prcnt);
        }
    }

    /* 4) Liberação de memória */
    free(cnpj_L);
    free(cnpj_V);
    free(peso_L);
    free(peso_V);
    free(imprimir);
    free(aux1);
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
