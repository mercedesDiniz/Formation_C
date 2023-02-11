#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pacman.h"
#include "mapa.h"

// Declarando Variaveis Globais
MAPA mp;
POSICAO pacman; // posição atual do pacman
int power_pill = 0; // qtd de pilulas de poder

// MAIN
int main(){	
	le_mapa(&mp); // Inicializa o mapa
	 encontrar_no_mapa(&mp, &pacman, PACMAN); // localizando posicao inicial do pacman no mapa
	
	// LOOP PRINCIPAL
	do{	
		printf("Tem power pill: %s\n", (power_pill ? "SIM" : "NÃO"));
		imprime_mapa(&mp);
		// Recebe novo comando
		char comando;
		scanf(" %c", &comando);
		
		move(comando); // executando o comando
		if(comando == BOMBA) explode_pill();
		
		fantasmas(); // habilitando os fantasmas
			
	}while(!acabou());
	
	libera_mapa(&mp); // Fecha o mapa
}

// FUNÇÕES:
int acabou(){
	POSICAO pos;
	int pacman_esta_no_mapa = encontrar_no_mapa(&mp, &pos, PACMAN); // verifica se o pacman está no mapa
	return !pacman_esta_no_mapa; // acaba quando o pacman morre
}

int ehdirecao(char direcao){
	return direcao == 'a' || direcao == 'd' || direcao == 'w' || direcao == 's';
}

void move(char direcao){	
	if(!ehdirecao(direcao)) return; //tratando entradas invalidas	
	
	 // verificar o que temos no mapa na posicao que o pacman deseja ir
	int prox_x = pacman.x; // posicao atual de x
	int prox_y = pacman.y; // posicao atual de y
	switch(direcao){
		case ESQUERDA: 
	 		prox_y--; // atualiando a localizacao em que o pacman deseja ir no mapa
	 		break;
	 	case DIREITA: 
	 		prox_y++; // atualiando a localizacao em que o pacman deseja ir no mapa
	 		break;
	 	case CIMA: 
	 		prox_x--; // atualiando a localizacao em que o pacman deseja ir no mapa
	 		break;
	 	case BAIXO: 
	 		prox_x++; // atualiando a localizacao em que o pacman deseja ir no mapa
	 		break;
	}  

	if(!pode_andar(&mp, PACMAN, prox_x, prox_y)) return; //tratrando a validacao do movimento 
	
	// verifica se na posição emque o pacman vai tem uma das power Pill
	if(ehpersonagem(&mp, PILL, prox_x, prox_y)){ 
		power_pill = 1;
	}
	
	// movenvo o pacman p/ um posicao valida no mapa
	anda_no_mapa(&mp, pacman.x, pacman.y, prox_x, prox_y);
	
	// atualizando a posicao atual no pacman
	pacman.x = prox_x;
	pacman.y =  prox_y;
}
 
void explode_pill(){
	// as power pill apaga o comteudo das tres casas para direita
	for(int i=1; i<=3; i++){
		if(ehvalida(&mp, pacman.x, pacman.y+i) ){
			if(ehparede(&mp, pacman.x, pacman.y+i)) break; // não explode parede
			mp.matriz[pacman.x][pacman.y+i] = VAZIO;
		} 
	}
	power_pill--;
}

int pra_onde_o_fantasma_vai(int x_atual, int y_atual, int* x_dest, int* y_dest){
	int opcoes[4][2] = {
		{x_atual, y_atual+1}, // p/ direita
		{x_atual+1, y_atual}, // p/ baixo
		{x_atual, y_atual-1}, // p/ esquerda
		{x_atual-1, y_atual}, // p/ cima
	};
	
	srand(time(0)); // semente
	for(int i=0; i < 10; i++){
		int posicao = rand() % 4; // chutando um numero aleatorio entre 0 e 3
		if(pode_andar(&mp, FANTASMA, opcoes[posicao][0], opcoes[posicao][1])){
			*x_dest = opcoes[posicao][0];
			*y_dest = opcoes[posicao][1];
			return 1;
		}
	}
	return 0;
}

void fantasmas(){
	MAPA copia;
	copia_mapa(&copia, &mp);
	// varendo a matriz original e armazenando na matriz copia
	for(int i = 0; i< mp.linhas; i++){
		for(int j = 0; j< mp.colunas; j++){
			if(copia.matriz[i][j] == FANTASMA){
				int x_dest;
				int y_dest;
				int encontrou = pra_onde_o_fantasma_vai(i, j, &x_dest, &y_dest);	
				if(encontrou){ 
					 anda_no_mapa(&mp, i, j, x_dest, y_dest); // vai p/ a nova posicao
				}			
			}	 
		}
	}
	
	libera_mapa(&copia);	
}