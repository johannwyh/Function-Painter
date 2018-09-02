#include <Windows.h>
 
#define INPUT_MAX 1000
#define NODE_MAX 500
#define NUM_OPT 20
#define LEN_OPT 6
#define NUM_OPS 200

#ifndef INITED
#define INITED
 
typedef enum
{
	Opd = 0,
	Opt = 1
}Type;

typedef struct
{
	int opt;
	double opd;
	Type tp;
}Node;

extern char* s;
extern char* str;

extern int bb;
extern char brackets[NUM_OPS];
extern int valid;

#endif

int input(Node *in, char* x);
int translate(char *p, Node *re, int *len);
double translateopd(char *p, int *len);
int translateopt(char *p, int *len);
int cmp(const char *s, const char *d);
int calculate(Node *n, double *r);
int sclt(int opr, double *opd);
int dclt(int opr, double *opd1, double opd2);
int prid(int c);
void inputFunction();
double getValue(char* x);

