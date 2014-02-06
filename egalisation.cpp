#include <fstream>
#include <iostream>
#include "ImageBase.h"

using namespace std;

int main(int argc, char **argv)
{
	char cNomImgLue[250];
	char cNomImgEcrite[250];

	//initialisation de l'histogramme et des probas de densité
	int hist[256];
	float ddp[256];
	float F[256];
	float N;
    float T[256];

	for (int i=0;i<256;++i)
	{
		hist[i]=0;
		ddp[i]=0.0;
		F[i]=0.0;
		T[i]=0.0;
	}

	if (argc != 3 )
	{
		printf("Usage: ImageIn.pgm ImageOut.pgm\n");
		return 1;
	}
	sscanf (argv[1],"%s",cNomImgLue) ;
	sscanf (argv[2],"%s",cNomImgEcrite) ;

	//ImageBase imIn, imOut;
	ImageBase imIn;
	imIn.load(cNomImgLue);

	//ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
	ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

	ofstream f2("histo.dat",ios::out | ios::trunc);
	ofstream f("ddp.dat",ios::out | ios::trunc);
	ofstream f3("Fonction_f.dat", ios::out | ios::trunc);
	ofstream f4("histoAP.dat",ios::out | ios::trunc);

	//Création de l'histogramme
	for (int x=0;x < imIn.getHeight();++x)
	{
		for (int y=0;y<imIn.getWidth();++y)
		{
			hist[imIn[x][y]]++;
		}
	}
	for (int i=0;i<256;++i)
	{
		f2 << i << " " <<hist[i] << endl;
	}
	//Création de ddp
	//Hist[i]/N
	//Calcul de N longueur * largeur
    N = imIn.getHeight() * imIn.getWidth();

    for (int i=1;i<256;i++)
    {
        ddp[i] = hist[i]/N;
    }

    //Calcul de la fonction F
    //F(0) = dpp[0]
	//F(i) = F(i-1) + ddp[i]
    F[0] = (float)ddp[0];
    for (int i=1;i<256;i++)
    {
        F[i]=(float)F[i-1] + (float)ddp[i];
    }

    //Calcul de T()
    //T(a) = 255*F(a)
    for (int i =1 ; i<256 ; i ++)
    {
        T[i]=255*F[i];
    }

    for (int x=0;x < imIn.getHeight() ; ++x)
    {
        for (int y=0;y<imIn.getWidth();++y)
        {
            imOut[x][y]  = T[imIn[x][y]];
        }
    }
    //Histogramme de sortie
    for (int x=0;x < imIn.getHeight();++x)
	{
		for (int y=0;y<imIn.getWidth();++y)
		{
			hist[imIn[x][y]]++;
		}
	}
    for (int x=0;x < imOut.getHeight();++x)
	{
		for (int y=0;y<imOut.getWidth();++y)
		{
			hist[imOut[x][y]]++;
		}
	}

	//Ecriture de l'histogramme et ddp dans le fichier
	for (int i=0;i<256;++i)
	{
		f4 << i << " " <<hist[i] << endl;
		f << i << " " <<ddp[i] << endl;
		f3 << i << " " << F[i] << endl;
	}

	//Ecriture de l'image
	imOut.save(cNomImgEcrite);

	f2.close();
	f.close();
	return 0;
}
