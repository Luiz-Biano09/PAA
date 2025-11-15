#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#define NUM_COPIAS 6

typedef struct {
    int32_t *valores;
    int32_t marcador;
} Numeros;

/* AQUI: cada Vetores guarda um vetor de int32_t,
   não um vetor de Numeros */
typedef struct {
    int32_t *valores;   // <-- corrigido
    char NOME[3];
    int32_t contador;
} Vetores;

typedef struct {
    int32_t n;                     // tamanho do conjunto
    int32_t *original;             // vetor original
    Vetores *copias[NUM_COPIAS];   // 6 cópias independentes
} Conjunto;

/* ---------- PROTÓTIPOS QUE SÃO USADOS ANTES DA DEFINIÇÃO ---------- */
int32_t hoareMEDIANA(int32_t* V, int32_t i, int32_t j, int32_t *contadorhp);
int32_t hoareP(int32_t* V, int32_t i, int32_t j, int32_t *contadorhp);
int32_t lomutoAleatorio(int32_t* V, int32_t i, int32_t j, int32_t *contadorhp);
int32_t lomutoMEDIANA(int32_t* V, int32_t i, int32_t j, int32_t *contadorhp);
int32_t HoAleatorio(int32_t* V, int32_t i, int32_t j, int32_t *contadorhp);
int32_t get_median_of_three_indices_stable(int32_t* V, int32_t i, int32_t j);

/* ----------------------- CÓPIAS ----------------------- */

void criarCopias(Conjunto *c) {
    for (int i = 0; i < NUM_COPIAS; i++) {
        c->copias[i] = (Vetores*) malloc(sizeof(Vetores));
        if (!c->copias[i]) {
            fprintf(stderr, "Erro ao alocar struct da copia %d\n", i);
            exit(1);
        }

        c->copias[i]->valores = (int32_t*) malloc(c->n * sizeof(int32_t));
        if (!c->copias[i]->valores) {
            fprintf(stderr, "Erro ao alocar vetor da copia %d\n", i);
            exit(1);
        }

        memcpy(c->copias[i]->valores, c->original, c->n * sizeof(int32_t));

        c->copias[i]->NOME[0] = '\0';
        c->copias[i]->NOME[1] = '\0';
        c->copias[i]->NOME[2] = '\0';
        c->copias[i]->contador = 0;
    }
}

/* ----------------------- LEITURA ----------------------- */

static Conjunto* lerConjuntos(const char *nomeArq, int32_t *totalConjuntos) {
    FILE *arq = fopen(nomeArq, "r");
    if (!arq) { perror("fopen"); exit(1); }

    if (fscanf(arq, "%d", totalConjuntos) != 1) {
        fprintf(stderr, "Erro ao ler número de conjuntos\n");
        fclose(arq); exit(1);
    }

    Conjunto *C = malloc(*totalConjuntos * sizeof(*C));
    if (!C) { perror("malloc"); fclose(arq); exit(1); }

    for (int c = 0; c < *totalConjuntos; c++) {
        if (fscanf(arq, "%d", &C[c].n) != 1) {
            fprintf(stderr, "Erro ao ler n do conjunto %d\n", c+1);
            fclose(arq); exit(1);
        }

        C[c].original = malloc(C[c].n * sizeof(int32_t));
        if (!C[c].original) { perror("malloc"); fclose(arq); exit(1); }

        for (int i = 0; i < C[c].n; i++) {
            if (fscanf(arq, "%d", &C[c].original[i]) != 1) {
                fprintf(stderr, "Erro ao ler elemento %d do conjunto %d\n", i, c+1);
                fclose(arq); exit(1);
            }
        }

        criarCopias(&C[c]);
    }
    fclose(arq);
    return C;
}

/* ----------------------- AUXILIARES ----------------------- */

void trocar(int32_t *array, int32_t x, int32_t y){
    int32_t temp = array[x];
    array[x] = array[y];
    array[y] = temp;
}

void trocar1(int32_t *array, int32_t x, int32_t y,int32_t *contadorhp){
    (*contadorhp)++;
    int32_t temp = array[x];
    array[x] = array[y];
    array[y] = temp;
}

int32_t Aleatorio(int32_t* V, int32_t ini, int32_t n) {
    int32_t valor_absoluto = abs(V[ini]);
    int32_t modulo = valor_absoluto % n;
    int32_t indice = ini + modulo;
    if (indice < ini || indice >= ini + n) {
        return ini + (modulo % n);
    }
    return indice;
}

/* ----------------------- (resto dos quicksorts igual ao seu) ----------------------- */
/* Mantive sua lógica original abaixo, apenas usando Vetores->valores como int32_t*.  */
/* ... TODO: suas funções lomutoP, quicksortLomutoPadrao, get_median_of_three_indices_stable,
   hoareMEDIANA, lomutoMEDIANA, HoAleatorio, etc, permanecem como estão no seu código. */

/* Para não poluir demais aqui, vou manter a parte relevante no main. */

/* ----------------------- IMPRESSÃO ----------------------- */



/* (sua função estocagem e ordenarPorContadorPtr permanecem iguais) */

/* ----------------------- MAIN ----------------------- */



int32_t lomutoP(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp) {
 // Declaração do pivô e índices
 int32_t P = V[j], x = i - 1, y = i;
 // Particionando o vetor pelo pivô
 for(y = i; y < j; y++){
if(V[y] <= P){
 trocar(V,++x,y);
(*contadorhp)++;
 }
}
 // Posicionando o pivô no vetor
 trocar(V,++x, j);
 (*contadorhp)++;
 // Retornando índice do pivô
 return x;
 }
void quicksortLomutoPadrao(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp) {
 // Caso base
 if(i < j) {
 // Particionamento do vetor
 int32_t p = lomutoP(V, i, j,contadorhp);
 // Divisão em subvetores
 quicksortLomutoPadrao(V, i, p - 1,contadorhp);
 (*contadorhp)++;
 quicksortLomutoPadrao(V, p + 1, j,contadorhp);
 (*contadorhp)++;
 }
 }
typedef struct { 
    int32_t value; 
    int32_t index; 
} Element;

void BubbleSort3(int32_t *vetor) {
    int n = 3;
    int32_t temp;

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (vetor[i] >= vetor[j]) { // usa > para não bagunçar estabilidade por valor igual
                temp = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = temp;
            }
        }
    }
}

int32_t get_median_of_three_indices_stable(int32_t* V, int32_t i, int32_t j) {
    int32_t N = j - i + 1;
   
    int32_t i1 = i + (N / 4);
    int32_t i2 = i + (N / 2);
    int32_t i3 = i + (3 * N / 4);

    int32_t Vetor[3];
    Vetor[0] = V[i1];
    Vetor[1] = V[i2];
    Vetor[2] = V[i3];

    BubbleSort3(Vetor);

    // Mediana é Vetor[1]; agora escolhemos o índice de forma estável
    if (Vetor[1] == V[i1]) return i1;
    if (Vetor[1] == V[i2]) return i2;
    return i3;
}


// --------------------------------------------------------
// FUNÇÃO DE TROCA PADRÃO
// --------------------------------------------------------
static inline void trocar3(int32_t* V, int32_t a, int32_t b) {
    int32_t tmp = V[a];
    V[a] = V[b];
    V[b] = tmp;
}


void quicksortHM(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp) {

    // Caso base
 if(i < j) {
 // Particionamento do vetor
 
 int32_t p = hoareMEDIANA(V, i, j,contadorhp);
  

 // Divisão em subvetores
 quicksortHM(V, i, p,contadorhp);
 (*contadorhp)++;

 
 quicksortHM(V, p + 1, j,contadorhp);
(*contadorhp)++;
  
 }
 }

    


int32_t hoareMEDIANA(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp){
 // Declaração do pivô e índices
 //int32_t tamanho = j - i + 1;
 int32_t indice = get_median_of_three_indices_stable(V,i,j);
 trocar(V,i,indice);
 (*contadorhp)++;
 int32_t P = V[i], x = i - 1, y = j + 1;

 
 // Particionando o vetor pelo pivô
 while(1) {
 while(V[--y] > P);
 while(V[++x] < P);
 if(x < y) {
    trocar(V,x, y);
    (*contadorhp)++;

}
 else return y;
 }
 }  







 
/**
 * @brief Calcula a Mediana de 3 elementos usando o índice original como critério de desempate.
 * @param V O vetor de valores.
 * @param tamanho O tamanho total do vetor.
 * @param i Índice inicial.
 * @param j Índice final (não utilizado, mantido para consistência).
 * @param contadorhp Ponteiro para o contador de comparações.
 * @return O índice original do elemento mediano.
 */

// Estrutura e Auxiliares (mantidas)


void trocar5(int32_t* a, int32_t* b,int32_t *contadorhp) {
    int32_t t = *a;
    *a = *b;
    *b = t;
    (*contadorhp)++;
}






// --------------------------------------------------------
// QUICKSORT HOARE MEDIANA 3 (HM) - CONTADOR DE CHAMADAS
// --------------------------------------------------------







 // Quicksort recursivo
void quicksortHP(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp) {

    // Caso base
 if(i < j) {
 // Particionamento do vetor
 
 int32_t p = hoareP(V, i, j,contadorhp);
  

 // Divisão em subvetores
 quicksortHP(V, i, p,contadorhp);
 (*contadorhp)++;

 
 quicksortHP(V, p + 1, j,contadorhp);
(*contadorhp)++;
  
 }
 }

    int32_t hoareP(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp){
 // Declaração do pivô e índices
 int32_t P = V[i], x = i - 1, y = j + 1;

 
 // Particionando o vetor pelo pivô
 while(1) {
 while(V[--y] > P){
 
 }
 while(V[++x] < P){
 
 }
 if(x < y) {
   
    trocar(V,x, y);
     (*contadorhp)++;
 

}
 else return y;
 }
 }  

void quicksortL_A(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp) {
 // Caso base
 if(i < j) {
 // Particionamento do vetor
 int32_t p = lomutoAleatorio(V, i, j,contadorhp);
 // Divisão em subvetores
 quicksortL_A(V, i, p - 1,contadorhp);
 (*contadorhp)++;
 quicksortL_A(V, p + 1, j,contadorhp);
 (*contadorhp)++;
 }
 }

int32_t lomutoAleatorio(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp) {
 // Declaração do pivô e índices
  int32_t tamanho = j - i + 1;
 int32_t valor = Aleatorio(V,i,tamanho);
 trocar(V,valor,j);
 (*contadorhp)++;
 int32_t P = V[j], x = i - 1, y = i;
 // Particionando o vetor pelo pivô
 for(y = i; y < j; y++){
if(V[y] <= P){ 
    trocar(V,++x,y);
     (*contadorhp)++;
}
}
 // Posicionando o pivô no vetor
 trocar(V,++x, j);
 (*contadorhp)++;
 // Retornando índice do pivô
 return x;
 }


void quicksortLM(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp) {
 // Caso base
 if(i < j) {
 // Particionamento do vetor
 int32_t p = lomutoMEDIANA(V, i, j,contadorhp);
 // Divisão em subvetores
 quicksortLM(V, i, p - 1,contadorhp);
 (*contadorhp)++;
 quicksortLM(V, p + 1, j,contadorhp);
 (*contadorhp)++;
 }
 }

int32_t lomutoMEDIANA(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp) {
 // Declaração do pivô e índices
 // int32_t tamanho = j - i + 1;
 int32_t valor = get_median_of_three_indices_stable(V,i,j);
 trocar(V,valor,j);
 (*contadorhp)++;
 int32_t P = V[j], x = i - 1, y = i;
 // Particionando o vetor pelo pivô
 for(y = i; y < j; y++){
if(V[y] <= P){ 
    trocar(V,++x,y);
     (*contadorhp)++;
}
}
 // Posicionando o pivô no vetor
 trocar(V,++x, j);
 (*contadorhp)++;
 // Retornando índice do pivô
 return x;
 }


 

void quickHoAleatorio(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp) {

    // Caso base
 if(i < j) {
 // Particionamento do vetor
 int32_t p = HoAleatorio(V, i, j,contadorhp);
 
 
 // Divisão em subvetores
  quickHoAleatorio(V, i, p,contadorhp);
  (*contadorhp)++;
 
 quickHoAleatorio(V, p + 1, j,contadorhp);
 (*contadorhp)++;
 
 }
 }

    int32_t HoAleatorio(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp){
 // Declaração do pivô e índices
 int32_t tamanho = j - i + 1;
 int32_t indice = Aleatorio(V,i,tamanho);
 trocar(V,indice,i);
 (*contadorhp)++;
 int32_t P = V[i], x = i - 1, y = j + 1;

 
 // Particionando o vetor pelo pivô
 while(1) {
 while(V[--y] > P);
 while(V[++x] < P);
 if(x < y) {
    trocar(V,x, y);
    (*contadorhp)++;

}
 else return y;
 }
 }  




int32_t estocagem(int32_t *V, int32_t i, int32_t nconjunto, int32_t *contadorhp, char *nome) {
    *contadorhp = 0;

    switch (i) {
        case 0: quicksortHP(V, 0, nconjunto - 1, contadorhp);  (*contadorhp)++;      strcpy(nome, "HP"); break;
        case 1: quicksortLomutoPadrao(V, 0, nconjunto - 1, contadorhp);(*contadorhp)++; strcpy(nome, "LP"); break;
        case 2:  quicksortHM(V, 0, nconjunto - 1, contadorhp);  (*contadorhp)++;     strcpy(nome, "HM"); break;
        case 3:quicksortLM(V, 0, nconjunto - 1, contadorhp); (*contadorhp)++;  strcpy(nome, "LM"); break;
        case 4: quickHoAleatorio(V, 0, nconjunto - 1, contadorhp);(*contadorhp)++;  strcpy(nome, "HA"); break;
        case 5: quicksortL_A(V, 0, nconjunto - 1, contadorhp);(*contadorhp)++;      strcpy(nome, "LA"); break;
        default:                                                    strcpy(nome, "??");  break;
    }
    return *contadorhp;
}
  
int prioridade_nome(const char *nome) {
    int p0, p1;

    // Prioridade da primeira letra
    // L tem mais prioridade (vem antes), depois H, depois qualquer outra
    if (nome[0] == 'L')      p0 = 0;
    else if (nome[0] == 'H') p0 = 1;
    else                     p0 = 2;

    // Prioridade da segunda letra: P > M > A > resto
    if (nome[1] == 'P')      p1 = 0;
    else if (nome[1] == 'M') p1 = 1;
    else if (nome[1] == 'A') p1 = 2;
    else                     p1 = 3;

    return p0 * 10 + p1; // combina as duas prioridades
}

void ordenarPorContadorPtr(Vetores *v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            int swap = 0;

            // 1) Ordena pelo contador (crescente)
            if (v[j]->contador > v[j + 1]->contador) {
                swap = 1;
            }
            // 2) Desempate pelo NOME com regra de prioridade
            else if (v[j]->contador == v[j + 1]->contador) {
                int prj  = prioridade_nome(v[j]->NOME);
                int prj1 = prioridade_nome(v[j + 1]->NOME);
                if (prj > prj1) {
                    swap = 1;
                }
            }

            if (swap) {
                Vetores *tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
            }
        }
    }
}





int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <entrada> <saida>\n", argv[0]);
        return 1;
    }

    int32_t total;
    Conjunto *C = lerConjuntos(argv[1], &total);

    FILE* output = fopen(argv[2], "w");
    if (!output) {
        perror("Erro ao abrir arquivo de saída");
        return 1;
    }

    for (int c = 0; c < total; c++) {
        for (int k = 0; k < NUM_COPIAS; k++) {
            estocagem(C[c].copias[k]->valores,
                      k,
                      C[c].n,
                      &C[c].copias[k]->contador,
                      C[c].copias[k]->NOME);
        }

        ordenarPorContadorPtr(C[c].copias, NUM_COPIAS);

       
    fprintf(output, "[%d]:", C[c].n);

for (int k = 0; k < NUM_COPIAS; k++) {
    Vetores *v = C[c].copias[k];
    fprintf(output, "%s(%d)", v->NOME, v->contador);

    // Adiciona vírgula entre elementos, exceto no último
    if (k < NUM_COPIAS - 1) {
        fprintf(output, ",");
    }
}

fprintf(output, "\n");
    }
    // libera memória
    for (int c = 0; c < total; c++) {
        free(C[c].original);
        for (int k = 0; k < NUM_COPIAS; k++) {
            free(C[c].copias[k]->valores);  // <-- importante
            free(C[c].copias[k]);
        }
    }
    free(C);
    fclose(output);

    return 0;
}
   

