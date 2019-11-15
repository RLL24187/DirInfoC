// list all the files in the directory
// specify which files are directories (if any)
// show the total size of all the regular files the directory
// note that you do not have to recursively go through any subdirectories to find their size for this part (unless you want to, but that is not a simple task)
// Have the program scan the current directory
// Possible enhancements:
// Recursively list the files in any subdirectories, update the total to include the total size of subdirectories
// Print out the size in a more readable format (like using KB, MB, GB for -byte prefixes)
// Make it look more like $ ls -l

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

int main(){
  DIR *d;
  d = opendir(".");
  if (errno < 0){
    printf("ERROR opening directory: %d: %s\n",errno, strerror);
  }
  struct dirent *file;
  file = readdir(d);
  struct stat buffer;
  stat(".", &buffer);
  if (errno < 0){
    printf("ERROR in stat directory: %d: %s\n",errno, strerror);
  }
  printf("Statistics for directory:\n");
  char reg[100];
  char dir[100];
  char s[100];
  char c;
  int bytes = 0;
  strcpy(reg, ""); //makes sure that they are empty strings
  strcpy(dir, ""); //for some reason reg and dir had weird chars and the beginning
  while (file){
    stat(file->d_name, &buffer);
    bytes += buffer.st_size;
    sprintf(s, "%o", buffer.st_mode);
    // printf("%o\n", buffer.st_mode);
    // printf("%s\n", s);
    c = s[0];
    // printf("%s\n", c);
    if (c =='1'){ //regular
      strcat(reg, "\t");
      strcat(reg, file->d_name);
      strcat(reg, "\n");
      // printf("Added %s to reg\n",file->d_name );
    }
    else{
      strcat(dir, "\t");
      strcat(dir, file->d_name);
      strcat(dir, "\n");
    }
    // printf("\t%s\n", file->d_name);
    file = readdir(d);
  }
  printf("Total Directory Size: %d Bytes\n", bytes);
  printf("Directories: \n");
  printf("%s\n", dir);
  printf("Regular files:\n");
  printf("%s\n", reg);
  return 0;
}
