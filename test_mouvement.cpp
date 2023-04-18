#include "monde.hpp"
#include "evolution.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <unistd.h>

using namespace std;

int main(int argc, char**argv){
  srand(atoi(argv[5]));
  Monde M = initialiser_Monde(atoi(argv[1]),atoi(argv[2]),atoi(argv[3]));
  //afficher_Monde(M);
  evolution_output(&M,atoi(argv[4]));
  //afficher_Monde(M);
  observer(&M);
  return 0;
}