#ifndef SITE_H
#define SITE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct site{
char* nom;
double LAT;
double LONG;
char* categorie;//cultural,natural,mixed
char* pays;
char* continent;
int enDanger;//0,1
} Site;

typedef struct distance{
int indice;
double dist;
double dist_depart;
struct distance * suivant;
struct distance * precedant;
} Distance;

typedef struct liste{
Distance * premier;
Distance * dernier;
} ListeDist;

typedef struct tabchar{
	char * pays;
	struct tabchar * suivant;
} Tabchar;

typedef struct debuttabchar{
	Tabchar * premier;
} Debuttabchar;


Site construireSite(char * nom, double LAT, double LONG, char* categorie, char* pays, char* continent, int enDanger);

void affichageSite(Site site);

void init_maille( double dist, double dist_depart);

void free_liste(ListeDist * site_visite);

void free_tabchar(Debuttabchar * debut);

#endif
