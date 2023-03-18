#include "tableauSite.h"

void remplirTab(Site tab[N_SITES], FILE * fid){
  double Lat, Long;
  int danger;
  for(int i = 0; i < N_SITES-1; i++){
    char nom[TMAX], cat[TMAX], pays[TMAX], cont[TMAX];
    GetChaine(fid, TMAX, nom);
    //Skip(fid);
    fscanf(fid,"%lf", &Lat);
    Skip(fid);
    fscanf(fid,"%lf",&Long);
    Skip(fid);
    GetChaine(fid, TMAX, cat);
    //Skip(fid);
    GetChaine(fid, TMAX, pays);
    //Skip(fid);
    GetChaine(fid, TMAX, cont);
    //Skip(fid);
    fscanf(fid,"%d", &danger);
    Skip(fid);

    tab[i] = construireSite(nom, Lat, Long, cat, pays, cont, danger);
  }
}


void freeTab(Site tab[N_SITES]){
  for(int i = 0; i<N_SITES-1; i++){
    free(tab[i].nom);
    free(tab[i].categorie);
    free(tab[i].pays);
    free(tab[i].continent);
  }
}
