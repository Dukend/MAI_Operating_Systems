#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void child_work(int from, int to)
{
	char buf[1];
	while (read(from, buf, 1) > 0)
	{
		char c = buf[0];
		if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' && c != 'y' &&
			c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U' && c != 'Y')
		{
			write(to, buf, 1);
		}
	}
	close(to);
	close(from);
}

void parrent_work(int child1, int child2)
{
	char buf[1];
	int is_even = 0;
	while (read(STDIN_FILENO, buf, 1) > 0)
	{
		if (!is_even)
		{
			write(child1, buf, 1);
		}
		else
		{
			write(child2, buf, 1);
		}
		if (buf[0] == '\n')
		{
			is_even = !is_even;
		}
	}

	close(child1);
	close(child2);
}

int open_file()
{
	const size_t NAME_SIZE = 64;
	char f_name[NAME_SIZE];
	char buf[1];
	int idx = 0;
	while (idx < NAME_SIZE && read(STDIN_FILENO, buf, 1) > 0)
	{
		if (buf[0] == '\n')
		{
			break;
		}
		f_name[idx++] = buf[0];
	}
	f_name[idx] = '\0';
	return open(f_name, O_CREAT | O_TRUNC, 00777);
}

int main(int argc, char *argv[])
{
	int f1 = open_file();
	if (f1 == -1)
	{
		perror("File not open");
		exit(1);
	}
	int f2 = open_file();
	if (f2 == -1)
	{
		perror("File not open");
		exit(2);
	}

	int pipefd1[2];
	if (pipe(pipefd1) == -1)
	{
		perror("Cannot create pipe");
		exit(3);
	}

	int child1 = fork();
	if (child1 == -1)
	{
		perror("Can not create process");
		exit(4);
	}
	if (child1 == 0)
	{
		close(pipefd1[1]);
		child_work(pipefd1[0], f1);
		return 0;
	}
	close(pipefd1[0]);

	int pipefd2[2];

	if (pipe(pipefd2) == -1)
	{
		perror("Cannot create pipe");
		exit(5);
	}

	int child2 = fork();
	if (child2 == -1)
	{
		perror("Can not create process");
		exit(6);
	}
	if (child2 == 0)
	{
		close(pipefd1[1]);
		close(pipefd2[1]);
		child_work(pipefd2[0], f2);
		return 0;
	}
	close(pipefd2[0]);
	parrent_work(pipefd1[1], pipefd2[1]);
	return 0;
}