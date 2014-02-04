#include "ImageBase.h"
#include <stdio.h>

#include <fstream>
 
using namespace std;

void profil(ImageBase imIn, char type, int indice);
int main(int argc, char **argv)
{
	///////////////////////////////////////// Exemple d'un seuillage d'image
	char cNomImgLue[250];
	int indice;
	char rowOrCol;
  
	if (argc != 4) 
	{
		printf("Usage: ImageIn.pgm (l || c) indice \n"); 
		return 1;
	}
	sscanf (argv[1],"%s", cNomImgLue) ;
	sscanf (argv[2],"%c", &rowOrCol);
	sscanf (argv[3],"%d", &indice);
	
	
	//ImageBase imIn, imOut;
	ImageBase imIn;
	imIn.load(cNomImgLue);

	if(rowOrCol != 'c' && rowOrCol != 'l'){
		printf("You must choose l or c to define if you would like to read line or a col.\n");
	}
	else{
		profil(imIn, rowOrCol, indice);
	}
	exit(EXIT_SUCCESS);
}

void profil(ImageBase imIn, char type, int indice){
	int i;
	ofstream fichier("profil.dat", ios::out | ios::trunc);
	if(type == 'c'){
		for(i = 0; i < imIn.getHeight(); i++){
			fichier << /*i << " " <<*/ (int)imIn[i][indice] << endl;
		}
	}else if(type == 'l'){
		for(i = 0; i < imIn.getWidth(); i++){
			fichier << /*i << " " <<*/ (int)imIn[i][indice] << endl;
		}
	}else{
		printf("Erreur: can't read if you want to read a line or a col.\n");
		exit(EXIT_SUCCESS);
	}
	fichier.close();
}
