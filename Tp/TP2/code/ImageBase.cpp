/******************************************************************************
* ICAR_Library
*
* Fichier : ImageBase.cpp
*
* Description : Voir le fichier .h
*
* Auteur : Mickael Pinto
*
* Mail : mickael.pinto@live.fr
*
* Date : Octobre 2012
*
*******************************************************************************/

#include "ImageBase.h"
#include "image_ppm.h"
#include <stdio.h>
#include <fstream>
#include <iostream>


using namespace std;

ImageBase::ImageBase(void)
{
	isValid = false;
	init();
}

ImageBase::ImageBase(int imWidth, int imHeight, bool isColor)
{
	isValid = false;
	init();

	color = isColor;
	height = imHeight;
	width = imWidth;
	nTaille = height * width * (color ? 3 : 1);
	
	if(nTaille == 0)
		return;
	
	allocation_tableau(data, OCTET, nTaille);
	dataD = (double*)malloc(sizeof(double) * nTaille);
	isValid = true;
}


ImageBase::~ImageBase(void)
{
	reset();
}

void ImageBase::init()
{
	if(isValid)
	{
		free(data);
		free(dataD);
	}

	data = 0;
	dataD = 0;
	height = width = nTaille = 0;
	isValid = false;
}

void ImageBase::reset()
{
	if(isValid)
	{
		free(data);
		free(dataD);
	}
	isValid = false;
}

void ImageBase::load(char *filename)
{
	init();

	int l = strlen(filename);

	if(l <= 4) // Le fichier ne peut pas etre que ".pgm" ou ".ppm"
	{
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut �tre que '.pgm' ou '.ppm'");
		exit(0);
	}

	int nbPixel = 0;

	if( strcmp(filename + l - 3, "pgm") == 0) // L'image est en niveau de gris
	{
		color = false;
		lire_nb_lignes_colonnes_image_pgm(filename, &height, &width);
		nbPixel = height * width;
  
		nTaille = nbPixel;
		allocation_tableau(data, OCTET, nTaille);
		lire_image_pgm(filename, data, nbPixel);
	}
	else if( strcmp(filename + l - 3, "ppm") == 0) // L'image est en couleur
	{
		color = true;
		lire_nb_lignes_colonnes_image_ppm(filename, &height, &width);
		nbPixel = height * width;
  
		nTaille = nbPixel * 3;
		allocation_tableau(data, OCTET, nTaille);
		lire_image_ppm(filename, data, nbPixel);
	}
	else 
	{
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut �tre que .pgm ou .ppm");
		exit(0);
	}
	
	dataD = (double*)malloc(sizeof(double) * nTaille);

	isValid = true;
}

bool ImageBase::save(char *filename)
{
	if(!isValid)
	{
		printf("Sauvegarde de l'image impossible : L'image courante n'est pas valide");
		exit(0);
	}

	if(color)
		ecrire_image_ppm(filename, data,  height, width);
	else
		ecrire_image_pgm(filename, data,  height, width);

	return true;
}

ImageBase *ImageBase::getPlan(PLAN plan)
{
	if( !isValid || !color )
		return 0;

	ImageBase *greyIm = new ImageBase(width, height, false);
	
	switch(plan)
	{
	case PLAN_R:
		planR(greyIm->data, data, height * width);
		break;
	case PLAN_G:
		planV(greyIm->data, data, height * width);
		break;
	case PLAN_B:
		planB(greyIm->data, data, height * width);
		break;
	default:
		printf("Il n'y a que 3 plans, les valeurs possibles ne sont donc que 'PLAN_R', 'PLAN_G', et 'PLAN_B'");
		exit(0);
		break;
	}

	return greyIm;
}

void ImageBase::copy(const ImageBase &copy)
{
	reset();
	
	isValid = false;
	init();
	
	color 	= copy.color;
	height 	= copy.height;
	width 	= copy.width;
	nTaille = copy.nTaille;
	isValid = copy.isValid;
	
	if(nTaille == 0)
		return;
	
	allocation_tableau(data, OCTET, nTaille);
	dataD = (double*)malloc(sizeof(double) * nTaille);
	isValid = true;

	for(int i = 0; i < nTaille; ++i)
	{
		data[i] = copy.data[i];
		dataD[i] = copy.dataD[i];
	}

}

unsigned char *ImageBase::operator[](int l)
{
	if(!isValid)
	{
		printf("L'image courante n'est pas valide");
		exit(0);
	}
	
	if((!color && l >= height) || (color && l >= height*3))
	{
		printf("L'indice se trouve en dehors des limites de l'image");
		exit(0);
	}
	
	return data+l*width;
}

/**
	Expension de l'image.
**/
int ImageBase::expand(int a0, int a1){
	unsigned char* data = this->getData();
	int alpha = -(0*a1 + 255*a0) / (a1 - a0); 
	int beta = (255 - 0) / (a1 - a0);

	for(int i = 0; i < this->getTotalSize(); i++){
		data[i] = alpha + beta * data[i];
	}
	return 0;
}

/**
	Expension de l'image (couleur).
**/
int ImageBase::expandColor(int a0R, int a1R, int a0G, int a1G, int a0B, int a1B){
	unsigned char* data = this->getData();

	//Red
	int alphaR = -(0*a1R + 255*a0R) / (a1R - a0R); 
	int betaR = (255 - 0) / (a1R - a0R);

	//Green
	int alphaG = -(0*a1G + 255*a0G) / (a1G - a0G); 
	int betaG = (255 - 0) / (a1G - a0G);

	//Blue
	int alphaB = -(0*a1B + 255*a0B) / (a1B - a0B); 
	int betaB = (255 - 0) / (a1B - a0B);

	for(int i = 0; i < this->getTotalSize(); i += 3){
		data[i] = alphaR + betaR * data[i];
		data[i+1] = alphaG + betaG * data[i+1];
		data[i+2] = alphaB + betaB * data[i+2];
	}
	return 0;
}

/**
	Cr�e et sauvegarde l'histogramme de l'image.
**/
int ImageBase::saveHisto(string path){
	ofstream file(path.c_str(), ios::out | ios::trunc);
	if(file){
		if(this->getColor()){//Pour une image en couleur.
			int histo[3][256];
			this->getHistoColor(histo);

			for(int i = 0; i < 256; i++)
			file << i << " " << histo[0][i] << " " << histo[1][i]<< " " << histo[2][i] << endl;
			file.close();
		}else{
			int histo[256];
			this->getHisto(histo);

			for(int i = 0; i < 256; i++)
			file << i << " " << histo[i] << endl;
			file.close();
		}
	}else{
		return -1;
	}
	return 0;
}
/**
	Retourne l'histogramme de l'image.
**/
int ImageBase::getHisto(int* tab){
	int nb, i, j;
	unsigned char* data = this->getData();
	
	for(i = 0; i < 256; ++i){//Pour chaque valeur possibles:0-255
		tab[i] = 0;
		for(j = 0; j < this->getTotalSize(); ++j){
			if(data[j] == i){
				tab[i]++;
			}
		}
	}
	
	return 0;
}

/*
	Retourne l'histogramme pour une image couleur.
*/
int ImageBase::getHistoColor(int tab[][256]){
	int i, j;
	unsigned char* data = this->getData();
	
	for(i = 0; i < 256; ++i){//Pour chaque valeur possibles:0-255
		tab[0][i] 	= 0;
		tab[1][i] 	= 0;
		tab[2][i] 	= 0;
		for(j = 0; j < this->getTotalSize(); j += 3){
			//Red
			if(data[j] == i){
				tab[0][i]++;
			}

			//Green
			if(data[j+1] == i){
				tab[1][i]++;
			}
			//Blue
			if(data[j+2] == i){
				tab[2][i]++;
			}
		}
	}	
	return 0;
}

int ImageBase::egaliser(){
	unsigned char* data = this->getData();
	unsigned char ddp[256];
	unsigned char F[256];
	unsigned char T[256];

	calculerDDP(ddp);

	//On applique la fonction F pour les 256 niveaux de gris
	F[0] = ddp[0];
	for(int i = 1; i < 256; i++){
		F[i] = F[i-1] + ddp[i];
	}

	//On calcule T pour 255 niveaux de gris
	for(int i =0; i < 256; i++){
		T[i] = 255*F[i];
	}

	for(int i = 0; i < this->getTotalSize(); i++){
		data[i] = T[data[i]];
	}
	return 0;
}

int ImageBase::calculerDDP(unsigned char* ddp){

	int histo[256];
	this->getHisto(histo);

	for(int i =0; i < 256; i++){
		ddp[i] = histo[i]/this->getTotalSize();
	}
	return 0;
}
