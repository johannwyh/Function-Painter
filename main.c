#include "acllib.h"
#include "FuncCalc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

const int Ox = 400;
const int Oy = 300;
const int leftMostx = 100;
const int rightMostx = 700;
const int upMosty = 10;
const int downMosty = 590;
const double errorTag = 770346427;
const double inputTag = 998244353;

const double INF = 290;

char x[200];
char tmp[200];
int i;
int tot;

double qx[1000];
double qy[1000];
 
typedef struct _point
{
	int x, y;
}Point;

void print(Point p)
{
	int px = Ox + p.x;
	int py = Oy - p.y;
	if (px < leftMostx || px > rightMostx) return;
	if (py < upMosty || py > downMosty) return;
	putPixel(px, py, CYAN);
	return;
}

bool inBox(Point p)
{
	if (p.x < leftMostx || p.x > rightMostx) return 0;
	if (p.y < upMosty || p.y > downMosty) return 0;
	return 1;
}

void printLine(Point p1, Point p2)
{
	p1.x = Ox + p1.x; p1.y = Oy - p1.y;
	p2.x = Ox + p2.x; p2.y = Oy - p2.y;
	
	bool f1 = inBox(p1), f2 = inBox(p2);
	if (f1) putPixel(p1.x, p1.y, BLACK);
	if (f2) putPixel(p2.x, p2.y, BLACK);
	if (f1 && f2) line(p1.x, p1.y, p2.x, p2.y);
	return;
}

void init()
{
	int i;
	line(leftMostx, Oy, rightMostx, Oy);
	line(Ox, upMosty, Ox, downMosty);
	for(i = leftMostx; i <= rightMostx; i += 60)
	{
		if (i == rightMostx)
		{
			line(695, 295, rightMostx, Oy);
			line(695, 305, rightMostx, Oy);
		}
		else line(i, 305, i, 295);
	}
	for(i = upMosty; i <= downMosty; i += 58)
	{
		if (i == upMosty)
		{
			line(395, 15, Ox, upMosty);
			line(405, 15, Ox, upMosty);
		}
		else line(395, i, 405, i);
	}
}

void getData(double l, double r)
{
	double delta = (r - l) / 600.0;
	double i;
	tot = 0;
	for(i = l; i <= r; i += delta)
	{
		sprintf(tmp, "%lf", i);
		x[0] = '('; x[1] = '\0';
		strcat(x, tmp);
		strcat(x, ")");
		
		double ANS = getValue(x);
		if (ANS == inputTag) break;
		if (ANS != errorTag)
		{
			qx[++tot] = i; qy[tot] = ANS;
		}
	}
}

int Setup(){
	
	int i;
	
	initWindow("Function Painter", DEFAULT, DEFAULT, 800, 600);
	initConsole();
	initFunction();
	
	beginPaint();
	
	init();
  	
  	getData(-300, 300);
	
	int lp = 1, rp = tot;
	while(fabs(qy[lp]) > INF && lp < rp) lp++;
	while(fabs(qy[rp]) > INF && lp < rp) rp--;
	
	double mx;
	for(i = lp; i <= rp; i++)
	{
		if (fabs(qy[i]) < INF) mx = max(mx, fabs(qy[i]));
	}
	double EPS = mx / 300.0;
	while(fabs(qy[lp]) < EPS && lp < rp) lp++;
	while(fabs(qy[rp]) < EPS && lp < rp) rp--; 
	
	double max_x = max(fabs(qx[lp]), qx[rp]), max_y = mx;
	max_x = min(max_x, max_y * 10);
	double kx = 300 / max_x, ky = 290 / max_y;
	
	getData(-max_x, max_x);
	
	for(i = 1; i < tot; i++)
	{
		Point p1, p2;
		p1.x = (int)(qx[i] * kx); p1.y = (int)(qy[i] * ky);
		p2.x = (int)(qx[i + 1] * kx); p2.y = (int)(qy[i + 1] * ky);
		printLine(p1, p2);
	}
	
	char xm[100], xp[100];
	char ym[100], yp[100];
	sprintf(xp, "%.0lf", max_x);
	sprintf(yp, "%.0lf", max_y);
	xm[0] = '-', xm[1] = '\0';
	ym[0] = '-', ym[i] = '\0';
	strcat(xm, xp); strcat(ym, yp);
	paintText(100, 308, xm);
	paintText(700, 308, xp);
	paintText(365, 10, yp);
	paintText(365, 590, ym);
	endPaint();
	
	return 0;
} 
