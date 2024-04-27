/********************************************************************
  Bot-exemplo

  Após receber as informações iniciais do jogo, a cada rodada esse
  bot irá se movimentar para esquerda.
  Cabe a você agora aprimorar sua estratégia!!!
 ********************************************************************/
#include <stdio.h>
#include <string.h>

#define MAX_STR 50

// Definição de constantes para os tipos de peixes
enum fishes {
    TAINHA = 1,
    CIOBA = 2,
    ROBALO = 3,
};

enum {
    TRUE,
    FALSE,
};

enum action {
    MOVE_UP = 0,
    MOVE_RIGHT = 1,
    MOVE_DOWN = 2,
    MOVE_LEFT = 3,
    SELL = 4,
};

// Estado do bot
// e.g.:
// indo para o porto
// No porto
// indo para ponto de pesca
// pescando
enum bot_state {
    GOING_TO_PORT,
    IN_PORT,
    SELLING,
    GOING_FISHING,
    FISHING,
};

struct bot {
    int x, y;       // posição do bot: x linha, y coluna
    int fish;       // quantidade de peixes: cada peixe pesa 1kg
    int capacity;   // capacidade máxima de peixes: 10kg
    int bot_state;  // O que o bot ta fazendo no momento
};

char *directions[] = {"UP", "RIGHT", "DOWN", "LEFT"};

/* ADAPTAR EM FUNÇÃO DE COMO OS DADOS SERÃO ARMAZENADOS NO SEU BOT */
void read_data(int h, int w, int board[h][w]) {
    char id[MAX_STR];
    int v, n, x, y;

    // lê os dados da área de pesca
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            scanf("%i", &board[i][j]);
        }
    }
    // lê os dados dos bots
    scanf(" BOTS %i", &n);
    // o " " antes de BOTS é necessário para ler o '\n' da linha anterior
    for (int i = 0; i < n; i++) {
        scanf("%s %i %i", id, &x, &y);
    }
}

// função para ver se o navio está cheio
int ship_full(struct bot *b) { return b->fish >= b->capacity; }

// Função para ver se vai levar multa, ponto de pesca sem peixe
// Sair do ponto de pesca, quando tiver 1 sobrando
int imminent_penalty(struct bot *ship, int h, int w, int board[h][w]) {
    return (board[ship->x][ship->y] - 1) % 10 == 0;
}

int at_least_2_fishes(int desired_row, int desired_col, int h, int w,
                      int board[h][w]) {
    return board[desired_row][desired_col] % 10 >= 2;
}

int at_fishing_point(struct bot *ship, int h, int w, int board[h][w]) {
    return board[ship->x][ship->y] > 1;
}

// Vou precisar de funções para calcular o ponto de pesca mais próximo em
// relação ao bot

// Gerenciador do bot
void bot_brain(struct bot *ship, int h, int w, int board[h][w]) {
    // Se estiver cheio,
    //   Se estiver no porto, descarregar navio
    //   Caso contrário, ir para o porto
    // Caso não esteja cheio,
    //  Se estiver em ponto de pesca, pescar até penultimo peixe.
    //  Caso contrário, ir para o ponto de pesca mais próximo
    if (ship_full(ship)) {
        if (ship->bot_state == IN_PORT) {
            ship->bot_state = SELLING;
        } else {
            ship->bot_state = GOING_TO_PORT;
        }
    } else {
        if (at_fishing_point(ship, h, w, board)) {
            if (imminent_penalty(ship, h, w, board)) {
                ship->bot_state = GOING_FISHING;
            } else {
                ship->bot_state = FISHING;
            }
        } else {
            ship->bot_state = GOING_FISHING;
        }
    }
}

// Critério de decisão
enum action decide(struct bot *ship, int h, int w, int board[h][w]) {
    if (ship->bot_state == SELLING) {
        return SELL;
    }

    if (ship->bot_state == GOING_TO_PORT || ship->bot_state == GOING_FISHING) {
        // Calcula o próximo movimento para ir para o porto
        // enum action move = calculate_next_move(ship, h, w, board);
        // return move
    } else if () {
    
    }

    return MOVE_LEFT;  // stub
}

int main() {
    char line[MAX_STR];  // dados temporários
    char myId[MAX_STR];  // identificador do bot em questão

    setbuf(stdin, NULL);   // stdin, stdout e stderr não terão buffers
    setbuf(stdout, NULL);  // assim, nada é "guardado temporariamente"
    setbuf(stderr, NULL);

    // === INÍCIO DA PARTIDA ===
    int h, w;
    scanf("AREA %i %i", &h,
          &w);  // lê a dimensão da área de pesca: altura (h) x largura (w)
    scanf(" ID %s", myId);  // ...e o id do bot
    // obs: o " " antes de ID é necessário para ler o '\n' da linha anterior

    int board[h][w];
    // Para "debugar", é possível enviar dados para a saída de erro padrão
    // (stderr). Esse dado não será enviado para o simulador, apenas para o
    // terminal. A linha seguinte é um exemplo. Pode removê-la se desejar.
    fprintf(stderr, "Meu id = %s\n", myId);

    // === PARTIDA ===
    // O bot entra num laço infinito, mas não se preocupe porque o simulador irá
    // matar o processo quando o jogo terminar.
    while (1) {
        // LÊ OS DADOS DO JOGO E ATUALIZA OS DADOS DO BOT
        read_data(h, w, board);

        // INSIRA UMA LÓGICA PARA ESCOLHER UMA AÇÃO A SER EXECUTADA

        // envia a ação escolhida (nesse exemplo, ir para esquerda)
        printf("LEFT\n");

        // lê qual foi o resultado da ação (e eventualmente atualiza os dados do
        // bot).
        scanf("%s", line);
    }

    return 0;
}
