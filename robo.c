#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definições conforme o documento [cite: 92, 108]
#define MALLOC(tipo, n) (tipo *)malloc((n) * sizeof(tipo))
#define FREE(p) free(p)

// declarações
typedef struct
{
    int r, c;
} Ponto;

typedef enum
{
    LIMPAR,
    MOVER_N,
    MOVER_S,
    MOVER_L,
    MOVER_O,
    FICAR
} Acao;


typedef struct {
    Acao *v;
    int cap, ini, sz;
} Log;

// Funções de Log [cite: 100]
void log_init(Log *l, int capacidade) {
    l->v = MALLOC(Acao, capacidade);
    l->cap = capacidade;
    l->ini = 0;
    l->sz = 0;
}

void log_push(Log *l, Acao a) {
    int pos = (l->ini + l->sz) % l->cap;
    l->v[pos] = a;
    if (l->sz < l->cap) l->sz++;
    else l->ini = (l->ini + 1) % l->cap;
} 

// Funções
int dentro(int r, int c, int N, int M)
{ // define limite
    return r >= 0 && r < N && c >= 0 && c < M;
}

int bloqueado(char mapa[100][100], int r, int c, int N, int M)
{

    if (r < 0 || r >= N || c < 0 || c >= M)
        return 1;

    if (mapa[r][c] == '#')
        return 1;

    return 0;
}

Acao decide_reflex(char mapa[][100], int N, int M, Ponto robo, int *bloqueios)
{

    // 1. limpar
    if (mapa[robo.r][robo.c] == '*')
    { // se na posição do robo é sugeira ent limpa
        printf("Regra 1: limpar sujeira atual\n");
        return LIMPAR;
    }

    // 2. procura sugeira
    int dr[4] = {-1, 1, 0, 0}; // direção linha/row e as codenadas dos "lados"
    int dc[4] = {0, 0, 1, -1}; // direção coluna e as codenadas de "cima/baixo"

    for (int i = 0; i < 4; i++)
    {                            // percorre a "cruz"
        int nr = robo.r + dr[i]; // a linha que esta vendo é a posição atual do robo mas as cordenadas da função
        int nc = robo.c + dc[i]; // mesma logica
        // se nr e nc esta dentro e se tem sujeira
        if (dentro(nr, nc, N, M) && mapa[nr][nc] == '*')
        {
            printf("Regra 2: sujeira vizinha encontrada\n");
            if (i == 0)
                return MOVER_N;
            if (i == 1)
                return MOVER_S;
            if (i == 2)
                return MOVER_L;
            if (i == 3)
                return MOVER_O;
        }
    }
    // 3.zig-zag
    if (robo.r % 2 == 0)
    { // se par
        printf("Regra 3 (zig-zag): coluna par\n");

        if (dentro(robo.r, robo.c + 1, N, M) && mapa[robo.r][robo.c + 1] != '#')
            return MOVER_L; // move pra direita
        else
            (*bloqueios)++;

        if (dentro(robo.r + 1, robo.c, N, M) && mapa[robo.r + 1][robo.c] != '#')
            return MOVER_S; // move pra baixo
        else
            (*bloqueios)++;

        if (dentro(robo.r, robo.c - 1, N, M) && mapa[robo.r][robo.c - 1] != '#')
            return MOVER_O; // move pra esquerda
        else
            (*bloqueios)++;
    }
    else
    { // se impar
        printf("Regra 3 (zig-zag): coluna impar\n");

        if (dentro(robo.r, robo.c - 1, N, M) && mapa[robo.r][robo.c - 1] != '#') // ve se da pra esquerda
            return MOVER_O;                                                      // move pra esquerda
        else
            (*bloqueios)++;

        if (dentro(robo.r + 1, robo.c, N, M) && mapa[robo.r + 1][robo.c] != '#')
            return MOVER_S; // move pra baixo
        else
            (*bloqueios)++;

        if (dentro(robo.r, robo.c + 1, N, M) && mapa[robo.r][robo.c + 1] != '#')
            return MOVER_L; // move pra direita
        else
            (*bloqueios)++;
    }

    // fallback
    printf("Fallback: tentando norte ou sul\n");
    if (dentro(robo.r - 1, robo.c, N, M))
        return MOVER_N;

    if (dentro(robo.r + 1, robo.c, N, M))
        return MOVER_S;
    return FICAR; // parado
}

void imprimir_mapa(char mapa[][100], int N, int M, Ponto robo)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            if (i == robo.r && j == robo.c)
                printf("R ");
            else
                printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    int N, M, T, modo_passo;
    scanf("%d %d %d", &N, &M, &T);

    char mapa[100][100];
    Ponto robo;
    int sujeira_total = 0;

    for (int i = 0; i < N; i++)
    {                         // percorre linha
        scanf("%s", mapa[i]); // lê a linha que ta
        printf("Linha %d: %s\n", i, mapa[i]);
        for (int j = 0; j < M; j++)
        { // percorre a coluna
            if (mapa[i][j] == 'S')
            {               // se é o robo
                robo.r = i; // posição do robo
                robo.c = j;
                mapa[i][j] = '.'; // troca o S por um . (ta limpo)
            }
            if (mapa[i][j] == '*') // se é sujeira
                sujeira_total++;   // conta a sujeira
        }
    }
    int sujeira_inicial = sujeira_total; 
    Log historico;
    log_init(&historico, 64);

    printf("Sujeira encontrada: %d\n", sujeira_total);
    printf("Posicao do robo: %d %d\n", robo.r, robo.c);

    printf("Deseja modo passo-a-passo? (1=sim, 0=nao): ");
    scanf("%d", &modo_passo);
    getchar();

    int passos = 0;    // contador de passos
    int limpezas = 0;  // contador de limpezas
    int bloqueios = 0; // contador de bloqueios

    clock_t inicio = clock();

    while (passos < T && sujeira_total > 0)
    {
        if (modo_passo)
        {
            printf("\n--- Passo %d ---\n", passos);
            imprimir_mapa(mapa, N, M, robo);
        }

        Acao a = decide_reflex(mapa, N, M, robo, &bloqueios);
        log_push(&historico, a);

        if (a == LIMPAR)
        {
            mapa[robo.r][robo.c] = '.';
            sujeira_total--;
            limpezas++;

            if (modo_passo)
                printf("Acao: LIMPAR\n");
        }
        else
        {
            int nr = robo.r;
            int nc = robo.c;

            if (a == MOVER_N)
            {
                nr--;
                if (modo_passo)
                    printf("Acao: MOVER NORTE\n");
            }

            if (a == MOVER_S)
            {
                nr++;
                if (modo_passo)
                    printf("Acao: MOVER SUL\n");
            }

            if (a == MOVER_L)
            {
                nc++;
                if (modo_passo)
                    printf("Acao: MOVER LESTE\n");
            }

            if (a == MOVER_O)
            {
                nc--;
                if (modo_passo)
                    printf("Acao: MOVER OESTE\n");
            }

            if (nr < 0 || nr >= N || nc < 0 || nc >= M || mapa[nr][nc] == '#')
            {
                bloqueios++;

                if (modo_passo)
                    printf("Movimento bloqueado!\n");
            }
            else
            {
                robo.r = nr;
                robo.c = nc;
            }
        }

        passos++;

        if (modo_passo)
        {
            printf("Robo agora em (%d,%d)\n", robo.r, robo.c);
            // printf("Pressione ENTER para continuar...");
            getchar();
        }
    }

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    double percentual = ((double)limpezas / sujeira_inicial) * 100;

    printf("RESULTADO:\n");
    printf("passos: %d\n", passos);
    printf("limpezas: %d\n", limpezas);
    printf("bloqueios: %d\n", bloqueios);
    printf("sujeira_total: %d\n", sujeira_total);
    printf("Percentual removido: %.2f%%\n", percentual);
    printf("tempo: %f seg\n", tempo);

    FREE(historico.v);
    return 0;
}
