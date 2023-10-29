#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdatomic.h>

#define type_t uint64_t
/* Itertations for one core when multithreading */
#define PARA 1000000
/*
* Collatz conjecture
* gcc  -O3  Collatz.c -o col
* gcc  -O3 -DMP -fopenmp Collatz.c -o col
*/

int main(int argc, char *argv[]){
	if(argc != 2){
		puts("Need Number of iterations...");
		return -1;
	}
	int total = atoi(argv[1]);
	int totalM = total;
	if(total <= 0){
		puts("Need a Number between 1 - 2^64-1..");
		return -1;
	}
	struct timeval start, end;
	clock_t begin = clock();
	gettimeofday(&start, NULL);
	type_t b = 5;
	#ifdef MP
		type_t _Atomic count = 0;
		totalM = ((totalM/PARA)*PARA);
		total = PARA;
		#pragma omp parallel for
		for(b = 5; b < totalM;b+=PARA)
	#else
		register type_t count = 0;
	#endif
	{
		register type_t cnt = 0;
		for(type_t i = b; i < total+b;i++){
			register type_t t = i;
			while(t > 1){
				register type_t tmp = t;
				t>>=1;
				t+=(tmp&1)*(tmp+1);
				cnt+=1+(tmp&1);
			}
			cnt += t>>1;
		}
		count += cnt; // so race conditions are kept low
	}
	gettimeofday(&end, NULL);
	clock_t stop = clock();
	printf("It took %lf seconds and %lf cpu seconds to do %lu iterations of the Collatz conjecture totaling %lu steps!\n", (end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec)/1000000,(double)(stop - begin) / CLOCKS_PER_SEC,totalM,count);
	return 0;
}
