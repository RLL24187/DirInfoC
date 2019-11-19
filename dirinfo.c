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

// int filecount(char * c, char * path, int prevsize){ //recursively counts number of files
//   DIR *d;
//   char s[100];
//   char x;
//   strcpy(s, path);
//   strcat(s, c); //create path
//   printf("Path %s\n", s);
//   d = opendir(path);
//   if (errno < 0){
//     printf("ERROR opening directory: %d: %s\n",errno, strerror);
//     return prevsize;
//   }
//   struct dirent *file;
//   file = readdir(d);
//   struct stat buffer;
//   while (file){ //while files are in this directory
//     stat(file->d_name, &buffer);
//     sprintf(s, "%o", buffer.st_mode);
//     x = s[0];
//     if (x == '1'){ //then regular file
//       prevsize ++;
//     } //otherwise directory and run function again on that filename
//     else{ //another directory
//       strcat(s, "/");
//       prevsize = filecount(file->d_name, s, prevsize);
//     }
//     file = readdir(d);
//   }
//   return prevsize;
// }

//attempted to count how many files were inside
void convertpermissions(char * c, int offset){ //offset for directory: 1, regular file: 0
  printf("%s\n", c);
  char s[100];
  strcpy(s, "");
  int i;
  // printf("-");
  for (i = 3 - offset; i < 6 - offset; i++){
    if (*(c + i) == '0'){
      strcat(s, "---");
      // printf("---");
    }
    else if (*(c + i) == '1'){
      strcat(s, "--x");
      // printf("--x");
    }
    else if (*(c + i) == '2'){
      strcat(s, "-w-");
      // printf("-w-");
    }
    else if (*(c + i) == '3'){
      strcat(s, "-wx");
      // printf("-wx");
    }
    else if (*(c + i) == '4'){
      strcat(s, "r--");
      // printf("r--");
    }
    else if (*(c + i) == '5'){
      strcat(s, "r-x");
      // printf("r-x");
    }
    else if (*(c + i) == '6'){
      strcat(s, "rw-");
      // printf("rw-");
    }
    else{ //file permissions is 7
      strcat(s, "rwx");
      // printf("rwx");
    }
  }
  strcpy(c, s);
// from last assignment
}

int main(int argc, char *argv[]){
  DIR *d;
  // Your program should handle user input as follows:
// If a command line argument is entered, use that as the directory to scan.
// If not, the program should ask the user to enter a directory to scan.
// In either case, don't assume your user knows what they are doing, if an invalid directory is entered, take reasonable action (*ahem* errno *cough cough*)
  if (argc > 1){
    d = opendir(argv[1]);
    printf("Printing information for directory '%s'\n", argv[1]);
  }
  else{
    char name[100];
    printf("Enter a file name: \n");
    fgets(name, 100, stdin);
    name[strlen(name)-1] = 0; //changes newline to NULL
    printf("Printing information for directory '%s'\n", name); //adds a newline to the end of name
    d = opendir(name);
    // d = opendir(".");
  }
  if (d == NULL || errno < 0){
    printf("ERROR opening directory: %d: %s\n",errno, strerror(errno));
    return 0;
  }
  struct dirent *file;
  file = readdir(d);
  struct stat buffer;
  stat(".", &buffer);
  if (errno < 0){
    printf("ERROR in stat directory: %d: %s\n",errno, strerror(errno));
    return 0;
  }
  printf("Statistics for directory:\n");
  char reg[200];
  char dir[200];
  char s[100];
  char c;
  int bytes = 0;
  // int files = 0;
  strcpy(reg, ""); //makes sure that they are empty strings
  strcpy(dir, ""); //for some reason reg and dir had weird chars and the beginning
  while (file){
    stat(file->d_name, &buffer);
    bytes += buffer.st_size;
    // files ++;
    sprintf(s, "%o", buffer.st_mode);
    // printf("%o\n", buffer.st_mode);
    // printf("File: %s | Permissions: %s\n", file->d_name, s);
    c = s[0];
    // printf("%s\n", c);
    if (c =='1'){ //regular
      convertpermissions(s, 0);
      strcat(reg, "-");
      strcat(reg, s);
      strcat(reg, "\t");
      strcat(reg, file->d_name);
      strcat(reg, "\n");
      printf("Added %s to reg\n",file->d_name );
    }
    else{
      convertpermissions(s, 1);
      strcat(dir, "d");
      strcat(dir, s);
      strcat(dir, "\t");
      strcat(dir, file->d_name);
      strcat(dir, "\n");
      // files = filecount(file->d_name, "./", files);
      printf("Added %s permissions %o to dir\n",file->d_name, buffer.st_mode );
    }
    // printf("\t%s\n", file->d_name);
    file = readdir(d);
  }
  printf("Total Directory Size: %d Bytes\n", bytes);
  // printf("Number of Files in Directory: %d\n", files);
  printf("Directories: \n");
  printf("%s\n", dir);
  printf("Regular files:\n");
  printf("%s\n", reg);
  d = opendir("testfolder");
  file = readdir(d);
  stat(file->d_name, &buffer);
  printf("filename: %s | permissions: %o\n", file->d_name, buffer.st_mode);
  file = readdir(d);
  stat(file->d_name, &buffer);
  printf("filename: %s | permissions: %o\n", file->d_name, buffer.st_mode);
  file = readdir(d);
  stat(file->d_name, &buffer);
  printf("filename: %s | permissions: %o\n", file->d_name, buffer.st_mode);
  return 0;
}
