#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
  if(argc != 3){
    printf("Requires two input arguments\n");
    return 1;
  }
  
  char *m3u = (char *) malloc(strlen(argv[1]) * sizeof(char));
  strcpy(m3u, argv[1]);

  char *directory = (char *) malloc(strlen(arv[2]) * sizeof(char));
  strcpy(directory, arv[2]);

  FILE *m3uFile;

  m3uFile = fopen(m3u, "r");

  while();
	 
  return 0;
}
