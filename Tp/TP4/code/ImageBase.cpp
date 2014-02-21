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
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut être que '.pgm' ou '.ppm'");
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
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut être que .pgm ou .ppm");
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
	Crée et sauvegarde l'histogramme de l'image.
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

/**
	Egalise une image
**/
int ImageBase::egaliser(){
	unsigned char* data = this->getData();
	double F[256];
	unsigned char T[256];

	this->getFa(F);

	//On calcule T pour 255 niveaux de gris
	for(int i =0; i < 256; i++){
		T[i] = (int)(255.*F[i]);
	}

	for(int i = 0; i < this->getTotalSize(); i++){
		data[i] = T[data[i]];
	}
	return 0;
}

/**
Déségalise une image en utilisant la fonction de répartition inverse d'une autre image.
**/
int ImageBase::desegaliser(ImageBase img){
	unsigned char* data = this->getData();
	double F[256];
	img.getFa(F);
	int indice = 0;
	double tmp = 0., space = 1000., current = 0.;
	
	cout << "test1" << endl;
	for(int i = 0; i < this->getTotalSize(); i++){
		current = (double)data[i]/(double)255.;
		for(int j = 0; j < 256; j++){
			tmp = current - F[j];
			if(tmp < 0.)
				tmp *= -1.;
				
			if(tmp < space){
				space = tmp;
				indice = j;
			}
		}
		data[i] = (unsigned char)(F[indice]*255.);
		space = 1000.;
		tmp = 0.;
		indice = 0;
	}
	cout << "test2" << endl;
	return 0;
}


/**
Calcule le ddp d'une image.
**/
int ImageBase::calculerDDP(double* ddp){

	int histo[256];
	this->getHisto(histo);

	for(int i =0; i < 256; i++){
		ddp[i] = (double)histo[i]/(double)this->getTotalSize();
	}
	return 0;
}

/**
	Calcule la fonction de repartition de l'image donnée.
**/
int ImageBase::getFa(double* F){
	double ddp[256];
	this->calculerDDP(ddp);
	
	F[0] = ddp[0];
	for(int i = 1; i < 256; i++){
		F[i] = F[i-1] + ddp[i];
	}
	return 0;
}

/**
	Calcule et sauvegarde le ddp de l'image.
**/
int ImageBase::saveDDP(string path){
	if(!this->getColor()){//Pour une image NB.
		ofstream file(path.c_str(), ios::out | ios::trunc);
		if(file){
			double ddp[256];
			this->calculerDDP(ddp);

			for(int i = 0; i < 256; i++)
			file << i << " " << ddp[i] << endl;
			file.close();
		}else{
			return -1;
		}
	}
	return 0;
}

/**
	Calcule et sauvegarde Fa
**/
int ImageBase::saveFa(string path){
	double F[256];
	this->getFa(F);
	
	ofstream file(path.c_str(), ios::out | ios::trunc);
		if(file){	
			for(int i = 0; i < 256; i++)
			file << i << " " << F[i] << endl;
			file.close();
		}else{
			return -1;
		}
		return 0;
}

/**
	Seuillage binaire.
	si pixel < seuil alors pixel = 255
	sinon pixel  = 0
**/
int ImageBase::seuillageBinaire(unsigned char seuil){
	unsigned char* data = this->getData();
	for(int i = 0; i < this->getTotalSize(); i++){
		if(data[i] < seuil){
			data[i] = 0;
		}else{
			data[i] = 255;
		}
	}
	return 0;
}

int ImageBase::eroder(){
	unsigned char* data = this->getData();
	unsigned char* voisins[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	bool toDel = false;

	unsigned char data2[this->getTotalSize()];
	for(int i = 0; i < this->getTotalSize(); i++)
		data2[i] = data[i];
	
	for(int i = 0; i < this->getTotalSize(); i++){
		if(data[i] == 255){
			this->getVoisins(this->getData(), i, voisins);
			for(int j = 0; j < 8; j++){
				if(voisins[j] != NULL && *voisins[j] == 0){
					toDel = true;
					break;
				}
			}
			if(toDel){
				toDel = false;
				this->getVoisins(data2, i, voisins);
				for(int i = 0; i < 8; i++){
					if(voisins[i] != NULL){
						*voisins[i] = 255;
					}
				}
			}
			voisins[0] = NULL;
			voisins[1] = NULL;
			voisins[2] = NULL;
			voisins[3] = NULL;
			voisins[4] = NULL;
			voisins[5] = NULL;
			voisins[6] = NULL;
			voisins[7] = NULL;
		}
	}

	for(int i = 0; i < this->getTotalSize(); i++)
		data[i] = data2[i];
	return 0;
}

int ImageBase::getVoisins(unsigned char* data, int i, unsigned char* res[]){
	int mSize = this->getTotalSize();
	int w = this->getWidth();

	if(i+w-1 < mSize)
		res[0] = &(data[i+w-1]);
	
	if(i-1 >= 0)
		res[1] = &(data[i-1]);
	

	if(i-w-1 >= 0)
		res[2] = &(data[i-w-1]);
	

	if(i-w >= 0)
		res[3] = &(data[i-w]);
	

	if(i-w+1 >= 0)
		res[4] = &(data[i-w+1]);
	

	if(i+1 < mSize)
		res[5] = &(data[i+1]);
	

	if(i+w+1 < mSize)
		res[6] = &(data[i+w+1]);
	

	if(i+w < mSize)
		res[7] = &(data[i+w]);
	
	return 0;
}

int ImageBase::dilater(){
	unsigned char* data = this->getData();
	unsigned char* voisins[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	bool toDel = false;

	unsigned char data2[this->getTotalSize()];
	for(int i = 0; i < this->getTotalSize(); i++)
		data2[i] = data[i];
	
	for(int i = 0; i < this->getTotalSize(); i++){
		if(data[i] == 0){
			this->getVoisins(this->getData(), i, voisins);
			for(int j = 0; j < 8; j++){
				if(voisins[j] != NULL && *voisins[j] == 255){
					toDel = true;
					break;
				}
			}
			if(toDel){
				toDel = false;
				this->getVoisins(data2, i, voisins);
				for(int i = 0; i < 8; i++){
					if(voisins[i] != NULL){
						*voisins[i] = 0;
					}
				}
			}
			voisins[0] = NULL;
			voisins[1] = NULL;
			voisins[2] = NULL;
			voisins[3] = NULL;
			voisins[4] = NULL;
			voisins[5] = NULL;
			voisins[6] = NULL;
			voisins[7] = NULL;
		}
	}

	for(int i = 0; i < this->getTotalSize(); i++)
		data[i] = data2[i];
	return 0;
}
int ImageBase::fermer(){
	this->dilater();
	this->eroder();
	return 0;
}
int ImageBase::ouvrir(){
	this->eroder();
	this->dilater();
	return 0;
}

int ImageBase::ouvrirFermer6(){
	this->dilater();
	this->dilater();
	this->dilater();

	this->eroder();
	this->eroder();
	this->eroder();

	this->eroder();
	this->eroder();
	this->eroder();

	this->dilater();
	this->dilater();
	this->dilater();
	return 0;
}

int ImageBase::diff(ImageBase seuille, ImageBase dilate){
	unsigned char* dataS = seuille.getData();
	unsigned char* dataD = dilate.getData();
	unsigned char* data = this->getData();

	for(int i = 0; i < this->getTotalSize(); i++){
		if(dataS[i] == dataD[i]){
			data[i] = 255;
		}else{
			data[i] = 0;
		}
	}
	return 0;
}

int ImageBase::toGrey(unsigned char dataGrey[]){
	unsigned char* data = this->getData();
	for(int i = 0; i < this->getTotalSize()/3; i++){
		dataGrey[i] = (unsigned char)(((double)data[i] + (double)data[i+1] + (double)data[i+2])/3.0);
	}
	return 0;
}
