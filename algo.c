#include "site.h"
#include "haversine.h"
#include "algo.h"
#include "affichage.h"

//permet d'échanger deux mailles
void echange_maille(Distance * prem, Distance * deux){
  int indice_temp = prem->indice;
  double dist_depart_temp = prem->dist_depart;

  prem->indice = deux->indice;
  prem->dist_depart = deux->dist_depart;

  deux->indice = indice_temp;
  deux->dist_depart = dist_depart_temp;
}

//renvoie 1 si l'indice renvoie vers un site dans un pays déjà visité
int pays_deja_visite(int indice, Debuttabchar * deb_pays,Site tab[TAILLE]){

//initialisation de la liste de pays visité
	Tabchar * pays_visite=deb_pays->premier;
	
	//nom du pays où l'on souhaite se rendre'
	char * c=tab[indice].pays;
	//booléen verifiant si on a deja été dans un pays
	int dejavu=0;
	
	while(pays_visite!=NULL && dejavu==0){

		if (strcmp(c,pays_visite->pays)==0){
			dejavu=1;
			}
		pays_visite=pays_visite->suivant;

	}

	return dejavu;
}


//retourne 1 si on a déja visité le site
int site_deja_visite(int indice, ListeDist * site_visite){
	Distance * site=site_visite->premier;
	//condition si on n'a pas encore visité de site
	if(site==NULL)
		return 0;

	while(site!=NULL){
		if (site->indice==indice){
			return 1;
		}
		site=site->suivant;
	}
	return 0;
}

//retourne l'indice de la destination choisie par l'algorythme*******************************************************************

int trouver_destination(Site tab[TAILLE],double LAT_actuel,double LONG_actuel, ListeDist * site_visite, int * alternance, Debuttabchar * deb_pays,int * premier_alternance,int meme_pays){

	double min=10000;
	int indice=0;
	int i=1;
	int taille=TAILLE-1;
//***********alternance*********************************
//on choisi d'alterner à chaque fois entre un Natural et un cultural
//le tableau est classé d'abord par les Cultural puis les mixed et enfin les Natural
//on utilise donc cela pour faire commencer ou terminer la recherche avant

//si on avait un Cultural
	if(*alternance==1)
	//meme fin pour taille
		i=NB_CULTURAL;

//si on avait un Natural
	else if(*alternance==-1)
	//meme depart pour i
		taille=NB_CULTURAL+NB_MIXED;
	
//si on se retrouve à égalité entre Natural et cultural
	else if(*alternance==0){
		
			if (*premier_alternance==1)	
				taille=NB_CULTURAL+NB_MIXED;	
				
			else if (*premier_alternance==-1)
				i=NB_CULTURAL;
		
				
	}
	//DEBUG printf("alternance %d\n",*alternance);
//******************algo de recherche de la destination***************
//On choisit la destionation finale selon ces règles:
//on change de pays à chaque site visité sauf si celui si est en danger
//on prend le site qui est le plus proche du point de départ parmis les 4 sites les plus proches du point précédant

	//***** permet de choir parmis les 4 plus proches
	//on stocke les indices des echantillons a partir de la case 4
	int nb_echant=4;
	int echantillon[nb_echant*2];
	int change=0;
	for (int e=0;e<nb_echant*2;e++)
		echantillon[e]=100000;
	
	//on cherche les 4 sites valables******
	for(int j=i;j<taille;j++){
		if ((pays_deja_visite(j, deb_pays,tab)==0 || meme_pays==1) || tab[j].enDanger==1 ){
			double dist=calculDistance(tab[j].LAT,tab[j].LONG , LAT_actuel, LONG_actuel);
			for (int e=0;e<nb_echant;e++){
				min=echantillon[e];
				if(min > dist && change==0){
					int dedans=site_deja_visite(j,site_visite);
					if (dedans==0){
						echantillon[e]=dist;
						echantillon[e+nb_echant]=j;
	//change permet de ne pas avoir les 4 meme sites dans le tableau
						change=1;
					}
				}
			}
			change=0;
		}
	}
	
	//on choisi parmis les 4 points le plus proche du point de départ**********************
	min=calculDistance(tab[echantillon[nb_echant]].LAT,tab[echantillon[nb_echant]].LONG , tab[TAILLE-1].LAT,tab[TAILLE-1].LONG);
	indice=echantillon[nb_echant];
	for (int e=nb_echant+1;e<nb_echant*2;e++){
		double dist=calculDistance(tab[echantillon[e]].LAT,tab[echantillon[e]].LONG , tab[TAILLE-1].LAT,tab[TAILLE-1].LONG);
		if(min > dist){
			min=dist;
			indice=echantillon[e];
		}
	}
	
	//on ajoute le pays visité à la liste des pays visité
	Tabchar * pays_visite=deb_pays->premier;
	Tabchar * pays_vis = malloc(sizeof(Tabchar));
	pays_vis->pays=strdup(tab[indice].pays);
	
	if (pays_visite==NULL){
		pays_vis->suivant=NULL;
		deb_pays->premier= pays_vis;
	}
	
	else {
		pays_vis->suivant=pays_visite->suivant;
		pays_visite->suivant=pays_vis;
	}
	
//****************************************************
	//on change l'alternance en fonction du site visité
	if (indice<NB_CULTURAL){
		*alternance+=1;
		if (*premier_alternance==0) *premier_alternance+=1;
	}
	
	if (indice>=NB_CULTURAL+NB_MIXED){
		*alternance-=1;
		if (*premier_alternance==0) *premier_alternance-=1;
	}
	//DEBUG affichageSite(tab[indice]);
	return indice;
}
//***************************************************************************************



//fonction qui affiche tous les site visité à la fin
void affichage_final(ListeDist * site_visite,Site tab[TAILLE]){
	int compt=1;
	Distance * dist=site_visite->premier;
	printf("0) LAT : %f , LONG : %f\n",tab[TAILLE-1].LAT,tab[TAILLE-1].LONG);
	while(dist!=NULL){
		if(dist->indice!=TAILLE-1){
			printf("%d) ",compt);
			affichageSite(tab[dist->indice]);
			compt++;
		}
		dist=dist->suivant;
	}
}



//déroulé de la recherche*************************************************************************
void deroule(double LAT_dep, double LONG_dep,Site tab[TAILLE]){
//initialisation des variables
	double temps_ecoule=0,temps_retour=0;
	int indice=1;
	int meme_pays=0;
	int alternance=0;
	int premier_alternance=0;
	int comptnatural=0,comptcultural=0,comptmixed=0;
	ListeDist * site_visite=malloc(sizeof(ListeDist));
	site_visite->premier=malloc(sizeof(Distance));
	site_visite->dernier=NULL;
	site_visite->premier->indice=TAILLE-1;
	site_visite->premier->dist=0;
	site_visite->premier->dist_depart=0;
	site_visite->premier->suivant=NULL;
	double LAT_actuel=LAT_dep,LONG_actuel=LONG_dep;
    //int opti = 0;
    int indice_dernier=-1;
	int compt_tour=0;
	Debuttabchar * deb_pays=malloc(sizeof(Debuttabchar));
	deb_pays->premier= NULL;
// recheche de l'itinéraire
    do{
    //tant qu'il nous reste du temps
      while(TEMPS_TOT-temps_ecoule>temps_retour){
      	
		//on cherche l'indice dans le tableau de la prochaine destination
		indice= trouver_destination ( tab, LAT_actuel, LONG_actuel, site_visite, &alternance, deb_pays, &premier_alternance, meme_pays);

		//on récupère le site trouvé
		Site site = tab[indice];

		//on initialise un maillon de la liste des sites visités
		Distance * dist=malloc(sizeof(Distance));
		Distance * parcours_liste=site_visite->premier->suivant;


		dist->indice=indice;
		dist->dist = calculDistance (tab[indice].LAT,tab[indice].LONG ,LAT_actuel, LONG_actuel);
		dist->dist_depart = calculDistance (tab[indice].LAT,tab[indice].LONG ,LAT_dep, LONG_dep);
		dist->suivant=NULL;


		//calcul temps
		temps_ecoule+=H_SUR_PLACE+dist->dist/VITESSE;
		temps_retour=dist->dist_depart/VITESSE;

		//si il nous reste du temps
		if (TEMPS_TOT-temps_ecoule>temps_retour){
		//on ajoute le site trouvé
			LAT_actuel=tab[indice].LAT;
			LONG_actuel=tab[indice].LONG;
          	site_visite->dernier=dist;
          	//la premiere fois
          	if (parcours_liste==NULL){
           		dist->precedant=NULL;
            	site_visite->premier->suivant=dist;
          	}
          	//le reste du temps
	      	else {
	        	while(parcours_liste->suivant!=NULL)
	          		parcours_liste=parcours_liste->suivant;
	        	dist->precedant=parcours_liste;
	        	parcours_liste->suivant=dist;
	      	}


          	//compte le nombre de sites de chaque sorte
          	char c=*site.categorie;
          	if (c=='N'){
            	comptnatural++;
          	}
          	else if (c=='C'){
            	comptcultural++;
          	}
          	else comptmixed++;
		}
		else {
			free(dist);	
		}
      }
      //DEBUG printf("temps ecoule av %lf\n", temps_ecoule);

//***************** OPTIMISATION  ************
indice_dernier=site_visite->dernier->indice;
		if(TEMPS_TOT-temps_ecoule<temps_retour){
		   	char c=*tab[site_visite->dernier->indice].categorie;
		    if (c=='N'){
		      comptnatural--;
		    }
		    else if (c=='C'){
		      comptcultural--;
		    }
		    else comptmixed--;
		    //permet de fixer alternance en cas de surboucle
		    alternance = comptcultural-comptnatural;

		  	site_visite->dernier=site_visite->dernier->precedant;
		  	free(site_visite->dernier->suivant);
		  	site_visite->dernier->suivant=NULL;
      	}
      	
      	//on recalcule les distances
		recalcul(site_visite,tab);
		//on optimise le parcours
		/*opti =*/opt(site_visite, tab);
		//on recalcule les distances
		recalcul(site_visite,tab);
		
       
      temps_retour=site_visite->dernier->dist_depart/VITESSE;
     
	  temps_ecoule = tpsEcoule(site_visite);
	  
	  
	  //DEBUG printf("temps ecoule ap %lf \n", temps_ecoule);
	  //DEBUG printf("%lf %lf\n",TEMPS_TOT-temps_ecoule,temps_retour);
	  
	  //permet de visiter des pays deja visité s'il nous reste du temps
      	if (compt_tour>=10){
      		meme_pays=1;
      	}
		compt_tour++;
    }while( compt_tour<15 /*&& (opti != 0 && TEMPS_TOT-temps_ecoule>temps_retour)*/ );
    
    if (TEMPS_TOT-temps_ecoule>temps_retour){
    	
    	
    	char c=*tab[indice_dernier].categorie;
		if (c=='N'){
	      comptnatural--;
	    }
	    else if (c=='C'){
	      comptcultural--;
	    }
	    else comptmixed--;
	    site_visite->dernier=site_visite->dernier->precedant;
		free(site_visite->dernier->suivant);
    	site_visite->dernier->suivant=NULL;
    }
	//remet les pays dans l'ordre de visite
    if (site_visite->premier->indice!=TAILLE-1)
    	remise_ordre( site_visite);

    //finalisation
	affichage_final(site_visite,tab);
	printf("N %d C %d M %d\n",comptnatural,comptcultural,comptmixed);
	score(site_visite,tab);
	creaTxt(tab,site_visite);
	free_liste(site_visite);
	free_tabchar(deb_pays);
}
//*******************************************************************************


//calcul du score*************************************************************
void score(ListeDist * site_visite,Site tab[TAILLE]){
	int compt_danger=0,compt_total=0,compt_pays=0;
	double temps=0;
	Distance * site=site_visite->premier->suivant;

//initialisation de la liste de pays visité
	Tabchar * pays_visite=malloc(sizeof(Tabchar));
	Debuttabchar * deb_pays=malloc(sizeof(Debuttabchar));
	deb_pays->premier= NULL;
	while(site!=NULL){

//************compt de temps
		temps+=site->dist/VITESSE+H_SUR_PLACE;
		if (site->suivant==NULL)
			temps+=site->dist_depart/VITESSE;

//********compteur de sites visité
		compt_total++;
//************compteur de site en danger
		if (tab[site->indice].enDanger==1)
			compt_danger++;

//**********compteur de pays visité
		pays_visite=deb_pays->premier;
		//nom du pays en cours
		char * c=tab[site->indice].pays;
		//booléen verifiant si on a deja été dans un pays
		int dejavu=0;

		pays_visite=deb_pays->premier;
		while(pays_visite!=NULL && dejavu==0){

			if (strcmp(c,pays_visite->pays)==0){
				dejavu=1;
				}
			pays_visite=pays_visite->suivant;
		}
		pays_visite=deb_pays->premier;
		if (dejavu==0){
			compt_pays++;

			Tabchar * pays_vis = malloc(sizeof(Tabchar));
			pays_vis->pays=strdup(tab[site->indice].pays);

			if (pays_visite==NULL){
				pays_vis->suivant=NULL;
				deb_pays->premier= pays_vis;
			}
			else {
				pays_vis->suivant=pays_visite->suivant;
				pays_visite->suivant=pays_vis;
			}
			
			
		}

		site=site->suivant;

	}
	
	free_tabchar(deb_pays);
	//print très long mais nous trouvions ça amusant
	printf("temps total: %lf\névaluation :\n\t%d sites visités * 10pts = %d\n\t%d pays visités *20 pts = %d\n\t%d sites en danger * 30pts = %d\nScore total = %d\n",temps,compt_total,compt_total*POINTS,compt_pays,compt_pays*PAYS,compt_danger, compt_danger*DANGER,compt_total*POINTS+compt_pays*PAYS+compt_danger*DANGER);


}

//*******************************************************************************

void remise_ordre(ListeDist * site_visite){
//on cherche le point de départ et on le remet au début
	Distance * site=site_visite->premier;
	while(site->indice!=TAILLE-1){
		site=site->suivant;
	}
	Distance * temp_prems=site_visite->premier;
	site_visite->premier=site;
	site_visite->dernier=site->precedant;
	while(site->suivant!=NULL){
		site=site->suivant;
	}
	site->suivant=temp_prems;
	site_visite->premier->precedant=NULL;
	site_visite->dernier->suivant=NULL;
}

//calcule le temps écoulé
double tpsEcoule(ListeDist * site_visite){
  Distance * site = site_visite->premier;
  double temps = 0;

  while(site != NULL){
    temps+=site->dist/VITESSE+H_SUR_PLACE;
    site = site->suivant;
  }

  return temps;
}


//fonction d'optimisation du trajet 2opt*********************************************************
//renvoie 1 si il y a eu un changement dans le trajet (non utilisé par manque de temps)
//principe : on echange de place deux sites et on regarde si la taille du trajet total a diminué
//si c'est le cas on garde le changement sinon on l'annule
int opt(ListeDist * site_visite, Site tab[TAILLE]){
  int opti=0;
  Distance * site_a;
  Distance * site_b;
  double renversement,non_renverse;
  
  for (int i=0;i<50;i++){
  	int compta=0;
  	site_a=site_visite->premier;
	while (site_a!=NULL){
		site_b=site_a->suivant;
		non_renverse=perimetre(site_visite,tab);
		//condition spéciale pour l'inversion entre le dernier et le premier
		if (site_b==NULL){
		  
		  	site_b=site_visite->premier;
		  		
	  		Distance * tempa=site_a;
			Distance * tempb=site_b;
			//échange la chaine entre deux sites donc inverse l'ordre de la chaine
	  		for (int e=0;e<=((compta)/2);e++){
	  			
	  			int temp = tempa->indice;
	  			tempa->indice=tempb->indice;
	  			tempb->indice=temp;
	  			
	  			tempb=tempb->suivant;
	  			tempa=tempa->precedant;
	  			
	  		}
		  	//calcul du nouveau périmetre
	  		renversement=perimetre(site_visite,tab);
	  		
	  		//annulation s'il est plus grand
	  		if (renversement>non_renverse){
		  		Distance * tempa=site_a;
  				Distance * tempb=site_b;
		  		for (int e=0;e<=(compta)/2;e++){
		  			int temp = tempa->indice;
		  			tempa->indice=tempb->indice;
		  			tempb->indice=temp;
		  			
		  			tempb=tempb->suivant;
	  				tempa=tempa->precedant;
		  		}
		  		recalcul(site_visite,tab);
	  		}
	  		//permet de vérifier si au moins 1 changement a été fait
	  		else opti=1;
	  		site_b=NULL;
		  }
		  
		  //meme fonctionement
		  	while (site_b!=NULL){
		  		
		  		Distance * compteur=site_a;
		  		int compt=0;
		  		while(compteur->indice!=site_b->indice){
		  			compt++;
		  			compteur=compteur->suivant;
		  		}
		  		Distance * tempa=site_a;
  				Distance * tempb=site_b;
		  		for (int e=0;e<=compt/2;e++){
		  			int temp = tempa->indice;
		  			tempa->indice=tempb->indice;
		  			tempb->indice=temp;
		  			
		  			tempa=tempa->suivant;
		  			tempb=tempb->precedant;
		  		}
		  		renversement=perimetre(site_visite,tab);
		  		if (renversement>non_renverse){
			  		Distance * tempa=site_a;
	  				Distance * tempb=site_b;
			  		for (int e=0;e<=compt/2;e++){
			  			int temp = tempa->indice;
			  			tempa->indice=tempb->indice;
			  			tempb->indice=temp;
			  			
			  			tempa=tempa->suivant;
			  			tempb=tempb->precedant;
			  		}
			  		recalcul(site_visite,tab);
		  		}
		  		else opti=1;
		  		site_b=site_b->suivant;
		  	}
	  		site_a=site_a->suivant;
	  		
	   		compta++;
	   }
	 
	 }
  return opti;
} 
//*********************************************************************************


//calcule le périmètre du trajet
double perimetre(ListeDist * site_visite, Site tab[TAILLE]){
  recalcul(site_visite,tab);
  Distance * site=site_visite->premier;
  double longueur=0;
  
  while(site->suivant!=NULL){
  	longueur+=site->dist;
  	site=site->suivant;
  }
  return longueur+site->dist+site->dist_depart;
}

//recalcule les ditance avaec les points précédants
void recalcul(ListeDist * site_visite,Site tab[TAILLE]){
	Distance * site=site_visite->premier->suivant;
	Distance * precedant=site_visite->premier;
	precedant->dist=0;
	while(site!=NULL){
		site->dist=calculDistance(tab[site->indice].LAT,tab[site->indice].LONG , tab[precedant->indice].LAT , tab[precedant->indice].LONG );
		precedant=precedant->suivant;
		site_visite->dernier=site;
		site=site->suivant;
	}
}

