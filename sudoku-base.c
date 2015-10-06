#include <stdio.h>
#include <pthread.h>

/* grid size = 9x9 */
#define SIZE 9
#define TASKS 3 * SIZE

int grid[SIZE][SIZE];
pthread_mutex_t mutex_erro, mutex_current;

int erro = 0;
int current = 0;

/* Funcao que le um grid do arquivo "filename" e o armazena em uma matriz */
int load_grid(int grid[][SIZE], char *filename) {

	FILE *input_file = fopen(filename, "r");

	if (input_file != NULL) {
		for(int i = 0; i < SIZE; i++)
			for(int j = 0; j < SIZE; j++)
				fscanf(input_file, "%d", &grid[i][j]);
		fclose(input_file);
		return 1;
	}

	return 0;
}

int verifica(int x, int max_x, int y, int max_y) {
	// Array de números verificados
	// Ao encontrar um número marca 1 no índice dele
	int verificados[SIZE] = { 0 };
	// printf("X %d, Y %d\n", max_x, max_y);

	int i = x;
	int j = y;

	// Percorre da linha X até a linha MAX_X
	for(int i = x; i <= max_x; ++i) {
		// Percorre da linha Y até a coluna MAX_Y
		for(int j = y; j <= max_y; ++j) {
			// Pega o número atual na posição X, Y
			int numero = grid[j][i];
			if(verificados[numero-1] == 1) {
				// Se o índice igual a esse número já estiver marcado incrementa erros e retorna
				pthread_mutex_lock(&mutex_erro);
				++erro;
				pthread_mutex_unlock(&mutex_erro);
				return 0;
			} else {
				// Se o índice igual ao número não estava marcado com 1, marca
				verificados[numero-1] = 1;
			}
		}
	}

	// Observação: fazemos numero-1 na posição porque os números vão de 1 a 9 e nosso array vai de 0 a 8
	return 1;
}

void * main_loop(void * param) {
	int thread_current;

	pthread_mutex_lock(&mutex_current);
	thread_current = current++;
	pthread_mutex_unlock(&mutex_current);

	while(thread_current < TASKS) {
		int tarefa, numero;
		int linha_inicial, coluna_inicial;

		//Indica se é linha (0), coluna (1) ou região (2)
		tarefa = thread_current / SIZE; //2
		//Número da linha, coluna ou região
		numero = thread_current % SIZE; //1
		
		switch(tarefa) {
			case 0:
				if(!verifica(0, SIZE - 1, numero, numero)) {
					printf("Thread: %d - Erro na linha: %d\n", (int) param, numero+1);
				}

				break;

			case 1:
				if(!verifica(numero, numero, 1, SIZE - 1)) {
					printf("Thread: %d - Erro na coluna: %d\n", (int) param, numero+1);
				}

				break;

			case 2:
				linha_inicial = numero % 3;
				coluna_inicial = numero / 3;

				if(!verifica(3 * linha_inicial, (3 * linha_inicial) + 2, 3 * coluna_inicial, (3 * coluna_inicial) + 2)) {
					printf("Thread: %d - Erro na região: %d\n", (int) param, numero+1);
				}

				break;
		}

		pthread_mutex_lock(&mutex_current);
		thread_current = current++;
		pthread_mutex_unlock(&mutex_current);
	}
}

int main(int argc, char *argv[]) {

	if(argc != 3) {
		printf("Erro: informe o arquivo de entrada e o número de threads a serem usadas.\nUso: %s <arquivo_de_entrada> <numero_de_threads>\n\n", argv[0]);
		return 1;
	}

	int max_threads = atoi(argv[2]);

	/* Le o grid do arquivo, armazena na matriz grid e imprime */
	if(load_grid(grid, argv[1])) {
		printf("Quebra-cabeças fornecido:\n");
		for(int i = 0; i < 9; i++) {
			for(int j = 0; j < 9; j++)
				printf("%d ", grid[i][j]);
			printf("\n");
		}
		printf("\n");

		pthread_t threads[max_threads];
		pthread_mutex_init(&mutex_erro, NULL);
		pthread_mutex_init(&mutex_current, NULL);

		// Cria o número de threads solicitadas
		for(int i = 0; i < max_threads; ++i) {
            pthread_create(&threads[i], NULL, main_loop, (void *) i);
        }

        for(int i = 0; i < max_threads; ++i) {
            pthread_join(threads[i], NULL);
        }

        printf("Total de erros: %d\n", erro);
	}

	return 0;
}

