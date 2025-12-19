#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* =====================================================
   STRUCT DA PEÇA
   ===================================================== */
typedef struct {
    char tipo;   // I, O, T, L
    int id;      // identificador sequencial
} Peca;

/* =====================================================
   FILA CIRCULAR (5 PEÇAS FUTURAS)
   ===================================================== */
#define MAX 5

typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

/* ---------- Funções da Fila ---------- */

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX;
    f->total++;
}

Peca dequeue(Fila *f) {
    Peca r;

    if (filaVazia(f)) {
        r.tipo = 'X';
        r.id = -1;
        return r;
    }

    r = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
    return r;
}

/* Insere peça na FRENTE da fila (usado na inversão) */
void inserirNaFrente(Fila *f, Peca p) {
    f->inicio = (f->inicio - 1 + MAX) % MAX;
    f->itens[f->inicio] = p;
    f->total++;
}

void mostrarFila(Fila *f) {
    printf("Fila: ");
    for (int i = 0, idx = f->inicio; i < f->total; i++) {
        printf("[%c,%d] ", f->itens[idx].tipo, f->itens[idx].id);
        idx = (idx + 1) % MAX;
    }
    printf("\n");
}

/* =====================================================
   PILHA DE RESERVA (3 PEÇAS)
   ===================================================== */
#define MAX_ALT 3

typedef struct {
    Peca itens[MAX_ALT];
    int topo;
} Pilha;

/* ---------- Funções da Pilha ---------- */

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_ALT - 1;
}

void push(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->itens[p->topo] = nova;
}

Peca pop(Pilha *p) {
    Peca r;

    if (pilhaVazia(p)) {
        r.tipo = 'X';
        r.id = -1;
        return r;
    }

    r = p->itens[p->topo];
    p->topo--;
    return r;
}

void mostrarPilha(Pilha *p) {
    printf("Pilha (topo->base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c,%d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n");
}

/* =====================================================
   GERADOR DE PEÇAS
   ===================================================== */
Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.tipo = tipos[rand() % 4];
    p.id = id;
    return p;
}

/* =====================================================
   BACKUP PARA DESFAZER
   ===================================================== */
typedef struct {
    Fila fila;
    Pilha pilha;
} EstadoJogo;

/* =====================================================
   AÇÕES DO JOGO
   ===================================================== */

void jogarPeca(Fila *f, int *idSeq) {
    Peca r = dequeue(f);
    printf("Jogou: [%c,%d]\n", r.tipo, r.id);
    enqueue(f, gerarPeca((*idSeq)++));
}

void reservarPeca(Fila *f, Pilha *p, int *idSeq) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia!\n");
        return;
    }

    Peca r = dequeue(f);
    printf("Reservou: [%c,%d]\n", r.tipo, r.id);
    push(p, r);
    enqueue(f, gerarPeca((*idSeq)++));
}

void usarPecaReservada(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia!\n");
        return;
    }

    Peca r = pop(p);
    printf("Usou da reserva: [%c,%d]\n", r.tipo, r.id);
}

/* TROCA TOPO DA PILHA COM FRENTE DA FILA */
void trocarTopoComFrente(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("Troca nao permitida.\n");
        return;
    }

    Peca temp = f->itens[f->inicio];
    f->itens[f->inicio] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("Troca realizada.\n");
}

/* DESFAZER AÇÃO */
void desfazer(EstadoJogo *ant, Fila *f, Pilha *p, int *temBackup) {
    if (!(*temBackup)) {
        printf("Nada para desfazer.\n");
        return;
    }

    *f = ant->fila;
    *p = ant->pilha;
    *temBackup = 0;

    printf("Ultima acao desfeita.\n");
}

/* INVERTER FILA COM PILHA */
void inverterFilaComPilha(Fila *f, Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Pilha vazia.\n");
        return;
    }

    while (!pilhaVazia(p)) {
        inserirNaFrente(f, pop(p));
    }

    printf("Fila invertida com pilha.\n");
}

/* =====================================================
   MAIN
   ===================================================== */
int main() {
    Fila fila;
    Pilha pilha;
    EstadoJogo backup;
    int temBackup = 0;

    int opcao;
    int idSeq = 1;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Inicializa fila com 5 peças */
    for (int i = 0; i < MAX; i++)
        enqueue(&fila, gerarPeca(idSeq++));

    do {
        // system("clean");
        mostrarFila(&fila);
        mostrarPilha(&pilha);

        printf("\n--- MENU ---\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("4 - Trocar topo da pilha com frente da fila\n");
        printf("5 - Desfazer ultima jogada\n");
        printf("6 - Inverter fila com pilha\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        /* Salva backup antes das ações críticas */
        if (opcao >= 1 && opcao <= 6 && opcao != 5) {
            backup.fila = fila;
            backup.pilha = pilha;
            temBackup = 1;
        }

        switch (opcao) {
            case 1: jogarPeca(&fila, &idSeq); break;
            case 2: reservarPeca(&fila, &pilha, &idSeq); break;
            case 3: usarPecaReservada(&pilha); break;
            case 4: trocarTopoComFrente(&fila, &pilha); break;
            case 5: desfazer(&backup, &fila, &pilha, &temBackup); break;
            case 6: inverterFilaComPilha(&fila, &pilha); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
