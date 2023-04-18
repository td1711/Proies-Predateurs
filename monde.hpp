#ifndef MONDE_HPP_
#define MONDE_HPP_
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

struct Case{
  int lig;
  int col;
};

struct Individu{
  int age;
  int energie; // -1 pour les proies
  int TypeIndiv; // 1 pour proies et 2 pour prédateurs
  bool mort;
  Case coord; // Coordonnées
  string raison_mort;
};


struct Monde{
  float taux_rep_proie;
  float taux_rep_pred;
  long hauteur;
  long largeur;
  Individu *Individus; // Tableau des individus
  int **Indices; // Grille contenant les indices de chaque individu dans Individus
  int nb_individus;
  int nbproie;
  int nbpred;
  float energieMoy; // Energie moyenne des prédateurs
  int gen;
};

// Crée un Monde de dimensions hauteurxlargeur passés en paramètres 
Monde creer_Monde(long hauteur, long largeur){
  Monde M;
  M.hauteur = hauteur;
  M.largeur = largeur;
  M.Indices = new int *[M.hauteur];
  M.Individus = new Individu[M.hauteur*M.largeur];
  for(int i=0; i<M.hauteur; i++){ 
    M.Indices[i] = new int[M.largeur];
    for(int j=0; j<M.largeur; j++){
      M.Indices[i][j] = -1; // Initialise chaque emplacement de Indices à -1
    }
  }
  M.nb_individus = 0;
  M.gen = 0;
  return M;
}

// Détruit le Monde M passé en paramètre
void detruire_Monde(Monde *M){
  for(int i=0; i<M->hauteur; i++){
    delete[] M->Indices[i];
  }
  delete[] M->Individus;
  M->nb_individus = 0;
}

// Renvoie le type d'un individu en fonction de la valeur de a passée en paramètre
// a peut prendre les valeurs 1 et 2
string typeIndividu(int a){
  if(a == 1)
    return "proie";
  return "predateur";
}

// Affiche les caractéristiques de tous les individus du Monde M passé en paramètre
// et affiche la grille des indices de M
void afficher_Monde(Monde M){
  for(int k=0; k<M.nb_individus; k++){
    cout << "\n" << k << " : " << endl;
    cout << "Age : " << M.Individus[k].age << endl;
    cout << "Energie : " << M.Individus[k].energie << endl;
    cout << "Mort ? : " << M.Individus[k].mort << endl;
    cout << "Type : " << typeIndividu(M.Individus[k].TypeIndiv) << endl;
    cout << "Coord : " << M.Individus[k].coord.lig << "," << M.Individus[k].coord.col << endl;
    cout << "Raison_mort : " << M.Individus[k].raison_mort <<endl;
  }
  
  cout << endl;
  for(int i=0; i<M.hauteur; i++){
    for(int j=0; j<M.largeur; j++){
      if(M.Indices[i][j] == -1)
        cout << ".";
      else if(M.Individus[M.Indices[i][j]].TypeIndiv == 1)
        cout << "O";
      else if(M.Individus[M.Indices[i][j]].TypeIndiv == 2)
        cout << "X";
      cout << " ";
    }
    cout << endl;
  }
}

// Cherche et renvoie une position aléatoire vide dans la grille des indices
// du Monde M passé en paramètre
Case position_aleatoire(Monde M){
  Case c;
    c.col = rand()%M.largeur;
    c.lig = rand()%M.hauteur;
    while(M.Indices[c.lig][c.col] != -1){
      c.col = rand()%M.largeur;
      c.lig = rand()%M.hauteur;
    }
  return c;
}

// Ajoute une proie dans le Monde M passé en paramètre et définie ses caractéristiques
void ajouter_Proie(Monde *M){
  Individu I;
  I.TypeIndiv = 1;
  I.mort = false;
  I.age = 0;
  I.energie = -1;
  I.coord = position_aleatoire(*M);
  M->Individus[M->nb_individus] = I;
  M->Indices[I.coord.lig][I.coord.col] = M->nb_individus;
  M->nb_individus++;
}

// Ajoute un prédateur dans le Monde M passé en paramètre et définie ses caractéristiques
void ajouter_Predateur(Monde *M){
  Individu I;
  I.TypeIndiv = 2;
  I.mort = false;
  I.age = 0;
  I.energie = 50;
  I.coord = position_aleatoire(*M);
  M->Individus[M->nb_individus] = I;
  M->Indices[I.coord.lig][I.coord.col] = M->nb_individus;
  M->nb_individus++;
}

// Creé l'entête du fichier svg en fonction des dimensions du Monde M passé en paramètres
void svg_entete(Monde M, fstream *f){
  *f << "<svg height=\""<<M.hauteur<<"\" width=\""<<M.largeur<<"\" xmlns=\""<<"http://www.w3.org/2000/svg"<<"\">"<<endl;
}

// Creé la fin du fichier svg 
void svg_fin(fstream *f){
  *f << "</svg>" << endl;
}

// Crée le corps du fichier svg en fonction du Monde M passé en paramètres
// Affiche chaque poisson selon son type
void svg_corps(Monde M, fstream *f){
  *f << "<rect width=\""<<M.largeur<<"\" height=\""<<M.hauteur<<"\" x=\""<<0<<"\" y=\""<<0<<"\" fill=\"#7F9ECF\"/>" << endl;
  for(int i=0; i<M.hauteur;i++){
    for(int j=0; j<M.largeur;j++){
      if(M.Indices[i][j]!=-1){
        if(M.Individus[M.Indices[i][j]].TypeIndiv == 1){ // Proies
          *f << "<ellipse cx=\""<<j+0.45<<"\" cy=\""<<i+0.5<<"\" rx=\"0.43\" ry=\"0.2\" fill=\"lightgrey\" />";
          *f << "<polygon points=\""<<j+0.75<<","<<i+0.5<<","<<j+1<<","<<i+0.2<<","<<j+1<<","<<i+0.8<<"\" fill=\"lightgrey\"/>";
          *f << "<circle  cx=\""<<j+0.28<<"\" cy=\""<<i+0.38<<"\" r=\"0.05\" fill=\"black\" />";
          *f << "<polygon points=\""<<j+0<<","<<i+0.4<<","<<j+0.25<<","<<i+0.5<<","<<j+0<<","<<i+0.6<<"\" fill=\"#7F9ECF\"/>";
          
        }  
        else if(M.Individus[M.Indices[i][j]].TypeIndiv == 2){ // Prédateurs
          *f << "<ellipse cx=\""<<j+0.45<<"\" cy=\""<<i+0.5<<"\" rx=\"0.43\" ry=\"0.3\" fill=\"darkred\" />";
          *f << "<polygon points=\""<<j+0.75<<","<<i+0.5<<","<<j+1<<","<<i<<","<<j+1<<","<<i+1<<"\" fill=\"darkred\"/>";
          *f << "<circle  cx=\""<<j+0.28<<"\" cy=\""<<i+0.35<<"\" r=\"0.05\" fill=\"black\" />";
          *f << "<polygon points=\""<<j+0<<","<<i+0.3<<","<<j+0.25<<","<<i+0.5<<","<<j+0<<","<<i+0.8<<"\" fill=\"#7F9ECF\"/>";
        }
      }
    }
  }
}

// Exporte le monde M pasé en paramètres dans un fichier svg nom
void exporter_monde(Monde M, const char *nom){
  fstream f(nom,ios::out);
  if(f){
    svg_entete(M,&f);
    svg_corps(M,&f);
    svg_fin(&f);
  }
  else{
    cout << "Problème" << endl;
  }
  f.close();
}


string a = "Vieillesse";
string b = "Energie";
// Fait disparaître l'individu situé à l'indice i passé en paramètres de la liste des invididus du Monde M
// et le fait disparaître de la grille des Indices du Monde M
void disparition_individu(Monde *M, int i){
  Case CaseMort = M->Individus[i].coord;
  Case CaseDern = M->Individus[M->nb_individus-1].coord;
  if(not M->Individus[M->nb_individus-1].mort)
      M->Indices[CaseDern.lig][CaseDern.col] = i;
  
  if(M->Individus[i].raison_mort == a or M->Individus[i].raison_mort == b){ // Si mort naturelle
    if(M->Individus[M->Indices[CaseMort.lig][CaseMort.col]].mort){
      M->Indices[CaseMort.lig][CaseMort.col] = -1;
    }
    M->Individus[i] = M->Individus[M->nb_individus-1];
  }
  else{ // Si mort par prédation
    M->Individus[i] = M->Individus[M->nb_individus-1];
    if(M->Individus[M->Indices[CaseMort.lig][CaseMort.col]].mort){
      M->Indices[CaseMort.lig][CaseMort.col] = -1;
    }
  }
  M->nb_individus--;
}


#endif