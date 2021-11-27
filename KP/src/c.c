#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	int pac = atoi(argv[1]);
	int pca = atoi(argv[2]);
	int pcb = atoi(argv[3]);

	size_t size;

	while (read(pac, &size, sizeof(size_t)) > 0)
	{
		char *str = (char *)malloc(size);
		if (str == NULL)
		{
			printf("MALLOC from C\n");
		}
		read(pac, str, size);
		printf("[C] - From A: %s\n", str);
		write(pcb, &size, sizeof(size_t));
		int ok = 1;
		write(pca, &ok, sizeof(int));
		free(str);
	}

	close(pac);
	close(pca);
	close(pcb);

	return 0;
}
