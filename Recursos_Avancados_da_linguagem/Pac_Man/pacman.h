#define CIMA 'w'
#define BAIXO 's'
#define DIREITA 'd'
#define ESQUERDA 'a'

int acabou();
int ehdirecao(char direcao);
void move(char direcao);
void fantasmas();
int pra_onde_o_fantasma_vai(int x_atual, int y_atual, int* x_dest, int* y_dest);
