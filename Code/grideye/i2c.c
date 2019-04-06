#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>


#define I2C_PERIPH "/dev/i2c-2"
#define ADDR_GRIDEYE 0x68
#define MAX_BUFF_SIZE 64

main()
{
	FILE* fichier = NULL;
 	//fichier = fopen("/root/resultat.txt", "w");
 
  
	int i2cFile;
	int flag;
	float tableau[8][8];
	float s = 0;
	int index = 0;
	float somme[6];
	short value;
	unsigned char I2C_WR_Buf[MAX_BUFF_SIZE];
	unsigned char I2C_RD_Buf[MAX_BUFF_SIZE];
while(1){
	i2cFile = open(I2C_PERIPH, O_RDWR);
	if(i2cFile < 0) {perror("i2cOpen error"); exit(1);}
	
	flag = ioctl(i2cFile, I2C_SLAVE_FORCE, ADDR_GRIDEYE);
	if(flag<0) {perror("i2cSetAddress error"); exit(1);}
	
	sleep(1);
	unsigned char MSB,LSB;
	float Temperature;
	int i,k;
	int n = 16;
	int j = 0;
	int pixel = 1;
	int retour = 0;
	for(i = 0; i < 8; i = i + 1)
	{
		j = 0;
		for(k = 256 - n ;k >= 128 + i*2; k = k - 16)
		{
			I2C_WR_Buf[0] = k; 
			flag = write(i2cFile, I2C_WR_Buf, 1);
			if(flag!=1){perror("Error in First Send I2C"); exit(1);}
			flag = read(i2cFile, I2C_RD_Buf, 2);
			if(flag!=2){perror("Error "); exit(1);}
	
	
			//Conversion de la valeur en degré
			value = ((short)(I2C_RD_Buf[1] & 0x0F) << 8) | I2C_RD_Buf[0];
	 		
			//LSB = I2C_RD_Buf[0];
			//MSB = I2C_RD_Buf[1] & 0x0F;

			if(I2C_RD_Buf[1] & 0x08 == 0x08)
			{
				//printf("Pixel %d : %x %x \n",pixel, I2C_RD_Buf[1], I2C_RD_Buf[0]);
				value = (~value)& 0x0FFF;
				//printf("inversion = %d\n", value);
		
				value++;
				//printf("+1 = %d\n", value);	
				value = - value;
				//printf("neg = %d\n\n", value);
			}

			Temperature = (int)value * 0.25;

			/*Ecriture des resultats dans le fichier "resultat.txt"
			if (fichier != NULL)
		    	{
				fprintf(fichier, "%3.1f ", Temperature);
		    	}*/
			// stocker les donnes dans un tableau 
			
			tableau[retour][j] = Temperature;
	
			j++;
			//Affichage des resultats sur la console
			//printf("Pixel %d : %x %x Temperature : %3.1f\n",pixel, I2C_RD_Buf[1], I2C_RD_Buf[0], Temperature);
			//pixel++;
		}
		retour++;
		printf("\n");
		//fprintf(fichier, "\n");
		n = n - 2;
	}
	for( i = 0; i < 8; i++)
	{
		tableau[i][0] = tableau[i][0] * 0.9;
	}
	for( i = 0; i < 8; i++)
	{
		tableau[i][1] = tableau[i][1] * 0.95;
	}
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			printf("%3.1f  ",tableau[i][j]);
		}
		printf("\n");
	}
	
	for(j = 0; j < 6 ;j++)
	{
		somme[j] = 0;
		for(i = 0; i < 8; i++)	
			somme[j] = tableau[i][j] + tableau[i][j+1] + tableau[i][j+2] + somme[j];
		printf("somme %d = %3.1f    ",j,somme[j]);
	}
	s = somme[0];
	for (i = 0; i<6; i++)
	{
		if( somme[i] > s)
		{
			s = somme[i];
			index = i;
		}
	}
	printf(" \n index = %d \n", index);
}
	//fclose(fichier);
	close(i2cFile);
}