#include "site.h"

Site construireSite(char * nom,double LAT,double LONG, char* categorie, char* pays, char* continent, int enDanger){
	Site site;
	site.nom=strdup(nom);
	site.LAT=LAT;
	site.LONG=LONG;
	site.categorie=strdup(categorie);
	site.pays=strdup(pays);
	site.continent=strdup(continent);
	site.enDanger=enDanger;
	return site;
}

void affichageSite(Site site){
	printf("%s ",site.nom);
	printf("%.2lf %.2lf ",site.LAT,site.LONG);
	printf("%s ",site.categorie);
	printf("%s ",site.pays);
	printf("%s ",site.continent);
	printf("%d\n",site.enDanger);
}

void init_maille( double dist, double dist_depart){
	Distance * maille = malloc(sizeof(Distance));
	maille->dist=dist;
	maille->dist_depart=dist_depart;
	maille->suivant=NULL;
}


void free_liste(ListeDist * site_visite){
	Distance * site= site_visite->premier;
	while(site!=NULL){
		site_visite->premier=site->suivant;
		free(site);
		site=site_visite->premier;
	}
	free(site_visite);
}

void free_tabchar(Debuttabchar * debut){
	Tabchar * tab= debut->premier;
	while(tab!=NULL){
		debut->premier=tab->suivant;
		free(tab->pays);
		free(tab);
		tab=debut->premier;
	}
	free(debut);
}


