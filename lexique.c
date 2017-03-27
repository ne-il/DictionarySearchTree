/*	DESCRIPTION ETUDIANT	*/

/*
 *	auteurs/mails			: ANTEUR Neil, HUDELOT Matthias
 * 	groupe de TD			: 1
 *      groupe de TP			: 1
 *	date de création 		: 7/12/2014
 *	description 			: PROJET : Arbre lexicographique
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# define MAX 512

typedef struct noeud{
	struct noeud* freregauche;
	struct noeud* freredroit;
	struct noeud* fils;
	char lettre;
}Noeud, *Arbre;



Arbre AlloueNoeud(char mot);


int lexique(FILE *f, Arbre *a);
int insertion(Arbre *a, char *mot);

void affichage(Arbre a);
void affichageaux(Arbre a, char buffer[], int indice);

int recherche(Arbre a, char *mot);

void sauvegarde_mots(Arbre a,FILE* sauvegarde, char buffer[], int indice);
void sauvegarde_arbre(Arbre a, FILE* save);

void charge_arbre(Arbre* a, FILE* fichier);
void charge_arbre_aux(Arbre* a, FILE* fichier);




/*Alloue la memoire d'un noeud*/
Arbre AlloueNoeud(char mot){
	Arbre tmp;
	tmp = (Arbre) malloc(sizeof(Noeud));
	if(tmp != NULL){
		tmp->lettre = mot;
		
        tmp->fils = NULL;
        tmp->freregauche = NULL;
		tmp->freredroit = NULL;
	
	}
	return tmp;
}

/*Insert un mot dans un arbre lexicographique*/
int insertion(Arbre *a, char *mot){

	if(*a == NULL){
		*a = AlloueNoeud(*mot);

		if(*a == NULL)  /**Erreur d'alloc*/
			return 0;

		if(*mot == '\0') {       /*Insertion terminée*/
			return 1;
		}

		return insertion(&((*a)->fils), mot+1); /*On continue l'insertion de la suite du mot*/
	}

	if((*a)->lettre == *mot){

		if(*mot == '\0') /*Insertion terminée*/
			return 1;

		return insertion(&((*a)->fils), mot+1);

	}

	if( (*mot)< (*a)->lettre){
		return insertion(&((*a)->freregauche), mot);
	}

	else if( (*mot) > (*a)->lettre){
		return insertion(&((*a)->freredroit), mot);
	}

	return 0;
}

/*Construit un arbre lexical representant les mots du fichier controle par *f*/
int lexique(FILE *f, Arbre *a){

	int compteur = 0;
	char tmp[MAX]={0,};

	while(fscanf(f,"%s",tmp) != EOF){
		insertion(a, tmp);
		++compteur;
	}

	return compteur;

}

/*Recherche un mot dans un arbre lexicographique*/

int recherche(Arbre a, char *mot){
	if(a == NULL)
		return 0;

	else if(a->lettre == *mot){
		if(*mot == '\0')
			return 1;
		return recherche((a->fils), mot+1);
	}

	else if( *mot  < a->lettre)
		return recherche((a->freregauche), mot);


	else if( *mot  > a->lettre)
		return recherche((a->freredroit), mot);

	return 0;
}




/*Fonction auxiliaire d'affichage*/
void affichageaux(Arbre a, char buffer[], int indice){

	if(a != NULL){
		affichageaux(a->freregauche, buffer, indice);

		buffer[indice] = a->lettre;
		if(a->lettre == '\0')
			printf(" %s\n",buffer);

		affichageaux(a->fils, buffer, indice+1);
		affichageaux(a->freredroit, buffer, indice);
	}
}


/*Affiche le mot de l'arbre bianaire lexicographique*/
void affichage(Arbre a){
	char buffer [MAX]={0,};
	affichageaux(a, buffer, 0);
}



void sauvegarde_mots(Arbre a,FILE* sauvegarde, char buffer[], int indice){

	if(a != NULL){


		sauvegarde_mots(a->freregauche,sauvegarde, buffer, indice);


		buffer[indice] = a->lettre;


		if(a->lettre == '\0'){
			fprintf(sauvegarde, "%s \n" ,buffer);
		}

		sauvegarde_mots(a->fils,sauvegarde, buffer, indice+1);


		sauvegarde_mots(a->freredroit,sauvegarde, buffer, indice);
	}
}



void sauvegarde_arbre(Arbre a, FILE* save){

	if( a!= NULL){

		if(a->lettre== '\0')
			fprintf( save, "~");

		if(a->lettre != '\0')
			fprintf(save, "%c", a->lettre);



		if( a->lettre != '\0'){
			sauvegarde_arbre ( a->freregauche, save );
			sauvegarde_arbre ( a->fils, save);
		}

		sauvegarde_arbre ( a->freredroit, save );
	}

	else
		fprintf(save," ");

}

void charge_arbre_aux(Arbre* a, FILE* fichier){
	
	char c;
	
	if ( fscanf( fichier, "%c", &c ) != EOF ){
	
	
		if(c == '~')
			c='\0';

		if(c != ' '){
			

			(*a)= AlloueNoeud( c );

			if(c != '\0'){
				charge_arbre_aux( &((*a)->freregauche),  fichier);
				charge_arbre_aux( &((*a)->fils),  fichier);
			}

			charge_arbre_aux( &((*a)->freredroit),  fichier);

		}

	}
}

void charge_arbre(Arbre* a, FILE* fichier){


	charge_arbre_aux(a, fichier);

	fclose(fichier);

}


int choix_menu(char* nom_fichier){
	int choix=0;

	printf("----------------\n------MENU------\nFichier traité= %s\n\n action 1: afficher le lexique de %s \n action 2: sauvegarder les mot dans %s.LEX \n action 3: sauvegarder l'arbre du lexique dans %s.DIC \n action 4: rechercher un mot dans le lexique\n action 5: ouvrir un autre fichier\n action 6: QUITTER----------------\n ",nom_fichier, nom_fichier, nom_fichier, nom_fichier);

	do{

		printf("\n Entrez le numero de l'action à effectuer \n");
		scanf ("%d", &choix);

	}while(choix <= 0 || choix > 6);	

	return choix;
}

int main(int argc, char* argv[]){

	int compteur_de_mots;
	int choix=0;
	int fin = 0;
	int i;
	int r=0;

	char mot_recherche[MAX]={0,};

	char nom_fichier[MAX]={0,};
	char option[MAX]={0,};

	char save_lexique[MAX]={0,};
	char save_dico[MAX]={0,};
	char fichier_a_ouvrir[MAX]={0,};
	char buffer[MAX]={0,};
	FILE* fichier=NULL;
	FILE* sauvegarde=NULL;
	FILE* dico=NULL;

	Arbre arbre=NULL;

	


/*-----------------------------------------------------------------------------------------------*/
/*Bloc qui gere le nombre de parametre passé en ligne de commande et interprete les different parametres*/
	if(argc == 1){
		printf("ERREUR: PAS DE NOM_DE_FICHIER VOUS DEVEZ PRECISER LE NOM DU FICHIER A TRAITER DE LA SORTE: './Lexique nom_du_fichier'\n ");
		exit(EXIT_FAILURE);
	}

	if(argc == 2)
		i=1;

	else if (argc == 3){
		i=2;
		strcpy(option, argv[1]);
		printf("On a en entrer l'option suivant : %s \n", option);
		

		if( strcmp( option, "-l") == 0){
			printf("affiche lexique\n\n\n");
			choix=1;
		}

		else if( strcmp( option, "-s") == 0){
			printf("sauvegarde les mots\n\n\n");
			choix=2;
		}

		else if( strcmp( option, "-S") == 0){
			printf("sauvegarde l'arbre\n\n\n");
			choix=3;
		}
		else{
			printf("option invalide\n");
			choix=0;
		}
	}

	else if (argc == 4){
		i=3;
		strcpy(option, argv[1]);

		strcpy(mot_recherche, argv[2]);

		if( strcmp( option, "-r") == 0){
			printf("Recherche de %s\n\n", mot_recherche);
			choix=4;
			r=1;
		}
		else{
			printf("option invalide\n");
		}


	}
	else{
		printf("MAUVAIS NOMBRE DE PARAMETRE SUR LA LIGNE DE COMMANDE\n");
		exit(EXIT_FAILURE);
	}


	strcpy(nom_fichier, argv[i]);
	strcpy(save_lexique, argv[i]);
	strcpy(save_dico, argv[i]);


	strcat(save_lexique, ".LEX");
	strcat(save_dico, ".DIC");

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/*Bloc qui tente de creer l'arbre lexicographique a partir d'un .DIC si il existe. Sinon ouvre le fichier passé en argument et creer l'arbre lexicographique, si le nom_fichier n'existe pas on arrette le programme.*/

	dico=fopen(  save_dico , "r" );

	if(dico!=NULL && strcmp( option, "-S") != 0 ){
		printf("LE DICTIONNAIRE '%s' EXISTE DEJA", save_dico);
		charge_arbre(&arbre, dico);

	}
	
	else{

		fichier=fopen(  nom_fichier , "r" );
		if(fichier!=NULL){
			compteur_de_mots = lexique(fichier,&arbre);
			printf("Il y a %d mot\n", compteur_de_mots);

		}
	
	
		else{
			printf("le nom du fichier en ligne de commande n'existe pas ou ligne de commande mal ordonnée \n\n");
			exit(EXIT_FAILURE);
		}
	}
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/



	do{
		if(choix == 0)
			choix  = choix_menu( nom_fichier );
	
		switch(choix){
			case 1:
				affichage(arbre);
				choix=0;
			break;

			case 2:
				sauvegarde = fopen( save_lexique , "w");
				sauvegarde_mots(arbre, sauvegarde,  buffer, 0);
				fclose(sauvegarde);
				choix=0;
			break;

			case 3:
				
				sauvegarde=fopen( save_dico , "w");

				sauvegarde_arbre (arbre, sauvegarde);
				fclose(sauvegarde);
				choix=0;
			break;

			case 4:

				if(r == 0){
					printf("Quelle mot recherchez vous?\n");
					scanf("%s", mot_recherche);
					
				}



				if( recherche(arbre, mot_recherche) )	
					printf("\nOUI, LE MOT ' %s ' EST PRESENT DANS LE LEXIQUE \n", mot_recherche);
				else
					printf("\nNON, LE MOT ' %s ' EST ABSENT DU LEXIQUE \n", mot_recherche);

				choix = 0;
				r=0;
			break;
		
			case 5:
				printf("Entrez le nom du fichier à ouvrir\n");
				scanf("%s",fichier_a_ouvrir);

				fichier=fopen(  fichier_a_ouvrir , "r" );
				if(fichier!=NULL){
					arbre=NULL;
					compteur_de_mots = lexique(fichier,&arbre);
					printf("Il y a %d mot\n", compteur_de_mots);

					strcpy(nom_fichier, fichier_a_ouvrir);
					strcpy(save_lexique, nom_fichier);
					strcpy(save_dico, nom_fichier);


					strcat(save_lexique, ".LEX");
					strcat(save_dico, ".DIC");
					
				}
				else
					printf("le fichier n'existe pas\n");

				choix=0;
			break;

			case 6:
				fin=1;
			break;

			default:
				choix=0;
			break;


		
		}
	
	}while(fin != 1);	

	return 0;

	

}

