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

int main() {
	const size_t ARRAY_SIZE = 1 << 20; //modify to large values (1 << 20, 26, 30)
	const size_t ARRAY_BYTES = ARRAY_SIZE * sizeof(int32_t);
	const size_t loopcount = 30;
	int i;
	int32_t * x, * y;
	clock_t start, end;
	double time_taken, time_total, time_average;
	size_t Y_ARRAY_SIZE = ARRAY_SIZE - 6;
	size_t Y_ARRAY_BYTES = Y_ARRAY_SIZE * sizeof(int32_t);

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
		x[i] = (int32_t)rand() % 16;
	
	for (i = 0; i < Y_ARRAY_SIZE; i++)
		y[i] = 0;

	//C function 
	start = clock();
	c_1D_stencil(ARRAY_SIZE, x, y); //call function
	end = clock();
	time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
	printf("First run initialization. Time in C: %lf ms\n", time_taken);

	time_total = 0.0;
	for (i = 0; i < loopcount; i++) {
		start = clock();
		c_1D_stencil(ARRAY_SIZE, x, y); //call function
		end = clock();
		time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
		printf("Run #%d. Time in C: %lf ms\n", i, time_taken);
		time_total += time_taken;
	}
	printf("Output Y: \n");
	printf("First 10 elements: ");
	for (int j = 0; j < 10; j++)
		printf("%d ", y[j]);
	printf("\nLast 10 elements: ");
	for (int j = Y_ARRAY_SIZE - 10; j < Y_ARRAY_SIZE; j++)
		printf("%d ", y[j]);
	
	printf("\n");
	time_average = time_total / loopcount;
	printf("Total time taken in C: %lf ms.\nAverage C runtime across 30 executions: %lf ms.\n", time_total, time_average);

	//x86-64 ASM function
	start = clock();
	asm_1D_stencil(ARRAY_SIZE, x, y); //call function
	end = clock();
	time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
	printf("First run initialization. Time in x86-64 ASM: %lf ms\n", time_taken);

	time_total = 0.0;
	for (i = 0; i < loopcount; i++) {
		start = clock();
		asm_1D_stencil(ARRAY_SIZE, x, y); //call function
		end = clock();
		time_taken = (double)(end - start) * 1e3 / CLOCKS_PER_SEC;
		printf("Run #%d. Time in x86-64 ASM: %lf ms\n", i, time_taken);
		time_total += time_taken;
	}
	printf("Output Y: \n");
	printf("First 10 elements: ");
	for (int j = 0; j < 10; j++)
		printf("%d ", y[j]);
	printf("\nLast 10 elements: ");
	for (int j = Y_ARRAY_SIZE - 10; j < Y_ARRAY_SIZE; j++)
		printf("%d ", y[j]);

	printf("\n");
	time_average = time_total / loopcount;
	printf("Total time taken in C: %lf ms.\nAverage C runtime across 30 executions: %lf ms.\n", time_total, time_average);




	free(x);
	free(y);
	return 0;
}