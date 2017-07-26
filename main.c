#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

char *separateString(char *input, char delimiter){//extracts file name from the absolute path given in file
  char *tok, token[2];
  token[0] = delimiter;
  token[1] = '\0';

  int i = 0, length = 0, tokenCount = 0;

  for(length = 0; input[length]; length++){
    if(input[length] == '/'){
      tokenCount++;
    }
  }

  char *input2 = malloc((length+2) * sizeof(input2));

  //copy input to another string because strtok destroys the original string
  strcpy(input2,input);

  char *separatedString = malloc((length) * sizeof(separatedString));

  input2[length+1] = 0;

  tok = strtok(input2, token);
  ++tok;
    
  for(i = 0; tok != NULL; i++){
    strcpy(separatedString, tok);
    tok = strtok(NULL, token);
  }

  separatedString[strlen(separatedString)] = '\0';

  free(input2);

  return separatedString;
}

char *checkBeginning(char *input){//checks the beginning of each string to get rid of chars not needed for directory
  int a = 0, b = 0;

  while(a*b == 0){
    a = 1;
    b = 1;
    if(input[1] == '/' && input[0] == '/'){
      *input++;
      a = 0;
    }

    if(!strchr("/~", input[0])){
      *input++;
      b = 0;
    }
  }

  return input;
}

int main(int argc, char **argv){
  if(argc != 3){//0: function, 1: m3u/music list file, 2: target directory to copy to
    printf("Requires two input arguments\n");
    return 1;
  }

  int sourceFileDesc, targetFileDesc, error, n;
  char  *fileName;
  struct stat st;

  char *directory = malloc((strlen(argv[2]) + 1024) * sizeof(directory));
  char *m3uLine = malloc(1024 * sizeof(m3uLine));

  char *m3u = malloc((strlen(argv[1])+2) * sizeof(m3u));
  strcpy(m3u, argv[1]);

  FILE *m3uFile;
  m3uFile = fopen(m3u, "r");

  while(fgets(m3uLine, 1024, m3uFile)){

    //Prepare string to be read
    strcpy(directory, argv[2]);

    for(int i = strlen(m3uLine)-1; m3uLine[i]; i--){
      if(m3uLine[i] == '\n'){
	m3uLine[i] = '\0';
      }	
    }

    m3uLine = checkBeginning(m3uLine);

    fileName = separateString(m3uLine, '/');

    printf("Copying: %s\n", m3uLine);

    strcpy(directory, argv[2]);
    if(directory[strlen(directory)-1] != '/'){ //for if the target directory is input without a '/' at the end
      strcat(directory, "/");
    }

    strcat(directory, fileName);
    printf("Destination: %s\n\n", directory);
    //

    sourceFileDesc = open(m3uLine, O_RDONLY); 
    targetFileDesc = open(directory, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXOTH); //file is written as 644
    
    stat(m3uLine, &st);
    n = st.st_size; //file size

    char *buffer = malloc(n*sizeof(buffer));
    error = read(sourceFileDesc, buffer, n);

    if(error == -1){
      printf("Error reading.\n");
      return 1;
    }

    error = write(targetFileDesc, buffer, error);

    if(error == -1){
      printf("Error writing.\n");
      return 1;
    }

    free(fileName);
    free(buffer);

    close(sourceFileDesc);
    close(targetFileDesc);
  }
  printf("Done.\n");

  free(m3u);
  fclose(m3uFile);

  return 0;
}
