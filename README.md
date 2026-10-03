# MiniMap

Mini-game de terminal em C: mova o jogador por um mapa 7x7, desvie dos obstáculos e chegue ao objetivo.

## O que faz

O mapa é uma matriz 7x7 desenhada no terminal:

| Símbolo | Significado |
|---|---|
| `P` | jogador |
| `X` | obstáculo (encostou, perdeu) |
| `.` | caminho livre |
| `O` | objetivo (avança de level) |

Controles: `W` (cima), `A` (esquerda), `S` (baixo), `D` (direita). O jogo lê uma tecla por vez, sem precisar apertar Enter.

### Modos

- **História** — carrega os mapas fixos `mapa1.txt` a `mapa5.txt`, em ordem. Ao chegar no `O`, passa para o próximo arquivo; quando não há mais mapas, a história termina.
- **Hardcore** — mapas gerados aleatoriamente, com o jogador sempre começando no centro. Escolha a dificuldade (Fácil, Médio, Difícil, Impossível); a quantidade de obstáculos aumenta com a dificuldade e com o level. Não há garantia de que exista caminho até o `O`.

### Formato dos mapas

Arquivo de texto com 7 linhas de 7 caracteres, usando `P`, `X`, `.` e `O`. Qualquer outro caractere vira caminho livre. Exemplo (`mapa1.txt`):

```
P......
.....X.
.......
.X..O..
.......
...X...
.......
```

Para criar mais fases, adicione `mapa6.txt`, `mapa7.txt` etc. na mesma pasta.

## Como rodar

Compile e execute de dentro da pasta do projeto (os mapas são lidos do diretório atual):

```bash
gcc main.c -o minimap
./minimap
```

## Requisitos

- Linux, macOS ou WSL (usa `termios.h`, `unistd.h` e `system("clear")`, que não existem no Windows nativo)
- Compilador C com suporte a C99 (ex.: `gcc`)

## Estrutura

```
main.c                 # jogo (menu, modos, leitura de mapas, entrada de teclado)
mapa1.txt ... mapa5.txt  # fases do modo História
```

## Contexto

Projeto de estudo para praticar matrizes, leitura de arquivos e entrada de teclado em C.
