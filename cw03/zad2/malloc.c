#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	const int n = 20;
	char *p;
	int j;
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < 1024; ++j)
			for(int k = 0; k < 1024; ++k)
				if((p = malloc(sizeof(char) * 1000)) == NULL) {
					printf("Error While allocating memory\n");
					exit(1);
				}
    return 0;
}
