#include "ImageBase.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	char img_seuil[250];
	if (argc != 2)
	{
		printf("Usage: img_seuil.pgm\n"); 
		return 1;
	}
	sscanf (argv[1],"%s", img_seuil);

	/**********************
		TP3 Exercice 1 
	**********************/
	//Seuil 1 
	ImageBase img_seuil_im;
	ImageBase img_seuil_im2;
	ImageBase img_seuil_im3;

	img_seuil_im.load(img_seuil);
	img_seuil_im.seuillageBinaire(150);
	img_seuil_im.save("../Rapport/img_seuil_binaire_150.pgm");

	exit(EXIT_SUCCESS);
}
