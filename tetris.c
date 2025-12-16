#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// - Crie uma struct Peca com os campos: tipo (char) e id (int).
typedef struct {
    char tipo; // I, O, T, L
    int id;
} Peca;

// - Implemente uma fila circular com capacidade para 5 peças.
#define MAX 5

typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

// - Crie funções como inicializarFila(), enqueue(), dequeue(), filaCheia(), filaVazia().
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia!\n");
        return;
    }

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}

Peca dequeue(Fila *f) {
    Peca removida;

    if (filaVazia(f)) {
        printf("Fila vazia!\n");
        removida.tipo = 'X';
        removida.id = -1;
        return removida;
    }

    removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;

    return removida;
}

// - Exiba a fila após cada ação com uma função mostrarFila().
void mostrarFila(Fila *f) {
    printf("\nFila atual: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++) {
        printf("[%c, %d] ", f->itens[idx].tipo, f->itens[idx].id);
        idx = (idx + 1) % MAX;
    }
    printf("\n");
}


   // - Cada peça deve ser gerada automaticamente com um tipo aleatório e id sequencial.
Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.tipo = tipos[rand() % 4];
    p.id = id;
    return p;
}

// - A cada remoção, insira uma nova peça ao final da fila.
void jogarPeca(Fila *f, int *idSeq) {
    Peca removida = dequeue(f);
    printf("Peça jogada: [%c, %d]\n", removida.tipo, removida.id);

    Peca nova = gerarPeca(*idSeq);
    (*idSeq)++;

    enqueue(f, nova);
    mostrarFila(f);
}


int main() {

    // - Use um menu com opções como:
    //      1 - Jogar peça (remover da frente)
    //      0 - Sair

    Fila f;
    int opcao;
    int idSeq = 1;

    srand(time(NULL));
    inicializarFila(&f);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX; i++) {
        enqueue(&f, gerarPeca(idSeq));
        idSeq++;
    }

    mostrarFila(&f);

    do {
        printf("\n--- MENU ---\n");
        printf("1 - Jogar peça\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&f, &idSeq);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

        // Depois realizarei o nível aventureiro;

        
    // 🧠 Nível Aventureiro: Adição da Pilha de Reserva
    //
    // - Implemente uma pilha linear com capacidade para 3 peças.
    // - Crie funções como inicializarPilha(), push(), pop(), pilhaCheia(), pilhaVazia().
    // - Permita enviar uma peça da fila para a pilha (reserva).
    // - Crie um menu com opção:
    //      2 - Enviar peça da fila para a reserva (pilha)
    //      3 - Usar peça da reserva (remover do topo da pilha)
    // - Exiba a pilha junto com a fila após cada ação com mostrarPilha().
    // - Mantenha a fila sempre com 5 peças (repondo com gerarPeca()).


    // 🔄 Nível Mestre: Integração Estratégica entre Fila e Pilha
    //
    // - Implemente interações avançadas entre as estruturas:
    //      4 - Trocar a peça da frente da fila com o topo da pilha
    //      5 - Trocar os 3 primeiros da fila com as 3 peças da pilha
    // - Para a opção 4:
    //      Verifique se a fila não está vazia e a pilha tem ao menos 1 peça.
    //      Troque os elementos diretamente nos arrays.
    // - Para a opção 5:
    //      Verifique se a pilha tem exatamente 3 peças e a fila ao menos 3.
    //      Use a lógica de índice circular para acessar os primeiros da fila.
    // - Sempre valide as condições antes da troca e informe mensagens claras ao usuário.
    // - Use funções auxiliares, se quiser, para modularizar a lógica de troca.
    // - O menu deve ficar assim:
    //      4 - Trocar peça da frente com topo da pilha
    //      5 - Trocar 3 primeiros da fila com os 3 da pilha


    return 0;
}

