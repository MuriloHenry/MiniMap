/************************************************************
 *                                                          *        
 *  Mini-Game: MAP                                          *
 *  Modo historia:                                          *
 *  Mapas predefinidos (mapa1.txt, mapa2.txt, ...)          *
 *  Modo hardcore:                                          *
 *  Tudo random (gerado automaticamente)                    *
 *                                                          *
 ************************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// ==============================
// Carrega mapa do modo história
// ==============================
int carregarMapa(const char *arquivo, int size, int matriz[size][size], int *player_y, int *player_x) {
    FILE *f = fopen(arquivo, "r");
    if (!f) {
        return 0; // erro ao abrir
    }

    char c;
    int y = 0, x = 0;
    while ((c = fgetc(f)) != EOF && y < size) {
        if (c == '\n') {
            y++;
            x = 0;
            continue;
        }

        if (x >= size) continue;

        switch (c) {
            case 'P': matriz[y][x] = 0; *player_y = y; *player_x = x; break;
            case 'X': matriz[y][x] = 1; break;
            case '.': matriz[y][x] = 2; break;
            case 'O': matriz[y][x] = 3; break;
            default: matriz[y][x] = 2; break;
        }
        x++;
    }

    fclose(f);
    return 1;
}

void setCampo(int size, int matriz[size][size], int level, int dif){
    int y, x;
    srand(time(0));
    
    for(y = 0; y < size; y++){
        for(x = 0; x < size; x++){
            matriz[y][x] = (rand() % 100 > (90 - (level * dif))) ? 1 : 2;
        }
    }

    matriz[size/2][size/2] = 0; // jogador
    do{
        x = rand() % size;
        y = rand() % size;
    }while((x == size/2) && (y == size/2));
    
    matriz[y][x] = 3;
}

void campo(int size, int matriz[size][size]){
    for(int y = 0; y < size; y++){
        for(int x = 0; x < size; x++){
            if(matriz[y][x] == 0){
                printf("P ");
            } else if(matriz[y][x] == 1){
                printf("X ");
            } else if(matriz[y][x] == 2){
                printf(". ");
            } else if(matriz[y][x] == 3){
                printf("O ");
            }
        }
        printf("\n");
    }
}

int main()
{
    int modo = 0, dif = 0;
    do{
        system("clear");
        printf("Mini-Game\n\n");
        printf("Escolha o modo que deseja!\n");
        printf("\t1. Historia\n\t2. Hardcore\n");
        printf("Digite a opcao: ");
        scanf("%d", &modo);
        fflush(stdin);
    }while(modo != 1 && modo != 2);

    // ==============================
    // MODO HISTÓRIA LINEAR
    // ==============================
    if (modo == 1) {
        int size = 7, vida = 1;
        int matriz[size][size];
        int player_y = 0, player_x = 0;
        int level = 1;
        char nomeMapa[64];

        while (vida == 1) {
            sprintf(nomeMapa, "mapa%d.txt", level);

            if (!carregarMapa(nomeMapa, size, matriz, &player_y, &player_x)) {
                system("clear");
                printf("\nFim da história!\nVocê completou todos os mapas disponíveis.\n\n");
                break;
            }

            while (vida == 1) {
                system("clear");
                printf("== MODO HISTÓRIA ==\nLevel %d\n\n", level);
                campo(size, matriz);

                while (!kbhit()) {
                    usleep(330000);
                }

                char c = getchar();
                matriz[player_y][player_x] = 2; // limpa posição atual

                if ((c == 'w' || c == 'W') && player_y > 0)
                    player_y--;
                else if ((c == 's' || c == 'S') && player_y < size - 1)
                    player_y++;
                else if ((c == 'a' || c == 'A') && player_x > 0)
                    player_x--;
                else if ((c == 'd' || c == 'D') && player_x < size - 1)
                    player_x++;

                if (matriz[player_y][player_x] == 1) {
                    vida = 0;
                    system("clear");
                    printf("\nVocê perdeu no Level %d!\n", level);
                } else if (matriz[player_y][player_x] == 3) {
                    level++;
                    break;
                }

                matriz[player_y][player_x] = 0;
            }
        }

        return 0;
    }

    // ==============================
    // MODO HARDCORE (original)
    // ==============================
    if(modo == 2){
        do{
            system("clear");
            printf("Mini-Game\n\n");
            printf("Escolha a dificuldade que deseja!\n");
            printf("\t1. Facil\n\t2. Medio\n\t3. Dificil\n\t4. Impossivel\n");
            printf("Digite a opcao: ");
            scanf("%d", &dif);
            fflush(stdin);
        }while(dif < 1 || dif > 4);
    }

    int size = 7, level = 1, vida = 1;
    int matriz[size][size];
    int player_y = size / 2;
    int player_x = size / 2;
    
    setCampo(size, matriz, level, dif);
    
    while(vida == 1){
        system("clear");
        printf("Level: %d\n\n", level);
        campo(size, matriz);

        while (!kbhit()) {
            usleep(330000);
        }

        char c = getchar();

        matriz[player_y][player_x] = 2; // limpa posição atual

        if((c == 'w' || c == 'W') && player_y > 0){
            player_y--;
        } else if((c == 's' || c == 'S') && player_y < size - 1){
            player_y++;
        } else if((c == 'a' || c == 'A') && player_x > 0){
            player_x--;
        } else if((c == 'd' || c == 'D') && player_x < size - 1){
            player_x++;
        }
        
        if(matriz[player_y][player_x] == 1){
            vida = 0;
            system("clear");
            printf("\nVocê perdeu no Level %d!\n", level);
        } else if(matriz[player_y][player_x] == 3){
            level++;
            player_y = size / 2;
            player_x = size / 2;
            setCampo(size, matriz, level, dif);
        }
        matriz[player_y][player_x] = 0; // nova posição
    }

    return 0;
}
