#include "ImageBase.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	char black_pgm[250], black_ppm[250], pathImgColor[250], pathImgGris[250];
	if (argc != 5)
	{
		printf("Usage: black.pgm black.ppm imgColor.ppm imgGris.pgm\n"); 
		return 1;
	}
	sscanf (argv[1],"%s", black_pgm);
	sscanf (argv[2],"%s", black_ppm);
	sscanf (argv[3],"%s", pathImgColor);
	sscanf (argv[4],"%s", pathImgGris);

	ImageBase black_pgm_im;
	black_pgm_im.load(black_pgm);

	ImageBase black_ppm_im;
	black_ppm_im.load(black_ppm);

	ImageBase img_Color;
	img_Color.load(pathImgColor);

	ImageBase img_Gris;
	img_Gris.load(pathImgGris);

	/*////////////////////////////////////////
	//////////////////////////////////////////
	Tp2 Exercice 1
	//////////////////////////////////////////
	////////////////////////////////////////*/

	//Modifications de black.pgm
	black_pgm_im.saveHisto("../Rapport/Histo_Black_pgm.dat");
	black_pgm_im.save("../Rapport/Black.pgm");
	black_pgm_im.expand(0, 10);
	black_pgm_im.saveHisto("../Rapport/New_Histo_Black_pgm.dat");
	black_pgm_im.save("../Rapport/New_Black.pgm");

	//Modifications de black.ppm
	black_ppm_im.saveHisto("../Rapport/Histo_Black_ppm.dat");
	black_ppm_im.save("../Rapport/Black.ppm");
	black_ppm_im.expandColor(0, 15, 0, 15, 0, 15);
	black_ppm_im.saveHisto("../Rapport/New_Histo_Black_ppm.dat");
	black_ppm_im.save("../Rapport/New_Black.ppm");


	/*////////////////////////////////////////
	//////////////////////////////////////////
	Tp2 Exercice 2
	//////////////////////////////////////////
	////////////////////////////////////////*/
	img_Color.saveHisto("../Rapport/histo_img_color.dat");
	img_Color.save("../Rapport/img_color.ppm");

	//On prepare les valeurs de seuils.
	int SminR = 50, SminG = 67, SminB = 18, SmaxR = 230, SmaxG = 192, SmaxB = 240;

	//On seuille l'image (data4)
	unsigned char* data = img_Color.getData();
	for(int i = 0; i < img_Color.getTotalSize(); i += 3){
		if(data[i] < SminR) //Red
			data[i] = SminR;
		if(data[i+1] < SminG) //Green
			data[i+1] = SminG;
		if(data[i+2] < SminB) //Blue
			data[i+2] = SminB;

		if(data[i] > SmaxR)
			data[i] = SmaxR;
		if(data[i+1] > SmaxG)
			data[i+1] = SmaxG;
		if(data[i+2] > SmaxB)
			data[i+2] = SmaxB;
	}
	img_Color.saveHisto("../Rapport/histo_img_color_seuil.dat");
	img_Color.save("../Rapport/img_color_seuil.ppm");

	img_Color.expandColor(SminR, SmaxR, SminG, SmaxG, SminB, SmaxB);
	img_Color.saveHisto("../Rapport/new_img_color.dat");
	img_Color.save("../Rapport/new_img_color.ppm");



	/*////////////////////////////////////////
	//////////////////////////////////////////
	Tp2 Exercice 3
	//////////////////////////////////////////
	////////////////////////////////////////*/
	img_Gris.saveHisto("../Rapport/histo_Img_Gris.dat");
	img_Gris.save("../Rapport/Img_Gris.pgm");

	img_Gris.egaliser();
	img_Gris.saveHisto("../Rapport/histo_Img_Gris_egalise.dat");
	img_Gris.save("../Rapport/Img_Gris_egalise.pgm");

	return 0;
}
