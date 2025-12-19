#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* =====================================================
   STRUCT DA PEÇA
   Cada peça possui:
   - tipo: I, O, T ou L
   - id: identificador sequencial
   ===================================================== */
typedef struct {
    char tipo;
    int id;
} Peca;

/* =====================================================
   FILA CIRCULAR (PEÇAS FUTURAS)
   Capacidade fixa de 5 peças
   ===================================================== */
#define MAX 5

typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

/* ---------- Funções da Fila ---------- */

// Inicializa a fila vazia
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->total == MAX;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->total == 0;
}

// Insere uma peça no final da fila
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia!\n");
        return;
    }

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}

// Remove a peça da frente da fila e retorna
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

// Mostra o estado atual da fila
void mostrarFila(Fila *f) {
    printf("\nFila: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++) {
        printf("[%c, %d] ", f->itens[idx].tipo, f->itens[idx].id);
        idx = (idx + 1) % MAX;
    }
    printf("\n");
}

/* =====================================================
   PILHA DE RESERVA
   Capacidade máxima de 3 peças
   ===================================================== */
#define MAX_ALT 3

typedef struct {
    Peca itens[MAX_ALT];
    int topo;
} Pilha;

/* ---------- Funções da Pilha ---------- */

// Inicializa a pilha vazia
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == MAX_ALT - 1;
}

// Empilha uma peça
void push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia! Nao e possivel reservar.\n");
        return;
    }

    p->topo++;
    p->itens[p->topo] = nova;
}

// Desempilha uma peça e retorna
Peca pop(Pilha *p) {
    Peca removida;

    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        removida.tipo = 'X';
        removida.id = -1;
        return removida;
    }

    removida = p->itens[p->topo];
    p->topo--;

    return removida;
}

// Mostra o estado atual da pilha
void mostrarPilha(Pilha *p) {
    printf("Pilha (topo -> base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c, %d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n");
}

/* =====================================================
   GERADOR DE PEÇAS
   Gera tipo aleatório e id sequencial
   ===================================================== */
Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.tipo = tipos[rand() % 4];
    p.id = id;
    return p;
}

/* =====================================================
   AÇÕES DO JOGO
   ===================================================== */

// Jogar peça: remove da fila e insere outra automaticamente
void jogarPeca(Fila *f, int *idSeq) {
    Peca removida = dequeue(f);
    printf("Peca jogada: [%c, %d]\n", removida.tipo, removida.id);

    enqueue(f, gerarPeca(*idSeq));
    (*idSeq)++;

    mostrarFila(f);
}

// Reservar peça: fila -> pilha
void reservarPeca(Fila *f, Pilha *p, int *idSeq) {
    if (pilhaCheia(p)) {
        printf("Nao e possivel reservar. Pilha cheia.\n");
        return;
    }

    Peca removida = dequeue(f);
    printf("Peca reservada: [%c, %d]\n", removida.tipo, removida.id);

    push(p, removida);

    enqueue(f, gerarPeca(*idSeq));
    (*idSeq)++;

    mostrarFila(f);
    mostrarPilha(p);
}

// Usar peça reservada: remove do topo da pilha
void usarPecaReservada(Pilha *p, Fila *f) {
    if (pilhaVazia(p)) {
        printf("Nenhuma peca reservada.\n");
        return;
    }

    Peca usada = pop(p);
    printf("Peca usada da reserva: [%c, %d]\n", usada.tipo, usada.id);

    mostrarFila(f);
    mostrarPilha(p);
}

/* =====================================================
   FUNÇÃO PRINCIPAL
   ===================================================== */
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int idSeq = 1;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX; i++) {
        enqueue(&fila, gerarPeca(idSeq));
        idSeq++;
    }

    mostrarFila(&fila);
    mostrarPilha(&pilha);

    do {
        printf("\n--- MENU ---\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        system("clear");
        
        switch (opcao) {
            case 1:
                jogarPeca(&fila, &idSeq);
                mostrarPilha(&pilha);
                break;

            case 2:
                reservarPeca(&fila, &pilha, &idSeq);
                break;

            case 3:
                usarPecaReservada(&pilha, &fila);
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
