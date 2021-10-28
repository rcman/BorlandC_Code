
#include "main.h"
#include "anim.h"

extern struct animshape animobjects[TOTALANIMS];

extern int offset;
extern int x,y,val,rw,sp,j;
extern char far *screen;
extern char far *savearea;
extern int memflag;


void CutObject(int rw, int sp)
{
	int i=0;
	int tempx,tempy;

	for (i=0;i<TOTALOBJECTS;i++)
	{
	    tempx = animobjects[rw].animx;
	    tempy = animobjects[rw].animy * 320 ;
	    offset = tempx + tempy;

	    for (x=0;x<16;x++)
	    {
		for (y=0;y<16;y++)
		{
	       //	savedarea[i].backgnd[i*16+j]=*(screen+offset+y);
		}
		offset = offset + 320;      //bytes to next line
	    }

	offset=0;
	}
}

void PutObject(void)
{
	// put objects on screen
	// save object to oldobject coords
	// add speed to object
	// add animation to next frame
	// check if object is at end of anim - if yes set to 0
	int i;

	for (i=0;i<TOTALOBJECTS;i++)
	{
	    for (x=0;x<16;x++)
	    {
		for (y=0;y<16;y++)
		{
		  *(screen+y+offset+val)=animobjects[rw].fshp[sp]->shp[i*16+j];
		}
		offset = offset + 320;      //bytes to next line
	    }
	offset=0;
	}
}

void RestoreObject(void)
{
	int i=0;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
//			 *(screen+y+offset+val)=*(savearea+i);
			i++;
                }
		offset = offset + 320;      //bytes to next line
        }
        offset=0;
}


void CutImage(void)
{
	int i=0;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			*(savearea+i)=*(screen+y+offset+val);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}

	offset=0;
}

void PutImage(void)
{

	for (x=0;x<4;x++)
	{
		for (y=0;y<4;y++)
		{
		  *(screen+y+offset+val)=j;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;

}

void RestoreBack(void)
{
	int i=0;

	for (x=0;x<16;x++)
	{
		for (y=0;y<16;y++)
		{
			 *(screen+y+offset+val)=*(savearea+i);
			i++;
		}
		offset = offset + 320;      //bytes to next line
	}
	offset=0;
}


void nextshape(int rw , int sp)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
			animobjects[rw].fshp[sp]->shp[i*16+j]=*(screen+j+36430+vz);

		}
		vz = vz + 320;
	    }

}

void placeshape(int rw, int sp)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=animobjects[rw].fshp[sp]->shp[i*16+j];

		}
		vz = vz + 320;

	    }


}

void Draw_Data_to_Grid(int rw , int sp)

{

	int blkclr,gx,gy,cx,cy,incr=0,dninc=0;
        int st;
	st = 65 * 320+51;
	dninc = st;

	for (gx=0;gx<16;gx++)
	{
	    for (gy=0;gy<16;gy++)
	    {
		for (cy=0;cy<4;cy++)
		{
			for (cx=0;cx<4;cx++)             // 60 Lines high and 60 accross
			{
			*(screen+cx+st+offset)=animobjects[rw].fshp[sp]->shp[gx*16+gy];

			}
			offset = offset + 320;
		}
		st = st +5;
		offset=0;
	    }
	    incr = incr + 5;
	    st = dninc + (incr*320);

	}
}


void SaveRow(void)
{

    int cp,dn,i,v;


    if (memflag==0)
    {
	gotoxy (1,25);
	printf("Source :");
	cp=getch()-'0';



	if (cp < 0 || cp > 9)
		printf("Object out of Row Range");


	{
		for (v=0;v<10;v++)
		{
		      for (i=0;i<=256;i++)
		      {
		      savedarea[v].backgnd[i]=animobjects[cp].fshp[v]->shp[i];
		      }
		}
		memflag=1;

	}
    }
    else
    {
    memflag=0;
    gotoxy (1,25);
	printf("Destin :");
	dn=getch()-'0';
		if (dn <0 || dn >9)
		{
			gotoxy (1,25);
			printf("Object destination out of Range");
		}

		else
			{


			 for (v=0;v<10;v++)
			{
			    for (i=0;i<=256;i++)
			    {
			     animobjects[dn].fshp[v]->shp[i]=savedarea[v].backgnd[i];
			    }
			    animobjects[dn].fshp[v]->flag=1;
			 }
			memflag=0;
			}

     }
}


void clearshape(void)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
	    {
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=0;

		}
		vz = vz + 320;
	    }



}

void UndoLastChange(void)
{

	int i,j,vz;

	vz=0;

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{
			*(screen+j+36430+vz)=undoarea[i*16+j];

		}
		vz = vz + 320;
	    }




}



void Animate(void)
{

    int cp,dn,i,spp;
    long int t;

    gotoxy (1,25);
    printf("Start :      ");
    cp=getch()-'0';

    if (cp < 0 || cp > 9)
	printf("Object out of Row Range");
    else
    {
	gotoxy (1,25);
	printf("Destin :          ");
	dn=getch()-'0';

	if (dn <0 || dn >9)
	{
		gotoxy (1,25);
		printf("Object destination out of Range");
	}
	else
	{
		gotoxy (1,25);
		printf("Speed 1 -8:");
		spp=getch()-'0';

		if (spp <0 || spp >8)
		{
		gotoxy (1,25);
		printf("Wrong Speed    ");
		}

		else
		{
		gotoxy (1,25);
		printf("                   ");

		while(!bioskey(1))
		{
		for (i=cp;i<dn;i++)
		    {
		    placeshape(rw,i);
			delay(50*spp);
		}

		    }
		}
		getch();
	}


    }
    placeshape(rw,sp);
    Draw_Data_to_Grid(rw,sp);


}

void Debug(void)
{


    int cp;

    gotoxy (1,23);
    printf("Debug Max Objects:      ");
    scanf("%d",&cp);

    if (cp < 0 || cp > 10)
	printf("Object out of Row Range");
    else
    {
	animobjects[rw].max=cp;
    }


}


void layout_screen(int rw, int sp)
{
	int ap=0;
	int dn=0;
	int cnt=0;
	int t,s,y;
	long int clr;
	int rc,hc=0,lc=0;
	int bg=1;
	int cp,i,spp;
	int bn=0;
	int sk;
	int c,d;
	int ext;
	int saveblk;
	int saverw;

	offset=160*320;
	BigBox(rw,sp);

	for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

	DrawRow(rw);


    ap=0;
    dn=0;
    offset=0;
    ga=0;
    gb=0;
    gc=190;
    gd=250;
    sval=16;	// set step value to 1
    saveblk=sp;
    saverw=rw;
    gotoxy (20,23);
    printf("Right Mouse to Exit");

    draw_plus_minus();

    while (button!=2)
    {
	  //  gotoxy (1,24);
	  //  printf("%d  %d  %d ",xm,ym,sp);


		Get_Mouse(0);
		Calc_Coords();


		layout=1;

		Check_Draw();


		Check_Select();





     }
	gotoxy (1,25);
	printf("Press Any Key to Return");
	getch();

		for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

	placeshape(rw,sp);
	Draw_Data_to_Grid(rw,sp);
	PutColor();
	DrawGrid();
	gotoxy (29,25);
	printf("F1 - Help ");

	layout=0;
	ga=47;
	gb=61;
	gc=144;
	gd=126;
	sval=5;
	sp=saveblk;	    //	Reset
	rw=saverw=rw;


}

void flipshape(void)
{

	int i,j,vz,dn;

	vz=0;

	gotoxy (1,25);
	printf("Flip (x or y)`:");

	dn=getch();

	if (dn == 'Y' || dn == 'y')
	{

	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=animobjects[rw].fshp[sp]->shp[(15-i)*16+j];

		}
		vz = vz + 320;

	    }
	}

	if (dn == 'X' || dn == 'x')
	{
	for (i=0;i<16;i++)
		{
		for (j=0;j<16;j++)
		{

			*(screen+j+36430+vz)=animobjects[rw].fshp[sp]->shp[i*16+15-j];

		}
		vz = vz + 320;

	    }
	}



}


void draw_screen(int rw, int sp)
{
	int ap=0;
	int dn=0;
	int cnt=0;
	int t,s,y;
	long int clr;

	for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

       //       printf("%d %d",rw,sp);

	for (t=0;t<9;t++)
	{
		for (s=0;s<=9;s++)
		{
			if (animobjects[t].fshp[s]->flag==1)
			{
				for (x=0;x<16;x++)
				{
					for (y=0;y<16;y++)
					{
					*(screen+y+offset+ap+dn)=animobjects[t].fshp[s]->shp[x*16+y];
					}
					offset = offset + 320;      //bytes to next line
				}

				offset = 0;
				ap = ap +18;
				//getch();
			}
		}
		cnt = cnt + 18;
		dn = cnt * 320;
		offset=0;
		ap=0;


	}
	gotoxy (1,25);
	printf("Press Any Key to Return");
	getch();
		for (clr=0;clr<64000;clr++)
		{
		*(screen+clr)=0;
		}

	placeshape(rw,sp);
	Draw_Data_to_Grid(rw,sp);
	PutColor();
	DrawGrid();
	gotoxy (29,25);
	printf("F1 - Help ");


}


void cleargrid(void)
{

	int blkclr,gx,gy,cx,cy,incr=0,dninc=0;


	st = 65 * 320+51;

	dninc = st;

	offset=0;

	for (gx=0;gx<16;gx++)
	{
	    for (gy=0;gy<16;gy++)
	    {
		for (cy=0;cy<4;cy++)
		{
			for (cx=0;cx<4;cx++)             // 60 Lines high and 60 accross
			{
			*(screen+cx+st+offset)=0;
			}
			offset = offset + 320;
		}
		st = st +5;
		offset=0;
	    }
	    incr = incr + 5;
	    st = dninc + (incr*320);

	}




}

