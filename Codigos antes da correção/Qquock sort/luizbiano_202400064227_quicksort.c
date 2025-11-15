#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>



#define NUM_COPIAS 6

typedef struct 
{
    int32_t *valores;
     int32_t marcador;
}Numeros;



typedef struct 
{
   Numeros *valores;
    char NOME[3];
     int32_t contador; 
    /* data */
}Vetores;


typedef struct {
    int32_t n;                    // tamanho do conjunto
    int32_t *original;            // vetor original
   Vetores *copias[NUM_COPIAS];  // 6 cópias independentes
    int32_t cont[NUM_COPIAS];     // contador por variante
    char NOME[3];
} Conjunto;

// Função que faz cópias de um vetor
void criarCopias(Conjunto *c) {
    for (int i = 0; i < NUM_COPIAS; i++) {
        // Aloca a struct Vetores (uma por cópia)
        c->copias[i] = (Vetores*)malloc(sizeof(Vetores));
        if (!c->copias[i]) {
            fprintf(stderr, "Erro ao alocar struct da copia %d\n", i);
            exit(1);
        }

        // Aloca o vetor de dados da cópia
        c->copias[i]->valores = (int32_t*)malloc(c->n * sizeof(int32_t));
        if (!c->copias[i]->valores) {
            fprintf(stderr, "Erro ao alocar vetor da copia %d\n", i);
            exit(1);
        }

        // Copia os valores do original para a cópia
        memcpy(c->copias[i]->valores, c->original, c->n * sizeof(int32_t));

        // Inicializa nome e contador (se existirem no Conjunto)
        c->copias[i]->NOME[0] = '\0';  // ou "??"
        if (i < NUM_COPIAS) c->copias[i]->contador = 0;
    }
}

// Lê todos os conjuntos do arquivo
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


void trocar(int32_t *array, int32_t x, int32_t y){
    
    int temp = array[x];
            array[x] = array[y];
            array[y] = temp;
}

void trocar1(int32_t *array, int32_t x, int32_t y,int32_t *contadorhp){
    (*contadorhp)++;        
    int temp = array[x];
            array[x] = array[y];
            array[y] = temp;
}

int32_t Aleatorio(int32_t* V, int32_t ini, int32_t n) {
    int32_t valor_absoluto = abs(V[ini]);
    int32_t modulo = valor_absoluto % n;
    int32_t indice = ini + modulo;
    // CORREÇÃO: Limites são [ini, ini + n - 1]
    // ini + n - 1 = j (último elemento do subarray atual)
    if (indice < ini || indice >= ini + n) {
        // Se fora dos limites, retorna um índice válido dentro do subarray
        printf("Ajustando índice %d para dentro de [%d, %d]\n", indice, ini, ini + n - 1);
        return ini + (modulo % n); // Garante que está dentro
    }
    return indice;
}

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




int32_t get_median_of_three_indices_stable(int32_t* V, int32_t i, int32_t j) {
    int32_t N = j - i + 1;
    if (N < 3) return i;

    int32_t i1 = i + (N / 4);
    int32_t i2 = i + (N / 2);
    int32_t i3 = j - (N / 4);

    Element p[3] = {
        { V[i1], i1 },
        { V[i2], i2 },
        { V[i3], i3 }
    };

    // Bubble sort estável por (valor, índice)
    for (int k = 0; k < 3; k++) {
        for (int l = k + 1; l < 3; l++) {
            int should_swap = 0;
            if (p[k].value > p[l].value) {
                should_swap = 1;
            } else if (p[k].value == p[l].value && p[k].index > p[l].index) {
                should_swap = 1;
            }
            if (should_swap) {
                Element tmp = p[k];
                p[k] = p[l];
                p[l] = tmp;
            }
        }
    }

    return p[1].index; // mediana
}

// --------------------------------------------------------
// FUNÇÃO DE TROCA PADRÃO
// --------------------------------------------------------
static inline void trocar3(int32_t* V, int32_t a, int32_t b) {
    int32_t tmp = V[a];
    V[a] = V[b];
    V[b] = tmp;
}

// --------------------------------------------------------
// PARTIÇÃO LOMUTO + MEDIANA DE 3 ESTÁVEL + CONTAGEM
// --------------------------------------------------------
int32_t lomutoMediana3(int32_t* V, int32_t i, int32_t j, int32_t *contadorhp) {

    // +1: chamada de partição (LM)
    (*contadorhp)++;

    // Escolhe pivô pela mediana de três estável
    int32_t indice_pivo = get_median_of_three_indices_stable(V, i, j);

    // Move o pivô mediano para o final (V[j])
    trocar3(V, indice_pivo, j);
    (*contadorhp)++;  // +1: troca (pivô -> fim)

    int32_t P = V[j];
    int32_t x = i - 1;

    // Particiona em torno de P
    for (int32_t y = i; y < j; y++) {
        if (V[y] <= P) {
            x++;
            trocar3(V, x, y);
            (*contadorhp)++;  // +1: cada troca feita no laço
        }
    }

    // Coloca pivô na posição correta
    x++;
    trocar(V, x, j);
    (*contadorhp)++;  // +1: troca final do pivô

    return x; // índice final do pivô
}

// --------------------------------------------------------
// QUICKSORT LOMUTO MEDIANA 3 COM CONTAGEM ESTILO GABARITO
// (só conta recursão em subvetores de tamanho >= 2)
// --------------------------------------------------------
void quicksortLM(int32_t* V, int32_t i, int32_t j, int32_t *contadorhp) {

    if (i < j) {

        // Particiona
        int32_t p = lomutoMediana3(V, i, j, contadorhp);

        // Chamada recursiva para o subvetor esquerdo, se tiver tamanho >= 2
        if (i < p - 1) {
            (*contadorhp)++;                      // +1: chamada recursiva "útil"
            quicksortLM(V, i, p - 1, contadorhp);
        }

        // Chamada recursiva para o subvetor direito, se tiver tamanho >= 2
        if (p + 1 < j) {
            (*contadorhp)++;                      // +1: chamada recursiva "útil"
            quicksortLM(V, p + 1, j, contadorhp);
        }
    }
}


// --------------------------------------------------------
// QUICKSORT LOMUTO COM MEDIANA DE TRÊS ESTÁVEL (Com Lógica de Contagem do Usuário)
// --------------------------------------------------------



 void BubbleSort3(Numeros vetor[], int32_t *contadorhp) {
    int n = 3;
    Numeros temp;

    // Duas passagens são suficientes para ordenar 3 elementos
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            
            // Incrementa o contador para a comparação principal de valor
           // (*contadorhp)++; 
            
            int deve_trocar = 0;

            // 1. Ordem Crescente de Valores
            if (vetor[j].valores > vetor[j+1].valores) {
                deve_trocar = 1;
            } 
            // 2. Critério de Desempate: Se valores iguais, ordena pelo Marcador (crescente)
            else if (vetor[j].valores == vetor[j+1].valores) {
                // Incrementa o contador para a comparação de desempate
                //(*contadorhp)++; 
                
                // Troque se o marcador atual for MAIOR que o próximo, forçando ordem CRESCENTE de marcadores.
                if (vetor[j].marcador > vetor[j+1].marcador) {
                    deve_trocar = 1;
                }
            }

            if (deve_trocar) {
                temp = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = temp;
            }
        }
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
int32_t Mediana3(int32_t* V, int32_t tamanho, int32_t i, int32_t j, int32_t *contadorhp) {
    if (tamanho <= 2) return i + (tamanho - 1) / 2;

    // Seleção dos 3 índices (aproximadamente 1/4, 1/2 e 3/4 do subvetor)
    int32_t indiceA =  tamanho / 4;
    int32_t indiceB =  tamanho / 2;
    int32_t indiceC =  3 * tamanho / 4;

    // 1. Prepara o vetor de 3 elementos com seus índices originais como marcadores
    Numeros vetormediana[3];
    vetormediana[0].valores = V[indiceA];
    vetormediana[0].marcador = indiceA;
    
    vetormediana[1].valores = V[indiceB];
    vetormediana[1].marcador = indiceB;

    vetormediana[2].valores = V[indiceC];
    vetormediana[2].marcador = indiceC;
    
    // Zera o contador para contar as comparações APENAS nesta chamada
   
    
    // 2. Ordena o vetor de 3 elementos, incrementando contadorhp internamente
    BubbleSort3(vetormediana, contadorhp);
    (*contadorhp)++;

    printf("\nValores para mediana3 (V[%d], V[%d], V[%d]): %d||%d||%d\n", indiceA, indiceB, indiceC, V[indiceA], V[indiceB], V[indiceC]);
    
    // 3. Resultados da Ordenação
    printf("Resultados Ordenados (Valor, Marcador/Índice Original):\n");
    printf("  Menor: (%d, %d)\n", vetormediana[0].valores, vetormediana[0].marcador);
    printf("  Mediano: (%d, %d)\n", vetormediana[1].valores, vetormediana[1].marcador);
    printf("  Maior: (%d, %d)\n", vetormediana[2].valores, vetormediana[2].marcador);
           
    // 4. Retorna o MARCADOR (índice original) do elemento que está no meio (índice 1)
    return vetormediana[1].marcador;
}

// Estrutura e Auxiliares (mantidas)


void trocar5(int32_t* a, int32_t* b,int32_t *contadorhp) {
    int32_t t = *a;
    *a = *b;
    *b = t;
    (*contadorhp)++;
}




// --------------------------------------------------------
// PARTIÇÃO DE HOARE COM MEDIANA DE TRÊS (HM)
// --------------------------------------------------------
int32_t hoareMediana3(int32_t* V, int32_t i, int32_t j, int32_t *contadorhp) {
    
    // 1. Escolhe o pivô usando Mediana de Três Estável
    int32_t indice_pivo = get_median_of_three_indices_stable(V, i, j);
    
    // 2. Move o pivô mediano para a posição inicial (V[i])
    trocar(V, indice_pivo, i);
    (*contadorhp)++; // +1 Troca (Mediana para V[i])

    // 3. Partição Hoare (mantendo o pivô em V[i])
    int32_t P = V[i], x = i - 1, y = j + 1;

    while(1) {
        // As lógicas de incremento e decremento são mantidas como no seu código
        while(V[--y] > P) {
            // Comparações de V[y] > P não contam como passo neste gabarito
        }
        while(V[++x] < P) {
            // Comparações de V[x] < P não contam como passo neste gabarito
        }
        
        if(x < y) {
            trocar(V, x, y);
            (*contadorhp)++; // +1 Troca (Contagem de passo)
        } else {
            return y;
        }
    }
}


// --------------------------------------------------------
// QUICKSORT HOARE MEDIANA 3 (HM) - CONTADOR DE CHAMADAS
// --------------------------------------------------------
void quicksortHM(int32_t* V, int32_t i, int32_t j, int32_t *contadorhp) {

    // Caso base
    if(i < j) {
        // Particionamento do vetor
        int32_t p = hoareMediana3(V, i, j, contadorhp);
        
        // Divisão em subvetores
        quicksortHM(V, i, p, contadorhp);
        (*contadorhp)++; // +1 Chamada Recursiva (primeira metade)
        
        quicksortHM(V, p + 1, j, contadorhp);
        (*contadorhp)++; // +1 Chamada Recursiva (segunda metade)
    } else {
        // No seu padrão, a contagem de chamadas recursivas acontece APÓS as chamadas,
        // o que não inclui a chamada inicial (i, j) nem as do caso base (i >= j).
        // Isso é diferente do padrão que gerou 37, mas estou seguindo a sua lógica de contagem atual.
    }
}

 


int32_t qksHMediana_3 (int32_t* V, int32_t tamanho, int32_t i, int32_t j,int32_t *contadorhp) {
 (*contadorhp)++;
 int32_t valor = Mediana3(V,tamanho,i,j,contadorhp);
 trocar(V,valor,i);
 

 
//
 {
 // Declaração do pivô e índices
 int32_t P = V[i], x = i - 1, y = j + 1;

 
 // Particionando o vetor pelo pivô
 while(1) {
 while(V[--y] > P);
 while(V[++x] < P);
 if(x < y) {
    trocar(V,x, y);
//(*contadorhp)++;

}

 else 
 
 return y;
 }
 
 }  

}

void quickHM3(int32_t* V, int32_t i, int32_t j,int32_t *contadorhp) {
   (*contadorhp)++;
    if (i >= j) return;
    int32_t tamanho = j - i + 1;

    int32_t p = qksHMediana_3(V, tamanho, i, j,contadorhp);

    quickHM3(V, i, p,contadorhp);
  
    quickHM3(V, p + 1, j,contadorhp);
   
}

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


void imprimirArray(int32_t* V, int32_t tamanho, const char* nome) {
    printf("%s: [", nome);
    for (int32_t i = 0; i < tamanho; i++) {
        printf("%d", V[i]);
        if (i < tamanho - 1) printf(", ");
    }
    printf("]\n");
}

int32_t estocagem(int32_t *V, int32_t i, int32_t nconjunto, int32_t *contadorhp, char *nome) {
    *contadorhp = 0;

    switch (i) {
        case 0: quicksortHP(V, 0, nconjunto - 1, contadorhp);  (*contadorhp)++;      strcpy(nome, "HP"); break;
        case 1: quicksortLomutoPadrao(V, 0, nconjunto - 1, contadorhp);(*contadorhp)++; strcpy(nome, "LP"); break;
        case 2:  quicksortHM(V, 0, nconjunto - 1, contadorhp);       strcpy(nome, "HM"); break;
        case 3:quicksortLM(V, 0, nconjunto - 1, contadorhp);  strcpy(nome, "LM"); break;
        case 4: quickHoAleatorio(V, 0, nconjunto - 1, contadorhp);(*contadorhp)++;  strcpy(nome, "HA"); break;
        case 5: quicksortL_A(V, 0, nconjunto - 1, contadorhp);(*contadorhp)++;      strcpy(nome, "LA"); break;
        default:                                                    strcpy(nome, "??");  break;
    }
    return *contadorhp;
}
  
void ordenarPorContadorPtr(Vetores *v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            int swap = 0;
            if (v[j]->contador > v[j + 1]->contador) {
                swap = 1;
            } else if (v[j]->contador == v[j + 1]->contador) {
                // desempate por nome (opcional)
                if (strcmp(v[j]->NOME, v[j + 1]->NOME) > 0) swap = 1;
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

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    if (!input || !output) {
        perror("Erro ao abrir arquivos");
        if (input) fclose(input);
        if (output) fclose(output);
        return 1;
    }
    
    int32_t totalConjuntos;
    int32_t total;
    Conjunto *C = lerConjuntos("quicksortinput.txt", &total);

    ;
int32_t contadortotal = 0;
    for (int c = 0; c < total; c++) {
        // roda as 6 variantes, cada uma em sua cópia
        for (int k = 0; k < NUM_COPIAS; k++) {
            estocagem(C[c].copias[k]->valores, k, C[c].n, &C[c].copias[k]->contador,C[c].copias[k]->NOME);
        }
        // imprime um resumo (opcional)
        printf(" Conjunto %d:\n", c+1);
        for (int k = 0; k < NUM_COPIAS; k++) {
            char tag[32];
            sprintf(tag, "Copia_%d_ordenada", k+1);
           ordenarPorContadorPtr(C[c].copias,NUM_COPIAS);
            imprimirArray(C[c].copias[k]->valores, C[c].n, tag);
            printf("%s//Contador variante %d = %d\n",C[c].copias[k]->NOME, k, C[c].copias[k]->contador);
        }
        puts("");
    }
for(int b = 0; b<total;b++){
    for(int32_t a = 0; a<NUM_COPIAS;a++){
    if(a == 0){
        fprintf(output,"[%d]:",C[b].n,C[b].copias[a]->NOME,C[b].copias[a]->contador);
    }
        fprintf(output,"%s(%d)",C[b].copias[a]->NOME,C[b].copias[a]->contador); 
        if(a==5){
            fprintf(output,"\n");
        }   
}

}
    // libera memória
    for (int c = 0; c < total; c++) {
        free(C[c].original);
        for (int k = 0; k < NUM_COPIAS; k++) free(C[c].copias[k]);
    }
    free(C);
    return 0;
}



   

