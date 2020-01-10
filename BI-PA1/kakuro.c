#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>

#define WRONG_INPUT {printf("Nespravny vstup.\n"); return 1;}

struct WHITE;
struct BLACK;

typedef struct WHITE {
	short x;
	short y;
	BLACK *b[2];
	short vals;
	short val;
	char saved;
} white;


typedef struct BLACK {
	short index;
	short sets[12];
	char n_sets;
	WHITE *whites[9];
	char n_whites;
	short mask;
	short vals;
	short sum;
	short clue;
	char saved;
} black;


typedef struct {
	white whites[32][32];
	short w, h;
	black blacks[32 * 32];
	short n_blacks;
} field;


typedef struct {
	short vals;
	short x;
	short y;
} w_b;


typedef struct {
	black b;
	short index;
} b_b;


typedef struct {
	w_b *data;
	short n_saved;
	short n_allocated;
} white_backup;


typedef struct {
	b_b *data;
	short n_saved;
	short n_allocated;
} black_backup;


void pb(short vals){
	for (short i = 0; i < 9; i ++, vals >>= 1)
		if (vals & 1){
			printf("%d", i + 1);
			if (vals > 1)
				printf("+");
		}
	printf(", ");
}


void pField(field *f){
	for (short y = 0; y < f->h; y++){
		for (short x = 0; x < f->w; x++){
			char b[20] = {};
			short j = 0;
			white w = f->whites[x][y];
			if (w.val == 10000)
				snprintf(b, 7, "X");
			else if (w.val == 10001)
				snprintf(b, 7, "X\\%d", w.y);
			else if (w.val == 10002)
				snprintf(b, 7, "%d\\X", w.x);
			else if (w.val == 10003)
				snprintf(b, 7, "%d\\%d", w.x, w.y);
			else{
				short vals = w.vals;
				b[j++] = '(';
				for (short i = 0; i < 9; i ++, vals >>= 1)
					if (vals & 1)
						b[j++] = i + 1 + '0';
				b[j++] = ')';
			}
			printf("%-12s", b);
		}
		printf("\n");
	}
	printf("--------------------------------------------\n");
}


int un(char c){
	ungetc(c, stdin);
	return 0;
}


short is_nl(){
	char c, d;
	if (scanf("%c%c", &c, &d) == 1 && c == '\n')
		return 2;
	if (c == '\n')
		return un(d) || 1;
	return un(d) || un(c);
}

short setWhite(field *f, white *w, short x, short y, short pb_v, short pb_h){
	if (!(pb_v + 1) || !(pb_h + 1) ||
		f->blacks[pb_v].n_whites == 9 || f->blacks[pb_h].n_whites == 9)
		return 0;

	w->vals = 0b1111111111;
	w->b[0] = &f->blacks[pb_v];
	w->b[1] = &f->blacks[pb_h];
	w->x = x;
	w->y = y;

	f->blacks[pb_v].whites[f->blacks[pb_v].n_whites++] = w;
	f->blacks[pb_h].whites[f->blacks[pb_h].n_whites++] = w;

	return 1;
}

int readCell(field *f, short x, short y, short *pb_v, short *pb_h){
	white *w = &f->whites[x][y];
	black *b1 = &f->blacks[f->n_blacks];
	black *b2 = &f->blacks[f->n_blacks + 1];

	char c;
	scanf(" %c", &c);

	if (c == '.')
		return setWhite(f, w, x, y, *pb_v, *pb_h);

	if ((*(pb_v) + 1 && f->blacks[*pb_v].n_whites == 0) ||
		(*(pb_h) + 1 && f->blacks[*pb_h].n_whites == 0))
		return 0;

	if (c == 'X'){
		char c2;
		*pb_v = -1;
		if (scanf("%c", &c2) && (un(c2) || c2 == ' ' || c2 == '\n')){
			*pb_h = -1;
			w->val = 10000;
			return 1;
		}
		if (scanf("\\%hd", &b1->clue) && b1->clue > 0 && b1->clue < 46){
			b1->index = f->n_blacks;
			*pb_h = f->n_blacks;
			w->val = 10001;
			w->y = b1->clue;
			f->n_blacks++;
			return 1;
		}
	}
	un(c);
	if (scanf("%hd\\", &b1->clue) && b1->clue > 0 && b1->clue < 46){
		b1->index = f->n_blacks;
		*pb_v = f->n_blacks;
		w->x = b1->clue;
		f->n_blacks++;
		if (scanf("%c", &c) && c == 'X'){
			w->val = 10002;
			return 1;
		}
		un(c);
		if (scanf("%hd", &b2->clue) && b2->clue > 0 && b2->clue < 46){
			b2->index = f->n_blacks;
			*pb_h = f->n_blacks;
			w->val = 10003;
			w->y = b2->clue;
			f->n_blacks++;
			return 1;
		}
	}
	
	return 0;
}


short checkEnd(field *f, short *x, short *y, short *pb_h){
	int nl = is_nl();
	
	if (nl){
		if ((f->w && *x != f->w)
			|| f->whites[*x - 1][*y].val == 10001 || f->whites[*x - 1][*y].val == 10003
			|| (nl == 1 && *y == 31))
			return 1;
		f->w = *x;
		*pb_h = -1;
		*x = 0;
		(*y)++;
		if (nl == 2){
			f->h = *y;
			short b = 0;
			for (short i = 0; i < f->w; i++)
				b += (f->whites[i][*y - 1].val == 10002 || f->whites[i][*y - 1].val == 10003);
			return !b + 1;
		}
		return 0;
	}
	return *x == 32;
}


short readField(field *f){
	short x = 0;
	short y = 0;

	short pb_v[32] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	short pb_h[1] = {-1};

	while (1){
		int e;
		if ((e = checkEnd(f, &x, &y, &pb_h[0])))
			return e - 1;

		if (!readCell(f, x, y, &pb_v[x], &pb_h[0]))
			return 0;
		x++;
	}
}




void backup_w(white_backup *wb, white *w){
	if (!w->saved && wb){
		if (wb->n_saved == wb->n_allocated)
			wb->data = (w_b*) realloc(wb->data, (wb->n_allocated *= 1.5) * sizeof(wb->data[0]));
		wb->data[wb->n_saved].vals = w->vals;
		wb->data[wb->n_saved].x = w->x;
		wb->data[wb->n_saved].y = w->y;
		wb->n_saved++;
		w->saved = 1;
	}
}


void backup_b(black_backup *bb, black *b){
	if (!b->saved && bb){
		if (bb->n_saved == bb->n_allocated)
			bb->data = (b_b*) realloc(bb->data, (bb->n_allocated *= 1.5) * sizeof(bb->data[0]));
		bb->data[bb->n_saved].b = *b;
		bb->data[bb->n_saved].index = b->index;
		bb->n_saved++;
		b->saved = 1;
	}
}


void rm_child(black *b, white *w){
	for (short i = 0; i < b->n_whites; i++)
		if(b->whites[i] == w){
			b->whites[i] = b->whites[--b->n_whites];
			break;
		}
}


void rm_set(black *b, short index){
	b->sets[index] = b->sets[--b->n_sets];
}

short b1(black *b, white_backup *wb, black_backup *bb);


short r2(black *b, white *w, white_backup *wb, black_backup *bb){
	short b_vals = 0;
	for (short i = 0; i < b->n_sets; i++){
		if ((b->sets[i] & w->vals) == 0){
			backup_b(bb, b);
			rm_set(b, i--);
		}
		else
			b_vals |= b->sets[i];
	}
	if (b_vals != b->vals){
		b->vals = b_vals;
		if(!b1(b, wb, bb))
			return 0;
		return 2;
	}
	return !!b_vals;
}


short is_unique(white *w){
	switch (w->vals){
		case 1: return 1;
		case 2: return 2;
		case 4: return 3;
		case 8: return 4;
		case 16: return 5;
		case 32: return 6;
		case 64: return 7;
		case 128: return 8;
		case 256: return 9;
		default: return 0;
	}
}


int unique(white *w, short val, white_backup *wb, black_backup *bb){
	if ((w->b[0]->n_whites == 1 && w->b[0]->sum + val != w->b[0]->clue) ||
		(w->b[1]->n_whites == 1 && w->b[1]->sum + val != w->b[1]->clue))
		return 0;
	backup_b(bb, w->b[0]);
	backup_b(bb, w->b[1]);
	rm_child(w->b[0], w);
	rm_child(w->b[1], w);
	w->b[0]->mask &= ~w->vals;
	w->b[1]->mask &= ~w->vals;
	w->b[0]->sum += val;
	w->b[1]->sum += val;
	short r1;
	if (!(r1 = r2(w->b[0], w, wb, bb)))
		return 0;
	if (!(r1 - 1) && !b1(w->b[0], wb, bb))
		return 0;

	short r1_b;
	if (!(r1_b = r2(w->b[1], w, wb, bb)))
		return 0;
	if (!(r1_b - 1) && !b1(w->b[1], wb, bb))
		return 0;
	return 1;
}



short b1(black *b, white_backup *wb, black_backup *bb){
	for (short i = 0; i < b->n_whites; i++){
		white *w = b->whites[i];
		black *b2 = w->b[b == w->b[0]];

		short intersection = b->mask & b->vals & b2->mask & b2->vals;

		if (!intersection)
			return 0;
		if (w->vals != intersection){
			backup_w(wb, w);
			w->vals = intersection;

			short val;
			if ((val = is_unique(w)))
				return unique(w, val, wb, bb);

			if (!r2(b2, w, wb, bb))
				return 0;
		}
	}
	return 1;
}


int initField(field *f){
	short u[32][32] = {};
	for (short y = 0; y < f->h; y++)
		for (short x = 0; x < f->w; x++)
			if (f->whites[x][y].val < 10000){
				if (!(f->whites[x][y].vals = f->whites[x][y].b[0]->vals & f->whites[x][y].b[1]->vals))
					return 0;
				u[x][y] = is_unique(&f->whites[x][y]);
			}

	for (short y = 0; y < f->h; y++)
		for (short x = 0; x < f->w; x++)
			if (u[x][y])
				if(!unique(&f->whites[x][y], u[x][y], NULL, NULL))
					return 0;
	return 1;
}


short maxb;
short maxw;


void whiten(field *f, white_backup *wb, black_backup *bb){
	maxb = maxb > bb->n_saved ? maxb : bb->n_saved;
	maxw = maxw > wb->n_saved ? maxw : wb->n_saved;
	for (short i = 0; i < wb->n_saved; i++)
		f->whites[wb->data[i].x][wb->data[i].y].saved = 0;
	for (short i = 0; i < bb->n_saved; i++)
		f->blacks[bb->data[i].index].saved = 0;
}


void restore(field *f, white_backup *wb, black_backup *bb){
	for (short i = 0; i < wb->n_saved; i++){
		f->whites[wb->data[i].x][wb->data[i].y].vals = wb->data[i].vals;
		f->whites[wb->data[i].x][wb->data[i].y].saved = 0;
	}
	for (short i = 0; i < bb->n_saved; i++)
		f->blacks[bb->data[i].index] = bb->data[i].b;

	wb->n_saved = 0;
	bb->n_saved = 0;
}


void fillVal(field *f){
	for (short y = 0; y < f->h; y++)
		for (short x = 0; x < f->w; x++)
			for (short tmp = f->whites[x][y].vals; tmp; tmp >>= 1)
				f->whites[x][y].val++;
}


void pSol(field *f){
	printf("Kakuro ma jedno reseni:\n");
	for (short y = 0; y < f->h; y++){
		for (short x = 0; x < f->w; x++){
			char b[7];
			white w = f->whites[x][y];
			if (w.val == 10000)
				snprintf(b, 7, "X");
			else if (w.val == 10001)
				snprintf(b, 7, "X\\%d", w.y);
			else if (w.val == 10002)
				snprintf(b, 7, "%d\\X", w.x);
			else if (w.val == 10003)
				snprintf(b, 7, "%d\\%d", w.x, w.y);
			else
				snprintf(b, 7, "%d", w.val);
			if (x + 1 != f->w)
				printf("%-6s", b);
			else
				printf("%s", b);
		}
		printf("\n");
	}
}


void solve_r(field *f, short x, short y, int *n_solutions){
	white *w = &f->whites[x][y];	
	if (w->val > 9999 || is_unique(w))	
		return solve_r(f, (x+1) % f->w, y + (x + 1 == f->w), n_solutions);	
	
	if (y == f->h){	
		if (!*n_solutions)	
			fillVal(f);	
		(*n_solutions)++;	
		return;
	}

	white_backup wb;
	wb.n_saved = 0;
	wb.n_allocated = maxw;
	wb.data = (w_b*) malloc( wb.n_allocated * sizeof(wb.data[0]) );

	black_backup bb;
	bb.n_saved = 0;
	bb.n_allocated = maxb;
	bb.data = (b_b*) malloc( bb.n_allocated * sizeof(bb.data[0]) );

	for (short i = 0, tmp = w->vals; tmp; i++, tmp >>= 1){
		if (tmp & 1){
			backup_w(&wb, w);
			w->vals = pow(2, i);
			if (unique(w, i + 1, &wb, &bb)){
				whiten(f, &wb, &bb);
				solve_r(f, (x+1) % f->w, y + (x+1 == f->w), n_solutions);
			}
			restore(f, &wb, &bb);
		}		
	}
	free(wb.data);
	free(bb.data);
}


short setBlack(black *b){
	for (short k = 0; k < 512; k++){
		short n_bits = 0;
		short sum = 0;
		short a = k;
		for (short l = 0; a; a >>= 1, l++){
			n_bits += a & 1;
			sum += (l+1) * (a & 1);
		}
		if (sum == b->clue && n_bits == b->n_whites){
			b->sets[b->n_sets++] = k;
			b->vals |= k;
		}
	}
	b->mask = 0b111111111;
	return b->vals;
}


short initBlacks(field *f){	
	for (int i = 0; i < f->n_blacks; i++)
		if (!setBlack(&f->blacks[i]))
			return 0;
	return 1;
}


void solve(field *f){
	if (!initBlacks(f) || !initField(f)){
		printf("Reseni neexistuje.\n");
		return;
	}
	int n_solutions = 0;
	solve_r(f, 0, 0, &n_solutions);
	if (!n_solutions)
		printf("Reseni neexistuje.\n");
	else if (n_solutions == 1)
		pSol(f);
	else
		printf("Celkem ruznych reseni: %d\n", n_solutions);
}


int main(){
	printf("Zadejte kakuro:\n");
	field f = {};

	if (!readField(&f) || !f.w)
		WRONG_INPUT

	f.whites[f.h][0].vals = 3;
	maxw = 18;
	maxb = 12 > f.n_blacks ? f.n_blacks : 12;


	solve(&f);
}