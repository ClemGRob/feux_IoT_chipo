#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// Global variable:
int i = 2;

void* foo(void* p){
  // Print value received as argument:
  printf("Value recevied as argument in starting routine: ");
  for(int i = 0; i<10; i++)
  {
    sleep(1);
    printf("%i\n", * (int*)p);
    }

  // Return reference to global variable:
  pthread_exit(&i);
}

int main(void){
  // Declare variable for thread's ID:
  pthread_t id[2];

  int j = 1;
  int k = 7;
  pthread_create(&id[0], NULL, foo, &j);
  pthread_create(&id[1], NULL, foo, &k);
    
  int* ptr;

  // Wait for foo() and retrieve value in ptr;
  pthread_join(id[0], (void**)&ptr);
  pthread_join(id[1], (void**)&ptr);
  printf("Value recevied by parent from child: ");
  printf("%i\n", *ptr);
}