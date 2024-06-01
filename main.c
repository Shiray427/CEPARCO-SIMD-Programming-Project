//Template C to x86 call
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

extern int asmfunc();

int main() {
	asmfunc();
	return 0;
}