#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *temp = fopen(*++argv, "r");
	size_t c = 0;
	char a[128] = {0};

	while ((c = fread(a, sizeof(char), 128, temp))) {
		for (int c = 0; c < 128; c++) {
			if (a[c]) {
				for (int b = 7; b >= 0; b--) {
					if (a[c] & (1<<b)) {
						printf("1");
					} else {
						printf("0");
					}
				}
			}
		}
	}

	fclose(temp);
}

