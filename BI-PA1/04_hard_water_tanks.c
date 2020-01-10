#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#define WRONG_INPUT {printf("Nespravny vstup.\n"); return 1;}


typedef struct {
	int altitude;
	int surface;
} threshold;


int getN(int* n){
	printf("Zadejte pocet nadrzi:\n");
	if (scanf("%d", n) != 1 || *n < 1 || *n > 200000)
		return 0;
	return 1;
}


int getData(threshold* ths, int n){
	int alt, H, W, D;
	for (int i = 0; i < n; i+=2){
		if (scanf("%d%d%d%d", &alt, &H, &W, &D) != 4 || H < 1
													 || W < 1
													 || D < 1)
			return 0;
		ths[i].altitude = alt;
		ths[i].surface = W * D;
		ths[i+1].altitude = alt + H;
		ths[i+1].surface = - W * D;
	}
	return 1;
}


int cmpFunction(const void* a, const void* b){
	return (((threshold*)a)->altitude - ((threshold*)b)->altitude);
}


void transformArr(threshold* ths, int n){
	int surface = 0;
	int total_volume = 0;
	int prev_alt_index = 0;

	for (int i = 0; i < n; i++){
		if (i && ths[i-1].altitude == ths[i].altitude){
			ths[i].surface = total_volume;
			continue;
		}

		total_volume += surface * (ths[i].altitude - ths[prev_alt_index].altitude);

		int a = i;
		while (a < n && ths[i].altitude == ths[a].altitude){
			surface += ths[a].surface;
			a++;
		}

		ths[i].surface = total_volume;
		prev_alt_index = i;
	}
}


int binarySearch(threshold* ths, int n, int volume){
	int l = 0;
	int r = n;

	while (l < r){
		int m = l + (r - l) / 2;
		if ((m - 1 < 0 || ths[m-1].surface < volume) && ths[m].surface >= volume)
			return m;

		if (ths[m].surface >= volume)
			r = m;

		if (ths[m].surface < volume)
			l = m + 1;
	}
	return n;
}


void solveQuery(threshold* ths, int n, int volume){
	int i = binarySearch(ths, n, volume);
	if (i == 0)
		printf("Prazdne.\n");
	else if (i == n)
		printf("Pretece.\n");
	else{
		double altitude = ths[i-1].altitude + (ths[i].altitude - ths[i-1].altitude) * (volume - ths[i-1].surface) / double(ths[i].surface - ths[i-1].surface);
		printf("h = %f\n", altitude);
	}
}


// void printT(threshold* t, int n){
// 	for (int i = 0; i < n; i++){
// 		printf("%5d", t[i].altitude);
// 	}
// 	printf("\n");
// 	for (int i = 0; i < n; i++){
// 		printf("%5d", t[i].surface);
// 	}
// 	printf("\n");

// }


int main(){
	int n;
	if (!getN(&n))
		WRONG_INPUT

	n *= 2;
	threshold* ths = (threshold*) malloc( n * sizeof(ths[0]) );

	printf("Zadejte parametry nadrzi:\n");
	if (!getData(ths, n)){
		free(ths);
		WRONG_INPUT
	}

	qsort(ths, n, sizeof(ths[0]), cmpFunction);

	transformArr(ths, n);

	// printT(ths, n);
	printf("Zadejte objem vody:\n");
	int volume;
	while (1){
		int n_read = scanf("%d", &volume);

		if (n_read == 0 || volume < 0){
			printf("Nespravny vstup.\n");
			break;
		}

		if (n_read == EOF){
			break;
		}

		solveQuery(ths, n, volume);
	}

	free(ths);
}
