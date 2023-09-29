#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Name: Justin Lugo
Class: CIS 3360
Teacher: Professor McAlpin
Assignment: HW2 - Checksum
*/

unsigned int long bitmask(char *word, int size){
	int i = 0;
	unsigned int long mask;
	mask = word[i];

	for(size -=8; size != 0; size -= 8){
		mask = (mask << 8) + word[i + 1];
		i++;
	}

	return mask;
}

void checksum(int size, char *input, unsigned int long *check){
	unsigned int long word;

	word = bitmask(input, size);

	*check = word + *check;

	*check = *check << (64 - size);
	*check = *check >> (64 - size);

	printf("%s", input);
}

void readFile(char *filename, int size){
	FILE *fp;
	int buffer, i = 0, j = 0, k, characterCount = 0, wordLength = size / 8 + 1;
	unsigned int long *check = calloc(1, sizeof(unsigned int long));
	char *output = malloc(sizeof(char) * wordLength);

	if (size != 8 && size != 16 && size != 32){
		fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n"); // Making sure that the entered checksum size is one of the three.
		return;
	}

	fp = fopen(filename, "r");
	printf("\n");

	if(fp == NULL){
		printf("Unable to open file.\n");
		return;
	}

	while(buffer != EOF){
		if (wordLength - i == 1){
			output[i] = '\0';
			checksum(size, output, check);

			j++;
			i = 0;

			if (j * (wordLength - 1) == 80) // Print 80 characters per line.
				printf("\n");
		}
		else{
			buffer = fgetc(fp);

			if (buffer == EOF){
				if (size == 16 || size == 32){
					if (wordLength - i != 1 && wordLength - i != wordLength){
						for (k = 0; wordLength - i != 1 && wordLength - i != wordLength; i++, k++)
							output[i] = 'X';

						output[i] = '\0';

						checksum(size, output, check);

						j++;
						characterCount += k - 1;
						i = 0;

						if (j * (wordLength - 1) == 80) // Print 80 characters per line
							printf("\n");

					} else
						break;

				} else
					break;
			} else
				output[i] = buffer;

			characterCount++;
			i++;
		}

	}

	printf("\n");
	printf("%2d bit checksum is %8lx for all %4d chars\n", size, *check, characterCount);

	fclose(fp);
}

int main(int argc, char *argv[]){
	readFile(argv[1], atoi(argv[2]));

	return 0;
}
