#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char buffer[256];
	char command[256];
	strcpy(command, "ls");
	FILE *output = popen(command, "r");
	fread(buffer, 1, 255, output);
	pclose(output);
	printf("buffer is: %s\n", buffer);
	return 0;
}