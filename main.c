#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

//extracts file name from the absolute path given in file
char *separateString(char *input, char delimiter) {
        int length = strlen(input);
	int length2 = length;
	//empty for
	for(; (length2 > 0) && (input[length2] != delimiter); --length2);

	if(length != length2)
		input += length2;

	return input;
}


//checks the beginning of each string to get rid of chars not needed for directory
char *checkBeginning(char *input) {
	int a = 0, b = 0;

	while(a * b == 0) {
		a = 1;
		b = 1;
		if(input[1] == '/' && input[0] == '/') {
			*input++;
			a = 0;
		}

		if(!strchr("/~", input[0])) {
			*input++;
			b = 0;
		}
	}

	return input;
}


int main(int argc, char **argv) {
	//0: function, 1: m3u/music list file, 2: target directory to copy to
	if(argc != 3) {
		printf("Requires two input arguments\n");
		return 1;
	}

	int error = 0;
	int size = 1024;
	int buf_size = 4096;

	char *mem = malloc( (size + size + buf_size) * sizeof(*mem));

	char *buffer = mem;
	char *dest = mem + buf_size;
	char *line = dest + size;

	char *begin_line = line;

	FILE *m3uFile = fopen(argv[1], "r");

	strncpy(dest, argv[2], size);
	char *dest_end = dest + strlen(dest);
	
	while(fgets(line, size, m3uFile)) {
		//Prepare string to be read
		int len = strlen(line);
		if(line[len - 1] == '\n')
			line[len - 1] = '\0';

		line = checkBeginning(line);

		char *file_name = separateString(line, '/');
		strncpy(dest_end, file_name, size - strlen(dest));
		//

		int src_fd = open(line, O_RDONLY);

		//file is written as 644
		int dest_fd = open(dest,
				   O_CREAT | O_WRONLY,
				   S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXOTH);

		printf("Copying: %s\n", line);
		printf("Destination: %s\n\n", dest);
		while( (error = read(src_fd, buffer, buf_size)) ) {
			if(error == -1) {
				printf("Error reading.\n");
				break;
			}

			error = write(dest_fd, buffer, error);
			if(error == -1) {
				printf("Error writing.\n");
				break;
			}
		}

		close(src_fd);
		close(dest_fd);
		line = begin_line;
	}
	printf("Done.\n");

	free(mem);
	fclose(m3uFile);

	return 0;
}
