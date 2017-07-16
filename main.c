#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

char *separateString(char *input){
  char *tok, *input2 = (char *) malloc(strlen(input) * sizeof(char)), *token = "/";
  int i = 0, length = 0, tokenCount = 0;

  for(length = 0; input[length]; length++){
    if(input[length] == '/'){
      tokenCount++;
    }      
  }
  
  strcpy(input2,input);

  //allocate double array output
  char *separatedString = (char *) malloc(length * sizeof(char));

  input2[length+1] = 0;

  tok = strtok(input2, token);
  ++tok;

  for(i = 0; tok != NULL; i++){
    strcpy(separatedString, tok);
    tok = strtok(NULL, token);
  }
  int end = strlen(separatedString);

  separatedString[end-1] = '\0';

  free(input2);

  return separatedString;
}

char *checkBeginning(char *input){
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

  char *output = (char *) malloc((strlen(input))*sizeof(char));

  strcpy(output, input);
  return output;
}

int main(int argc, char **argv){
  if(argc != 3){
    printf("Requires two input arguments\n");
    return 1;
  }

  int sourceFileDesc, targetFileDesc, error = 0, n;  
  char buffer[1024], *fileName;//, m3uLine[1024], *temp;

  char *m3uLine = (char *) malloc(1024*sizeof(char));

  char *m3u = (char *) malloc(strlen(argv[1]) * sizeof(char));
  strcpy(m3u, argv[1]);

  char *directory = (char *) malloc((strlen(argv[2])+1024) * sizeof(char));

  FILE *m3uFile;
  m3uFile = fopen(m3u, "r");

  while(fgets(m3uLine, 1024, m3uFile)){

    //Prepare string to be read
    m3uLine = checkBeginning(m3uLine);
    fileName = separateString(m3uLine);

    strcpy(directory, argv[2]);

    for(int i = strlen(m3uLine)-1; m3uLine[i]; i--){
      if(m3uLine[i] == '\n'){
	m3uLine[i] = '\0';
      }	
    }
    printf("Copying: %s\n", m3uLine);

    if(directory[strlen(directory)] != '/'){
      strcat(directory, "/");
    }
    strcat(directory, fileName);

    printf("Destination: %s\n\n", directory);
    //

    
    sourceFileDesc = open(m3uLine, O_RDONLY);
    targetFileDesc = open(directory, O_CREAT | O_WRONLY | O_APPEND, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH | S_IXUSR | S_IXOTH);

    error = read(sourceFileDesc, buffer, 10000000000);
    if(error == -1){
      printf("Error reading.\n");
      return 1;
    }

    n = error;
    printf("n = %d\n", n);
    
    error = write(targetFileDesc, buffer, n);

    if(error == -1){
      printf("Error writing.\n");
      return 1;
    }

    close(sourceFileDesc);
    close(targetFileDesc);

  }

  free(m3u);
  free(directory);
  free(m3uLine);
    
  fclose(m3uFile);
  
  return 0;
}
