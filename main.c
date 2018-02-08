#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * 判断两个数是否互质
 */
int isCoprime(unsigned int a, unsigned int b) {
	unsigned int buff;
	if (a < b) {
		buff = a;
		a = b;
		b = buff;
	}
	do {
		buff = a % b;
		a = b;
		b = buff;
	} while (buff != 0);
	if (a == 1)
		return 1;
	else
		return 0;
}

/*
 * 结构体complex
 * 定义一个复数，因为只计算整数所以使用的是int
 * real:实部
 * imag:虚部
 * module:模
 */
typedef struct {
	unsigned int real;
	unsigned int imag;
	unsigned int module;
} complex;

/*
 * 计算两个复数的乘积
 */
complex complex_times(const complex *complex1, const complex *complex2) {
	//返回值
	complex ret;
	//计算模
	ret.module = complex1->real * complex2->real + complex1->imag * complex2->imag;
	//实部
	ret.real = complex1->real * complex2->real - complex1->imag * complex2->imag;
	//虚部
	ret.imag = complex1->real * complex2->imag + complex1->imag * complex2->real;
	//返回结果
	return ret;
}

//获得斜边长小于某个长度的所有勾股数的对数的总数
//计算速度很快
unsigned int getLenthOfPythagoreanTriple(int maxnum) {
	int n2 = (int) sqrt(maxnum);
	complex v;
	unsigned int n = 0;
	for (unsigned int a = 1; a <= n2; a++) {
		unsigned int b = 1;
		v.real = a;
		v.imag = b;
		v = complex_times(&v, &v);
		while (b < a && v.module <= maxnum) {
			if (!isCoprime(a, b) || !isCoprime(v.real, v.imag)) {
				b++;
				v.real = a;
				v.imag = b;
				v = complex_times(&v, &v);
				continue;
			}
			for (unsigned int t = 1; v.module * t <= maxnum; t++)
				n++;
			b++;
			v.real = a;
			v.imag = b;
			v = complex_times(&v, &v);
		}
	}
	return n;
}

/*
 * 结构体PythagoreanTriple
 * 定义复数数组
 * size是数组元素总数
 */
typedef struct {
	complex *c;
	int size;
} PythagoreanTriple;

/*
 * 获取斜边小于某个长度的所有勾股数
 * 返回值为PythagoreanTriple
 * 返回值默认不排序
 *
 * maxnum:斜边的最大值
 * compbuff:复数缓冲区
 * buffsize:复数缓冲区大小
 */
PythagoreanTriple getPythagoreanTriple(int maxnum, complex *compbuff, unsigned int buffsize) {
	int n2 = (int) sqrt(maxnum);
	PythagoreanTriple list;
	list.c = compbuff;
	list.size = 0;
	complex v;
	for (unsigned int a = 1; a <= n2; a++) {
		unsigned int b = 1;
		v.real = a;
		v.imag = b;
		v = complex_times(&v, &v);
		if (v.real > v.imag) {
			unsigned int buff;
			buff = v.real;
			v.real = v.imag;
			v.imag = buff;
		}
		while (b < a && v.module <= maxnum && list.size < buffsize) {
			if (!isCoprime(a, b) || !isCoprime(v.real, v.imag)) {
				b++;
				v.real = a;
				v.imag = b;
				v = complex_times(&v, &v);
				continue;
			}
			for (unsigned int t = 1; v.module * t < maxnum; t++) {
				list.c[list.size].real = v.real * t;
				list.c[list.size].imag = v.imag * t;
				list.c[list.size].module = v.module * t;
				list.size++;
			}
			b++;
			v.real = a;
			v.imag = b;
			v = complex_times(&v, &v);
			if (v.real > v.imag) {
				unsigned int buff;
				buff = v.real;
				v.real = v.imag;
				v.imag = buff;
			}
		}
	}
	return list;
}

/*
 * 根据复数的模对复数进行由小到大的二分法排序
 * buff为长度大于等于sizeof(complex) * p->size的缓冲区
 * 这样设计是为了节省申请内存的时间
 */
void sortPythagoreanTripleReal(PythagoreanTriple *p, complex *buff) {
	if (p->size <= 1) {
		return;
	}
	PythagoreanTriple p1 = *p;
	PythagoreanTriple p2 = *p;
	p1.size /= 2;
	p2.c = p1.c + p1.size;
	p2.size -= p1.size;
	sortPythagoreanTripleReal(&p1, buff);
	sortPythagoreanTripleReal(&p2, buff);

	//complex *buff = malloc(sizeof(complex) * p->size);
	int i1 = 0, i2 = 0;
	for (int i = 0; i < p->size; i++) {
		if (i1 == p1.size) {
			buff[i] = p2.c[i2];
			i2++;
		} else if (i2 == p2.size || p1.c[i1].module < p2.c[i2].module) {
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
}

/*
 * 对模进行由小到大的排序
 * 然后在此基础上对实部进行由小到大的排序
 */
void sortPythagoreanTriple(PythagoreanTriple *p) {
	//缓存
	complex buff;
	//根据复数的模对复数进行由小到大的二分法排序
	complex *sbuff = malloc(sizeof(complex) * p->size);
	sortPythagoreanTripleReal(p, sbuff);
	free(sbuff);
	//在复数的模排序的基础上再对实部进行由小到大的排序
	for (int size = 1; size < p->size; size++) {
		for (int i = size; i > 0; i--) {
			if (p->c[i].module <= p->c[i - 1].module && p->c[i].real < p->c[i - 1].real) {
				buff = p->c[i];
				p->c[i] = p->c[i - 1];
				p->c[i - 1] = buff;
			} else {
				break;
			}
		}
	}
}

/*
 * 判断字符串s1是否比s2小
 */
int str_smaller(char *s1, char *s2) {
	return *s2 == '-' || strlen(s1) < strlen(s2) || strcmp(s1, s2) < 0;
}

/*
 * 程序入口函数
 *
 * 程序第一个参数为最大值，默认为0
 * 程序第二个参数为计算模式，默认为0
 *  0：得到所有的勾股数
 *  1：仅得到勾股数的数目
 * 程序第三个参数为是否输出，默认为1（输出）
 * 程序第四个参数为是否排序，默认为0（不排序）
 */
int main(int argc, char *argv[]) {
	//斜边最大值，默认为0
	int maxnum = 0;
	//如果给了最大值
	if (argc > 1) {
		//判断最大值是否溢出
		char c[32] = {'\0'};
		unsigned int a = 1;
		sprintf(c, "%d", (a << (sizeof(a) * 8 - 1)) - 1);
		if (str_smaller(c, argv[1])) {
			//如果溢出
			//输出错误提示
			printf("too large maxmiun number");
			//程序退出
			//返回1表示最大值溢出
			return 1;
		}
		//如果没有溢出
		maxnum = (unsigned int) atol(argv[1]);
	}

	int type = 0;
	if (argc > 2)
		type = atoi(argv[2]);

	int output = 0;
	if (argc > 3)
		output = !atoi(argv[3]);

	int sort = 0;
	if (argc > 4)
		sort = atoi(argv[4]);

	//判断计算方式
	if (!type) {
		//如果是0
		//获取元素总数
		unsigned int buffsize = getLenthOfPythagoreanTriple(maxnum);
		//申请内存
		complex *c = malloc(sizeof(complex) * buffsize);
		//获取结果数组
		PythagoreanTriple p = getPythagoreanTriple(maxnum, c, buffsize);
		//判断是否需要输出
		if (!output) {
			//如果需要输出
			//判断是否需要排序
			if (sort)
				//如果需要排序，则进行排序
				sortPythagoreanTriple(&p);
			//进行输出
			//for循环遍历每个元素
			for (int i = 0; i < p.size; i++) {
				//输出遍历到的元素
				printf("%d,\t%d,\t%d\n", p.c[i].real, p.c[i].imag, p.c[i].module);
			}
			//打印元素总数
			printf("%unumbers\n", p.size);
		}
		//释放内存
		free(c);
	} else {
		//如果是1
		//获取元素数目
		unsigned int number = getLenthOfPythagoreanTriple(maxnum);
		//判断是否需要输出
		if (!output)
			//打印元素数目
			printf("%unumbers\n", number);
	}

	//程序结束
	return 0;
}