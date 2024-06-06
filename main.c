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
		y[i - 3] = x[i - 3] + x[i - 2] + x[i - 1] + x[i] + x[i + 1] + x[i + 2] + x[i + 3]; 
}

void error_counter(size_t ARRAY_SIZE, int32_t* in, int32_t* out) {
	size_t i, count = 0;
	for (i = 3; i < (ARRAY_SIZE - 3); i++)
		if (out[i - 3] != in[i - 3] + in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] + in[i + 3])
			count++;
	printf("Output value comparison with C output vector error count: %zd\n", count);
}

void print_text(double time_average, double time_total, size_t Y_ARRAY_SIZE, int32_t* out, char text[]) {
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
	const size_t ARRAY_SIZE = (1 << 20) + 7; //modify to large values (1 << 20, 26, 30)
	const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(int32_t);
	const size_t Y_ARRAY_SIZE = ARRAY_SIZE - 6;
	const size_t Y_ARRAY_BYTES = Y_ARRAY_SIZE * sizeof(int32_t);
	const size_t loopcount = 30;
	size_t i, error_count;
	int32_t * x, * y;
	double time_taken, time_total, time_average;
	LARGE_INTEGER start, end, freq;
	QueryPerformanceFrequency(&freq);

	printf("Number of elements = %zd\n", ARRAY_SIZE);
	x = (int32_t*)malloc(ARRAY_BYTES);
	if (x == NULL)
		exit(1);
	y = (int32_t*)malloc(Y_ARRAY_BYTES);
	if (y == NULL)
		exit(1);

	time_t t;
	srand((unsigned int)time(&t));
	for (i = 0; i < ARRAY_SIZE; i++)
		x[i] = (int32_t)rand();

	//C function
	for (i = 0; i < Y_ARRAY_SIZE; i++)
		y[i] = 0;
	QueryPerformanceCounter(&start);
	c_1D_stencil(ARRAY_SIZE, x, y); //call function
	QueryPerformanceCounter(&end);
	time_taken = (double)(end.QuadPart - start.QuadPart) * 1e3 / freq.QuadPart;
	printf("First run initialization. Time in C: %lf ms\n", time_taken);
	
	time_total = 0.0;
	for (i = 0; i < loopcount; i++) {
		QueryPerformanceCounter(&start);
		c_1D_stencil(ARRAY_SIZE, x, y); //call function
		QueryPerformanceCounter(&end);
		time_taken = (double)(end.QuadPart - start.QuadPart) * 1e3 / freq.QuadPart;
		printf("Run #%zd. Time in C: %lf ms\n", i+1, time_taken);
		time_total += time_taken;
	}
	time_average = time_total / loopcount;
	print_text(time_average, time_total, Y_ARRAY_SIZE, y, "C");
	

	
	//x86-64 ASM function
	for (i = 0; i < Y_ARRAY_SIZE; i++)
		y[i] = 0;
	QueryPerformanceCounter(&start);
	asm_1D_stencil(Y_ARRAY_SIZE, x, y); //call function
	QueryPerformanceCounter(&end);
	time_taken = (double)(end.QuadPart - start.QuadPart) * 1e3 / freq.QuadPart;
	printf("First run initialization. Time in x86-64 ASM: %lf ms\n", time_taken);

	time_total = 0.0;
	for (i = 0; i < loopcount; i++) {
		QueryPerformanceCounter(&start);
		asm_1D_stencil(Y_ARRAY_SIZE, x, y); //call function 
		QueryPerformanceCounter(&end);
		time_taken = (double)(end.QuadPart - start.QuadPart) * 1e3 / freq.QuadPart;
		printf("Run #%zd. Time in x86-64 ASM: %lf ms\n", i+1, time_taken);
		time_total += time_taken;
	}
	time_average = time_total / loopcount;
	print_text(time_average, time_total, Y_ARRAY_SIZE, y, "x86-64 ASM");
	error_counter(Y_ARRAY_SIZE, x, y);
	
	
	
	//x86 SIMD AVX2 ASM function using XMM register
	for (i = 0; i < Y_ARRAY_SIZE; i++)
		y[i] = 0;
	QueryPerformanceCounter(&start);
	xmm_1D_stencil(Y_ARRAY_SIZE, x, y); //call function
	QueryPerformanceCounter(&end);
	time_taken = (double)(end.QuadPart - start.QuadPart) * 1e3 / freq.QuadPart;
	printf("First run initialization. Time in x86 SIMD AVX2 using XMM register: %lf ms\n", time_taken);

	time_total = 0.0;
	for (i = 0; i < loopcount; i++) {
		QueryPerformanceCounter(&start);
		xmm_1D_stencil(Y_ARRAY_SIZE, x, y); //call function
		QueryPerformanceCounter(&end);
		time_taken = (double)(end.QuadPart - start.QuadPart) * 1e3 / freq.QuadPart;
		printf("Run #%zd. Time in x86 SIMD AVX2 using XMM register: %lf ms\n", i+1, time_taken);
		time_total += time_taken;
	}
	time_average = time_total / loopcount;
	print_text(time_average, time_total, Y_ARRAY_SIZE, y, "x86 SIMD AVX2 using XMM register");
	error_counter(Y_ARRAY_SIZE, x, y);
	
	

	//x86 SIMD AVX2 ASM function using YMM register
	for (i = 0; i < Y_ARRAY_SIZE; i++)
		y[i] = 0;
	QueryPerformanceCounter(&start);
	ymm_1D_stencil(Y_ARRAY_SIZE, x, y); //call function
	QueryPerformanceCounter(&end);
	time_taken = (double)(end.QuadPart - start.QuadPart) * 1e3 / freq.QuadPart;
	printf("First run initialization. Time in x86 SIMD AVX2 using YMM register: %lf ms\n", time_taken);

	time_total = 0.0;
	for (i = 0; i < loopcount; i++) {
		QueryPerformanceCounter(&start);
		ymm_1D_stencil(Y_ARRAY_SIZE, x, y); //call function
		QueryPerformanceCounter(&end);
		time_taken = (double)(end.QuadPart - start.QuadPart) * 1e3 / freq.QuadPart;
		printf("Run #%zd. Time in x86 SIMD AVX2 using YMM register: %lf ms\n", i+1, time_taken);
		time_total += time_taken;
	}
	time_average = time_total / loopcount;
	print_text(time_average, time_total, Y_ARRAY_SIZE, y, "x86 SIMD AVX2 using YMM register");
	error_counter(Y_ARRAY_SIZE, x, y);
	
	free(x);
	free(y);
	return 0;
}