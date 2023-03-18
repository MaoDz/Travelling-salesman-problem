#include <ctype.h>
#include <stdio.h>
#include "lectureFichiers.h"


/*LIT LE PROCHAIN CARACTERE NON VIDE */
char ReadChar(FILE *f){
  char c ;
  do
    c = getc(f) ;
  while (isspace(c)) ;
  return c ;
}

/* LECTURE DES CARACTERES JUSQU'A LA FIN DU FICHIER OU DE LA LIGNE */
void SkipLine(FILE *f){
  char c ;
  do
    c = getc(f) ;
  while ((c!=EOF)&&(c!='\n')) ;
}

/*TESTE SI UN CARACTERE EST UN ESPACE OU UNE VIRGULE*/

int isCommaOrSpace(char c){
	if ((c==',') || (isspace(c))) return 1;
	return 0;
}

/*SAUT DES ESPACES ET VIRGULE LORS DE LA LECTURE DANS UN FICHIER POINTE PAR f */

void Skip(FILE *f) {
  char c ;
  while (isCommaOrSpace(c=getc(f))) ;
  ungetc(c,f) ;
}

/* LECTURE DE CHAINE DE CARACTERES JUSQU'A UNE VIRGULE LA FIN DE LA LIGNE OU LA FIN DE FICHIER */
int GetChaine(FILE *f,int taille_max,char *s){
  char c ;
  int i;

  
  i=0;
  c = getc(f) ;
  while ((c!=',')&&(c!=EOF)&&(c!='\n')&&(i<taille_max-1)){
    s[i]=c;
	//printf("%c\n",c);
    c = getc(f) ;    
    i++;
  }
  s[i]='\0';
  ungetc(c,f) ;
  Skip(f) ;
  return (i); //longueur de la chaine lue
 }


