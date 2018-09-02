#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include "FuncCalc.h"

#define INPUT_ERROR 998244353
#define CALC_ERROR 770346427

#ifndef WYH
#define WYH

char* s;
char* str;

int bb;
int valid;
char brackets[NUM_OPS];

#endif

int input(Node *in, char* x)
{
	int lenx = strlen(x);
	int lens = strlen(s);

	char ins[INPUT_MAX] = {0};
	int insi = 0;
	char temp_in = 0;
	int len = 0;
	
	int i;
	for(i = 0; i < lens; i++)
	{
		if (s[i] != 'x') ins[insi++] = s[i];
		else
		{
			int j;
			for(j = 0; j < lenx; j++) ins[insi++] = x[j];
		}
	}
	ins[insi] = 0;

	insi = 0;
	if (ins[0] == '\n')
	{
		in->tp = Opd, in->opd = 0.0, in++;
	}
	else
	{
		while (ins[insi])
		{
			if (translate(ins + insi, in++, &len)) insi += len;
			else return 0;
		}
	}
	if ((in - 1)->opt != '=')
        in->opt = '=', in->tp = Opt, in++;
	in->opt = -1, in->tp = Opt;
	return 1;
}

int translate(char *p, Node *re, int *len)
{
	if (*p >= '0' && *p <= '9')
	{
		re->tp = Opd;
		re->opd = translateopd(p, len);
		return 1;
	}
	else if (*p == 'e' || *p == 'p')
	{
		if (*p == 'e')
		{
			re->tp = Opd;
			re->opd = M_E;
			*len = 1;
			if(p[1] == ' ') (*len)++;
			return 1;
		}
		else
		{
			if (p[1] == 'i')
			{
				re->tp = Opd;
				re->opd = M_PI;
				*len = 2;
				if(p[2] == ' ') (*len)++;
				return 1;
			}
			else return 0;
		}
	}
	else
	{
		re->tp = Opt;
		re->opt = translateopt(p, len);
		if (re->opt) return 1;
		else return 0;
	}
}

double translateopd(char *p, int *len)
{
	int flag = 0;
	int pointnum = 1;
	double temp = 0;
	int i = 0;
	do
	{
		if (!flag && p[i] != '.')
			temp = temp * 10 + p[i++] - '0';
		else if (!flag && p[i] == '.')
			flag = 1, i++;
		else
			temp += (p[i++] - '0') * pow(0.1, pointnum), 
			pointnum++;
	}while((p[i] >= '0' && p[i] <= '9') || p[i] == '.');
	
	if (p[i] == ' ') i++;
	*len=i;
	return temp;
}

int translateopt(char *p, int *len)
{
	char fu[NUM_OPT][LEN_OPT] = {"+", "-", "*", "/", 
								 "^", "(", ")", "sin",
								 "cos", "tan", "asin",
								 "acos", "atan", "ln",
								 "lg", "="};
	int fu_int[NUM_OPT] = {'+', '-', '*', '/',
						   '^', '(', ')', 's',
						   'c', 't', 's'+'a', 'c'+'a',
						   't'+'a', 'n', 'g', '='};
	int i = 0;
	for (i = 0; i < NUM_OPT; i++)
	{
		if ((*len = cmp(p, fu[i])) > 0) break;
	}
	if (i == NUM_OPT) return 0;
	if (p[*len] == ' ') (*len)++;
	return fu_int[i];
}

int cmp(const char *s, const char *d)
{
	char *dd = (char*)d;
	while (*d) if (*(d++) != *(s++)) return 0;
	return d - dd;
}

int calculate(Node *n, double *r)
{
	double OPRD[NUM_OPS] = {0};
	int OPRT[NUM_OPS] = {0};
	int db = 0;
	int tb = 0;

	int top = 0;
	double a = 0.0, b = 0.0;
	int o = 0;
	int len = 0;
	Node *nb = n;

	if (n->tp == Opt && n->opt == '-')
		OPRD[db] = 0.0, db++;	//Push(&OPRD,0.0); 
	
	while(1)
	{
		if (!valid)
		{
			puts("BOOM");
			*r = CALC_ERROR;
			return 0;
		}
		if (n->tp == Opd)
		{
			OPRD[db] = n->opd, db++, n++;	//Push(&OPRD,n->opd),n++;
		}
		else
		{
            if (prid(n->opt))
            {
                //if(!GetTop(OPRT,&top))
				if (tb) top = OPRT[tb - 1];
                if (!tb)
                {
                    if (n->opt == '=' || n->opt == ')')
                    {
						if (n->opt == ')')
						{
							bb--;	//if(!Pop(&brackets,&temp))
							if (bb < 0)
							{
								//printf("Bracket does not match!");
								return 1;
							}
						}
                        break;
                    }
                    else OPRT[tb] = n->opt, tb++, n++;	//Push(&OPRT,n->opt),n++;
                    continue;
                }
                if (prid(top) == 0)
                {
                    //Pop(&OPRD,&a);
                    //Pop(&OPRT,&o);
					db--;
					a = OPRD[db];
					tb--;
					o = OPRT[tb];
                    if (sclt(o, &a))
                    {
						OPRD[db] = a, db++;	//Push(&OPRD,a);
                        continue;
                    }
                    else return 0;
                }
                if (prid(top) >= prid(n->opt))
                {
                    //Pop(&OPRD,&b);
                    //Pop(&OPRD,&a);
                    //Pop(&OPRT,&o);
					b = OPRD[--db];
					a = OPRD[--db];
					o = OPRT[--tb];
                    if (dclt(o, &a, b))
                    {
						OPRD[db] = a, db++;	//Push(&OPRD,a);
                    }
                    else return 0;
                }
                else
                {
					OPRT[tb] = n->opt, tb++, n++;	//Push(&OPRT,n->opt),n++;
                }
            }
            else
            {
                double x = 0.0;
                if (n->opt == '(')
                {
					brackets[bb] = '(', bb++;	//Push(&brackets,'(');
                    if ((len = calculate(n + 1, &x)) > 0)
                    {
						OPRD[db] = x, db++;	//Push(&OPRD,x);
                        n += len, n++;
						if (n->tp == Opt && n->opt == -1)
						{
							//printf("Bracket does not match!");
							bb = -1;
							return 1;
						}
                    }
                    else return 0;
                }
                else
                {
					OPRT[tb] = n->opt, tb++;	//Push(&OPRT,n->opt);
                    n++;
                }
            }
        }
	}
	*r = OPRD[db - 1];
	return n - nb + 1;
}
int sclt(int opr, double *opd)
{
    switch(opr)
    {
	    case 's':
	        *opd = sin(*opd);
	        break;
	    case 'c':
	        *opd = cos(*opd);
	        break;
	    case 't':
	        *opd = tan(*opd);
	        break;
	    case 'a' + 's':
	        if (*opd < -1 || *opd > 1)
	        {
	            //printf("Beyond asin()!");
	            valid = 0;
	            return 0;
	        }
	        else *opd = asin(*opd);
	        break;
	    case 'a' + 'c':
	        if (*opd < -1 || *opd > 1)
	        {
	            //printf("Beyond acos()!");
	            valid = 0;
	            return 0;
	        }
	        else *opd = acos(*opd);
	        break;
	    case 'a' + 't':
	        if (*opd > -3.141592654 / 2 && *opd < 3.141592654 / 2)
	            *opd=atan(*opd);
	        else
	        {
	            //printf("Beyond atan()!");
	            valid = 0;
	            return 0;
	        }
	        break;
	    case 'n':
	        if (*opd > 0)
	            *opd = log(*opd);
	        else
	        {
	            //printf("Beyond ln()!");
	            valid = 0;
	            return 0;
	        }
	        break;
	    case 'g':
	        if (*opd > 0)
	            *opd = log10(*opd);
	        else
	        {
	            //printf("Beyond lg()!");
	            valid = 0;
	            return 0;
	        }
	        break;
    }
    return 1;
}

int dclt(int opr, double *opd1, double opd2)
{
    switch(opr)
    {
        case '+':
            *opd1 = *opd1 + opd2; break;
        case '-':
            *opd1 = *opd1 - opd2; break;
        case '*':
            *opd1 = (*opd1) * opd2; break;
        case '/':
            if (opd2 > 1e-8 || opd2 < 0 - 1e-8)
                *opd1 = (*opd1) / opd2;
            else
            {
                //printf("Error 1/0 !");
                valid = 0;
                return 0;
            }
            break;
        case '^':
            *opd1 = pow(*opd1, opd2); break;
    }
    return 1;
}

int prid(int c)
{
    switch(c)
    {
        case '+':
        case '-':
		case '=':
		case ')':
        return 1; break;
        case '*':
        case '/':
        return 2; break;
        case '^':
        return 3; break;
        default : return 0; break;
    }
}

void inputFunction()
{
	puts("Now you can input a function, denoted by f(x)");
	puts("");
	puts("The functions you can use are listed as follow.");
	puts("");
	puts("Triangle:");
	puts("sin(P), cos(P), tan(P)");
	puts("");
	puts("Anti-triangle:");
	puts("asin(P), acos(P), atan(P)");
	puts("");
	puts("Power:");
	puts("P1^P2");
	puts("Attention: to use sqrt(x), you can use x^0.5");
	puts("");
	puts("Log:");
	puts("ln(P), lg(P)");
	puts("Attention: To use log(a, b), you can input ln(a) / ln(b)");
	puts("");
	puts("All P above means a valid expression");
	puts("");
	puts("ATTENTION: IF NO GRAPH IS PRINTED, YOUR FUNCTION IS LIKELY TO BE ERROR !");
	puts("E.g. 1 / (x - x)");
	puts("");
	printf("Your expression is: ");
	gets(str);
	puts("");
}

void initFunction()
{
	s = (char*)malloc(200 * sizeof(char));
	str = (char*)malloc(200 * sizeof(char));
	inputFunction();
	int p = 0;
	int n = strlen(str);
	int i;
	for(i = 0; i < n; i++)
	{
		if (str[i] != ' ') s[p++] = str[i];
	}
	s[p] = 0;
	//printf("%s\n", s);
}

double getValue(char* x)
{
	Node nodes[NODE_MAX] = {0};
	valid = 1;
	double r = 0.0;
	bb = 0;
	if (!input(nodes, x))
	{
		printf("Input Error!");
		return INPUT_ERROR;
	}
	if (calculate(nodes, &r))
	{
		if (bb)
		{
			printf("Bracket does not match!");
			return INPUT_ERROR;
		}
		return r;
	}
	return CALC_ERROR;
}

