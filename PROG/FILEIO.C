#include "main.h"
#include "anim.h"
#include <conio.h>
#include <stdio.h>
#include <string.h>


extern struct animshape animobjects[TOTALANIMS];

extern char *st1;

FILE *in, *out, *outdata, *indata, *std;

int filewrite(void)
{

   int a,b;
   int i,numobj,numrow;
   char label[20];
   char infst[20];
   char stan[20];

   gotoxy (1,23);
   printf("Please enter Filename: ");
   fflush(stdin);  /* flush the input stream in case of bad input */

   scanf("%8s",label);
   strcpy(infst, label);
   strcpy(stan, label);

   strcat(label,".dat");
   strcat(infst,".inf");
   strcat(stan,".std");


   if ((std = fopen(stan, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open Standard file.\n");
	  return 1;
   }


   if ((out = fopen(infst, "wb"))
	   == NULL)
   {
	  fprintf(stderr, "Cannot open output file.\n");
	  return 1;
   }

	if ((outdata = fopen(label, "wb"))
		   == NULL)
		{
		  fprintf(stderr, "Cannot open output file.\n");
		  return 1;
		}


       //	for (a=0;a<9;a++)
       //	{
	 //	animobjects[a].max=0;
	   //	for(b=0;b<9;b++)
	    //	{
	     //	  if (animobjects[a].fshp[b]->flag==1)
	      //		{
	       //		printf("");
		//	animobjects[a].max+=1;
		     //	printf("%d",animobjects[a].max);
	       //		}
	       //	}

//`	}

		fwrite(animobjects,sizeof(animobjects),1,outdata);


	for (numrow=0;numrow<10;numrow++)
	{
		for (numobj=0;numobj<=animobjects[numrow].max;numobj++)
		{
			if (animobjects[numrow].fshp[numobj]->flag==1)
			{
			  fwrite(animobjects[numrow].fshp[numobj],272l,1,out);
			  fwrite(animobjects[numrow].fshp[numobj]->shp,256,1,std);
			}
		}
	}

   fclose(out);
   fclose(outdata);
   fclose(std);
   return 1;

}

int fileread(void)
{

	int i,j,numobj,numrow;
	char far *p;
	char label[20];
	char infst[20];

	gotoxy (1,23);
	printf("Please enter Filename: ");
	fflush(stdin);  /* flush the input stream in case of bad input */

	scanf("%8s",label);

	strcpy(infst, label);

	strcat(label,".dat");
	strcat(infst,".inf");


	if ((in = fopen(infst, "rb"))
	   == NULL)
	{
	  fprintf(stderr, "Cannot open input file.\n");
	  return 1;
	}

	if ((indata = fopen(label, "rb"))
		   == NULL)
		{
		  fprintf(stderr, "Cannot open output file.\n");
		  return 1;
		}

		fread(animobjects,sizeof(animobjects),1,indata);

	p = st1;

	for (i=0;i<TOTALANIMS;i++)
	{
		for (j=0;j<TOTALSHAPE;j++)

			{
			animobjects[i].fshp[j]=(struct fshape *)p;
			p+=sizeof(struct fshape);

			}
	}


	for (numrow=0;numrow<10;numrow++)
	{
//		printf ("%d\n",animobjects[numrow].max);
		for (numobj=0;numobj<=animobjects[numrow].max;numobj++)
		{
//		printf ("%d\n",animobjects[numrow].max);
		fread(animobjects[numrow].fshp[numobj],272l,1,in);
		}
	}
//	getch();
//        getch();






   fclose(in);
   fclose(indata);


   return 1;



}

