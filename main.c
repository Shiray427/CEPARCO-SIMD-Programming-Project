//Template C to x86 call
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdint.h>

extern void asm_1D_stencil(size_t ARRAY_SIZE, int32_t* x, int32_t* y);
extern void xmm_1D_stencil(size_t ARRAY_SIZE, int32_t* x, int32_t* y);
extern void ymm_1D_stencil(size_t ARRAY_SIZE, int32_t* x, int32_t* y);

void c_1D_stencil(size_t ARRAY_SIZE, int32_t* x, int32_t* y) {
	size_t i;
	for (i = 3; i < (ARRAY_SIZE - 3); i++)
		y[i-3] = x[i - 3] + x[i - 2] + x[i - 1] + x[i] + x[i + 1] + x[i + 2] + x[i + 3]; 
}

size_t error_counter(size_t ARRAY_SIZE, int32_t* y1, int32_t* y2) {
	size_t i, count = 0;
	for (i = 0; i < ARRAY_SIZE; i++)
		if (y1[i] != y2[i])
			count++;
	return count;
}

void print_text(double time_average, double time_total, size_t loopcount, size_t Y_ARRAY_SIZE, int32_t out[], char text[]) {
	time_average = time_total / loopcount;
	printf("Total time taken in %s: %lf ms.\nAverage runtime across 30 %s executions: %lf ms.\n", text, time_total, text, time_average);
	printf("Output Y: \n");
	printf("First 10 elements: ");
	for (int j = 0; j < 10; j++)
		printf("%d ", out[j]);
	printf("\nLast 10 elements: ");
	for (int j = Y_ARRAY_SIZE - 10; j < Y_ARRAY_SIZE; j++)
		printf("%d ", out[j]);
	printf("\n");
}
int main() {
	const size_t ARRAY_SIZE = (1 << 20) + 3; //modify to large values (1 << 20, 26, 30)
	const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(int32_t);
	const size_t loopcount = 30;
	int i;
	int32_t * x, * y_c, * y_asm, * y_xmm, * y_ymm;
	clock_t start, end;
	double time_taken, time_total, time_average;
	size_t Y_ARRAY_SIZE = ARRAY_SIZE - 6;
	size_t Y_ARRAY_BYTES = Y_ARRAY_SIZE * sizeof(int32_t);
	size_t error_count;

	printf("Number of elements = %zd\n", ARRAY_SIZE);
	x = (int32_t*)malloc(ARRAY_BYTES);
	if (x == NULL)
		exit(1);
	y_c = (int32_t*)malloc(Y_ARRAY_BYTES);
	if (y_c == NULL)
		exit(1);

	y_asm = (int32_t*)malloc(Y_ARRAY_BYTES);
	if (y_asm == NULL)
		exit(1);

	y_xmm = (int32_t*)malloc(Y_ARRAY_BYTES); 
	if (y_xmm == NULL) 
		exit(1);

	y_ymm = (int32_t*)malloc(Y_ARRAY_BYTES); 
	if (y_ymm == NULL) 
		exit(1);
	
	time_t t;
	srand((unsigned int)time(&t));
	for (i = 0; i < ARRAY_SIZE; i++)
		x[i] = (int32_t)rand() % 16;
	
	for (i = 0; i < Y_ARRAY_SIZE; i++) {
		y_c[i] = 0;
		y_asm[i] = 0;
		y_xmm[i] = 0;
		y_ymm[i] = 0;
	}		


	//C function 
	start = clock();
	c_1D_stencil(ARRAY_SIZE, x, y_c); //call function
	end = clock();
	time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
	printf("First run initialization. Time in C: %lf ms\n", time_taken);

	time_total = 0.0;
	for (i = 0; i < loopcount; i++) {
		start = clock();
		c_1D_stencil(ARRAY_SIZE, x, y_c); //call function
		end = clock();
		time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
		printf("Run #%d. Time in C: %lf ms\n", i+1, time_taken);
		time_total += time_taken;
	}
	time_average = time_total / loopcount;
	print_text(time_average, time_total, loopcount, Y_ARRAY_SIZE, y_c, "C");
	
	
	//x86-64 ASM function
	start = clock();
	asm_1D_stencil(Y_ARRAY_SIZE, x, y_asm); //call function
	end = clock();
	time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
	printf("First run initialization. Time in x86-64 ASM: %lf ms\n", time_taken);

	time_total = 0.0;
	for (i = 0; i < loopcount; i++) {
		start = clock();
		asm_1D_stencil(Y_ARRAY_SIZE, x, y_asm); //call function 
		end = clock();
		time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
		printf("Run #%d. Time in x86-64 ASM: %lf ms\n", i+1, time_taken);
		time_total += time_taken;
	}
	time_average = time_total / loopcount;
	print_text(time_average, time_total, loopcount, Y_ARRAY_SIZE, y_asm, "x86-64 ASM");
	error_count = error_counter(Y_ARRAY_SIZE, y_c, y_asm);
	printf("Output value comparison with C output vector error count: %zd\n", error_count);

	
	//x86 SIMD AVX2 ASM function using XMM register
	start = clock();
	xmm_1D_stencil(Y_ARRAY_SIZE, x, y_xmm); //call function
	end = clock();
	time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
	printf("First run initialization. Time in x86 SIMD AVX2 using XMM register: %lf ms\n", time_taken);

	time_total = 0.0;
	for (i = 0; i < loopcount; i++) {
		start = clock();
		xmm_1D_stencil(Y_ARRAY_SIZE, x, y_xmm); //call function 
		end = clock();
		time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
		printf("Run #%d. Time in x86 SIMD AVX2 using XMM register: %lf ms\n", i+1, time_taken);
		time_total += time_taken;
	}
	time_average = time_total / loopcount;
	print_text(time_average, time_total, loopcount, Y_ARRAY_SIZE, y_c, "x86 SIMD AVX2 using XMM register");
	error_count = error_counter(Y_ARRAY_SIZE, y_c, y_xmm);
	printf("Output value comparison with C output vector error count: %zd\n", error_count);
	

	//x86 SIMD AVX2 ASM function using YMM register
	start = clock();
	ymm_1D_stencil(Y_ARRAY_SIZE , x, y_ymm); //call function
	end = clock();
	time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
	printf("First run initialization. Time in x86 SIMD AVX2 using YMM register: %lf ms\n", time_taken);

	time_total = 0.0;
	for (i = 0; i < loopcount; i++) {
		start = clock();
		ymm_1D_stencil(Y_ARRAY_SIZE, x, y_ymm); //call function 
		end = clock();
		time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
		printf("Run #%d. Time in x86 SIMD AVX2 using YMM register: %lf ms\n", i+1, time_taken);
		time_total += time_taken;
	}
	time_average = time_total / loopcount;
	print_text(time_average, time_total, loopcount, Y_ARRAY_SIZE, y_c, "x86 SIMD AVX2 using YMM register");
	error_count = error_counter(Y_ARRAY_SIZE, y_c, y_ymm);
	printf("Output value comparison with C output vector error count: %zd\n", error_count);
	
	free(x);
	free(y_c);
	free(y_asm);
	free(y_xmm);
	free(y_ymm);
	return 0;
}