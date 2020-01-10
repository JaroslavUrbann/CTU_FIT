#ifndef __PROGTEST__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#endif /* __PROGTEST__ */


typedef struct line{
	double a, b, c, u, l;
} line;



int isEqual(double x, double y){
	return fabs(x - y) <= DBL_EPSILON * 100 * fabs(x);
}


int isBigger(double x, double y){
	return x - y > DBL_EPSILON * 100 * fabs(x);
}


int cut(line ml, line l, double* p){
	double x = - l.c / l.a;
	*p = - (ml.a * x + ml.c) / ml.b;
	if ((isBigger(l.u, *p) || isEqual(l.u, *p)) && (isBigger(*p, l.l) || isEqual(*p, l.l)))
		return 1;
	return 0;
}


line getLine(double x1,
			double y1,
			double x2,
			double y2){
	line l;
	l.u = y1 > y2 ? y1 : y2;
	l.l = y1 > y2 ? y2 : y1;
	l.a = isEqual(y1, y2) ? 0 : y1 - y2;
	l.b = isEqual(x1, x2) ? 0 : x2 - x1;
	l.c = - (l.a * x1 + l.b * y1);
	return l;
}


int clipLine(double rx1,
			 double ry1,
			 double rx2,
			 double ry2,
			 double* ax,
			 double* ay,
			 double* bx,
			 double* by){

	line ml, ml_i, l1, l2, l3, l4;
	ml = getLine(*ax, *ay, *bx, *by);
	ml_i = getLine(*ay, *ax, *by, *bx);
	l1 = getLine(rx1, ry1, rx1, ry2);
	l2 = getLine(rx2, ry1, rx2, ry2);
	l3 = getLine(ry1, rx1, ry1, rx2);
	l4 = getLine(ry2, rx1, ry2, rx2);

	if (l1.c / l1.a < l2.c / l2.a){
		line temp = l1;
		l1 = l2;
		l2 = temp;
	}

	if (l3.c / l3.a < l4.c / l4.a){
		line temp = l3;
		l3 = l4;
		l4 = temp;
	}

	double p1, p2, p3, p4;
	p1 = -l1.c / l1.a;
	p2 = -l2.c / l2.a;
	p3 = -l3.c / l3.a;
	p4 = -l4.c / l4.a;

	// printf("%.50f %f %f %f\n", *ax, *bx, *ay, *by);
	// printf("%.50f %f %f %f\n", p1, p2, p3, p4);
	// printf("%d\n", isEqual(*ax, p1));

	if ((isBigger(p2, *ax) || isEqual(p2, *ax)) && (isBigger(*ax, p1) || isEqual(*ax, p1))
		&& (isBigger(p2, *bx) || isEqual(p2, *bx)) && (isBigger(*bx, p1) || isEqual(*bx, p1))
		&& (isBigger(p4, *ay) || isEqual(p4, *ay)) && (isBigger(*ay, p3) || isEqual(*ay, p3))
		&& (isBigger(p4, *by) || isEqual(p4, *by)) && (isBigger(*by, p3) || isEqual(*by, p3)))
		return 1;


	double l, r, d, u;
	l = r = d = u = 0;
	int is_l, is_r, is_d, is_u;
	is_l = is_r = is_d = is_u = 0;

	if (ml.b != 0){
		is_l = cut(ml, l1, &l);
		is_r = cut(ml, l2, &r);
	}
	if (ml_i.b != 0){
		is_d = cut(ml_i, l3, &d);
		is_u = cut(ml_i, l4, &u);
	}

	// printf("did cross1: %d %d %d %d\n", is_l, is_r, is_d, is_u);

	if (is_l + is_r + is_d + is_u > 2){
		if (isEqual(l, p3) && is_l && is_d)
			is_l = 0;

		if (isEqual(r, p4) && is_r && is_u)
			is_r = 0;

		if (isEqual(d, p2) && is_d && is_r)
			is_d = 0;

		if (isEqual(u, p1) && is_l && is_u)
			is_u = 0;
	}

	int did_cut = 0;
	double b_l, b_r, b_d, b_u;
	b_l = b_r = b_d = b_u = 0;
	if(is_l){
		if ((isBigger(p1, ml_i.l) || isEqual(p1, ml_i.l)) && (isBigger(ml_i.u, p1) || isEqual(ml_i.u, p1))){
		// if(p1 > ml_i.l && p1 < ml_i.u){
			did_cut = 1;
			b_l = p1;
			if(is_u)
				b_d = l;
			if(is_d)
				b_u = l;
			if(is_r && (isBigger(l, r) || isEqual(l, r)))
				b_u = l;
			if(is_r && (isBigger(r, l) || isEqual(l, r)))
				b_d = l;
		}
		else{
			b_l = ml_i.l;
			if(is_u)
				b_d = ml.l;
			if(is_d)
				b_u = ml.u;
			if(is_r && (isBigger(l, r) || isEqual(l, r)))
				b_u = ml.u;
			if(is_r && (isBigger(r, l) || isEqual(l, r)))
				b_d = ml.l;
		}
	}
	// primka prochazi pres r
	if(is_r){
		// usecka prochazi pres r
		if ((isBigger(p2, ml_i.l) || isEqual(p2, ml_i.l)) && (isBigger(ml_i.u, p2) || isEqual(ml_i.u, p2))){
		// if(p2 > ml_i.l && p2 < ml_i.u){
			did_cut = 1;
			b_r = p2;
			// usecka prochazi pres r a miri do upper, nastavim lower.
			if(is_u)
				b_d = r;
			if(is_d)
				b_u = r;
			if(is_l && (isBigger(l, r) || isEqual(l, r)))
				b_d = r;
			if(is_l && (isBigger(r, l) || isEqual(l, r)))
				b_u = r;
		}
		// usecka neprochazi pres r
		else{
			b_r = ml_i.u;
			// usecka neprochazi pres r a miri nahoru, nastavim lower na hodnotu lower teto usecky
			if(is_u)
				b_d = ml.l;
			if(is_d)
				b_u = ml.u;
			if(is_l && (isBigger(l, r) || isEqual(l, r)))
				b_d = ml.l;
			if(is_l && (isBigger(r, l) || isEqual(l, r)))
				b_u = ml.u;
		}
	}

	
	if(is_d){
		if ((isBigger(ml.u, p3) || isEqual(ml.u, p3)) && (isBigger(p3, ml.l) || isEqual(p3, ml.l))){
		// if (p3 < ml.u && p3 > ml.l){
			did_cut = 1;
			b_d = p3;
			if(is_l)
				b_r = d;
			if(is_r)
				b_l = d;
			if(is_u && (isBigger(u, d) || isEqual(u, d)))
				b_l = d;
			if(is_u && (isBigger(d, u) || isEqual(u, d)))
				b_r = d;
		}
		else{
			b_d = ml.l;
			if(is_l)
				b_r = ml_i.u;
			if(is_r)
				b_l = ml_i.l;
			if(is_u && (isBigger(u, d) || isEqual(u, d)))
				b_l = ml_i.l;
			if(is_u && (isBigger(d, u) || isEqual(u, d)))
				b_r = ml_i.u;
		}
	}

	
	if(is_u){
		if ((isBigger(p4, ml.l) || isEqual(p4, ml.l)) && (isBigger(ml.u, p4) || isEqual(ml.u, p4))){
		// if (p4 > ml.l && p4 < ml.u){
			did_cut = 1;
			b_u = p4;
			if(is_l)
				b_r = u;
			if(is_r)
				b_l = u;
			if(is_u && (isBigger(d, u) || isEqual(u, d)))
				b_l = u;
			if(is_u && (isBigger(u, d) || isEqual(u, d)))
				b_r = u;
		}
		else{
			b_u = ml.u;
			if(is_l)
				b_r = ml_i.u;
			if(is_r)
				b_l = ml_i.l;
			if(is_d && (isBigger(u, d) || isEqual(u, d)))
				b_r = ml_i.u;
			if(is_d && (isBigger(d, u) || isEqual(u, d)))
				b_l = ml_i.l;
		}
	}


	if (!(is_l || is_r || is_d || is_u))
		return 0;

	// printf("did cross: %d %d %d %d\n", is_l, is_r, is_d, is_u);
	// printf("rect: %f %f %f %f\n", p1, p2, p3, p4);
	// printf("ml_i.l: %f\nml_i.u: %f\nml.l:   %f\nml.u:   %f\n", ml_i.l, ml_i.u, ml.l, ml.u);
	// printf("guessed borders: %f %f %f %f\n", b_l, b_r, b_d, b_u);

	*ax = isBigger(*ax, *bx) ? b_r : b_l;
	*bx = isBigger(*ax, *bx) ? b_l : b_r;
	*ay = isBigger(*ay, *by) ? b_u : b_d;
	*by = isBigger(*ay, *by) ? b_d : b_u;

	// printf("output: %f %f %f %f\n", *ax, *bx, *ay, *by);
	if(is_d && is_u && isEqual(*ax, *bx) && (isEqual(*ax, p2) || isEqual(*ax, p1)) && isBigger(p4, ml.l) && isBigger(ml.u, p3))
		return 1;
	if(is_l && is_r && isEqual(*ay, *by) && (isEqual(*ay, p3) || isEqual(*ax, p4)) && isBigger(p2, ml_i.l) && isBigger(ml_i.u, p1))
		return 1;
	// printf("asdfdsfa\n");
	return did_cut;
}


#ifndef __PROGTEST__
int main(void){
	double x1, y1, x2, y2;



x1 = -1072; y1 = -1746; x2 = -545.5; y2 = -328.5; assert (clipLine(-604, -486, -370, 144, &x1, &y1, &x2, &y2) && isEqual(x1, -604) && isEqual(y1, -486) && isEqual(x2, -545.5) && isEqual(y2, -328.5));

x1 = -450; y1 = -386.25; x2 = -904; y2 = -978; assert (clipLine(-450, -452, -223, -189, &x1, &y1, &x2, &y2) && isEqual(x1, -450) && isEqual(y1, -386.25) && isEqual(x2, -450) && isEqual(y2, -386.25));

x1 = 11; y1 = 1316.75; x2 = 11; y2 = 1012.25; assert (clipLine(11, 1469, -830, 860, &x1, &y1, &x2, &y2) && isEqual(x1, x1) && isEqual(y1, y1) && isEqual(x2, x2) && isEqual(y2, y2));

x1 = 2500; y1 = -96; x2 = 1138; y2 = 290; assert (!clipLine(457, -289, 1138, -96, &x1, &y1, &x2, &y2) && isEqual(x1, 2500) && isEqual(y1, -96) && isEqual(x2, 1138) && isEqual(y2, 290));

x1 = -149; y1 = -6; x2 = 691; y2 = 925.5; assert (clipLine(691, 822, 1111, 1236, &x1, &y1, &x2, &y2) && isEqual(x1, 691) && isEqual(y1, 925.5) && isEqual(x2, 691) && isEqual(y2, 925.5));

x1 = 1096; y1 = 261; x2 = 1594; y2 = -139; assert (!clipLine(1096, -139, 847, -339, &x1, &y1, &x2, &y2) && isEqual(x1, 1096) && isEqual(y1, 261) && isEqual(x2, 1594) && isEqual(y2, -139));

x1 = -2412; y1 = -2721; x2 = -654; y2 = -885; assert (clipLine(-654, 33, 225, -885, &x1, &y1, &x2, &y2) && isEqual(x1, -654) && isEqual(y1, -885) && isEqual(x2, -654) && isEqual(y2, -885));

x1 = 0; y1 = 0.8448; x2 = 3.685; y2 = 0; assert (clipLine(0.335, 0.768, 1, 1, &x1, &y1, &x2, &y2) && isEqual(x1,0.335) && isEqual(y1,0.768) && isEqual(x2,0.335) && isEqual(y2,0.768));

x1 = 0; y1 = 0.6083; x2 = 3.047; y2 = 0; assert (clipLine ( 0.277, 0.553, 1, 1, &x1, &y1, &x2, &y2) && isEqual(x1,0.277) && isEqual(y1,0.553) && isEqual(x2,0.277) && isEqual(y2,0.553));

x1 = 755; y1 = 774; x2 = -1143; y2 = 774; assert (clipLine(755, 774, 1704, 1492, &x1, &y1, &x2, &y2) && isEqual(x1, 755) && isEqual(y1, 774) && isEqual(x2, 755) && isEqual(y2, 774));

x1 = 0; y1 = 3; x2 = 10; y2 = 3; assert (clipLine(3, 5, 1, 1, &x1, &y1, &x2, &y2) && isEqual(x1, 1) && isEqual(y1, 3) && isEqual(x2, 3) && isEqual(y2, 3));

x1 = 0; y1 = 1; x2 = 10; y2 = 1; assert (clipLine(3, 5, 1, 1, &x1, &y1, &x2, &y2) && isEqual(x1, 1) && isEqual(y1, 1) && isEqual(x2, 3) && isEqual(y2, 1));

x1 = 1; y1 = 1; x2 = 1; y2 = 5; assert (clipLine(3, 5, 1, 1, &x1, &y1, &x2, &y2) && isEqual(x1, x1) && isEqual(y1, y1) && isEqual(x2, x2) && isEqual(y2, y2));

x1 = -1115; y1 = 145; x2 = -1627; y2 = 145; assert (!clipLine ( -603, 145, -91, 690, &x1, &y1, &x2, &y2) && isEqual(x1, -1115) && isEqual(y1, 145) && isEqual(x2, -1627) && isEqual(y2, 145));


	x1 = 0;
	y1 = 0;
	x2 = 9;
	y2 = 9;
	assert ( clipLine ( 0, 0, 10, 10, &x1, &y1, &x2, &y2 )
	          && isEqual ( x1, x1 )
	          && isEqual ( y1, y1 )
	          && isEqual ( x2, x2 )
	          && isEqual ( y2, y2 ) );

	  x1 = 0;
	  y1 = 0.8448;
	  x2 = 3.685;
	  y2 = 0;
	  assert ( clipLine ( 0.335, 0.768, 1, 1, &x1, &y1, &x2, &y2 )
	            && isEqual ( x1, 0.335 )
	            && isEqual ( y1, 0.768 )
	            && isEqual ( x2, 0.335 )
	            && isEqual ( y2, 0.768 ) );


	  x1 = -572;
	  y1 = -965;
	  x2 = -1832;
	  y2 = -1607;
	  assert ( clipLine ( -572, -965, 58, -644, &x1, &y1, &x2, &y2 )
	            && isEqual ( x1, -572 )
	            && isEqual ( y1, -965 )
	            && isEqual ( x2, -572 )
	            && isEqual ( y2, -965 ) );


	x1 = -1072;
	y1 = -1746;
	x2 = -545.5;
	y2 = -328.5;

	assert (clipLine ( -604, -486, -370, 144, &x1, &y1, &x2, &y2 )
				&& isEqual(x1, -604)
				&& isEqual(y1, -486)
				&& isEqual(x2, -545.5)
				&& isEqual(y2, -328.5));

	x1 = -450;
	y1 = -386.25;
	x2 = -904;
	y2 = -987;

	assert (clipLine ( -450, -452, -223, -189, &x1, &y1, &x2, &y2 )
				&& isEqual(x1, -450)
				&& isEqual(y1, -386.25)
				&& isEqual(x2, -450)
				&& isEqual(y2, -386.25));

  	  x1 = -2412;
	  y1 = -2721;
	  x2 = -654;
	  y2 = -885;

	  assert (clipLine ( -654, 33, 225, -885, &x1, &y1, &x2, &y2 )
	  			&& isEqual(x1, -654)
	  			&& isEqual(y1, -885)
	  			&& isEqual(x2, -654)
	  			&& isEqual(y2, -885));

	  x1 = 0;
	  y1 = 0.6083;
	  x2 = 3.047;
	  y2 = 0;

	  assert (clipLine ( 0.277, 0.553, 1, 1, &x1, &y1, &x2, &y2 )
	  			&& isEqual(x1, 0.277)
	  			&& isEqual(y1, 0.553)
	  			&& isEqual(x2, 0.277)
	  			&& isEqual(y2, 0.553));

	  x1 = 11;
	  y1 = 1316.75;
	  x2 = 11;
	  y2 = 1012.25;
	  
	  assert ( clipLine ( 11, 1469, -830, 860, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, x1 )
	           && isEqual ( y1, y1 )
	           && isEqual ( x2, x2 )
	           && isEqual ( y2, y2 ) );

	  x1 = -149;
	  y1 = -6;
	  x2 = 691;
	  y2 = 925.5;
	  assert ( clipLine ( 691, 822, 1111, 1236, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, 691 )
	           && isEqual ( y1, 925.5 )
	           && isEqual ( x2, 691 )
	           && isEqual ( y2, 925.5 ) );

	  x1 = 0;
	  y1 = 20;
	  x2 = 10;
	  y2 = 20;
	  assert ( clipLine ( 10, 10, 20, 20, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, 10 )
	           && isEqual ( y1, 20 )
	           && isEqual ( x2, 10 )
	           && isEqual ( y2, 20 ) );

	  x1 = 11;
	  y1 = 10;
	  x2 = 11;
	  y2 = 20;
	  assert ( clipLine ( 10, 10, 20, 20, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, 11 )
	           && isEqual ( y1, 10 )
	           && isEqual ( x2, 11 )
	           && isEqual ( y2, 20 ) );

	  x1 = 10;
	  y1 = 11;
	  x2 = 20;
	  y2 = 11;
	  assert ( clipLine ( 10, 10, 20, 20, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, 10 )
	           && isEqual ( y1, 11 )
	           && isEqual ( x2, 20 )
	           && isEqual ( y2, 11 ) );

	  x1 = 10;
	  y1 = 10;
	  x2 = 20;
	  y2 = 20;
	  assert ( clipLine ( 10, 10, 20, 20, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, 10 )
	           && isEqual ( y1, 10 )
	           && isEqual ( x2, 20 )
	           && isEqual ( y2, 20 ) );

	  x1 = 60;
	  y1 = 40;
	  x2 = 70;
	  y2 = 50;
	  assert ( clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, 60 )
	           && isEqual ( y1, 40 )
	           && isEqual ( x2, 70 )
	           && isEqual ( y2, 50 ) );

	  x1 = 0;
	  y1 = 50;
	  x2 = 20;
	  y2 = 30;
	  assert ( clipLine ( 90, 100, 10, 20, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, 10 )
	           && isEqual ( y1, 40 )
	           && isEqual ( x2, 20 )
	           && isEqual ( y2, 30 ) );

	  x1 = 0;
	  y1 = 30;
	  x2 = 120;
	  y2 = 150;
	  assert ( clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, 10 )
	           && isEqual ( y1, 40 )
	           && isEqual ( x2, 70 )
	           && isEqual ( y2, 100 ) );

	  x1 = -10;
	  y1 = -10;
	  x2 = -20;
	  y2 = -20;
	  assert ( ! clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 ) );

	  x1 = 0;
	  y1 = 30;
	  x2 = 20;
	  y2 = 10;
	  assert ( clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, 10 )
	           && isEqual ( y1, 20 )
	           && isEqual ( x2, 10 )
	           && isEqual ( y2, 20 ) );

	  x1 = 0;
	  y1 = 0.3553;
	  x2 = 10.45;
	  y2 = 0;
	  assert ( clipLine ( 0.95, 0.323, 1, 1, &x1, &y1, &x2, &y2 )
	           && isEqual ( x1, 0.95 )
	           && isEqual ( y1, 0.323 )
	           && isEqual ( x2, 0.95 )
	           && isEqual ( y2, 0.323 ) );

	  x1 = 1096;
  	  y1 = 261;
  	  x2 = 1594;
  	  y2 = -139;
  	  assert (  ! clipLine ( 1096, -139, 847, -339, &x1, &y1, &x2, &y2)
	           && isEqual ( x1, 1096 )
	           && isEqual ( y1, 261 )
	           && isEqual ( x2, 1594 )
	           && isEqual ( y2, -139 ) );

  	  x1 = 2500;
  	  y1 = -96;
  	  x2 = 1138;
  	  y2 = 290;
  	  assert ( ! clipLine ( 457, -289, 1138, -96, &x1, &y1, &x2, &y2)
  	             && isEqual ( x1, 2500 )
  	             && isEqual ( y1, -96 )
  	             && isEqual ( x2, 1138 )
  	             && isEqual ( y2, 290 ));
	  return 0;
}
#endif /* __PROGTEST__ */
