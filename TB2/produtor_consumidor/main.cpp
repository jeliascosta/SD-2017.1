#include <iostream>
#include <pthread.h>

using namespace std;

void *print(void*){
  cout << "Thread" << endl;
}

void *consumidor(void*){

}

void *produtor(void*){
  
}

int main(int argc, char *argv[]){
  cout << "Hello World" << endl;
  cout << "thread principal" << endl;

  pthread_t t1;
  pthread_create(&t1, NULL, &print, NULL);
  void* result;
  pthread_join(t1, &result);
  cout << "novamente na thread principal" << endl;


  return 0;
}
