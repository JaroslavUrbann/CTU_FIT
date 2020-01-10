#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define WRONG_INPUT {printf("Nespravny vstup.\n"); return 1;}

// refering to airplanes as points with names :)
typedef struct {
	double x, y;
	int name_index;
} point;


typedef struct {
	double distance;
	int *planes;
	int n_planes;
	int allocated_planes;
} shortest_distance;


int readPoint(point **points, int *pts_allocated, int *point_index){
	char c;
	// printf("f index %d allocated %d\n", *point_index, *pts_allocated);
	// resizes points array if necessary
	if (*point_index == *pts_allocated)
		*points = (point*) realloc(*points, (*pts_allocated*=1.5) * sizeof(*points[0]) );

	// printf("index %d allocated %d\n", *point_index, *pts_allocated);

	// reads left bracket
	while (scanf("%c", &c) != EOF && (c == ' ' || c == '\t')) {}
	if (c != '[')
		return 0;

	// reads X
	while (scanf("%c", &c) != EOF && (c == ' ' || c == '\t')) {}
	if ((c < '0' || c > '9') && c != '-' && c != '.')
		return 0;
	ungetc(c, stdin);

	if (scanf("%lf", &(*points + *point_index)->x) != 1)
		return 0;

	// reads comma
	while (scanf("%c", &c) != EOF && (c == ' ' || c == '\t')) {}
	if (c != ',')
		return 0;

	// reads Y
	while (scanf("%c", &c) != EOF && (c == ' ' || c == '\t')) {}
	if ((c < '0' || c > '9') && c != '-' && c != '.')
		return 0;
	ungetc(c, stdin);

	if (scanf("%lf", &(*points + *point_index)->y) != 1)
		return 0;

	// reads right bracket
	while (scanf("%c", &c) != EOF && (c == ' ' || c == '\t')) {}
	if (c != ']')
		return 0;

	(*point_index)++;
	return 1;
}


int readName(char **names, int *chars_allocated, int *name_index){

	// reads whitespaces before name and the first character
	char c = ' ';
	while (scanf("%c", &c) != EOF && (c == ' ' || c == '\t')) {}
	if (c != ' ' && c != '\t')
		ungetc(c, stdin);

	// reads name
	while (1){
		if (*name_index == *chars_allocated)
			*names = (char*) realloc(*names, (*chars_allocated*=1.5) * sizeof(*names[0]) );

		if (scanf("%c", *names + *name_index) == EOF){
			*(*names + (*name_index)++) = '\n';
			break;
		}

		if (*(*names + (*name_index)++) == '\n')
			break;
	}
	return 1;
}


int getData(point *points_arr[], char *names_arr[], int *n_points){
	printf("Zadejte lety:\n");

	int pts_allocated = 4;
	int chars_allocated = 50;

	// array of points
	point *points = (point*) malloc( pts_allocated * sizeof(points[0]) );

	// array of names of points
	char *names = (char*) malloc( chars_allocated * sizeof(names[0]) );


	int point_index = 0;
	int name_index = 0;
	while (1){
		// checks for EOF
		char c;
		if (scanf("%c", &c) == EOF){
			if (point_index < 2)
				break;
			*points_arr = points;
			*names_arr = names;
			*n_points = point_index;
			return 1;
		}
		ungetc(c, stdin);

		if (!readPoint(&points, &pts_allocated, &point_index))
			break;

		points[point_index-1].name_index = name_index;

		if (!readName(&names, &chars_allocated, &name_index))
			break;
	}
	free(points);
	free(names);
	return 0;
}


void printP(point p, char *names){
	int i = p.name_index;
	printf("x: %f y: %f index: %d name: ", p.x, p.y, i);
	do{
		printf("%c", names[i]);
	} while (names[i++] != '\n');
}


void printPts(point *points, char *names, int n_points){
	for (int i = 0; i < n_points; i++)
		printP(points[i], names);
}


//-----------------------------------------------------------------
// the actual algorithm: (explanation: https://www.youtube.com/watch?v=3pUOv_ocJyA)
//-----------------------------------------------------------------


int isEqual(double a, double b){
	// float xd = FLT_EPSILON * fmax(a, b);
	// printf("%.30f\n", 0.00000005 * fmax(a, b));
	// printf("%.30f\n", FLT_EPSILON * fmax(a, b));
	// printf("%.30f\n", fabs(a - b));
	// printf("%.30f\n", xd);
	// printf("----------\n");
	int i = fabs(a - b) <= FLT_EPSILON * fmax(a, b);
	// printf("%d\n", i);
	return i;
}


int cmpFunctionX(const void* a, const void* b){
	if ((double)((point*)a)->x > (double)((point*)b)->x)
		return 1;
	else if ((double)((point*)a)->x < (double)((point*)b)->x)
		return -1;
	return 0;	
}


int cmpFunctionY(const void* a, const void* b){
	if ((double)((point*)a)->y > (double)((point*)b)->y)
		return 1;
	else if ((double)((point*)a)->y < (double)((point*)b)->y)
		return -1;
	return 0;	
}


double distance(point *p, point *q){
	return sqrt((p->x - q->x) * (p->x - q->x) + (p->y - q->y) * (p->y - q->y));
}


void addPlanes(point *p, point *q, shortest_distance *sd){
	if (sd->n_planes + 2 > sd->allocated_planes)
		sd->planes = (int*) realloc(sd->planes, (sd->allocated_planes*=1.5) * sizeof(sd->planes[0]) );
	sd->planes[sd->n_planes++] = p->name_index;
	sd->planes[sd->n_planes++] = q->name_index;
}


void bruteForce(point *points, int n_points, shortest_distance *sd){
	for (int i = 0; i < n_points; i++){
		for (int j = i + 1; j < n_points; j++){
			// printf("starting now:\n");
			// for (int a = 0; a < sd->n_planes; a++){
				// printf("%d\n", sd->planes[a]);
			// }
			double dist = distance(points + i, points + j);
			// printf("brute \n%.30f, %d %d\n", fabs(dist - sd->distance), points[i].name_index, points[j].name_index);
			if (isEqual(dist, sd->distance)){
				// printf("brute %d %d\n", points[i].name_index, points[j].name_index);
				// printf("went through\n");
				addPlanes(points + i, points + j, sd);
			}
			else if (dist < sd->distance){
				sd->n_planes = 0;
				sd->distance = dist;
				addPlanes(points + i, points + j, sd);
			}
		}
	}
}


void getShortestDistance(point *Px, point *Py, int n_points, shortest_distance *sd){

	if (n_points < 5)
		return bruteForce(Px, n_points, sd);

	int n_on_split = 0;
	int n_points_Q = n_points / 2;
	int n_points_R = n_points - n_points_Q;

	while (n_points_Q > 0 && Px[n_points_Q-1].x == Px[n_points_Q].x){
		n_points_Q--;
		n_on_split++;
	}

	while (n_points_Q + n_on_split + 1 < n_points && Px[n_points_Q + n_on_split].x == Px[n_points_Q + n_on_split + 1].x){
		n_points_R--;
		n_on_split++;
	}
	n_points_R--;
	n_on_split++;
	
	point *Qy = (point*) malloc( n_points_Q * sizeof(Qy[0]) );
	point *Ry = (point*) malloc( n_points_R * sizeof(Ry[0]) );

	// for (int i = 0; i < n_points; i++){
	// 	printf("x: %f y: %f\n", Px[i].x, Px[i].y);
	// }

	// printf("-----------------\n");

	// for (int i = 0; i < n_points; i++){
	// 	printf("x: %f y: %f\n", Py[i].x, Py[i].y);
	// }

	point mid_point = Px[n_points_Q];
	// printf("n_points: %d split_points: %d qy: %d ry: %d\n", n_points, n_on_split, n_points_Q, n_points_R);
	int qi = 0, ri = 0;
	for (int i = 0; i < n_points; i++){
		if (Py[i].x < mid_point.x){
			// printf("hello %f\n", mid_point.x);
			Qy[qi++] = Py[i];
		}
		if (Py[i].x > mid_point.x)
			Ry[ri++] = Py[i];
	}

	getShortestDistance(Px, Qy, n_points_Q, sd);
	free(Qy);
	getShortestDistance(Px + n_points_Q + n_on_split, Ry, n_points_R, sd);
	free(Ry);

	int strip_allocated = 4;
	int *strip_indexes = (int*) malloc( strip_allocated * sizeof(strip_indexes[0]) );
	int n_pts = 0;

	for (int i = 0; i < n_points; i++){
		// printf("magic \n%.30f, %d %d\n", fabs(fabs(Py[i].x - mid_point.x) - sd->distance), Py[i].name_index, mid_point.name_index);

		if (fabs(Py[i].x - mid_point.x) < sd->distance || isEqual(fabs(Py[i].x - mid_point.x), sd->distance) ){

			// printf("x %f y %f index %d\n", Py[i].x, Py[i].y, Py[i].name_index);

			for (int j = n_pts - 1; j > -1; j--){

				if (Py[strip_indexes[j]].y + sd->distance < Py[i].y && !isEqual(Py[strip_indexes[j]].y + sd->distance, Py[i].y))
					break;

				if (!((Py[strip_indexes[j]].x < mid_point.x && Py[i].x < mid_point.x)
						|| (Py[strip_indexes[j]].x > mid_point.x && Py[i].x > mid_point.x))){

					double dist = distance(Py + strip_indexes[j], Py + i);
					// printf("magic2 \n%.30f, %d %d\n", fabs(dist - sd->distance), Py[Py_indexes[pt_i]].name_index, Py[i].name_index);

					if (isEqual(dist, sd->distance)){
						// printf("went through\n");
						addPlanes(Py + strip_indexes[j], Py + i, sd);

						// printf("hello there\n");
						// printf("%d %d\n", Py[Py_indexes[pt_i]].name_index, Py[i].name_index);				
					}
					
					else if (dist < sd->distance){
							// printf("fucked everything\n");
							sd->n_planes = 0;
							sd->distance = dist;
							addPlanes(Py + strip_indexes[j], Py + i, sd);
					}
				}
			}
			if (n_pts == strip_allocated)
				strip_indexes = (int*) realloc(strip_indexes, (strip_allocated*=1.5) * sizeof(strip_indexes[0]) );

			strip_indexes[n_pts] = i;
			n_pts++;
		}
	}
	free(strip_indexes);
}


void printSD(shortest_distance *sd, char *names){
	printf("Nejmensi vzdalenost: %f\n", sd->distance);
	for (int i = 0; i < sd->n_planes; i++){
		
		int j = sd->planes[i];
		while (names[j] != '\n')
			printf("%c", names[j++]);

		if (i % 2)
			printf("\n");
		else
			printf(" - ");
	}
}


void printShortestDistances(point *Px, char *names, int n_points){
	point *Py = (point*) malloc( n_points * sizeof(Py[0]) );

	shortest_distance sd;

	sd.allocated_planes = 4;
	sd.n_planes = 0;
	sd.planes = (int*) malloc( sd.allocated_planes * sizeof(sd.planes[0]) );
	sd.distance = distance(Px, Px + 1);

	for (int i = 0; i < n_points; i++)
		Py[i] = Px[i];

	qsort(Px, n_points, sizeof(Px[0]), cmpFunctionX);
	qsort(Py, n_points, sizeof(Py[0]), cmpFunctionY);

	getShortestDistance(Px, Py, n_points, &sd);
	free(Py);

	printSD(&sd, names);
	free(sd.planes);
}



int main(){
	point *points = NULL;
	char *names = NULL;
	int n_points;

	if (!getData(&points, &names, &n_points))
		WRONG_INPUT

	// printPts(points, names, n_points);

	printShortestDistances(points, names, n_points);
	free(points);
	free(names);
	return 0;
}