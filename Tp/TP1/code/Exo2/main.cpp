#include "ImageBase.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	///////////////////////////////////////// Exemple d'un seuillage d'image
	char cNomImgLue[250], cNomImgEcrite[250];
	int S1, S2, S3, S4;
  
	if (argc < 6 || argc > 7) 
	{
		printf("Usage: ImageIn.pgm ImageOut.pgm Seuil_1 Seuil_2 Seuil_3 [Seuil_4] \n"); 
		return 1;
	}
	sscanf (argv[1],"%s",cNomImgLue) ;
	sscanf (argv[2],"%s",cNomImgEcrite);
	sscanf (argv[3],"%d",&S1);
	sscanf (argv[4],"%d",&S2);
	sscanf (argv[5],"%d",&S3);
	if(argc == 7){sscanf (argv[6],"%d",&S4);}
	
	
	//ImageBase imIn, imOut;
	ImageBase imIn;
	imIn.load(cNomImgLue);

	//ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
	ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

	if(argc == 6){
		for(int x = 0; x < imIn.getHeight(); ++x){
			for(int y = 0; y < imIn.getWidth(); ++y)
			{
				if (imIn[x][y] < S1) 
					imOut[x][y] = 0;
				else if (imIn[x][y] < S2)
					imOut[x][y] = 85;
				else if (imIn[x][y] < S3)
					imOut[x][y] = 170;
				else imOut[x][y] = 255;
			}
		}
	}else{
		for(int x = 0; x < imIn.getHeight(); ++x){
			for(int y = 0; y < imIn.getWidth(); ++y)
			{
				if (imIn[x][y] < S1) 
					imOut[x][y] = 0;
				else if (imIn[x][y] < S2)
					imOut[x][y] = 64;
				else if (imIn[x][y] < S3)
					imOut[x][y] = 128;
				else if (imIn[x][y] < S4)
					imOut[x][y] = 192;
				else imOut[x][y] = 255;
			}
		}
	}

	imOut.save(cNomImgEcrite);
		

	
	
	///////////////////////////////////////// Exemple de création d'une image couleur
	ImageBase imC(50, 100, true);

	for(int y = 0; y < imC.getHeight(); ++y)
		for(int x = 0; x < imC.getWidth(); ++x)
		{
			imC[y*3][x*3+0] = 200; // R
			imC[y*3][x*3+1] = 0; // G
			imC[y*3][x*3+2] = 0; // B
		}
		
	imC.save("imC.ppm");
		



	///////////////////////////////////////// Exemple de création d'une image en niveau de gris
	ImageBase imG(50, 100, false);

	for(int y = 0; y < imG.getHeight(); ++y)
		for(int x = 0; x < imG.getWidth(); ++x)
			imG[y][x] = 50;

	imG.save("imG.pgm");




	ImageBase imC2, imG2;
	
	///////////////////////////////////////// Exemple lecture image couleur
	imC2.load("imC.ppm");
	///////////////////////////////////////// Exemple lecture image en niveau de gris
	imG2.load("imG.pgm");
	
	

	///////////////////////////////////////// Exemple de récupération d'un plan de l'image
	ImageBase *R = imC2.getPlan(ImageBase::PLAN_R);
	R->save("R.pgm");
	delete R;
	


	return 0;
}
