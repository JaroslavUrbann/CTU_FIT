#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


#define WRONG_INPUT {printf("Nespravny vstup.\n"); return 1;}
#define bool int
#define true 1
#define false 0


typedef struct BOARD {
	// board dimensions
	int width;
	int height;
	// pieces of a board made by a cut
	BOARD *child1;
	BOARD *child2;
} board;


typedef struct CACHE{
	// width of the 2D matricies
	int width;
	// height of the 2D matricies
	int height;
	// 2D matrix with counts of cuts for a corresponding board in the matrix
	short int **cut_counts;
	// 2D matrix of cached boards
	board **boards;
} cache;


int cutBoard(board *b, int max_area, cache *c);


// reads dimensions of the main board
int readInput(board *main_board, int *max_area){

	printf("Velikost:\n");
	if (scanf("%d%d", &(main_board->width), &(main_board->height)) != 2 
		|| main_board->width < 1 
		|| main_board->height < 1)
		return 0;

	printf("Maximalni plocha:\n");
	if (scanf("%d", max_area) != 1 || *max_area < 1)
		return 0;

	main_board->child1 = NULL;
	main_board->child2 = NULL;

	return 1;
}

// recursively frees a tree of boards
void freeChildren(board *b, cache *c, bool free_cache){
	if (b->child1 != NULL && (b->child1->width >= c->width ||  b->child1->height >= c->height)){
		// printf("hahb\n");
		freeChildren(b->child1, c, free_cache);
		free(b->child1);
		b->child1 = NULL;
	}
	
	if (b->child2 != NULL && (b->child2->width >= c->width ||  b->child2->height >= c->height)){

		// printf("c1 w %d h %d\n", b->child1->width, b->child1->height);

		freeChildren(b->child2, c, free_cache);
		// printf("b w %d h %d\n", b->width, b->height);
		// printf("c2 w %d h %d\n", b->child2->width, b->child2->height);
		free(b->child2);
		b->child2 = NULL;
	}
}


// takes in dimensions of the leftover pieces of a cut board (child1 and child2) 
// and calculates the total amount of cuts of the current board
void updateBest(int width1, 
				int width2, 
				int height1, 
				int height2, 
				board **best_child1, 
				board **best_child2, 
				int *min_cuts, 
				int max_area,
				cache *c){

	board tmp_child1;
	tmp_child1.width = width1;
	tmp_child1.height = height1;
	tmp_child1.child1 = NULL;
	tmp_child1.child2 = NULL;

	board tmp_child2;
	tmp_child2.width = width2;
	tmp_child2.height = height2;
	tmp_child2.child1 = NULL;
	tmp_child2.child2 = NULL;

	// cuts needed to split child1 and child2
	int cuts1;
	int cuts2;

	if (width1 < c->width && height1 < c->height){
		if (c->cut_counts[height1][width1] != -1){
			cuts1 = c->cut_counts[height1][width1];
		}
		else{
			cuts1 = cutBoard(&tmp_child1, max_area, c);
			c->cut_counts[height1][width1] = cuts1;
			c->boards[height1][width1] = tmp_child1;
		}
	}
	else
		cuts1 = cutBoard(&tmp_child1, max_area, c);

	if (width2 < c->width && height2 < c->height){
		if (c->cut_counts[height2][width2] != -1){
			cuts2 = c->cut_counts[height2][width2];
		}
		else{
			cuts2 = cutBoard(&tmp_child2, max_area, c);
			c->cut_counts[height2][width2] = cuts2;
			c->boards[height2][width2] = tmp_child2;
		}
	}
	else{
		cuts2 = cutBoard(&tmp_child2, max_area, c);
		// printf("gotem w %d h %d\n", tmp_child2.width, tmp_child2.height);
	}

	// calculates the total amount of cuts of the current board
	int total_cuts = cuts1 + cuts2 + 1;

	// checks if that amount is less than the current best
	if (total_cuts < *min_cuts){
		// replaces the best children with the current ones
		if (*min_cuts != INT_MAX){
			freeChildren(*best_child1, c, false);
			freeChildren(*best_child2, c, false);
		}
		if (width1 < c->width && height1 < c->height){
			if (*min_cuts == INT_MAX || (**best_child1).width >= c->width || (**best_child1).height >= c->height )
				free(*best_child1);
			*best_child1 = &(c->boards[height1][width1]);
		}
		else{
			if (*min_cuts != INT_MAX && (**best_child1).width < c->width && (**best_child1).height < c->height )
				*best_child1 = (board*) malloc(sizeof(**best_child1));
			**best_child1 = tmp_child1;
		}
		
		if (width2 < c->width && height2 < c->height){
			if (*min_cuts == INT_MAX || (**best_child2).width >= c->width || (**best_child2).height >= c->height )
				free(*best_child2);
			*best_child2 = &(c->boards[height2][width2]);
		}
		else{
			if (*min_cuts != INT_MAX && (**best_child2).width < c->width && (**best_child2).height < c->height )
				*best_child2 = (board*) malloc(sizeof(**best_child2));
			**best_child2 = tmp_child2;
		}

		*min_cuts = total_cuts;
	}
	else{
		// otherwise frees the current children
		freeChildren(&tmp_child1, c, false);
		freeChildren(&tmp_child2, c, false);
	}
}


// recursive function that returns the minimum amount of cuts that need to be taken
int cutBoard(board *b, int max_area, cache *c){
	// returns 0 if the current boards' area is less than the max_area
	// and if its' sides are at most 2:1 ratio and at least 1:2 ratio
	if (b->width * b->height <= max_area 
		&& 2 * b->width >= b->height 
		&& 2 * b->height >= b->width)
		return 0;

	// allocates space for split boards
	b->child1 = (board*) malloc(sizeof(b->child1[0]));
	b->child2 = (board*) malloc(sizeof(b->child2[0]));

	// current minimum of cuts needed
	int min_cuts = INT_MAX;

	// always splits the bigger side
	if (b->width >= b->height){
		// checking the number for cuts for all possible new board widths (w)
		for (int w = b->width / 2; w > 0; w--){
			updateBest(w,
					b->width - w, 
					b->height, 
					b->height, 
					&(b->child1),
					&(b->child2),
					&min_cuts,
					max_area,
					c);

			// breaks after trying a width that would make for a smaller area than max_area and
			// before trying a width that would make for a board with lower ratio than 1:2
			if (w * b->height < max_area && w <= 2 * b->height)
				break;
		}
		// checking the number for cuts for all possible new board heights (h)
		for (int h = b->height / 2; h > 0; h--){
			updateBest(b->width,
					b->width, 
					h, 
					b->height - h, 
					&(b->child1),
					&(b->child2),
					&min_cuts,
					max_area,
					c);

			// breaks after trying a height that would make for a smaller area than max_area and
			// before trying a height that would make for a board with higher ratio than 2:1
			if (h * b->width < max_area && h <= 2 * b->width)
				break;
		}
	}
	else{
		// checking the number for cuts for all possible new board heights (h)
		for (int h = b->height / 2; h > 0; h--){
			updateBest(b->width,
					b->width, 
					h, 
					b->height - h, 
					&(b->child1),
					&(b->child2),
					&min_cuts,
					max_area,
					c);

			// breaks after trying a height that would make for a smaller area than max_area and
			// before trying a height that would make for a board with higher ratio than 2:1
			if (h * b->width < max_area && h <= 2 * b->width)
				break;
		}
		// checking the number for cuts for all possible new board widths (w)
		for (int w = b->width / 2; w > 0; w--){
			updateBest(w,
					b->width - w, 
					b->height, 
					b->height, 
					&(b->child1),
					&(b->child2),
					&min_cuts,
					max_area,
					c);

			// breaks after trying a width that would make for a smaller area than max_area and
			// before trying a width that would make for a board with lower ratio than 1:2
			if (w * b->height < max_area && w <= 2 * b->height)
				break;
		}
	}
	// printf( "Address of p1: %p\n", ( void * )b->child1 );
	return min_cuts;
}


// recursive function that returns the maximum depth of a board tree
int getTreeDepth(board *b){
	// returns 0 if its a leaf node
	if (b == NULL)
		return 0;

	// gets the depth of children
	int child1_depth = getTreeDepth(b->child1);
	int child2_depth = getTreeDepth(b->child2);

	// returns the depth of the deeper one and adds 1
	return (child1_depth > child2_depth ? child1_depth : child2_depth) + 1;
}


// recursive function that prints out all children of a board
void printChildren(board *b, bool *printed_depth, int depth){
	// returns when its a leaf node
	if (b == NULL)
		return;

	// prints "|" or " " for each depth
	// (depending on whether both children at that depth have been printed)
	for (int i = 0; i < depth - 1; i++){
		if (printed_depth[i])
			printf(" ");
		else
			printf("|");
		printf(" ");
	}

	// prints \- if its a second child, otherwise prints +-
	if (depth > 0){
		if (printed_depth[depth - 1])
			printf("\\-");
		else
			printf("+-");
	}

	// prints the current board
	printf("[%d,%d]\n", b->width, b->height);

	// sets the printed_depth and prints all children
	printed_depth[depth] = false;
	printChildren(b->child1, printed_depth, depth + 1);

	printed_depth[depth] = true;
	printChildren(b->child2, printed_depth, depth + 1);
}


// function for printing a tree of boards
void printTree(board *main_board){
	int max_depth = getTreeDepth(main_board);

	// true if the children at that depth have been printed
	// false if not
	bool *printed_depth = (int*) calloc( max_depth, sizeof(printed_depth[0]) );

	// prints the tree
	printChildren(main_board, printed_depth, 0);

	free(printed_depth);
}


void calculateCuts(board *main_board, int max_area){

	// how much does the board need to be downscaled (aproximately)
	int downscale_factor = main_board->width * main_board->height / max_area + 1;

	cache c;
	c.width = downscale_factor > main_board->width ? main_board->width + 1 : downscale_factor;
	c.height = downscale_factor * downscale_factor / c.width > main_board->height ? main_board->height + 1 : downscale_factor * downscale_factor / c.width;

	// printf("cw %d, ch %d\n", c.width, c.height);

	c.boards = (board**) malloc( c.height * sizeof(c.boards[0]) );
	c.cut_counts = (short int**) malloc( c.height * sizeof(c.cut_counts[0]) );

	for (int h = 0; h < c.height; h++){
		c.boards[h] = (board*) malloc( c.width * sizeof(c.boards[h][0]) );
		c.cut_counts[h] = (short int*) malloc( c.width * sizeof(c.cut_counts[h][0]) );
		for (int w = 0; w < c.width; w++)
			c.cut_counts[h][w] = -1;
	}
	int min_cuts = cutBoard(main_board, max_area, &c);
	printf("Rezu: %d\n", min_cuts);

	printTree(main_board);

	freeChildren(main_board, &c, false);

	for (int h = 0; h < c.height; h++){
		// for (int w = 0; w < c.width; w++)
			// if (c.cut_counts[h][w] != -1){
				// printf( "Address o: %p\n", ( void * )&(c.boards[h][w]) );
				// free(&(c.boards[h][w]));
			// }
		free(c.boards[h]);
		free(c.cut_counts[h]);
	}
	free(c.boards);
	free(c.cut_counts);


}


int main(){
	board main_board;
	int max_area;
	
	// reads the input
	if (!readInput(&main_board, &max_area))
		WRONG_INPUT

	// gets the minimum amount of cuts needed
	calculateCuts(&main_board, max_area);
	
	// prints the tree of boards

	
	// frees the tree of boards
}
