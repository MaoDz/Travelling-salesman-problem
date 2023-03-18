#ifndef ALGO_H
#define ALGO_H

#define TAILLE 1053
#define TEMPS_TOT 504
#define H_SUR_PLACE 7
#define VITESSE 85
#define NB_CULTURAL 814
#define NB_MIXED 35
#define POINTS 10
#define PAYS 20
#define DANGER 30


void echange_maille(Distance * prem, Distance * deux);

int pays_deja_visite(int indice, Debuttabchar * deb_pays,Site tab[TAILLE]);

//retourne 1 si i est dedans
int site_deja_visite(int indice,ListeDist * site_visite);

//retourne l'indice de la destination
int trouver_destination(Site tab[TAILLE],double LAT_actuel,double LONG_actuel, ListeDist * site_visite, int * alternance, Debuttabchar * deb_pays,int * premier_alternance, int meme_pays);

void affichage_final(ListeDist * site_visite,Site tab[TAILLE]);

void deroule(double LAT_dep, double LONG_dep,Site tab[TAILLE]);

void score(ListeDist * site_visite,Site tab[TAILLE]);

void remise_ordre(ListeDist * site_visite);

double tpsEcoule(ListeDist * site_visite);

int opt(ListeDist * site_visite, Site tab[TAILLE]);

double perimetre(ListeDist * site_visite, Site tab[TAILLE]);

void recalcul(ListeDist * site_visite,Site tab[TAILLE]);
#endif
