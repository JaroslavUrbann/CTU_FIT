#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define WRONG_INPUT {printf("Nespravny vstup.\n"); return 1;}
#define ll long long int


// mode, low, high
int getData(char* mode, long long int* low, long long int* high){
	int status = scanf(" %c%lld%lld", mode, low, high);
	
	if (status == EOF)
		return 0;

	if (status == 3 && (*mode == 'l' || *mode == 'c') && *low <= *high && *low >= 0)
		return 1;

	return -1;
}


int shouldUpperBaseBeLower(int base, ll num, int power1, int power2){
	if (power2 < 0)
		return 0;

	if (num / (int)pow(base, power1) % base > num / (int)pow(base, power2) % base)
		return 1;
	if (num / (int)pow(base, power1) % base < num / (int)pow(base, power2) % base)
		return 0;

	return shouldUpperBaseBeLower(base, num, power1 + 1, power2 - 1);
}

// don't ask
int shouldUpperBaseBeHigher(int base, ll num, int power1, int power2){
	if (power2 < 0)
		return 0;

	int num1 = num / (int)pow(base, power1) % base;
	int num2 = num / (int)pow(base, power2) % base;

	if (num1 > num2)
		return 0;
	if (num1 < num2)
		return 1;

	return shouldUpperBaseBeHigher(base, num, power1 + 1, power2 - 1);
}


// calculates the number of palindroms from 0 to max
long long int getNumberOfPalindromes(int base, ll max){
	if (max < 1)
		return max + 1;

	int max_power = log(max) / log(base);
	max_power += 100 * DBL_EPSILON * pow(base, max_power + 1) > pow(base, max_power + 1) - pow(base, log(max) / log(base));

	long long int numm = 1;
	for (int i = 0; i < max_power; i++){
		numm *= base;
	}

	if (max == numm){
		max_power--;
		max--;
	}

	ll upper_base;

	if (max_power % 2){
		// gets 654 from 654321
		upper_base = max / pow(base, max_power / 2 + 1);
		// changes 654 to 653
		upper_base -= shouldUpperBaseBeLower(base, max, max_power / 2 + 1, max_power / 2);
		// +1 for "0"
		return upper_base * 2 + pow(base, max_power / 2 + 1) - upper_base - 1 + 1;
	}
	else{
		// gets 7654 from 7654321
		upper_base = max / pow(base, max_power / 2);
		// changes 7654 to 7653
		upper_base -= shouldUpperBaseBeLower(base, max, max_power / 2 + 1, max_power / 2 - 1);

		int n_pol_twice = pow(base, max_power / 2) - 1;
		// + 1 for "0"
		return n_pol_twice * 2 + upper_base - n_pol_twice + 1;
	}
}


void palindromizeNumber(int base, ll* num, int power1, int power2){
	if (power2 < 0)
		return;
	*num = *num - (*num / (int)pow(base, power2) % base) * (int)pow(base, power2) + *num / (int)pow(base, power1) % base * (int)pow(base, power2);
	return palindromizeNumber(base, num, power1 + 1, power2 - 1);
}


int propagateOne(int base, ll* num, int pointer1, int pointer2){
	if (pointer2 < 0)
		return 0;
	if (*num / (int)pow(base, pointer1) % base != base - 1){
		*num += pow(base, pointer1) + pow(base, pointer2);
		return 1;
	}
	else{
		int status = propagateOne(base, num, pointer1 + 1, pointer2 - 1);
		if (status)
			*num -= (base - 1) * (pow(base, pointer1) + pow(base, pointer2));
		return status;
	}
}


void nextPalindrome(int base, ll* num, int* pointer1, int* pointer2){
	int middle = (*pointer1 + *pointer2) / 2;
	if (middle != *pointer2){
		if (*num / (int)pow(base, middle) % base != base - 1){
			*num += pow(base, middle);
		}
		else if (propagateOne(base, num, *pointer1, *pointer2)){
			*num -= (base - 1) * pow(base, middle);
		}
		else{
			*num += 2;
			(*pointer2)++;
		}
	}
	else{
		if (propagateOne(base, num, *pointer1, *pointer2))
			return;
		*num += 2;
		(*pointer1)++;
	}
}


void printPalindrome(int base, ll num, int max_power){
	printf("%lld = ", num);

	while (max_power >= 0){
		printf("%lld", num / (int)pow(base, max_power) % base);
		max_power--;
	}

	printf("b\n");
}


void listPalindromes(int base, ll low, ll high){
	long long int i = 0;
	if (!low){
		i++;
		low++;
		printf("0 = 0b\n");
	}
	int low_power = log(low) / log(base);
	low_power += 100 * DBL_EPSILON * pow(base, low_power + 1) > pow(base, low_power + 1) - pow(base, log(low) / log(base));

	int pointer1 = low_power / 2 + 1;
	int pointer2 = low_power / 2 - !(low_power % 2);

	low += shouldUpperBaseBeHigher(base, low, pointer1, pointer2) * pow(base, pointer2 + 1);

	palindromizeNumber(base, &low, pointer1, pointer2);

	while (low <= high){
		i++;
		// printf("p1 %d p2 %d\n", pointer1, pointer2);
		printPalindrome(base, low, pointer1 + pointer2);
		nextPalindrome(base, &low, &pointer1, &pointer2);
	}
}


void solve(int base, char mode, ll low, ll high){
	// printf("%lld\n", getNumberOfPalindromes(base, high) - getNumberOfPalindromes(base, low - 1));
	if (mode == 'c')
		printf("Celkem: %lld\n", getNumberOfPalindromes(base, high) - getNumberOfPalindromes(base, low - 1));
	if (mode == 'l')
		listPalindromes(base, low, high);
}


int main(){
	// mode, low, high
	char mode;
	int base = 2;
	long long int low, high;

	printf("Vstupni intervaly:\n");

	int status;
	while (1){
		status = getData(&mode, &low, &high);
		if (status == -1)
			WRONG_INPUT
		if (status == 1)
			solve(base, mode, low, high);
		if (status == 0)
			return 0;
	}
}
