#include "tableauSite.h"
#include "algo.h"
#include "affichage.h"

int main(int argc, char **argv){
  // Securite en cas d'argument absent
  if (argc<3)
    {
      printf("./site <latitude start> <longitude start>\n");
      exit(1);
    }

  // ouverture du fichier
  FILE * fid = fopen("sites.csv","r");
  if(fid == NULL){
    printf("Erreur de lecture du fichier");
    exit(-1);
  }
  SkipLine(fid);

  // creation tableau
  Site tabSites[N_SITES];
  remplirTab(tabSites, fid);
	tabSites[N_SITES-1].LAT=atof(argv[1]);
	tabSites[N_SITES-1].LONG=atof(argv[2]);
  // fermeture fichier
  fclose(fid);

  // algo de voyage
	deroule(atof(argv[1]),atof(argv[2]),tabSites);
	affichageCarte();

  // liberation de la memoire
  freeTab(tabSites);

  return 0;
}
