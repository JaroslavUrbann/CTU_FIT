#include <stdio.h>


int main(){
	// short int db[45][10][12] = { { {}, { 0b000000111111111, 0b111111111, 0b111101111, }, }, };

	// printf("%d\n", db[0][1][2]);

	// [clue][n_whites][n_sets]
	// short db[45][10][12];
	// for (short i = 0; i < 45; i++)
	// 	for (short j = 0; j < 10; j++)
	// 		for (short k = 0; k < 12; k++)
	// 			db[i][j][k] = 0;

	for (int k = 0; k < 512; k++){
		int n_bits = 0;
		int sum = 0;
		int a = k;
		for (int l = 0; l < 9; a >>= 1, l++){
			n_bits += a & 1;
			sum += (l+1) * (a & 1);
		}
		if (sum == i && n_bits == j)
			b->sets[b->n_sets++] = k;		}
	}


	printf("{");
	for (int i = 0; i < 46; i++){
		printf("{");
		for (int j = 0; j < 10; j++){
			printf("{");
			int n = 0;
			for (int k = 0; k < 512; k++){
				int n_bits = 0;
				int sum = 0;
				int a = k;
				for (int l = 0; l < 9; a >>= 1, l++){
					n_bits += a & 1;
					sum += (l+1) * (a & 1);
				}
				if (sum == i && n_bits == j){
					n++;
					printf("0b");
					for (int m = 8; m >= 0; m--)
						printf("%d", (k >> m) & 1);
					printf(",");
				}
			}
			// if (n)
				// printf("\n%d in %d = %dx\n", i, j, n);
			printf("},");
		}
		printf("},");
	}
	printf("}");
}