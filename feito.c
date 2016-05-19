#include <stdio.h>
#include <SDL/SDL.h>
#include "SDL/SDL_ttf.h"
#include <string.h>
#include <stdlib.h>

#define LARGURA 800
#define ALTURA  600


typedef struct animal{
	int especie[3];
	char nome[20];
	int peso;
	struct animal *seguinte;
	struct animal *anterior;
}animal;


/* verifica se foi premido a tecla n , deverá ser criado e inserido na lista um novo animal.*/
int n_pressed(SDL_Event e){
	if( e.type == SDL_KEYDOWN ) {
		if( e.key.keysym.sym == SDLK_n){
			return 1;
		}
	}
	return 0;
		
}

/* verifica se foi premido a tecla RIGHT */
int right_pressed(SDL_Event e){
	if( e.type == SDL_KEYDOWN ) {
		if( e.key.keysym.sym == SDLK_RIGHT){
			return 1;
		}
	}
	return 0;
		
}

/* verifica se foi premido a tecla LEFT */
int left_pressed(SDL_Event e){
	if( e.type == SDL_KEYDOWN ) {
		if( e.key.keysym.sym == SDLK_LEFT){
			return 1;
		}
	}
	return 0;
		
}

/* verifica se foi premido a tecla a , todos os animais da quinta serão alimentados.*/
int a_pressed(SDL_Event e){
	if( e.type == SDL_KEYDOWN ) {
		if( e.key.keysym.sym == SDLK_a){
			return 1;
		}
	}
	return 0;
		
}

/* verifica se foi premido a tecla e , todos os animais da quinta serão exercitados.*/
int e_pressed(SDL_Event e){
	if( e.type == SDL_KEYDOWN ) {
		if( e.key.keysym.sym == SDLK_e){
			return 1;
		}
	}
	return 0;
		
}

/* verifica se foi premido o space */
int space_bar_pressed(SDL_Event e){
	if( e.type == SDL_KEYDOWN ) {
		if( e.key.keysym.sym == SDLK_SPACE){
			return 1;
		}
	}
	return 0;
		
}

/* inicializa o sdl e abre janela */
SDL_Surface * open_window(int width, int height){
	SDL_Surface * window;
	
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
		printf("SDL not loaded\n");
		exit(-1);
	}
	window = SDL_SetVideoMode(LARGURA, ALTURA, 32,SDL_SWSURFACE);
	if(window == NULL){
		printf("Window not created\n");
		exit(-2);
	}
	SDL_WM_SetCaption("Quinta dos malandros!", NULL);
	
	if(TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	
	return window;
	
}

/* imprime uma string na janela */
SDL_Surface * write_text(char * str){

	TTF_Font *font;
	SDL_Surface * rendered_text;
	SDL_Color textColor = { 255, 255, 255 };

	font = TTF_OpenFont("acmesa.ttf", 30);
	if(font == NULL){ 
		printf("Font not found!\n"); 
		exit(-4);   
	}
	rendered_text = TTF_RenderText_Solid(font, 
								str, 
								textColor);
	return rendered_text;
	   
}

/* imprime uma imagem na superficie */
void print_image(SDL_Surface * dest,SDL_Surface * image,int x, int y){
					
	SDL_Rect position;
	position.x = x;
	position.y = y;
	
	SDL_BlitSurface(image,  NULL,
       dest, &position);
	SDL_Flip(dest);
}

/* detecta o carregar do rato */
void getMouseClick(SDL_Event evento, int *x, int *y){
	*x = evento.button.x;
	*y = evento.button.y;
	
}


void exercita_todos(animal *inicio){
	animal *aux;
	
	aux = inicio;
	while (aux != NULL){
		(*aux).peso = (*aux).peso-(*aux).especie[2];
		aux = (*aux).seguinte;
	}

}

void alimenta_todos(animal *inicio){
	animal *aux;
	
	aux = inicio;
	while (aux != NULL){
		(*aux).peso = (*aux).peso+(*aux).especie[1];
		aux = (*aux).seguinte;
	}
}

animal *criar_animal(animal *inicio,float *num_a, int matriz[6][3]){
	animal *aux;
	int end = 0, tipo;
	char linha[100], nome[20];
	
	while (!end){
		fgets (linha, 100, stdin);
		if (sscanf(linha, "%d %s", &tipo, nome) != 2){
			printf ("\nERRO: escolha inválida de espécie e nome.\nEscolha novamente:\n");
		}else{
			if (tipo > -1 && tipo < 7){
				end = 1;
			}else{
				printf ("Espécie Inexistente. Volte a escolher:\n");
			}
		}
	}
	
	if (*num_a == 1){
		inicio = malloc(sizeof(animal));
		(*inicio).seguinte = NULL;
		(*inicio).anterior = NULL;
	}else{
		aux = inicio;
		inicio = malloc(sizeof(animal));
		(*inicio).seguinte = aux;
		(*inicio).anterior = NULL;
		(*aux).anterior = inicio;
	}
	(*inicio).especie[0] = tipo;
	(*inicio).especie[1] = matriz[tipo-1][1];
	(*inicio).especie[2] = matriz[tipo-1][2];
	strcpy((*inicio).nome,nome);
	(*inicio).peso = 50;
	
	return inicio;
}

animal *actualizar_animais(animal *inicio,float *num_a,animal ** selecionado){
	animal *aux, *aux1, *aux2;
	
	aux = inicio;
	if (inicio != NULL){
		while (inicio != NULL && ((*inicio).peso <= 0 || (*inicio).peso >= 100)){	/* avalia o 1º animal */
			if (inicio == *selecionado){							/* verifica se o selecionado era o 1º se for passa para o próximo */
				*selecionado = (*inicio).seguinte;
			}
			aux=inicio;
			if ((*inicio).seguinte != NULL){
				inicio = (*aux).seguinte;
				(*inicio).anterior = NULL;
			}else{
				inicio = NULL;
			}
			free (aux);
			*num_a=*num_a-1;
		}
		if (inicio != NULL &&(*inicio).seguinte != NULL){		/* avalia animais do meio */
			aux = inicio;
			aux1 = (*inicio).seguinte;
			while ((*aux1).seguinte != NULL){
				if ((*aux1).peso <= 0 || (*aux1).peso >= 100){
					(*aux).seguinte = (*aux1).seguinte;
					aux2 = (*aux1).seguinte;
					if (aux1 ==*selecionado){
						*selecionado = aux;
					}
					(*aux2).anterior = aux;
					free (aux1);
					*num_a=*num_a-1;
					aux1 = (*aux).seguinte;
				}else{
					aux1 = (*aux1).seguinte;
					aux = (*aux).seguinte;
				}
			}
			if ((*aux1).peso <= 0 || (*aux1).peso >= 100){	/* Avalia o último animal. */
				(*aux).seguinte = NULL;
				*num_a = *num_a-1;
				if (aux1 == *selecionado){
					*selecionado = (*aux1).anterior;
				}
				free (aux1);

			}
		}
	}
	return inicio;
}

void imprime_lista(animal *inicio){
	animal *aux;
	
	aux = inicio;
	printf ("Lista dos animais:\n");
	if (aux != NULL){
		while (aux != NULL){
			printf ("Nome: %s\tTipo:%d\tPeso:%d\n", (*aux).nome, (*aux).especie[0], (*aux).peso);
			aux = (*aux).seguinte;
		}
	}else{
		printf ("Não existem animais na quinta.\n");
	}
}

animal *p_tempo(animal *inicio){
	animal *aux;
	aux =inicio;
	while ( aux !=NULL){
		if((*aux).peso > 0 && (*aux).peso <= 25){
			(*aux).peso=(*aux).peso-(*aux).especie[2];
		}
		if((*aux).peso >= 75 && (*aux).peso < 100){
			(*aux).peso=(*aux).peso+(*aux).especie[1];
		}
		aux=(*aux).seguinte;
	}
	return inicio;
}

void imprime_imagem_sel(animal* selecionado,SDL_Surface * window){
	SDL_Surface * sonicm;
	SDL_Surface * sonic;
	SDL_Surface * sonicg;
	SDL_Surface * tailsm;
	SDL_Surface * tails;
	SDL_Surface * tailsg;
	SDL_Surface * shadowm;
	SDL_Surface * shadow;
	SDL_Surface * shadowg;
	SDL_Surface * knucklesm;
	SDL_Surface * knuckles;
	SDL_Surface * knucklesg;
	SDL_Surface * amym;
	SDL_Surface * amy;
	SDL_Surface * amyg;
	SDL_Surface * chao_chaom;
	SDL_Surface * chao_chao;
	SDL_Surface * chao_chaog;
	
	sonicm = SDL_LoadBMP("sonic-magro.bmp");
	sonic = SDL_LoadBMP("sonic-normal.bmp");
	sonicg = SDL_LoadBMP("sonic-obeso.bmp");
	
	tailsm = SDL_LoadBMP("tails-magro.bmp");
	tails = SDL_LoadBMP("tails-normal.bmp");
	tailsg = SDL_LoadBMP("tails-obeso.bmp");
	
	shadowm = SDL_LoadBMP("shadow-magro.bmp");
	shadow = SDL_LoadBMP("shadow-normal.bmp");
	shadowg = SDL_LoadBMP("shadow-obeso.bmp");
	
	knucklesm = SDL_LoadBMP("knuckles-magro.bmp");
	knuckles = SDL_LoadBMP("knuckles-normal.bmp");
	knucklesg = SDL_LoadBMP("knuckles-obeso.bmp");
	
	amym = SDL_LoadBMP("amy-magra.bmp");
	amy = SDL_LoadBMP("amy-normal.bmp");
	amyg = SDL_LoadBMP("amy-obeso.bmp");
	
	chao_chaom = SDL_LoadBMP("chao_chao-magro.bmp");
	chao_chao = SDL_LoadBMP("chao_chao-normal.bmp");
	chao_chaog = SDL_LoadBMP("chao_chao-obeso.bmp");
	
	
	if ((*selecionado).especie[0] == 1){
		if((*selecionado).peso <= 25){
			/* imagem magro */
			print_image(window, sonicm, 150, 100);
		}
		if((*selecionado).peso > 25 && (*selecionado).peso < 75){
			/* imagem normal */
			print_image(window, sonic, 150, 100);
		}
		if((*selecionado).peso >= 75){
			/* imagem obeso */ 
			print_image(window, sonicg, 150, 100);
		}
	}
	if ((*selecionado).especie[0] == 2){
		if((*selecionado).peso <= 25){
		/* imagem magro */
			print_image(window, tailsm, 150, 100);
		}
		if((*selecionado).peso > 25 && (*selecionado).peso < 75){
		/* imagem normal */
			print_image(window, tails, 150, 100);
		}
		if((*selecionado).peso >= 75){
		/* imagem obeso */ 
			print_image(window, tailsg, 150, 100);
		}
	}
	if ((*selecionado).especie[0] == 3){
		if((*selecionado).peso <= 25){
		/* imagem magro */
			print_image(window, shadowm, 150, 100);
		}
		if((*selecionado).peso > 25 && (*selecionado).peso < 75){
		/* imagem normal */
			print_image(window, shadow, 150, 100);
		}
		if((*selecionado).peso >= 75){
		/* imagem obeso */ 
			print_image(window, shadowg, 150, 100);
		}
	}
	if ((*selecionado).especie[0] == 4){
		if((*selecionado).peso <= 25){
		/* imagem magro */
			print_image(window, knucklesm, 150, 100);
		}
		if((*selecionado).peso > 25 && (*selecionado).peso < 75){
		/* imagem normal */
			print_image(window, knuckles, 150, 100);
		}
		if((*selecionado).peso >= 75){
		/* imagem obeso */ 
			print_image(window, knucklesg, 150, 100);
		}
	}
	if ((*selecionado).especie[0] == 5){
		if((*selecionado).peso <= 25){
		/* imagem magro */
			print_image(window, amym, 150, 100);
		}
		if((*selecionado).peso > 25 && (*selecionado).peso < 75){
		/* imagem normal */
			print_image(window, amy, 150, 100);
		}
		if((*selecionado).peso >= 75){
		/* imagem obeso */ 
			print_image(window, amyg, 150, 100);
		}
	}
	if ((*selecionado).especie[0] == 6){
		if((*selecionado).peso <= 25){
		/* imagem magro */
			print_image(window, chao_chaom, 150, 100);
		}
		if((*selecionado).peso > 25 && (*selecionado).peso < 75){
		/* imagem normal */
			print_image(window, chao_chao, 150, 100);
		}
		if((*selecionado).peso >= 75){
		/* imagem obeso */ 
			print_image(window, chao_chaog, 150, 100);
		}
	}
}

int num_obeso_sub(animal *inicio){
	int num_avisos = 0;
	
	while(inicio != NULL){
		if ((*inicio).peso <= 25 || (*inicio).peso >=75){
			num_avisos = num_avisos +1;
		}
		inicio = (*inicio).seguinte;
	}

	return num_avisos;
	
}

SDL_Surface * imprimir_aviso(float num_a,float num_avisos){
	SDL_Surface * marca;

	if((num_avisos/num_a) < 0.33)
	{
		marca = SDL_LoadBMP("aviso-verde.bmp");
	}
	if((num_avisos/num_a) >= 0.33 && (num_avisos/num_a) <= 0.66)
	{
		marca = SDL_LoadBMP("aviso-laranja.bmp");
	}
	if((num_avisos/num_a) > 0.66)
	{
		marca = SDL_LoadBMP("aviso-vermelho.bmp");
	}
	return marca;
	
}

void imprime_barra(animal* selecionado,SDL_Surface * window){
	SDL_Surface * barra;
	SDL_Surface * barrita_de;
	SDL_Surface * barrita_lho;
	int h = 410;

	barra = SDL_LoadBMP("barra.bmp");
	barrita_de = SDL_LoadBMP("barrita-verde.bmp");
	barrita_lho = SDL_LoadBMP("barrita-vermelha.bmp");

	print_image(window, barra, 550, 50);

	if((*selecionado).peso <= 25 || (*selecionado).peso >= 75){
		print_image(window, barrita_lho, 550, h-(3.6*(*selecionado).peso));
	}
	if((*selecionado).peso > 25 && (*selecionado).peso < 75){
		print_image(window, barrita_de, 550, h-(3.6*(*selecionado).peso));
	}

}

animal * backup_leitura(FILE * fp,float *num_a,int *tempo){
	animal * aux;
	char gravado[100];
	animal * inicio = NULL;
	
	*num_a = 1;
	while(fgets(gravado, 100, fp) != NULL){ 
		aux = malloc(sizeof(animal)); 
		sscanf(gravado, "%s %d %d %d %d %d\n", (*aux).nome, &(*aux).peso, &(*aux).especie[0], &(*aux).especie[1], &(*aux).especie[2], &(*tempo)); 
		printf("O animal %s da espécie do tipo %d e com peso %d foi lido.\n", (*aux).nome, (*aux).especie[0], (*aux).peso);
		if(inicio == NULL){ 
			inicio = aux;
			(*inicio).seguinte = NULL;
			(*inicio).anterior = NULL;
		}else{ 
			(*aux).seguinte = inicio;
			(*aux).anterior = NULL;
			(*inicio).anterior = aux; 
			inicio = aux;
		*num_a = *num_a +1;
		}
	}

	return inicio;
}

void escreve_backup(animal * inicio,int tempo){
	FILE * fp;


	fp = fopen("animal.txt", "w");
	
	while(inicio != NULL){ 
		fprintf(fp, "%s %d %d %d %d %d\n", (*inicio).nome, (*inicio).peso, (*inicio).especie[0], (*inicio).especie[1], (*inicio).especie[2], tempo);
		inicio = (*inicio).seguinte;
    }
    fclose(fp);
}


int main(){
		SDL_Surface * window;
		SDL_Surface * imageinicial;
		SDL_Surface * fundo_2;
		SDL_Surface * aviso_amar;
		SDL_Surface * game_over;
		SDL_Surface * marca;
		SDL_Surface * text;
		SDL_Event evento;
		FILE * especies;
		FILE * fp;
		animal *lista_ini = NULL;
		animal *selecionado = NULL;
		int end=0, i, x , y , num_avisos_ant = 0, tempo=0;
		float num_a, num_avisos=0;
		char linha[50];
		int matriz_esp[10][3];
		
/*load das imagens*/
	imageinicial = SDL_LoadBMP("fundoinicial.bmp");
	fundo_2 = SDL_LoadBMP("fundo_2.bmp");
	aviso_amar = SDL_LoadBMP("aviso-amarelo.bmp");
	game_over = SDL_LoadBMP("fundo_game.bmp");
	
/* leitura do ficheiro das especie*/		
	especies = fopen ("especies.txt", "r"); 
	if (especies == NULL){
		printf ("\nERRO: carregamento do ficheiro das espécies.\n");
		exit (1);
	}else{
		fgets (linha, 50, especies);
		for (i = 0; i < 6; i++){ /* carregar o ficheiro para a matriz */
			fgets (linha, 50, especies);
			if (sscanf(linha, "%d\t\t%d\t\t%d", &matriz_esp[i][0], &matriz_esp[i][1], &matriz_esp[i][2]) != 3){
				printf ("\nERRO: carregamento dos valores do ficheiro das espécies.\n");
				exit (2);
			}
		}
	}
	
	
	window = open_window(LARGURA, ALTURA);
	print_image(window, imageinicial, 000, 000);
	
	while(!end){
		if(SDL_WaitEvent( &evento)){
			if(evento.type == SDL_MOUSEBUTTONDOWN){
				getMouseClick(evento, &x, &y);
				if(x >= 200 && x <=500 && y >= 200 && y <=300){							/**novo jogo **/
					printf ("Número de animais: 1\n");
					num_a=1;
					printf ("Espécies:\n1-Sonic\n2-Tails\n3-Shadow\n4-Knuckles\n5-Amy\n6-Chao_chao\n");
					printf ("Escolha a sua espécie e nome do seu animal, na seguinte forma (Número da Espécie Nome) exemplo(1 Dorminhoco):\n");
					lista_ini = criar_animal(lista_ini, &num_a, matriz_esp);
					selecionado = lista_ini;
					end=1;
				}
				if(x >= 200 && x <=500 && y >= 400 && y <=500){						/** jogo gravado **/
					fp=fopen("animal.txt","r");
					if (fp == NULL){
						printf ("Não existe jogos gravados.\n");
					}else{
						lista_ini = backup_leitura(fp,&num_a,&tempo);
						selecionado = lista_ini;
						end = 1;

					}
				}
			}
		}
	}
	
	/*imprimir imagens 2ºfundo */
	print_image(window, fundo_2, 000, 000);
	imprime_imagem_sel(selecionado, window);
	imprime_barra(selecionado, window);
	text = write_text((*selecionado).nome);
	print_image(window, text, 200, 050);				/**posições renovadas **/
	num_avisos = num_obeso_sub(lista_ini);
	marca = imprimir_aviso(num_a,num_avisos);
	print_image(window , marca , 050 , 479);

	end=0;
	while(!end)	{
		if(SDL_WaitEvent( &evento)){
			if(evento.type == SDL_MOUSEBUTTONDOWN){
				getMouseClick(evento, &x, &y);
				if(x >= 539 && x <=639 && y >= 480 && y <=580){							/** come **/
					(*selecionado).peso = (*selecionado).peso+(*selecionado).especie[1];
					lista_ini = actualizar_animais(lista_ini, &num_a,&selecionado);
					if (num_a == 0){
						end= 1;
					}else{
						print_image(window, fundo_2, 000, 000);
						imprime_imagem_sel(selecionado, window);
						text = write_text((*selecionado).nome);
						print_image(window, text, 200, 050);
						imprime_barra(selecionado, window);
						num_avisos = num_obeso_sub(lista_ini);
						/* amarelo */
						if (num_avisos > num_avisos_ant){
							print_image(window, aviso_amar, 050, 479);
							SDL_Delay(2000);
						}
						marca = imprimir_aviso(num_a,num_avisos);
						print_image(window , marca , 050 , 479);
						num_avisos_ant = num_avisos;
						imprime_lista(lista_ini);
					}
				}
				if(x >= 677 && x <=777 && y >= 480 && y <=580){							/** exercita **/
					(*selecionado).peso = (*selecionado).peso-(*selecionado).especie[2];
					lista_ini = actualizar_animais(lista_ini, &num_a,&selecionado);
					if (num_a == 0){
						end= 1;
					}else{ 
						print_image(window, fundo_2, 000, 000);
						imprime_imagem_sel(selecionado, window);
						text = write_text((*selecionado).nome);
						print_image(window, text, 200, 050);
						imprime_barra(selecionado, window);
						num_avisos = num_obeso_sub(lista_ini);
						/* amarelo */
						if (num_avisos > num_avisos_ant){
							/*imprimir barra amarela */
							print_image(window, aviso_amar, 050, 479);
							SDL_Delay(2000);
						}
						marca = imprimir_aviso(num_a,num_avisos);
						print_image(window , marca , 050 , 479);
						num_avisos_ant = num_avisos;
						imprime_lista(lista_ini);
					}
				}
				if(x >= 700 && x <=800 && y >= 0 && y <=100){							/** exit e guarda **/
					escreve_backup(lista_ini,tempo);
					end=1;
				}
			}
			if(n_pressed(evento)){												/** criação de um novo animal **/
				num_a=num_a + 1;
				printf ("\nNúmero de animais: %f\n", num_a);
				printf ("Escolha a sua espécie e nome do seu animal, na seguinte forma (NºEspécie Nome):\n");
				lista_ini = criar_animal(lista_ini, &num_a, matriz_esp);
				lista_ini = actualizar_animais(lista_ini, &num_a,&selecionado);
				selecionado = lista_ini;
				print_image(window, fundo_2, 000, 000);
				imprime_imagem_sel(selecionado, window);
				text = write_text((*selecionado).nome);
				print_image(window, text, 200, 050);
				imprime_barra(selecionado, window);
				marca = imprimir_aviso(num_a,num_avisos);
				print_image(window , marca , 050 , 479);
				/*novo animal vai ser igual ao selecionado */
				/* apresentação do novo animal no display */
				imprime_lista(lista_ini);
			}
			if(a_pressed(evento)){											/** os animais da quinta serão alimentados **/
				alimenta_todos(lista_ini);
				lista_ini = actualizar_animais(lista_ini, &num_a,&selecionado);
				if (num_a == 0){
					end= 1;
				}else{
					print_image(window, fundo_2, 000, 000);
					imprime_imagem_sel(selecionado, window);
					text = write_text((*selecionado).nome);
					print_image(window, text, 200, 050);
					imprime_barra(selecionado, window);
					/*avisos */
					num_avisos = num_obeso_sub(lista_ini);
					/* amarelo */
					if (num_avisos > num_avisos_ant){
						/*imprimir barra amarela */
						print_image(window, aviso_amar, 050, 479);
						SDL_Delay(2000);
					}
					marca = imprimir_aviso(num_a,num_avisos);
					print_image(window , marca , 050 , 479);
					num_avisos_ant = num_avisos;
					imprime_lista(lista_ini);
					/*avisos e imprimir lista */
				}
			}
			if(e_pressed(evento)){												/** os animais da quinta serão exercitados **/
				exercita_todos(lista_ini);
				lista_ini = actualizar_animais(lista_ini, &num_a,&selecionado);
				if (num_a == 0){
					end= 1;
				}else{
					print_image(window, fundo_2, 000, 000);
					imprime_imagem_sel(selecionado, window);
					text = write_text((*selecionado).nome);
					print_image(window, text, 200, 050);
					imprime_barra(selecionado, window);
					/*avisos */
					num_avisos = num_obeso_sub(lista_ini);
					/* amarelo */
					if (num_avisos > num_avisos_ant){
						/*imprimir barra amarela */
						print_image(window, aviso_amar, 050, 479);
						SDL_Delay(2000);
					}
					marca = imprimir_aviso(num_a,num_avisos);
					print_image(window , marca , 050 , 479);
					num_avisos_ant = num_avisos;
					imprime_lista(lista_ini);
					/*avisos e imprimir lista (ver exercitar quando já não existe animais)*/
				}
			}
			if(space_bar_pressed(evento)){												/** passagem do tempo **/
				tempo = tempo + 1;
				if (tempo == 2){
					lista_ini = p_tempo(lista_ini);
					lista_ini = actualizar_animais(lista_ini, &num_a,&selecionado);
					if (num_a == 0){
						end= 1;
					}else{
						print_image(window, fundo_2, 000, 000);
						imprime_imagem_sel(selecionado, window);
						text = write_text((*selecionado).nome);
						print_image(window, text, 200, 050);
						imprime_barra(selecionado, window);
						/* avisos */
						num_avisos = num_obeso_sub(lista_ini);
						/* amarelo */
						if (num_avisos > num_avisos_ant){
							/*imprimir barra amarela */
							print_image(window, aviso_amar, 050, 479);
							SDL_Delay(2000);
						}
						marca = imprimir_aviso(num_a,num_avisos);
						print_image(window , marca , 050 , 479);
						num_avisos_ant = num_avisos;
						imprime_lista(lista_ini);
						/*avisos imprimir lista alterar animal*/
						tempo = 0;
					}
				}
			}
			if(right_pressed(evento)){												/** muda de animal para frente **/
				if ((*selecionado).seguinte != NULL){
					selecionado = (*selecionado).seguinte;
					print_image(window, fundo_2, 000, 000);
					imprime_imagem_sel(selecionado, window);
					imprime_barra(selecionado, window);
					text = write_text((*selecionado).nome);
					print_image(window, text, 200, 050);
					marca = imprimir_aviso(num_a,num_avisos);
					print_image(window , marca , 050 , 479);
				}
			}
			if(left_pressed(evento)){												/** muda de animal para trás **/
				if ((*selecionado).anterior != NULL){
					selecionado = (*selecionado).anterior;
					print_image(window, fundo_2, 000, 000);
					imprime_imagem_sel(selecionado, window);
					imprime_barra(selecionado, window);
					text = write_text((*selecionado).nome);
					print_image(window, text, 200, 050);
					marca = imprimir_aviso(num_a,num_avisos);
					print_image(window , marca , 050 , 479);
				}
			}	
		}
	}
	if (num_a == 0 ){
		print_image(window, game_over, 000, 000);
		SDL_Delay (3000);
		remove("animal.txt");
	}
		 
	SDL_Quit();
	exit(0);

}
