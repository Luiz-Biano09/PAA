#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int32_t prioridade;
    int32_t bits;
    char **conteudo;  // "01", "0F", "10", etc.
} roteador;


// ---------- Leitura do cabeçalho ----------

int ler_inteiros_header(FILE *in, int32_t *n, int32_t *limite) {
    // Aceita "6 8" ou "6,8"
    char linha[128];
    if (!fgets(linha, sizeof linha, in)) return 0;
    if (sscanf(linha, " %d %d", n, limite) == 2) return 1;
    if (sscanf(linha, " %d , %d", n, limite) == 2) return 1;
    if (sscanf(linha, " %d,%d", n, limite) == 2) return 1;
    return 0;
}

// ---------- Heap sort por prioridade ----------

static inline uint32_t esquerdo(uint32_t i) { return 2u * i + 1u; }
static inline uint32_t direito (uint32_t i) { return 2u * i + 2u; }

void trocar(roteador *a, uint32_t x, uint32_t y) {
    roteador tmp = a[x];
    a[x] = a[y];
    a[y] = tmp;
}

void heapify(roteador *a, uint32_t n, uint32_t i) {
    uint32_t maior = i;
    uint32_t l = esquerdo(i);
    uint32_t r = direito(i);

    if (l < n && a[l].prioridade < a[maior].prioridade) maior = l;
    if (r < n && a[r].prioridade < a[maior].prioridade) maior = r;

    if (maior != i) {
        trocar(a, maior, i);
        heapify(a, n, maior);
    }
}

void construir_heap(roteador *a, uint32_t n) {
    if (n == 0) return;
    for (int32_t i = (int32_t)n / 2 - 1; i >= 0; i--)
        heapify(a, n, (uint32_t)i);
}

void heapsort(roteador *a, uint32_t n) {
    construir_heap(a, n);
    for (int32_t i = (int32_t)n - 1; i > 0; i--) {
        trocar(a, 0u, (uint32_t)i);
        heapify(a, (uint32_t)i, 0u);
    }
}

// ---------- Impressão agrupada ----------
// Agrupa pacotes em linhas, soma(bits) <= limiteBits.
// Cada pacote impresso como |B1,B2,...,Bm|, usando as strings lidas.

// supondo que já existam:
// void trocar(roteador *a, uint32_t x, uint32_t y);
// void heapify(roteador *a, uint32_t n, uint32_t i);
// void construir_heap(roteador *a, uint32_t n);
// void heapsort(roteador *a, uint32_t n);

void imprimir_blocos(roteador *v, int32_t n, int32_t limiteBits, FILE *out) {
   
    int32_t start = 0;

    while (start < n) {
        int32_t soma = 0;
        int32_t end = start;

        // Define o grupo [start, end) pela ORDEM DE CHEGADA
        while (end < n && soma + v[end].bits <= limiteBits) {
            soma += v[end].bits;
            end++;
        }

        int32_t qtd = end - start;
        if (qtd <= 0) break; // só pra segurança

        // Copia o grupo para vetor temporário
        roteador *grupo = malloc((size_t)qtd * sizeof *grupo);
        if (!grupo) {
            perror("malloc grupo");
            exit(1);
        }

        for (int i = 0; i < qtd; i++) {
            grupo[i] = v[start + i];  // cópia rasa (conteudo é reaproveitado, só leitura)
        }

        // Ordena o grupo por prioridade usando SEU heapsort
        heapsort(grupo, (uint32_t)qtd);

        // Imprime linha desse grupo: cada pacote como |B1,B2,...,Bm|
        for (int i = 0; i < qtd; i++) {
            fprintf(out, "|");
            for (int j = 0; j < grupo[i].bits; j++) {
                fprintf(out, "%s", grupo[i].conteudo[j]);
                if (j < grupo[i].bits - 1)
                    fprintf(out, ",");
            }
        }
        fprintf(out, "|\n");

        free(grupo);

        // Próximo grupo começa de onde parou
        start = end;
    }

    
}


// ---------- MAIN com argv[1] / argv[2] ----------

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <entrada> <saida>\n", argv[0]);
        return 1;
    }

    FILE *input  = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");
    if (!input || !output) {
        perror("Erro ao abrir arquivos");
        if (input)  fclose(input);
        if (output) fclose(output);
        return 1;
    }

    int32_t vetor, limite;
    if (!ler_inteiros_header(input, &vetor, &limite)) {
        fprintf(stderr, "Erro lendo cabeçalho (n e qbytes).\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    roteador *Principal = malloc((size_t)vetor * sizeof *Principal);
    if (!Principal) {
        perror("malloc");
        fclose(input);
        fclose(output);
        return 1;
    }

    // leitura dos pacotes no formato:
    // prioridade bits B1 B2 ... Bbits
    for (int i = 0; i < vetor; i++) {
        if (fscanf(input, "%d %d", &Principal[i].prioridade,
                                     &Principal[i].bits) != 2) {
            fprintf(stderr, "Erro lendo cabecalho do pacote %d.\n", i);
            fclose(input);
            fclose(output);
            free(Principal);
            return 1;
        }

        Principal[i].conteudo =
            malloc((size_t)Principal[i].bits * sizeof *Principal[i].conteudo);
        if (!Principal[i].conteudo) {
            perror("malloc");
            fclose(input);
            fclose(output);
            free(Principal);
            return 1;
        }

        for (int j = 0; j < Principal[i].bits; j++) {
            Principal[i].conteudo[j] = malloc(4); // "FF" + '\0'
            if (!Principal[i].conteudo[j]) {
                perror("malloc");
                fclose(input);
                fclose(output);
                return 1;
            }

            if (fscanf(input, "%3s", Principal[i].conteudo[j]) != 1) {
                fprintf(stderr, "Erro lendo byte %d do pacote %d.\n", j, i);
                fclose(input);
                fclose(output);
                return 1;
            }
        }
    }

    // imprime agrupado no arquivo de saída
   imprimir_blocos(Principal, vetor, limite, output);

// (opcional) também no stdout
imprimir_blocos(Principal, vetor, limite, stdout);

// libera memória
for (int i = 0; i < vetor; i++) {
    for (int j = 0; j < Principal[i].bits; j++)
        free(Principal[i].conteudo[j]);
    free(Principal[i].conteudo);
}
free(Principal);

fclose(input);
fclose(output);
return 0;
}
