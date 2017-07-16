#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

char *separateString(char input[]){
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


int main(int argc, char **argv){
  if(argc != 3){
    printf("Requires two input arguments\n");
    return 1;
  }

  int sourceFileDesc, targetFileDesc, error = 0, n;  
  char buffer[1024], *fileName, m3uLine[1024];

  char *m3u = (char *) malloc(strlen(argv[1]) * sizeof(char));
  strcpy(m3u, argv[1]);

  char *directory = (char *) malloc((strlen(argv[2])+1024) * sizeof(char));
  strcpy(directory, argv[2]);

  FILE *m3uFile;

  m3uFile = fopen(m3u, "r");

  while(fgets(m3uLine, sizeof(m3uLine), m3uFile)){
    fileName = separateString(m3uLine);
    strcat(directory, fileName);
    
    sourceFileDesc = open(m3uLine, O_RDONLY);
    targetFileDesc = open(directory, O_CREAT | O_WRONLY);
    
    error = read(sourceFileDesc, buffer, n);
    if(error == -1){
      printf("Error reading.\n");
      return 1;
    }

    n = error;

    if(n == 0){
      break;
    }
    
    write(targetFileDesc, buffer, n);
    if(error == -1){
      printf("Error writing.\n");
      return 1;
    }
  }

  free(m3u);
  free(directory);
  
  close(sourceFileDesc);
  close(targetFileDesc);
  
  fclose(m3uFile);
  
  return 0;
}
