// ============================================================================
//  PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5
#define TAM_NOME 50
#define NUM_MISSOES 3

// --- Estrutura de Dados ---
typedef struct {
    char nome[TAM_NOME];
    char corExercito[TAM_NOME];
    int tropas;
} Territorio;

// --- Protótipos de Funções ---
Territorio* alocarMapa(void);
void inicializarTerritorios(Territorio *mapa);
void exibirMapa(const Territorio *mapa);
void liberarMemoria(Territorio *mapa);

int sortearMissao(void);
void exibirMissao(int idMissao);
int verificarVitoria(const Territorio *mapa, int idMissao, const char *corJogador);

void faseDeAtaque(Territorio *mapa, const char *corJogador);
void simularAtaque(Territorio *origem, Territorio *destino);

void exibirMenuPrincipal(void);
void limparBufferEntrada(void);

// --- Função Principal ---
int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio *mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa!\n");
        return 1;
    }

    inicializarTerritorios(mapa);

    char corJogador[TAM_NOME];
    printf("Digite a cor do seu exército: ");
    fgets(corJogador, TAM_NOME, stdin);
    corJogador[strcspn(corJogador, "\n")] = '\0';

    int missao = sortearMissao();
    int opcao, venceu = 0;

    do {
        system("clear || cls");
        printf("===== MUNDO WAR =====\n");
        exibirMapa(mapa);
        exibirMissao(missao);
        exibirMenuPrincipal();

        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                venceu = verificarVitoria(mapa, missao, corJogador);
                if (venceu)
                    printf("\n🎉 Parabéns! Você cumpriu sua missão!\n");
                else
                    printf("\nA missão ainda não foi concluída.\n");
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

        if (!venceu && opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}

// --- Implementações das Funções ---

Territorio* alocarMapa(void) {
    return (Territorio*) calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

void inicializarTerritorios(Territorio *mapa) {
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);
        printf("Nome: ");
        fgets(mapa[i].nome, TAM_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(mapa[i].corExercito, TAM_NOME, stdin);
        mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
    }
}

void exibirMapa(const Territorio *mapa) {
    printf("\n===== ESTADO ATUAL DO MAPA =====\n");
    printf("%-20s | %-15s | %-10s\n", "Território", "Cor do Exército", "Tropas");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-20s | %-15s | %-10d\n",
               mapa[i].nome, mapa[i].corExercito, mapa[i].tropas);
    }
}

void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

int sortearMissao(void) {
    return rand() % NUM_MISSOES;
}

void exibirMissao(int idMissao) {
    printf("\n===== SUA MISSÃO =====\n");
    switch (idMissao) {
        case 0:
            printf("Conquistar 3 territórios inimigos.\n");
            break;
        case 1:
            printf("Eliminar todos os territórios de uma cor específica.\n");
            break;
        case 2:
            printf("Manter 5 territórios com pelo menos 3 tropas cada.\n");
            break;
    }
}

int verificarVitoria(const Territorio *mapa, int idMissao, const char *corJogador) {
    int count = 0;
    switch (idMissao) {
        case 0:
            for (int i = 0; i < NUM_TERRITORIOS; i++)
                if (strcmp(mapa[i].corExercito, corJogador) == 0)
                    count++;
            return (count >= 3);
        case 1:
            for (int i = 0; i < NUM_TERRITORIOS; i++)
                if (strcmp(mapa[i].corExercito, "vermelho") == 0)
                    return 0;
            return 1;
        case 2:
            for (int i = 0; i < NUM_TERRITORIOS; i++)
                if (mapa[i].tropas >= 3 && strcmp(mapa[i].corExercito, corJogador) == 0)
                    count++;
            return (count >= 5);
    }
    return 0;
}

void faseDeAtaque(Territorio *mapa, const char *corJogador) {
    char nomeOrigem[TAM_NOME], nomeDestino[TAM_NOME];
    printf("\nDigite o território de origem: ");
    fgets(nomeOrigem, TAM_NOME, stdin);
    nomeOrigem[strcspn(nomeOrigem, "\n")] = '\0';

    printf("Digite o território de destino: ");
    fgets(nomeDestino, TAM_NOME, stdin);
    nomeDestino[strcspn(nomeDestino, "\n")] = '\0';

    Territorio *origem = NULL, *destino = NULL;
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        if (strcmp(mapa[i].nome, nomeOrigem) == 0)
            origem = &mapa[i];
        if (strcmp(mapa[i].nome, nomeDestino) == 0)
            destino = &mapa[i];
    }

    if (origem == NULL || destino == NULL) {
        printf("Território inválido!\n");
        return;
    }

    if (strcmp(origem->corExercito, corJogador) != 0) {
        printf("Você só pode atacar de territórios que controla!\n");
        return;
    }

    simularAtaque(origem, destino);
}

void simularAtaque(Territorio *origem, Territorio *destino) {
    if (origem->tropas < 2) {
        printf("Tropas insuficientes para atacar!\n");
        return;
    }

    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;

    printf("\nAtaque: %d | Defesa: %d\n", dadoAtaque, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("Vitória no ataque! Um inimigo foi derrotado.\n");
        destino->tropas--;
        if (destino->tropas <= 0) {
            printf("Você conquistou o território!\n");
            strcpy(destino->corExercito, origem->corExercito);
            destino->tropas = 1;
            origem->tropas--;
        }
    } else {
        printf("Defesa bem-sucedida! Você perdeu uma tropa.\n");
        origem->tropas--;
    }
}

void exibirMenuPrincipal(void) {
    printf("\n===== MENU =====\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar missão\n");
    printf("0 - Sair\n");
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
