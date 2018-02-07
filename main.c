#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int real;
	int imag;
} complex;

complex complex_times(const complex *complex1, const complex *complex2) {
	complex ret;
	ret.real = complex1->real * complex2->real - complex1->imag * complex2->imag;
	ret.imag = complex1->real * complex2->imag + complex1->imag * complex2->real;
	return ret;
}

int getLenthOfPythagoreanTriple(int maxnum) {
	int n2 = (int) sqrt(maxnum);
	complex v;
	int n = 0;
	for (int a = 1; a <= n2; a++) {
		int b = 1;
		v.real = a;
		v.imag = b;
		v = complex_times(&v, &v);
		while (b < a && v.real <= maxnum && v.imag <= maxnum) {
			n++;
			b++;
			v.real = a;
			v.imag = b;
			v = complex_times(&v, &v);
		}
	}
	return n;
}

typedef struct {
	complex *c;
	int size;
} PythagoreanTriple;

void sortPythagoreanTripleReal(PythagoreanTriple *p) {
	if (p->size <= 1) {
		return;
	}
	PythagoreanTriple p1 = *p;
	PythagoreanTriple p2 = *p;
	p1.size /= 2;
	p2.c = p1.c + p1.size;
	p2.size -= p1.size;
	sortPythagoreanTripleReal(&p1);
	sortPythagoreanTripleReal(&p2);

	complex *buff = malloc(sizeof(complex) * p->size);
	int i1 = 0, i2 = 0;
	for (int i = 0; i < p->size; i++) {
		if (i1 == p1.size) {
			buff[i] = p2.c[i2];
			i2++;
		} else if (i2 == p2.size || p1.c[i1].real < p2.c[i2].real) {
			buff[i] = p1.c[i1];
			i1++;
		} else {
			buff[i] = p2.c[i2];
			i2++;
		}
	}

	for (int i = 0; i < p->size; i++) {
		p->c[i] = buff[i];
	}
	free(buff);
}

void sortPythagoreanTriple(PythagoreanTriple *p) {
	complex buff;
	sortPythagoreanTripleReal(p);
	//sortPythagoreanTripleImag(p);
	/*
	for (int size = 1; size < p->size; size++) {
		for (int i = size; i > 0; i--) {
			if (p->c[i].real < p->c[i - 1].real) {
				buff = p->c[i];
				p->c[i] = p->c[i - 1];
				p->c[i - 1] = buff;
			} else {
				break;
			}
		}
	}/**/

	for (int size = 1; size < p->size; size++) {
		for (int i = size; i > 0; i--) {
			if (p->c[i].real <= p->c[i - 1].real && p->c[i].imag < p->c[i - 1].imag) {
				buff = p->c[i];
				p->c[i] = p->c[i - 1];
				p->c[i - 1] = buff;
			} else {
				break;
			}
		}
	}
}

PythagoreanTriple getPythagoreanTriple(int maxnum) {
	int n2 = (int) sqrt(maxnum);
	PythagoreanTriple list;
	list.c = malloc(sizeof(complex) * getLenthOfPythagoreanTriple(maxnum));
	list.size = 0;
	complex v;
	for (int a = 1; a <= n2; a++) {
		int b = 1;
		v.real = a;
		v.imag = b;
		v = complex_times(&v, &v);
		if (v.real > v.imag) {
			int buff;
			buff = v.real;
			v.real = v.imag;
			v.imag = buff;
		}
		while (b < a && v.real <= maxnum && v.imag <= maxnum) {
			list.c[list.size] = v;
			list.size++;
			b++;
			v.real = a;
			v.imag = b;
			v = complex_times(&v, &v);
			if (v.real > v.imag) {
				int buff;
				buff = v.real;
				v.real = v.imag;
				v.imag = buff;
			}
		}
	}
	return list;
}

int maxstr(char s1[], char s2[]) {
	return strlen(s1) > strlen(s2) || strcmp(s1, s2) > 0;
}

int main(int argc, char *argv[]) {
	int maxnum = 0;
	if (argc - 1) {
		char c[32] = {'\0'};
		int a = 1;
		sprintf(c, "%d", (a < 0 < (sizeof(a) * 8 - 1)) - 1);
		if (maxstr(c, argv[1])) {
			printf("too large maxmiun number");
			return 0;
		}
		maxnum = atoi(argv[1]);
	}

	int type = 0;
	if (argc - 2)
		type = atoi(argv[2]);

	int output = 1;
	if (argc - 3)
		output = atoi(argv[3]);

	if (!type) {
		PythagoreanTriple p = getPythagoreanTriple(maxnum);
		if (output) {
			sortPythagoreanTriple(&p);
			for (int i = 0; i < p.size; i++) {
				printf("%d+%di\n", p.c[i].real, p.c[i].imag);
			}
			printf("%dnumbers\n", p.size);
		}
		free(p.c);
	} else {
		int number = getLenthOfPythagoreanTriple(maxnum);
		if (output)
			printf("%dnumbers\n", number);
	}

	return 0;
}