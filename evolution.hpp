#ifndef EVOLUTION_HPP_
#define EVOLUTION_HPP_
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <random>
#include "monde.hpp"

// Crée un monde de dimensions largeurxhauteur, l'initialise
// et y ajoute nb individus dont 1/4 seront des prédateurs
Monde initialiser_Monde(long nb, long largeur, long hauteur){
  Monde M = creer_Monde(hauteur,largeur);
  Case c;
  if(nb > largeur*hauteur)
    nb = largeur*hauteur;
  for(int i=0; i<nb; i++){
    if(i >= round(nb/4.0)){ // 1 individu sur 4 sera un prédateur 
      ajouter_Proie(&M);
    }
    else{
      ajouter_Predateur(&M);
    }
  }
  M.taux_rep_proie = 0.2;
  M.taux_rep_pred = 0.1;
  return M;
}

//Compte le nombre de proies et de prédateurs et calcule l'énergie moyenne
//des prédateurs afin d'actualiser les champs de la structure Monde M passée en paramètre
void actualiser_Monde(Monde *M){
  float sommeEnergie = 0;
  int nbproie = 0;
  int nbpred = 0;
  for(int i=0; i<M->nb_individus; i++){
    if(M->Individus[i].TypeIndiv == 1)
      nbproie++;
    else{
      nbpred++;
      sommeEnergie += M->Individus[i].energie;
    }
  }
  M->nbproie = nbproie;
  M->nbpred = nbpred;
  M->energieMoy = sommeEnergie/nbpred;
}

//Affiche certaines informations du Monde M passé en paramètre
void observer(Monde *M){
  actualiser_Monde(M);
  cout << "Nombre espèces : " << M->nb_individus << endl;
  cout << "Nombre proies : " << M->nbproie << endl;
  cout << "Nombre prédateurs : " << M->nbpred << endl;
  cout << "Energie moyenne des prédateurs : " << M->energieMoy<< endl;
}

//Incrémente l'âge de l'individu I passé en paramètre
// et le déclare mort si son âge dépasse 10
void vieillir_individu(Individu *I){
  I->age++;
  if(I->age >= 10){
    I->mort = true;
    I->raison_mort = "Vieillesse";
  }
}

//Vieilli l'ensemble des individus du Monde M passé en paramètre
 void vieillir_population(Monde *M){
   for(int i=0;i<M->nb_individus;i++){
     if(not M->Individus[i].mort)
       vieillir_individu(&M->Individus[i]);
   }
 }

//Fait disparaître l'ensemble des individus morts du Monde M passé en paramètre
void disparition_population(Monde *M){
  int i = 0;
  while(i < M->nb_individus){
    if(M->nb_individus>0 and M->Individus[i].mort){
      disparition_individu(M,i);
    }
    else{
      i++;
    }
  }
}

//Simule la reproduction d'un individu en utilisant une Loi de Poisson
// en fonction du taux de reproduction lambda de l'espèce
int simu_poisson(double lambda) {
  int k=0;
  double p=1;
  while (p > exp(-lambda)) {
    double u = (float)(rand()) / (float)(RAND_MAX);
    p = p*u;
    k++;
  }
  return k-1;
}

//Fait reproduire une proie du Monde M passé en paramètre
//en suivant une Loi de Poisson
void reproduction_proie(Monde *M){
  int nvProies = simu_poisson(M->taux_rep_proie);
  for(int i=0; i<nvProies; i++){
    if(M->nb_individus < M->largeur*M->hauteur)
      ajouter_Proie(M);
  }
}

//Fait reproduire un prédateur du Monde M passé en paramètre
//en suivant une Loi de Poisson
void reproduction_pred(Monde *M){
  int nvPreds = simu_poisson(M->taux_rep_pred);
  for(int i=0; i<nvPreds; i++){
    if(M->nb_individus < M->largeur*M->hauteur)
      ajouter_Predateur(M);
  }
}

// Règle les taux de reproduction des prédateurs et des proies en fonction de l'énergie moyenne
//des prédateurs du Monde M passé en paramètre afin de maintenir un équilibre dans la simulation.
// Puis, fait reproduire l'ensemble des individus de la population du Monde M passée en paramètre
void reproduction_population(Monde *M){
  if(M->energieMoy >= 40){
    M->taux_rep_pred = 0.15;
    M->taux_rep_proie = 0.1;
  }
  else{
    M->taux_rep_pred = 0.1;
    M->taux_rep_proie = 0.3;
  }
  for(int i=0;i<M->nb_individus;i++){
    if(M->Individus[i].TypeIndiv == 1)
      reproduction_proie(M);
    else
      reproduction_pred(M);
  }
}

// Cherche toutes les positions possibles de déplacement pour l'individu I du Monde M passés en paramètres en fonction
// du type type de l'emplacement cherché et de la zone de recherche voisinage passés en paramètres en les ajoutant dans
// le tableau cases_possibles et en choisi aléatoirement une
// i représente la ligne de recherche des cases possibles qui sera incrémentée et k le nombre d'éléments de cases_possibles
Case position_possible(Monde M, Individu I, int type, int voisinage){
  int i=0; 
  int k=0;
  Case cases_possibles[voisinage];
  for(int j=0; j<9; j++){ // Représente la colonne de recherche des cases possibles qui sera incrémentée
    if(j%3 == 0 and j != 0){ 
      i++; // Incrémente la ligne i si la colonne est arrivée au bout de la ligne
    }
    // calcule les véritables colonne et lignes nommées x et y du Monde M en fonction de la colonne et de la ligne de recherche
    int x = j%3-1+I.coord.col; 
    int y = i-1+I.coord.lig; 
    if(voisinage == 8 or j%2 == 1){ // Si voisinage = 4 alors, on parcourt toutes les 2 cases
      if(y < M.hauteur and y >= 0 and x < M.largeur and x >= 0){
        // Si le type recherché n'est pas une case vide et la case correspond au type recherché
        if(type != -1 and M.Indices[y][x] != -1 and M.Individus[M.Indices[y][x]].TypeIndiv == type){ 
            Case c;
            c.lig = y;
            c.col = x;
            cases_possibles[k] = c; 
            k++;
        }
        // Si le type recherché est une case vide et que la case est aussi une case vide
        else if(type == -1 and M.Indices[y][x] == -1){
          Case c;
          c.lig = y;
          c.col = x;
          cases_possibles[k] = c; 
          k++;
        }
      }
    }
  }
  if(k==0){ // Si pas de case trouvée
    Case c;
    c.lig = -1;
    c.col = -1;
    return c;
  }
  int ind = rand()%k;
  return cases_possibles[ind];
}

// Déplace aléatoirement l'Individu I du Monde M passés en paramètres dans un voisinage de 4
// et diminue l'énergie de l'individu si celui ci est un prédateur et qu'il ne trouve pas de proie
void deplacer_aleatoire(Monde *M, Individu *I){
  Case c = position_possible(*M,*I,-1,4);
  if(c.lig != -1 and c.col != -1){
    M->Indices[c.lig][c.col] = M->Indices[I->coord.lig][I->coord.col];
    M->Indices[I->coord.lig][I->coord.col] = -1;
    I->coord.lig = c.lig;
    I->coord.col = c.col;
  }
  if(I->TypeIndiv == 2 and c.lig != -1 and c.col != -1){ // Si c'est un prédateur qui ne trouve pas de proies
    I->energie -= 8;
  }
  else if(I->TypeIndiv == 2 and c.lig == -1 and c.col == -1){ // Si c'est un prédateur qui ne se déplace pas
    I->energie -= 5;
  }
  if(I->TypeIndiv == 2 and I->energie <= 0){
    I->mort = true;
    I->raison_mort = "Energie";
  }
}

// Déplace aléatoirement l'Individu I dans le Monde M passés en paramètres dans un voisinage de 8 
// à condition qu'une proie s'y trouve, faisant mourir celle-ci et renvoie true sinon renvoie false
bool predation(Monde *M, Individu *I){
  Case c = position_possible(*M,*I,1,8);
  if(c.lig != -1 and c.col != -1){ // Si il se déplace
    if(M->Indices[c.lig][c.col] != -1 and M->Individus[M->Indices[c.lig][c.col]].TypeIndiv == 1){ // Si sur une proie
      M->Individus[M->Indices[c.lig][c.col]].mort = true;
      M->Individus[M->Indices[c.lig][c.col]].raison_mort = "Predation";
      I->energie += 10;
      if(I->energie > 50)
        I->energie = 50;

      M->Indices[c.lig][c.col] = M->Indices[I->coord.lig][I->coord.col];
      M->Indices[I->coord.lig][I->coord.col] = -1;
      I->coord.lig = c.lig;
      I->coord.col = c.col;
      return true;
    }
  }
  // Si il ne trouve pas de proie
  return false;
}
// Fait déplacer aléatoirement la proie I dans le Monde M passés en paramètres
void action_proie(Monde *M, Individu *I){
  deplacer_aleatoire(M,I);
}

// Déplace si possible le prédateur I passé en paramètres sur une proie dans le Monde M passé en paramètres
// Sinon, déplace le prédateur I aléatoirement
void action_predateur(Monde *M, Individu *I){
  bool p = predation(M,I);
  if(!p){
    deplacer_aleatoire(M,I);
  }
}

// Déplace tous les individus du Monde M passé en paramètre en fonction de leur type TypeIndiv
void action_population(Monde *M){
  for(int i=0; i<M->nb_individus; i++){
    if(not M->Individus[i].mort){
      if(M->Individus[i].TypeIndiv == 1)
        action_proie(M,&M->Individus[i]);
      else
        action_predateur(M,&M->Individus[i]);
    }
  }
}

// Renvoie true si une des deux espèce a disparu
bool test_fin(Monde *M){
  actualiser_Monde(M);
  return(M->nbproie == M->nb_individus or M->nbpred == M->nb_individus);
}

// Simule une génération en exécutant chaque étape de la vie des Individus
void generation(Monde *M){
  reproduction_population(M);
  action_population(M);
  vieillir_population(M);
  disparition_population(M);
  std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Ajoute un délai de 0.2 seconde entre chaque génération
}

// Exécute gen fois la fonction generation en fonction du Monde M passé en paramètres
// à condition que les deux espèces sont encore présentes
void evolution(Monde *M, int gen){
  for(int i=0; i<gen;i++){
    if(not test_fin(M)){
      generation(M);
    }
  }
}
// Exécute gen fois la fonction generation en fonction du Monde M passé en paramètres
// et apelle afficher_Monde en fonction du Monde M à chaque génération à condition
// que les deux espèces sont encore présentes
void evolution_print(Monde *M, int gen){
  afficher_Monde(*M);
  for(int i=0; i<gen;i++){
    if(not test_fin(M)){
      generation(M);
      afficher_Monde(*M);
    }
  }
}

// Exécute gen fois la fonction generation en fonction du Monde M passé en paramètres et exporte le
// Monde M en grâce à la fonction exporter_Monde à chaque génération à condition que les deux espèces sont encore présentes
void evolution_output(Monde *M, int gen){
  exporter_monde(*M,"out/out0.svg");
  for(int i=0; i<gen;i++){
    if(not test_fin(M)){
      generation(M);
      char filename[100];
      sprintf(filename,"out/out%d.svg",i+1);
      exporter_monde(*M,filename);
      exporter_monde(*M,"out.svg");
    }
  }
}



#endif