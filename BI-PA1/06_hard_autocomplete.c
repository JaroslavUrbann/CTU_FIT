#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>


// n = number of phrases
// l = average length of a phrase
// m = length of the query
//
// I'm using a suffix array algorithm that is able to answer a query in O(log(n * l)) in the best case
// and about O(n * l * m) in the worst case :)
//
// Construction of the structure is done in O( (n * l) * log(n * l) )



#define WRONG_INPUT {printf("Nespravny vstup.\n"); return 1;}
#define ASCII_CHARS 128


typedef struct PHRASE {
	double frequency;
	// the size of the phrase and the number of its suffixes at the same time
	int size;
	// the phrase content
	char *content;
	// same as the array above, but lower case
	char *lowercase_content;
} phrase;


typedef struct SUFFIX {
	// phrase that the suffix belongs to
	phrase *phrase_;
	// where the suffix starts
	int index;
} suffix;



/////////////////////////////////////////////////////////////
// HELPER FUNCTIONS
/////////////////////////////////////////////////////////////


// prints a single phrase
void printPhrase(phrase *phrase){
	printf("> ");
	for (int i = 0; i < phrase->size; i++)
		printf("%c", phrase->content[i]);
	printf("\n");
}


// frees the phrases array
void freePhrases(phrase *phrases, int n){
	// frees the content of all phrases
	for (int i = 0; i < n; i++){
		free(phrases[i].content);
		free(phrases[i].lowercase_content);
	}
	free(phrases);
}


// sorts phrases by frequency, highest to lowest
int cmpFrequencies(const void *a, const void *b){
	if ( ((phrase*)a)->frequency < ((phrase*)b)->frequency )
		return 1;
	if ( ((phrase*)a)->frequency > ((phrase*)b)->frequency )
		return -1;
	return 0;
}


// sorts suffixes alphabetically
int cmpSuffixes(const void *a, const void *b){
	int diff = 0;

	// the phrase they belong to
	char *ac = ((suffix*)a)->phrase_->lowercase_content;
	char *bc = ((suffix*)b)->phrase_->lowercase_content;
	
	// the size of the phrase
	int as = ((suffix*)a)->phrase_->size;
	int bs = ((suffix*)b)->phrase_->size;
	
	// index of their start in the phrase
	int ai = ((suffix*)a)->index;
	int bi = ((suffix*)b)->index;

	int i = 0;
	// compare characters one by one
	for (; i + ai < as && i + bi < bs; i++){
		diff = ac[i + ai] - bc[i + bi];
		if (diff != 0)
			break;
	}
	// a is later in the alphabet
	if ( diff > 0 )
		return 1;

	// b is later in the alphabet
	if ( diff < 0 )
		return -1;

	// they have the same length and letters
	if ( as - ai == bs - bi )
		return 0;

	// a is shorter
	if ( as - i - ai == 0)
		return -1;

	// b is shorter
	return 1;
}



/////////////////////////////////////////////////////////////
// READING DATA
/////////////////////////////////////////////////////////////


// reads phrase frequency and the delimiter
// takes a pointer to a phrase
// returns 0 if failed, 1 if ok
int readFrequency(phrase *phrase){

	char delimiter = ' ';
	int chars_read = scanf("%lf %c", &(phrase->frequency), &delimiter);

	// reading was unsuccessful
	if (chars_read != 2 || delimiter != ':')
		return 0;

	return 1;
}


// reads the content of a phrase
// takes a pointer to a phrase
// returns 0 if failed, 1 if ok
int readPhrase(phrase *phrase){

	size_t s = 20;
	phrase->content = (char*) malloc( s * sizeof(phrase->content[0]) );
	phrase->size = getline(&(phrase->content), &s, stdin);

	// nothing was read, wrong input
	if (phrase->size == EOF)
		return 0;

	// doesn't include newline for future processing
	phrase->size--;

	// creates and fills an array with the phrase in lowercase
	phrase->lowercase_content = (char*) malloc( phrase->size * sizeof(phrase->content[0]) );

	for (int i = 0; i < phrase->size; i++)
		phrase->lowercase_content[i] = tolower(phrase->content[i]);

	return 1;
}


// returns the number of read phrases
// takes in a pointer to an empty array of phrases
// and returns a filled array
int readPhrases(phrase *phrases[]){
	printf("Casto hledane fraze:\n");

	int allocated_phrases = 8;

	// phrases array that gets returned through the input parameter
	phrase *ph_arr = (phrase*) malloc(allocated_phrases * sizeof(ph_arr[0]));

	int i = 0;
	while (1){
		
		// checks for end of phrases
		char c = ' ';
		if (scanf("%c", &c) && c == '\n'){

			// frees ph_arr and returns 0 if no lines were read
			if (i == 0)
				break;

			// returns ph_arr and the number of phrases read
			*phrases = ph_arr;
			return i;
		}
		ungetc(c, stdin);

		// reallocates ph_arr if needed
		if (allocated_phrases == i){
			allocated_phrases *= 1.5;
			ph_arr = (phrase*) realloc(ph_arr, allocated_phrases * sizeof(ph_arr[0]));
		}

		if (!readFrequency(ph_arr + i))
			break;

		if (!readPhrase(ph_arr + i++))
			break;
	}

	// only happens with wrong input
	freePhrases(ph_arr, i);

	return 0;
}



/////////////////////////////////////////////////////////////
// THE DATA STRUCTURE
/////////////////////////////////////////////////////////////


// creates and sorts suffixes
// returns the count of all suffixes
int createSuffixes(phrase *phrases, int n, suffix **suffixes){

	// counts the total amount of all suffixes
	int total_chars = 0;
	for (int p = 0; p < n; p++)
		total_chars += phrases[p].size;

	// allocates space for suffixes
	*suffixes = (suffix*) malloc(total_chars * sizeof((*suffixes)[0]));

	// sets attributes to all suffixes
	int s = 0;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < phrases[i].size; j++){

			(*suffixes)[s].index = j;
			(*suffixes)[s].phrase_ = phrases + i;
			s++;
		}
	}

	// sorts the suffixes
	qsort(*suffixes, total_chars, sizeof((*suffixes)[0]), cmpSuffixes);

	return total_chars;
}


/////////////////////////////////////////////////////////////
// THE ALGORITHM
/////////////////////////////////////////////////////////////


// returns suffixes' index if it finds a matching suffix
// returns -1 if it doesn't
int binarySearch(suffix *suffixes, int m, char *query, int query_length){
	int l = 0;
	int r = m;
	int mid, difference = 0;

	while (l < r){
		mid = (l + r) / 2;

		for (int i = 0; i + suffixes[mid].index < suffixes[mid].phrase_->size && i < query_length; i++){
			// 0 == chars are equal
			// + == current suffix is after the query in the alphabet
			// - == current suffix is before the query in the alphabet
			difference = suffixes[mid].phrase_->lowercase_content[suffixes[mid].index + i] - query[i];

			if (difference != 0)
				break;
		}

		// current suffix is alphabetically after query
		if (difference > 0)
			r = mid;

		// current suffix is alphabetically before query
		else if (difference < 0)
			l = mid + 1;

		// suffix matches, but it is too short
		else if (suffixes[mid].index + query_length > suffixes[mid].phrase_->size)
			l = mid + 1;

		// suffix matches the query
		else
			return mid;
	}
	return -1;
}


// takes the index of a suffix that matches the query and finds other matching suffixes around it
int findAllPhrases(suffix *suffixes, int m, int *found_phrases, int n, int s_i, char *query, int query_length){
	
	found_phrases[(int) suffixes[s_i].phrase_->frequency] = 1;
	int phrases_found = 1;

	// checks for suffixes before s_i, that match the query
	int isdiff = 0;
	for (int i = s_i - 1; i >= 0; i--){

		// index the phrase that the current suffix is from
		int pi = (int) suffixes[i].phrase_->frequency;
		// content of the phrase
		char *pc = suffixes[i].phrase_->lowercase_content;
		// where the current suffix starts in a phrase
		int si = suffixes[i].index;


		// checks if the phrase of the current suffix is already detected
		if (found_phrases[pi] != 1){

			// checks if the suffix is big enough
			if (query_length + si > suffixes[i].phrase_->size)
				break;

			// compares the suffix and the query
			for (int j = 0; j < query_length; j++)
				if (pc[si + j] != query[j]){
					isdiff = 1;
					break;
				}

			// stops if they are different
			if (isdiff)
				break;

			// adds the phrase to other already detected phrases
			found_phrases[pi] = 1;
			phrases_found++;
		}
	}

	// checks for suffixes after s_i, that match the query
	isdiff = 0;
	for (int i = s_i + 1; i < m; i++){

		// index the phrase that the current suffix is from
		int pi = (int) suffixes[i].phrase_->frequency;
		// content of the phrase
		char *pc = suffixes[i].phrase_->lowercase_content;
		// where the current suffix starts in a phrase
		int si = suffixes[i].index;


		// checks if the phrase of the current suffix is already detected
		if (found_phrases[pi] != 1){

			// checks if the suffix is big enough
			if (query_length + si > suffixes[i].phrase_->size)
				break;

			// compares the suffix and the query
			for (int j = 0; j < query_length; j++)
				if (pc[si + j] != query[j]){
					isdiff = 1;
					break;
				}

			// stops if they are different
			if (isdiff)
				break;

			// adds the phrase to other already detected phrases
			found_phrases[pi] = 1;
			phrases_found++;
		}
	}
	return phrases_found;
}


// read a query and returns the number of chars read or EOF
int readQuery(char **query, size_t *s){
	// reads the query
	int chars_read = getline(query, s, stdin);

	// checks for the end of the file
	if (chars_read == EOF)
		return EOF;
	
	// doesn't include newline in the query
	if (chars_read > 0 && (*query)[chars_read-1] == '\n')
		chars_read--;

	for (int i = 0; i < chars_read; i++)
		(*query)[i] = tolower((*query)[i]);

	return chars_read;
}


// reads queries and prints out up to 50 phrases containing a given query
void resolveQueries(phrase *phrases, suffix *suffixes, int n, int m){
	// allocates space for a query
	size_t s = 20;
	char *query = (char*) malloc( s * sizeof(query[0]) );

	// each index represents a phrase
	// 1 if it contains a given query, 0 otherwise
	int *found_phrases  = (int*) calloc( n, sizeof(found_phrases[0]) );

	printf("Hledani:\n");
	while (1){

		// reads the query
		int query_length = readQuery(&query, &s);
		if (query_length == EOF)
			break;

		// gets the index of a matching suffix
		int s_i = binarySearch(suffixes, m, query, query_length);

		// goes to next for loop if there isn't
		if (s_i == -1){
			printf("Nalezeno: 0\n");
			continue;
		}

		// finds all other phrases around the matching suffix
		int phrases_found = findAllPhrases(suffixes, m, found_phrases, n, s_i, query, query_length);

		// prints out all found phrases
		int printed = 0;
		printf("Nalezeno: %d\n", phrases_found);
		for (int i = 0; i < n && printed < 50; i++)
			if (found_phrases[i]){
				printPhrase(phrases + i);
				printed++;
			}


		// resets the array of found phrases
		for (int i = 0; i < n; i++)
			found_phrases[i] = 0;
	}
	free(query);
	free(found_phrases);
}


int main(){
	phrase *phrases = NULL;
	suffix *suffixes = NULL;

	// the number of phrases
	int n = readPhrases(&phrases);
	
	// the number of read phrases is 0
	if (!n)
		WRONG_INPUT

	// sorts phrases by frequency
	qsort(phrases, n, sizeof(phrases[0]), cmpFrequencies);

	// converts changes ranks to go from 0 to n
	for (int i = 0; i < n; i++)
		phrases[i].frequency = i;

	// the number of all suffixes
	int m = createSuffixes(phrases, n, &suffixes);

    // answers queries
    resolveQueries(phrases, suffixes, n, m);

    freePhrases(phrases, n);
	free(suffixes);
}
