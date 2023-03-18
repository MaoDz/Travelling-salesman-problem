#ifndef TABLEAUSITE_H
#define TABLEAUSITE_H

#include <stdio.h>
#include <stdlib.h>
#include "site.h"
#include "lectureFichiers.h"

#define N_SITES 1053
#define TMAX 200

void remplirTab(Site tab[N_SITES], FILE * fid);
void freeTab(Site tab[N_SITES]);

#endif
