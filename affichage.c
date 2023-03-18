#include "affichage.h"

// creation de "Tour.txt"
void creaTxt(Site tab[TAILLE], ListeDist * site_visite){
  FILE * tour;
  tour = fopen("Tour.txt", "w+");
  fprintf(tour, "%lf, %lf\n", tab[TAILLE-1].LAT, tab[TAILLE-1].LONG);

  Distance * courant = site_visite->premier->suivant;
  while(courant != NULL){
    int ind = courant->indice; // ind prend la valeur de l'indice dans le tableau
    fprintf(tour, "%lf, %lf, %s\n", tab[ind].LAT, tab[ind].LONG, tab[ind].categorie);
    courant = courant->suivant;
  }

  fprintf(tour, "%lf, %lf\n", tab[TAILLE-1].LAT, tab[TAILLE-1].LONG);

  fclose(tour);
}

void affichageCarte(){
  const char* commandLine="java -jar UnescoMap.jar";
  system(commandLine);
}


