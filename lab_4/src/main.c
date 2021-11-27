#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int handler = 0;
const int OFFSET = sizeof(int);

void update_handler(int num)
{
	handler = 1;
}

void child_work(char *from, int to)
{
	int idx = OFFSET;
	while (1)
	{
		if (idx < OFFSET + ((int *)from)[0])
		{
			char c = from[idx];
			if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' && c != 'y' &&
				c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U' && c != 'Y')
			{
				write(to, &c, 1);
			}
			++idx;
		}
		else
		{
			if (handler)
			{
				break;
			}
		}
	}
	close(to);
}

void parrent_work(pid_t child1, char *child_map1, pid_t child2, char *child_map2)
{
	char ch;
	int is_even = 0;
	int idx1 = OFFSET;
	int idx2 = OFFSET;
	while (read(STDIN_FILENO, &ch, 1) > 0)
	{
		if (!is_even)
		{
			child_map1[idx1++] = ch;
		}
		else
		{
			child_map2[idx2++] = ch;
		}
		if (ch == '\n')
		{
			if (!is_even)
			{
				((int *)child_map1)[0] = idx1 - OFFSET;
			}
			else
			{
				((int *)child_map2)[0] = idx2 - OFFSET;
			}
			is_even = !is_even;
		}
	}

	kill(child1, SIGUSR1);
	kill(child2, SIGUSR1);
	int res1;
	int res2;
	waitpid(child1, &res1, 0);
	waitpid(child2, &res2, 0);
	if (res1 != 0 || res2 != 0)
	{
		fprintf(stderr, "Something went wrong!\n%d %d\n", res1, res2);
	}
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
	f_name[idx++] = '\0';
	return open(f_name, O_TRUNC | O_CREAT, 00777);
}

void *check_map_creation()
{
	void *m_file = mmap(NULL, 2048, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (m_file == MAP_FAILED)
	{
		perror("Cannot create mmap");
		exit(-2);
	}
	((int *)m_file)[0] = 0;
	return m_file;
}

void add_signals()
{
	void (*func)(int);
	func = signal(SIGUSR1, update_handler);
	if (func == SIG_IGN)
	{
		perror("Cannot add signal");
		exit(-4);
	}
}

int main(int argc, char *argv[])
{
	add_signals();

	int f1 = open_file();
	if (f1 == -1)
	{
		perror("File not open");
		exit(-1);
	}

	int f2 = open_file();
	if (f2 == -1)
	{
		perror("File not open");
		exit(-2);
	}

	char *m_file1 = check_map_creation();
	pid_t child1 = fork();
	if (child1 == -1)
	{
		perror("Cannot create process");
		exit(-3);
	}
	if (child1 == 0)
	{
		close(f2);
		child_work(m_file1, f1);
		return 0;
	}
	close(f1);

	char *m_file2 = check_map_creation();
	pid_t child2 = fork();
	if (child2 == -1)
	{
		perror("Cannot create process");
		exit(-4);
	}
	if (child2 == 0)
	{
		child_work(m_file2, f2);
		return 0;
	}
	close(f2);
	parrent_work(child1, m_file1, child2, m_file2);
	return 0;
}