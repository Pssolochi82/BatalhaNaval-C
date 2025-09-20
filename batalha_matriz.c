#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define LIN 6
#define COL 6

typedef enum { HORIZONTAL = 0, VERTICAL = 1 } Direcao;

/* --------- Utils da matriz --------- */
void limpar_matriz(int m[LIN][COL]) {
    for (int i = 0; i < LIN; i++)
        for (int j = 0; j < COL; j++)
            m[i][j] = 0; // 0 = água
}

void imprimir_matriz(int m[LIN][COL], bool revelar) {
    printf("    ");
    for (int c = 0; c < COL; c++) printf("%d ", c);
    printf("\n");
    printf("   -----------------\n");
    for (int i = 0; i < LIN; i++) {
        printf("%d | ", i);
        for (int j = 0; j < COL; j++) {
            if (revelar) {
                printf("%d ", m[i][j]); // mostra 0/1/2
            } else {
                // Oculto (para oponente): mostra água como '.' e embarcação como '#'
                printf("%c ", m[i][j] == 0 ? '.' : '#');
            }
        }
        printf("\n");
    }
}

/* Verifica se um navio/barco cabe a partir (r,c) numa direção e tamanho dados,
   sem sair da grelha e sem sobrepor outras peças. */
bool pode_colocar(int m[LIN][COL], int r, int c, int tamanho, Direcao dir) {
    if (dir == HORIZONTAL) {
        if (c + tamanho > COL) return false;
        for (int j = 0; j < tamanho; j++)
            if (m[r][c + j] != 0) return false;
    } else { // VERTICAL
        if (r + tamanho > LIN) return false;
        for (int i = 0; i < tamanho; i++)
            if (m[r + i][c] != 0) return false;
    }
    return true;
}

/* Coloca uma embarcação de "tamanho" e grava "codigo" (1=navio, 2=barco) */
bool colocar_uma_embarcacao(int m[LIN][COL], int tamanho, int codigo) {
    // tenta várias posições aleatórias até conseguir
    for (int tent = 0; tent < 200; tent++) {
        int r = rand() % LIN;
        int c = rand() % COL;
        Direcao dir = (rand() % 2 == 0) ? HORIZONTAL : VERTICAL;

        if (pode_colocar(m, r, c, tamanho, dir)) {
            if (dir == HORIZONTAL) {
                for (int j = 0; j < tamanho; j++) m[r][c + j] = codigo;
            } else {
                for (int i = 0; i < tamanho; i++) m[r + i][c] = codigo;
            }
            return true;
        }
    }
    return false; // não conseguiu (grelha cheia demais para esta peça)
}

/* Coloca várias embarcações segundo vetores de tamanhos e códigos. */
bool colocar_embarcacoes(int m[LIN][COL],
                         const int *tamanhos, const int *codigos, int n) {
    for (int k = 0; k < n; k++) {
        if (!colocar_uma_embarcacao(m, tamanhos[k], codigos[k])) {
            return false;
        }
    }
    return true;
}

int main(void) {
    srand((unsigned)time(NULL));

    int grelha[LIN][COL];
    limpar_matriz(grelha);

    /* === AJUSTA AQUI CONSOANTE O QUE O TEU GRUPO DECIDIR ===
       Exemplo clássico:
       - 1 "navio" tamanho 3 (código 1)
       - 2 "barcos" tamanho 2 (código 2)
       (Podes mudar as quantidades e tamanhos, mantendo 1 e 2 como códigos.)
    */
    int tamanhos[] = {3, 2, 2};
    int codigos[]  = {1, 2, 2};
    int qtd = sizeof(tamanhos) / sizeof(tamanhos[0]);

    if (!colocar_embarcacoes(grelha, tamanhos, codigos, qtd)) {
        printf("Falha ao colocar todas as embarcacoes. Tente novamente.\n");
        return 1;
    }

    printf("Grelha (revelada para testes):\n");
    imprimir_matriz(grelha, true);

    printf("\nGrelha (oculta para oponente):\n");
    imprimir_matriz(grelha, false);

    return 0;
}
