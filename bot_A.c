/********************************************************************
  Bot-exemplo

  Após receber as informações iniciais do jogo, a cada rodada esse
  bot irá se movimentar para esquerda.
  Cabe a você agora aprimorar sua estratégia!!!
 ********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR     50
#define PORT        1
#define DEFAULT_POS -1

typedef enum {
    TRUE = 1,
    FALSE = 0,
} MyBool;  // My bool

// Definição de constantes para os tipos de peixes
typedef enum {
    TAINHA = 1,
    CIOBA = 2,
    ROBALO = 3,
} fishes;

typedef enum {
    MOVE_UP = 0,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT,
    SELL,
    FISH,
    NOTHING,
} Action;

typedef enum {
    GOING_TO_PORT = 0,
    SELLING,
    GOING_FISHING,
    FISHING,
} BotState;

typedef struct {
    int x, y;  // Coordenada cartesiana
} Coordinate;

typedef struct {
    char id[MAX_STR];       // Identificador do bot
    Coordinate pos;         // Posição atual do bot
    Coordinate rival_pos;   // Posição do bot rival
    int fish;               // quantidade de peixes: cada peixe pesa 1kg
    const int capacity;     // capacidade máxima de peixes: 10kg
    BotState state;         // O que o bot ta fazendo no momento
    Coordinate locked_pos;  // Posição que o bot quer ir
} Bot;

// TODO: Futuramente armazenar os portos e as posições de pesca dinâmicamente
// TODO: Levar em consideração a posição do bot rival na tomada de decisão
// TODO: Se o bot chegar na posição que lockou, ele deve desbloquear a posição
//       Caso tenha outro bot na posição, ele deve tentar outra posição

void read_data(Bot* this_bot, const int heigth, const int width, int board[heigth][width]) {
    char identifier[MAX_STR];  // e.g. bot_A
    int num_of_bots, bot_pos_x, bot_pos_y;
    // lê os dados da área de pesca
    for (int i = 0; i < heigth; i++) {
        for (int j = 0; j < width; j++) {
            scanf("%i", &board[i][j]);
        }
    }
    // lê os dados dos bots
    scanf(" BOTS %i", &num_of_bots);
    // o " " antes de BOTS é necessário para ler o '\n' da linha anterior
    for (int i = 0; i < num_of_bots; i++) {
        scanf("%s %i %i", identifier, &bot_pos_x, &bot_pos_y);
        // Atualiza a posição do bot
        if (strcmp(identifier, this_bot->id) == 0) {
            this_bot->pos.x = bot_pos_x;
            this_bot->pos.y = bot_pos_y;
        } else {
            this_bot->rival_pos.x = bot_pos_x;
            this_bot->rival_pos.y = bot_pos_y;
        }
    }
}

// função para ver se o navio está cheio
// Se estiver, ele deve ir para o porto
const MyBool ship_full(const Bot* ship) { return ship->fish == ship->capacity; }

// Função para ver se vai levar multa, ponto de pesca sem peixe
// Sair do ponto de pesca, quando tiver 1 sobrando
const MyBool imminent_penalty(const Bot* ship,
                              const int heigth,
                              const int width,
                              const int board[heigth][width]) {
    fprintf(stderr, "Amount of fishes: %i\n", board[ship->pos.x][ship->pos.y] % 10);
    return ((board[ship->pos.x][ship->pos.y] - 1) % 10) < 1;
}

// Usada para verificar se num possível futuro ponto de pesca, haverá pelo menos 2 peixes
// Se não houver, a condição não vai ser atendida no find_nearest_location
const MyBool at_fishing_point(const Bot* ship,
                              const int height,
                              const int width,
                              const int board[height][width]) {
    return board[ship->pos.x][ship->pos.y] > 1;
}

// Distância de manhattan
// Usado para calcular posição de um porto ou ponto de pesca em relação ao bot
const int calculate_distance(const Coordinate a, const Coordinate b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Posição cartesiana de algo, mais próximo do bot
const Coordinate find_nearest_location(
  const Bot* ship,
  const int height,
  const int width,
  const int board[height][width],
  MyBool (*condition)(int, int, int, int, const int[height][width])) {
    Coordinate location = { 0, 0 };
    int min_distance = height + width;
    for (int line = 0; line < height; line++) {
        for (int column = 0; column < width; column++) {
            if (condition(line, column, height, width, board)) {
                int distance = calculate_distance(ship->pos, (Coordinate){ line, column });
                // Can't be the actual position of the bot
                if (distance != 0 && distance < min_distance) {
                    min_distance = distance;
                    location.x = line;
                    location.y = column;
                }
            }
        }
    }
    return location;
}

MyBool is_port(const int line,
               const int column,
               const int height,
               const int width,
               const int board[height][width]) {
    return board[line][column] == PORT;
}

MyBool has_at_least_2_fishes(const int desired_row,
                             const int desired_col,
                             const int height,
                             const int width,
                             const int board[height][width]) {
    return board[desired_row][desired_col] % 10 >= 2;
}

Coordinate find_nearest_port(Bot* ship,
                             const int height,
                             const int width,
                             const int board[height][width]) {
    return find_nearest_location(ship, height, width, board, is_port);
}

Coordinate find_nearest_fishing_point(Bot* ship,
                                      const int height,
                                      const int width,
                                      const int board[height][width]) {
    return find_nearest_location(ship, height, width, board, has_at_least_2_fishes);
}

Action calculate_next_x_move(const Bot* ship, const Coordinate some_point) {
    return (some_point.x < ship->pos.x)   ? MOVE_UP
           : (some_point.x > ship->pos.x) ? MOVE_DOWN
                                          : NOTHING;
}

Action calculate_next_y_move(const Bot* ship, const Coordinate some_point) {
    return (some_point.y < ship->pos.y)   ? MOVE_LEFT
           : (some_point.y > ship->pos.y) ? MOVE_RIGHT
                                          : NOTHING;
}

const Action calculate_move(const Bot* ship,
                            const int height,
                            const int width,
                            const int board[height][width]) {
    // Dado um ponto,
    // O bot vai decidir primeiro se consegue mover no eixo x
    // Se ele já estiver na mesma linha da posição travada, ele vai se mover no eixo y
    Action move = calculate_next_x_move(ship, ship->locked_pos);
    if (move == NOTHING) {
        move = calculate_next_y_move(ship, ship->locked_pos);
    }
    return move;
}

const MyBool bot_locked_a_coord(const Bot* ship) {
    return ship->locked_pos.x == DEFAULT_POS && ship->locked_pos.y == DEFAULT_POS;
}

const Action calculate_next_move(Bot* ship,
                                 const int height,
                                 const int width,
                                 const int board[height][width]) {
    // Se entrou aqui, é porque o bot ja decidiu se vai pescar ou se vai pra porto
    // Então, ele precisa calcular qual o próximo movimento que ele precisa fazer para chegar no
    // porto mais próximo ou no ponto de pesca mais próximo com ao menos 2 peixes
    // Existe também a possibilidade de ir para uma coordenada ocupada por outro bot (caso seja um
    // ponto de pesca ou agua)
    // TODO: Levar em consideração a posição do bot rival
    Action move = NOTHING;
    if (bot_locked_a_coord(ship)) {
        if (ship->state == GOING_TO_PORT) {
            fprintf(stderr, "Gonna find the nearest port\n");
            ship->locked_pos = find_nearest_port(ship, height, width, board);
            fprintf(stderr, "Finded\n");
        } else if (ship->state == GOING_FISHING) {
            fprintf(stderr, "Gonna find the nearest fishing_point\n");
            ship->locked_pos = find_nearest_fishing_point(ship, height, width, board);
            fprintf(stderr, "Finded\n");
        }
    }
    if (ship->state == GOING_TO_PORT) {
        move = calculate_move(ship, height, width, board);
    } else if (ship->state == GOING_FISHING) {
        move = calculate_move(ship, height, width, board);
    }
    return move;
}

// Dependendo do estado do bot, ele vai realizar uma ação correspondente
const Action decide(Bot* ship, const int height, const int width, const int board[height][width]) {
    if (ship->state == SELLING) {
        fprintf(stderr, "Bro is cooking\n");
        ship->fish = 0;
        return SELL;
    }
    if (ship->state == FISHING) {
        fprintf(stderr, "Bro is fishing\n");
        ship->fish++;
        return FISH;
    }
    return calculate_next_move(ship, height, width, board);
}

// Ação do bot para string
const char* actions[] = { "UP", "RIGHT", "DOWN", "LEFT", "SELL", "FISH" };

const char* action_to_string(const Action a) { return actions[a]; }

// Gerenciador do bot
void bot_brain(Bot* ship, const int height, const int width, const int board[height][width]) {
    // Se estiver cheio,
    //   Se estiver no porto, descarregar navio
    //   Caso contrário, ir para o porto
    // Caso não esteja cheio,
    //  Se estiver em ponto de pesca, pescar até penultimo peixe.
    //  Caso contrário, ir para o ponto de pesca mais próximo
    if (ship_full(ship)) {
        if (is_port(ship->pos.x, ship->pos.y, height, width, board)) {
            ship->state = SELLING;
            ship->locked_pos = (Coordinate){ DEFAULT_POS, DEFAULT_POS };
            fprintf(stderr, "Bro in port and reset'd the locked coordinate\n");
        } else {
            fprintf(stderr, "Bro is not in port and he wants the grand\n");
            ship->state = GOING_TO_PORT;
        }
    } else {
        if (at_fishing_point(ship, height, width, board)) {
            fprintf(stderr, "Bro is at fishing point: ");
            if (imminent_penalty(ship, height, width, board)) {
                fprintf(stderr, "Bro gonna get a penalty, will fish in another place\n");
                ship->state = GOING_FISHING;
            } else {
                fprintf(stderr, "That shit ain't finishing any time soon\n");
                ship->state = FISHING;
                ship->locked_pos
                  = (Coordinate){ DEFAULT_POS, DEFAULT_POS };  // TODO: Reset in a better place
                fprintf(stderr, " Reset'd the locked coordinate\n");
            }
        } else {
            fprintf(stderr, "Bro is not in a fishing point and he wants to cook!\n");
            ship->state = GOING_FISHING;
        }
    }
    const Action action = decide(ship, height, width, board);
    printf("%s\n", action_to_string(action));
    fprintf(stderr, "Bro Action: %s\n", action_to_string(action));
}

int main() {
    char line[MAX_STR];  // dados temporários
    char myId[MAX_STR];  // identificador do bot em questão

    setbuf(stdin, NULL);   // stdin, stdout e stderr não terão buffers
    setbuf(stdout, NULL);  // assim, nada é "guardado temporariamente"
    setbuf(stderr, NULL);

    // === INÍCIO DA PARTIDA ===
    int height, width;
    scanf(
      "AREA %i %i", &height, &width);  // lê a dimensão da área de pesca: altura (h) x largura (w)
    scanf(" ID %s", myId);             // ...e o id do bot
    // obs: o " " antes de ID é necessário para ler o '\n' da linha anterior

    int board[height][width];
    // Para "debugar", é possível enviar dados para a saída de erro padrão
    // (stderr). Esse dado não será enviado para o simulador, apenas para o
    // terminal. A linha seguinte é um exemplo. Pode removê-la se desejar.
    fprintf(stderr, "Meu id = %s\n", myId);

    // === INICIALIZAÇÃO DO BOT ===
    // Inicializa o bot sem peixes e com capacidade de 10kg e com sede de pesca
    Bot ship = { .fish = 0, .capacity = 10, .state = GOING_FISHING, .locked_pos = { 0, 0 } };
    strcpy(ship.id, myId);

    // === PARTIDA ===
    // O bot entra num laço infinito, mas não se preocupe porque o simulador irá
    // matar o processo quando o jogo terminar.
    while (1) {
        // LÊ OS DADOS DO JOGO E ATUALIZA OS DADOS DO BOT
        read_data(&ship, height, width, board);

        // INSIRA UMA LÓGICA PARA ESCOLHER UMA AÇÃO A SER EXECUTADA
        bot_brain(&ship, height, width, board);

        fprintf(stderr, "Bro statistics:\n");
        fprintf(stderr, "  Position: %i %i\n", ship.pos.x, ship.pos.y);
        fprintf(stderr, "  Fish: %i\n", ship.fish);
        fprintf(stderr, "  Capacity: %i\n", ship.capacity);
        fprintf(stderr, "  State: %i\n", ship.state);
        fprintf(stderr, "  Locked Position: %i %i\n", ship.locked_pos.x, ship.locked_pos.y);
        // lê qual foi o resultado da ação (e eventualmente atualiza os dados do
        // bot).
        scanf("%s", line);  // :P depois lido com isso, feita a ação ele ja atualiza o status XD
        fprintf(stderr, "Sim response: %s\n", line);
    }

    return 0;
}
