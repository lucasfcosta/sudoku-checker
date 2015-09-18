#include <stdio.h>
#include <pthread.h>

/* grid size = 9x9 */
#define SIZE 9

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

int main(int argc, char *argv[]) {

	if(argc != 3) {
		printf("Erro: informe o arquivo de entrada e o número de threads a serem usadas.\nUso: %s <arquivo_de_entrada> <numero_de_threads>\n\n", argv[0]);
		return 1;
	}

	int max_threads = argv[2];

	/* Le o grid do arquivo, armazena na matriz grid e imprime */
	int grid[9][9];
	if(load_grid(grid, argv[1])) {
		printf("Quebra-cabeças fornecido:\n");
		for(int i = 0; i < 9; i++) {
			for(int j = 0; j < 9; j++)
				printf("%d ", grid[i][j]);
			printf("\n");
		}
		printf("\n");
	}



	return 0;
}

