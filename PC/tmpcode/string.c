#include <stdio.h>
#include <string.h>
int main()
{
  char str[] = "Welcome to StackHowTo best online learning platform";
  int len = strlen(str);
  char delimiter[] = " ";
  
  char *p = strtok(str, delimiter);
  printf("'%s'\n", p);
  char *a = strtok(NULL, delimiter);
  printf("'%s'\n", a);
  while(p != NULL)
  {
    printf("'%s'\n", p);
    p = strtok(NULL, delimiter);
  }
  printf("\n");
  return 0;
}