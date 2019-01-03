/*
    BMP2C Version 1.1
	 
   1.2 - Peut générer un tableau de short (utile pour DSP 5502)
	1.1 - Gestion des caractères en 6x8 sur certains LCD
   1.0 - Version initiale
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <conio.h>
 
int main(int argv, char *argc[])
{
   FILE *fin;		// fichier BMP
   FILE *fout;		// code source généré

   int dbg, dbg2;
	
   int i, j;
	int width, height, end, real_width , offset, padding, pos, taille, data;
	
	int largeur_6_px = 0;	// 1 si 6px utilisés
   int tableau_16bits = 0; // 1 si tableau de 16 bits
   int tableau_32bits = 0; // 1 si tableau de 32 bits
	
	unsigned char byte1, byte2, byte3;
	unsigned char *scratch, *bmp_data;
	
   if(argv<3 || argv>4) {
     printf("Usage: %s input.bmp output.c [-six_pixels] [-16bits_array] [-32bits_array]\n",argc[0]);
     return (0);
   }

   if((fin=fopen(argc[1],"r"))==NULL) {
     printf("Error loading input file.\n");
     return (0);
   }
	
	if( argv==4 && strcmp( "-six_pixels", argc[3] )==0 ) {
		printf("\nOption enabled : six-pixel columns.\n\n");
		largeur_6_px = 1;
	}

   if( argv==4 && strcmp( "-16bits_array", argc[3] )==0 ) {
		printf("\nOption enabled : 16bits array.\n\n");
		tableau_16bits = 1;
	}

   if( argv==4 && strcmp( "-32bits_array", argc[3] )==0 ) {
		printf("\nOption enabled : 32bits array.\n\n");
		tableau_32bits = 1;
	}

   if( largeur_6_px+tableau_16bits+tableau_32bits>1 ) {
      printf("Cannot use multiple options at the same time.\n");
     return (0);
   }
	
   fseek(fin,0x1c,SEEK_SET);
   
   if(fgetc(fin)!=1) {
     printf("Bitmap must have 2 colours (1bit, black and white).\n");
     fclose(fin);
     return (0);
   }

   if((fout=fopen(argc[2],"w"))==NULL) {
      printf("Output file not found. Creating it now...");
      fclose(fin);
      return (0);
   }
    
#ifdef _DEBUG
		printf("***DEBUG MODE***\n\n");
#endif

   if( tableau_16bits ) {
      fprintf(fout,"const uint16 %s[] = {\n",strtok(argc[2],"."));
   } else if( tableau_32bits ) {
      fprintf(fout,"const uint32 %s[] = {\n",strtok(argc[2],"."));
   } else {
      fprintf(fout,"const uint8 %s[] = {\n",strtok(argc[2],"."));
   }
   fseek(fin,0x12,SEEK_SET);
   fread(&width,4,1,fin);
   fseek(fin,0x16,SEEK_SET);
   fread(&height,4,1,fin);
   printf( "width : %d height : %d \n",width, height );
    
    
   fseek(fin,0x0A,SEEK_SET); // indique l'offset du début des données
   fread(&offset,4,1,fin);
   
   // La largeur de l'image doit être un multiple de 32 ... on va donc ajuster :
   if( width%32 != 0)
      end = ((width/32)+1)*32;
	else
		end = width;
   	
   // On retire les octets de bourrage éventuels
   padding = (end - width)/8;
	
	// La vrai taille indique le nombre d'octets utiles
	real_width = (end/8 - padding);
	taille = real_width*height;
	
   printf( "bits utilises : %d padding : %d octets ajoutes.\n", end, padding );
	printf( "Taille du tableau : %d\n", taille);
	
	if( taille < 1 || taille > 65000) {
		printf("Cannot use this image size. Exiting...\n");
		return 1;
	}
	
	scratch = (unsigned char*)malloc( taille*sizeof(unsigned char) );
	if( scratch == NULL ) {
		printf("Memory allocation error. Exiting...\n");
		return 2;
	}

   bmp_data = (unsigned char*)malloc( (real_width+padding)*height*sizeof(unsigned char) );
	if( bmp_data == NULL ) {
		printf("Memory allocation error. Exiting...\n");
		return 2;
	}
	
   // On se positionne au début des données pour récupérer les octets bruts du BMP
   fseek(fin,offset,SEEK_SET);
   fread(bmp_data,1,(real_width+padding)*height,fin);

   // Retournement de l'image + inverse vidéo (le format BMP inverse l'image, et on veut noir = 1, blanc = 0)
   for(j=0; j<height; j++) {
      for(i=0; i<real_width; i++) {
         dbg = taille-real_width-j*real_width+i;
         dbg2 = j*real_width+j*padding+i;
         scratch[dbg] = ~bmp_data[dbg2];   
      }
   }
	
	// On génère le tableau en code C, l'image à l'endroit
	// (le premier octet représente le pixel en haut à gauche de l'image)
   if( !largeur_6_px ) {	// 8 pixels utiles par octets
		for(j=0; j<height; j++) {
			fprintf(fout,"   ");
         if( tableau_16bits ) {
            for(i=0; i<real_width; i+=2) {
               byte1 = scratch[j*real_width+i];
               if( i+1 == real_width )
                  byte2 = 0;
               else
                  byte2 = scratch[j*real_width+i+1];
				   fprintf(fout,"%0#.4x, ", ((unsigned short)byte1)<<8 | byte2 );
			   }
         } else if( tableau_32bits ) {
            for(i=0; i<real_width; i+=4) {
				   fprintf(fout,"0x%08x, ",  ((unsigned long)scratch[j*real_width+i])<<24 | 
                                       ((unsigned long)scratch[j*real_width+i+1])<<16 |
                                       ((unsigned long)scratch[j*real_width+i+2])<<8 |
                                       ((unsigned long)scratch[j*real_width+i+3])
                  );
			   }
         } else {
            for(i=0; i<real_width; i++) {
				   fprintf(fout,"%#.2x, ", scratch[j*real_width+i] );
			   }
         }
			fprintf(fout,"\n");
		}
		fprintf(fout,"};\n");
   } else {	// 6 pixels utils par octet
		for(j=0; j<height; j++) {
			fprintf(fout,"   ");
			for(i=0; i<real_width; i+=3) {	
				byte1 = scratch[ j*real_width+i ];
				data = (byte1 >> 2) & 0x3F;
				fprintf(fout,"%#.2x, ", data );
#ifdef _DEBUG
				printf("%#.2x, ", data );
#endif
				if( i+1 == real_width )
				{
					break;
				}
				else
				{
					byte2 = scratch[ j*real_width+i+1 ];
					data = ((byte1 << 4) | ((byte2 >> 4) & 0xF)) & 0x3F;
					fprintf(fout,"%#.2x, ", data );
#ifdef _DEBUG
					printf("%#.2x, ", data );
#endif

				}
				
				if( i+2 == real_width )
				{
					break;
				}
				else
				{
					byte3 = scratch[ j*real_width+i+2 ];
					data = ((byte2 << 2) | ((byte3 >> 6) & 0x3)) & 0x3F;
					fprintf(fout,"%#.2x, ", data );
#ifdef _DEBUG
					printf("%#.2x, ", data );
#endif
				}
	/*
				if( i+3 == real_width ) {
					break;
				} else {
*/
					data = byte3 & 0x3F;
					fprintf(fout,"%#.2x, ", data );
#ifdef _DEBUG
					printf("%#.2x, ", data  );
#endif
//				}
			
				
				
			}
			fprintf(fout,"\n");
		//	printf("\n");
		}
		fprintf(fout,"};\n");
		
	}
	
#ifdef _DEBUG
		_getch();
#endif
	
	
   fclose(fin);
   fclose(fout);
	free( scratch );
   free( bmp_data );
   return (0);
}

/*
   // On se positionne au début des données
   fseek(fin,offset,SEEK_SET);
   for(j=0; j<height; j++)
   {
      fprintf(fout,"   ");
      for(i=0; i<(end/8)-padding; i++)
      {
         fprintf(fout,"%#.2x, ", (unsigned char)(~fgetc(fin)) );
      }
      pos = ftell( fin );
      fseek(fin,pos+padding,SEEK_SET); // on avance le pointeur pour sauter le padding
      fprintf(fout,"\n");
   }
	
   fprintf(fout,"};\n");
   */

/* EOF */
