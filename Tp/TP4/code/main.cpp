#include "ImageBase.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
	char img_color[250];
	if (argc != 2)
	{
		printf("Usage: img_color.ppm\n"); 
		return 1;
	}
	sscanf (argv[1],"%s", img_color);
	
	ImageBaseimg1.load(img_color);
	
	exit(EXIT_SUCCESS);
}
