/*
  Lode Runner
  por Cassiano Lustosa Froes
  em janeiro/94
*/

#include "graphics.h"
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>

#define tijolo 1
#define tijolofalso 3
#define concreto 2
#define escada 4
#define bolinha 5
#define plr 7
#define ini 8
#define escadapassar 6
#define cima 10
#define baixo 11
#define esq 12
#define dir 13
#define furaesq 14
#define furadir 15
#define queda 16

/* variaveis globais */
int numfase=1,
    numbolinhas=0,
    numinimigos=0;
long int t_ant;
long int pontos=0,
	 pontosvida=10000;

short int pausa=30;
int vidas=5;
char tabfase[32][23],
	tabfaseclone[32][23];
struct buraco
	{
	   int posx,posy,tempo;
	   struct buraco *proximo;
	}  *ptrburaco=NULL,
	   *ptrprimeiro=NULL,
	   *ptrultimo=NULL;
struct jogador
	{
	   char posx,posy,movimento,
		posxant,posyant,
		img,
		elementoant;
	} plr1;

struct jogador inimigo[4];

void *pontimg[5];
int  tamimg;
FILE *arquivo;


void inicializa_grafico()
{
	int dr,mo;
	detectgraph(&dr,&mo);
	initgraph(&dr,&mo,"");
}

void desenhajogador(horiz,vert,indice)
int horiz,vert,indice;
{
	switch(indice)
	{
	case 2:
	line(horiz+9,vert,horiz+9,vert+12); /*corpo*/
	line(horiz+10,vert,horiz+10,vert+12); /*corpo*/

	line(horiz+8,vert+1,horiz+8,vert+3);    /*cabe‡a*/
	line(horiz+11,vert+1,horiz+11,vert+3);

	line(horiz+11,vert+12,horiz+13,vert+12); /*perna dir*/
	line(horiz+11,vert+13,horiz+13,vert+13);
	line(horiz+13,vert+14,horiz+13,vert+17);
	line(horiz+14,vert+14,horiz+14,vert+17);

	line(horiz+8,vert+12,horiz+6,vert+19); /*perna esq*/
	line(horiz+8,vert+13,horiz+7,vert+19); /*perna esq*/

	line(horiz+8,vert+6,horiz+4,vert+9); /*braco esq*/
	line(horiz+9,vert+6,horiz+5,vert+9); /*braco esq*/

	line(horiz+10,vert+6,horiz+12,vert+9);/*braco dir*/
	line(horiz+11,vert+6,horiz+13,vert+10);/*braco dir*/
	line(horiz+12,vert+9,horiz+15,vert+9);
	line(horiz+13,vert+10,horiz+15,vert+10);
	break;

	case 1:
	line(horiz+9,vert,horiz+9,vert+12); /*corpo*/
	line(horiz+10,vert,horiz+10,vert+12); /*corpo*/
	line(horiz+8,vert+1,horiz+8,vert+2);
	line(horiz+11,vert+1,horiz+11,vert+2);
	line(horiz+11,vert+12,horiz+11,vert+19); /*perna dir*/
	line(horiz+12,vert+13,horiz+12,vert+19);

	line(horiz+8,vert+12,horiz+8,vert+19); /*perna esq*/
	line(horiz+9,vert+13,horiz+9,vert+19); /*perna esq*/
	line(horiz+8,vert+6,horiz+6,vert+11); /*braco esq*/
	line(horiz+9,vert+6,horiz+7,vert+11); /*braco esq*/
	line(horiz+10,vert+6,horiz+13,vert+11);/*braco dir*/
	line(horiz+11,vert+6,horiz+14,vert+11);/*braco dir*/
	break;

	case 3:
        line(horiz+9,vert,horiz+9,vert+12); /*corpo*/
	line(horiz+10,vert,horiz+10,vert+12); /*corpo*/
	line(horiz+8,vert+1,horiz+8,vert+2);
	line(horiz+11,vert+1,horiz+11,vert+2);

	line(horiz+8,vert+13,horiz+6,vert+14); /*perna esq*/
	line(horiz+9,vert+13,horiz+6,vert+15);
	line(horiz+6,vert+14,horiz+4,vert+14);
	line(horiz+6,vert+15,horiz+5,vert+15);

	line(horiz+11,vert+12,horiz+14,vert+19); /*perna dir*/
	line(horiz+12,vert+13,horiz+15,vert+19);

	line(horiz+8,vert+6,horiz+5,vert+10); /*braco esq*/
	line(horiz+9,vert+6,horiz+6,vert+10); /*braco esq*/
	line(horiz+10,vert+6,horiz+13,vert+11);/*braco dir*/
	line(horiz+11,vert+6,horiz+14,vert+12);/*braco dir*/
	break;
	}
}
void inicializa_sistema()
{
	int i;
	tamimg=imagesize(0,0,19,19);               /**********/
	for(i=0;i<=4;pontimg[i]=malloc(tamimg),i++);/*  //    */
	randomize();
}  /* inicializa sistema */

void desenha(horiz,vert,elemento)
int horiz,vert,elemento;
{
	setfillstyle(SOLID_FILL,BLACK);
	bar(horiz*20,vert*20,horiz*20+19,vert*20+19);
	if (elemento==tijolofalso)elemento=tijolo;
	switch(elemento)
	{
	   case tijolo:
		setfillstyle(SOLID_FILL,BROWN);
		bar(horiz*20,vert*20,horiz*20+19,vert*20+19);
		setcolor(LIGHTRED);
		line(horiz*20,vert*20,horiz*20+19,vert*20);
		line(horiz*20,vert*20,horiz*20,vert*20+9);
		line(horiz*20,vert*20+10,horiz*20+9,vert*20+10);
		line(horiz*20+11,vert*20+10,horiz*20+19,vert*20+10);
		line(horiz*20+11,vert*20+10,horiz*20+11,vert*20+19);
		setcolor(RED);
		line(horiz*20,vert*20+9,horiz*20+19,vert*20+9);
		line(horiz*20+19,vert*20+9,horiz*20+19,vert*20);
		line(horiz*20,vert*20+19,horiz*20+19,vert*20+19);
		line(horiz*20+10,vert*20+19,horiz*20+10,vert*20+9);
		break;
	   case concreto:
		setfillstyle(SOLID_FILL,LIGHTGRAY);
		bar(horiz*20,vert*20,horiz*20+19,vert*20+19);
		setcolor(WHITE);
		line(horiz*20,vert*20,horiz*20+19,vert*20);
		line(horiz*20,vert*20,horiz*20,vert*20+19);
		setcolor(DARKGRAY);
		line(horiz*20+19,vert*20+19,horiz*20,vert*20+19);
		line(horiz*20+19,vert*20+19,horiz*20+19,vert*20);
		break;
	   case escada:
		setcolor(YELLOW);
		line(horiz*20+4,vert*20,horiz*20+4,vert*20+19);
		line(horiz*20+5,vert*20,horiz*20+5,vert*20+19);
		line(horiz*20+14,vert*20,horiz*20+14,vert*20+19);
		line(horiz*20+13,vert*20,horiz*20+13,vert*20+19);
		line(horiz*20+2,vert*20,horiz*20+16,vert*20);
		line(horiz*20+2,vert*20+10,horiz*20+16,vert*20+10);
		break;
	   case bolinha:
		setcolor(GREEN);
		setfillstyle(SOLID_FILL,LIGHTGREEN);
		fillellipse(horiz*20+10,vert*20+12,4,3);
		break;

	}

}   /* desenha */

void libera_memoria()

{
	struct buraco *ptraux;
	ptraux=ptrprimeiro;
	if (ptrprimeiro!=NULL)
	do
	{
		ptraux=ptrprimeiro;
		ptrprimeiro=(*ptrprimeiro).proximo;
		free(ptraux);
	}
	while(ptrprimeiro!=NULL);
}     /* libera mem¢ria */

void desenhafase()
{
	int i,j;
	libera_memoria();
	numinimigos=0;numbolinhas=0;
	for(i=0;i<=31;i++)
		for(j=0;j<=22;j++)
			tabfase[i][j]=tabfaseclone[i][j];
	for(j=22;j>=0;j--)
	{
	 for(i=0;i<=31;i++)
	 {
	 desenha(i,j,tabfase[i][j]);
	 if (tabfase[i][j]==bolinha)numbolinhas++;
	 if (tabfase[i][j]==plr){
				plr1.posx=i;
				plr1.posy=j;
				plr1.movimento=0;
				tabfase[i][j]=0;
				}
	 if (tabfase[i][j]==ini){
				inimigo[numinimigos].posx=i;
				inimigo[numinimigos].posy=j;
				inimigo[numinimigos].posxant=i;
				inimigo[numinimigos].posyant=j;
				inimigo[numinimigos].elementoant=0;
				inimigo[numinimigos].movimento=0;
				numinimigos++;
				tabfase[i][j]=0;
				}

	 }
	}
	if (numinimigos>0)numinimigos--;

	/* Salva Imagens sob bonecos */

	for(i=0;i<=numinimigos;getimage(inimigo[i].posx*20,inimigo[i].posy*20,
			inimigo[i].posx*20+19,inimigo[i].posy*20+19,
			pontimg[i]),i++);
	getimage(plr1.posx*20,plr1.posy*20,
		 plr1.posx*20+19,plr1.posy*20+19,pontimg[4]);

	/* Desenha */

	setcolor(WHITE);
	desenhajogador(plr1.posx*20,plr1.posy*20,1);
	setcolor(LIGHTBLUE);
	for(i=0;i<=numinimigos;desenhajogador(inimigo[i].posx*20,
				inimigo[i].posy*20,1),i++);

	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	setfillstyle(SOLID_FILL,BLACK);

        setcolor(WHITE);
	for(i=1;i<=vidas;desenhajogador(i*15-10,460,1),i++);
	setcolor(BLUE);
	rectangle(0,460,639,479);
	setcolor(LIGHTGRAY);
	outtextxy(201,463,"pontos                                  fase ");

	setcolor(LIGHTCYAN);
	outtextxy(200,462,"pontos                                  fase ");

	while(kbhit())getch();
	while(!kbhit()){};

}   /* fim desenhafase */

void soma_vidas(valor)
short int valor;
{
	int i,j;
	vidas+=valor;
        if (valor<0)
		{
		for(i=100;i<=500;i+=15)
		{
		sound(i);
		delay(10);
		sound(200);
		delay(10);nosound();
		sound(500-i);
		delay(5);
		}
		nosound();

		do {}while(!kbhit());
		for(j=0;j<=22;j++)
		 for(i=0;i<=31;tabfase[i][j]=tabfaseclone[i][j],i++);
		desenhafase();
		}
	setfillstyle(SOLID_FILL,BLACK);
	bar(1,461,198,478);
	setcolor(BLUE);
	rectangle(0,460,639,479);
	setcolor(WHITE);
	for(i=1;i<=vidas;desenhajogador(i*15-10,460,1),i++);
} /* soma_vidas */

void soma_pontos(valor)
int valor;
{
	int i;
	char *srg[10];
	*srg="";

	setfillstyle(SOLID_FILL,BLACK);
	bar(264,462,420,478);
	pontos+=valor;

	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	srg[0]=(ltoa(pontos,*srg,10));
	setcolor(LIGHTGRAY);
	outtextxy(266,463,*srg);
	setcolor(LIGHTCYAN);
	outtextxy(265,462,*srg);
	if (pontos>=pontosvida)
			{
			pontosvida+=10000;
			for(i=100;i<=300;sound(i),
					delay(10),
					sound(600),
					delay(10),i+=50);
			nosound();
			soma_vidas(1);
			}

}  /* soma pontos */



/*
   functions para uso da lista encadeada
   dos buracos
*/
void inclui(x,y)
int x,y;
{
	ptrburaco=(struct buraco*)malloc(sizeof(struct buraco));
	(*ptrburaco).posx=x;
	(*ptrburaco).posy=y;
	(*ptrburaco).tempo=0;
	if (ptrprimeiro==NULL)
	   {
	       (*ptrburaco).proximo=NULL;
	       ptrprimeiro=ptrburaco;
	       ptrultimo=ptrburaco;
	   }
	else
	   {
	      (*ptrultimo).proximo=ptrburaco;
	      ptrultimo=ptrburaco;
	      (*ptrburaco).proximo=NULL;
	   }
}

void passou_fase()
{
	int i,j;
	for(i=0;i<=31;i++)
		for(j=0;j<=22;j++)
			if (tabfase[i][j]==escadapassar)
				{
				tabfase[i][j]=escada;
				desenha(i,j,escada);
				sound(i*j);
				delay(20);
				nosound();
				}

       soma_pontos(50);
}

void le_fase()
{
	int i,j;
	char *srg[10];
	for(i=100;i<=1000;i+=100)
		{
		soma_pontos(10);
		sound(i);delay(50);nosound();
		}

	*srg="";

	setfillstyle(SOLID_FILL,BLACK);
	bar(564,462,638,478);
	numfase++;
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	srg[0]=(itoa(numfase,*srg,10));
	setcolor(LIGHTGRAY);
	outtextxy(566,463,*srg);
	setcolor(LIGHTCYAN);
	outtextxy(565,462,*srg);

	for(i=0;i<=31;i++)
		for(j=0;j<=22;j++)
			tabfase[i][j]=tabfaseclone[i][j]=getc(arquivo);
	desenhafase();

}
void come_bolinha()
{
	int i;
	for(i=500;i>=100;sound(i),
			delay(20),i-=100);
	nosound();
	tabfase[plr1.posx][plr1.posy]=0;
	setfillstyle(SOLID_FILL,BLACK);
	bar(plr1.posx*20,plr1.posy*20,plr1.posx*20+19,plr1.posy*20+19);
	getimage(plr1.posx*20,plr1.posy*20,
		 plr1.posx*20+19,plr1.posy*20+19,pontimg[4]);
	setcolor(WHITE);
	numbolinhas--;
	desenhajogador(plr1.posx*20,plr1.posy*20,2);
	soma_pontos(30);
	if (numbolinhas==0)passou_fase();
}

void fura(posicao)
int posicao;
{
	int i,j;
	if (posicao==esq)inclui(plr1.posx-1,plr1.posy+1);
		else inclui(plr1.posx+1,plr1.posy+1);
	for (i=10;i<=250;i+=50)
		{   sound(100);
		    setcolor(random(7)+7);
		    switch(posicao)
		    {
			case esq:
				 line((plr1.posx)*20,plr1.posy*20+12,
				      (plr1.posx-1)*20+10,plr1.posy*20+19);
				 line((plr1.posx)*20+1,plr1.posy*20+12,
				      (plr1.posx-1)*20+11,plr1.posy*20+19);
				 for(j=1;j<20;j++,
				 putpixel(random(12)+(plr1.posx-1)*20,
					random(12)+(plr1.posy+1)*20,BLACK));
				 break;
			case dir:line((plr1.posx+1)*20,plr1.posy*20+12,
				      (plr1.posx+1)*20+10,plr1.posy*20+19);
				 line((plr1.posx+1)*20-1,plr1.posy*20+12,
				      (plr1.posx+1)*20+9,plr1.posy*20+19);
				 for(j=1;j<20;j++,
				 putpixel(random(12)+(plr1.posx+1)*20,
					random(12)+(plr1.posy+1)*20,BLACK));
				 break;
		    }
		    delay(30);

		    setcolor(BLACK);
		    switch(posicao)
		    {
			case esq:
				 /*line((plr1.posx)*20,plr1.posy*20+12,
				      (plr1.posx-1)*20+10,plr1.posy*20+19);*/
				 for(j=1;j<20;j++,
				 putpixel(random(12)+(plr1.posx-1)*20,
					random(12)+(plr1.posy+1)*20,BLACK));
				 break;
			case dir:/*line((plr1.posx+1)*20,plr1.posy*20+12,
				      (plr1.posx+1)*20+10,plr1.posy*20+19);*/
				 for(j=1;j<20;j++,
				 putpixel(random(12)+(plr1.posx+1)*20,
					random(12)+(plr1.posy+1)*20,BLACK));
				 break;
		    }
		    sound(500);
		    delay(10);
		}
	if (posicao==esq)
	{
		line((plr1.posx)*20,plr1.posy*20+12,
				      (plr1.posx-1)*20+10,plr1.posy*20+19);
		line((plr1.posx)*20+1,plr1.posy*20+12,
				      (plr1.posx-1)*20+10+1,plr1.posy*20+19);
	}
	else
	{	line((plr1.posx+1)*20,plr1.posy*20+12,
				      (plr1.posx+1)*20+10,plr1.posy*20+19);
		line((plr1.posx+1)*20-1,plr1.posy*20+12,
				      (plr1.posx+1)*20+9,plr1.posy*20+19);
	}
	nosound();
}



/* verifica se h  buracos abertos */
void buracos_abertos()
{
	int n;
	struct buraco *ptraux;
	ptraux=ptrprimeiro;
	(*ptraux).tempo++;
	if ((*ptraux).proximo!=NULL)
	do
	{
		ptraux=(*ptraux).proximo;
		(*ptraux).tempo++;
	}
	while((*ptraux).proximo!=NULL);

		if ((*ptrprimeiro).tempo>=50)
			{
			desenha((*ptrprimeiro).posx,(*ptrprimeiro).posy,tijolo);
			tabfase[(*ptrprimeiro).posx][(*ptrprimeiro).posy]=tijolo;
			ptraux=ptrprimeiro;
			ptrprimeiro=(*ptrprimeiro).proximo;
			if (ptrprimeiro!=NULL)
				(*ptrprimeiro).tempo-=20;
			for(n=0;n<=numinimigos;n++)
				{
				if (((*ptraux).posx==inimigo[n].posx)&&
				   ((*ptraux).posy==inimigo[n].posy))
				   {
				   soma_pontos(15);
				   inimigo[n].posx=inimigo[n].posxant=29-n*2;
				   inimigo[n].posy=inimigo[n].posyant=0;
				   getimage(inimigo[n].posx*20,
					inimigo[n].posy*20,
					inimigo[n].posx*20+19,
					inimigo[n].posy*20+19,
					pontimg[n]);
		/****/		   inimigo[n].elementoant=tabfase
		/****/		       [inimigo[n].posx][inimigo[n].posy];
				   }
				}
			if ( ((*ptraux).posx==plr1.posx)&&
			     ((*ptraux).posy==plr1.posy))soma_vidas(-1);
			free(ptraux);
			}

}
void verifica_direcao()
{
	if (!(tabfase[plr1.posx][plr1.posy]!=bolinha))
		come_bolinha();
	if ((numbolinhas==0)&&(plr1.posy==0))le_fase();
	if(    ((tabfase[plr1.posx][plr1.posy+1]==0)||
		(tabfase[plr1.posx][plr1.posy+1]==escadapassar)||
		(tabfase[plr1.posx][plr1.posy+1]==tijolofalso)||
		(tabfase[plr1.posx][plr1.posy+1]==bolinha))&&
		(tabfase[plr1.posx][plr1.posy]!=escada)
	  ) plr1.movimento=queda;   /* queda */
	switch(plr1.movimento)
	{
	  case cima:
		if(
		  (tabfase[plr1.posx][plr1.posy]!=escada)||
		  (tabfase[plr1.posx][plr1.posy-1]==tijolo)||
		  (tabfase[plr1.posx][plr1.posy-1]==concreto)||
		  (tabfase[plr1.posx][plr1.posy-1]==tijolofalso)||
		  (plr1.posy==0)
		  ) plr1.movimento=0;
		break;
	  case baixo:
		if(
		  (tabfase[plr1.posx][plr1.posy+1]!=escada)||
		  (plr1.posy>21)
		  ) plr1.movimento=0;
		if ((tabfase[plr1.posx][plr1.posy+1]==bolinha)||
		    (tabfase[plr1.posx][plr1.posy+1]==0)||
		    (tabfase[plr1.posx][plr1.posy+1]==tijolofalso)
		   )plr1.movimento=queda;

		break;
	 case esq:
		if(!(
		((tabfase[plr1.posx-1][plr1.posy])!=(tijolo))&&
		(tabfase[plr1.posx-1][plr1.posy]!=concreto)&&
		(tabfase[plr1.posx-1][plr1.posy]!=tijolofalso)&&
		(plr1.posx>0)
		  )) plr1.movimento=0;

		/*if(!(tabfase[plr1.posx][plr1.posy+1]!=0)*/
		 /* ||  (tabfase[plr1.posx-1][plr1.posy+1]=tijolofalso)*/
		/*  ) plr1.movimento=queda;*/ /* queda */
		break;
	case dir:
		if(!(
		(tabfase[plr1.posx+1][plr1.posy]!=tijolo)&&
		(tabfase[plr1.posx+1][plr1.posy]!=concreto)&&
		  (tabfase[plr1.posx+1][plr1.posy]!=tijolofalso)&&
		  (plr1.posx+1<32)
		  )) plr1.movimento=0;
		/*if(!(tabfase[plr1.posx][plr1.posy+1]!=0)*/
		 /*  (tabfase[plr1.posx+1][plr1.posy+1]=tijolofalso)*/
		 /* ) plr1.movimento=queda; */ /* queda */
		break;
	case furaesq:
		if (  ((tabfase[plr1.posx-1][plr1.posy]!=0)&&
		       (tabfase[plr1.posx-1][plr1.posy]!=escadapassar))||
		      (tabfase[plr1.posx-1][plr1.posy+1]!=tijolo)
		   ) plr1.movimento=0;
		break;
	case furadir:
		if (  ((tabfase[plr1.posx+1][plr1.posy]!=0)&&
		      (tabfase[plr1.posx+1][plr1.posy]!=escadapassar))||
		      (tabfase[plr1.posx+1][plr1.posy+1]!=tijolo)
		   ) plr1.movimento=0;
		break;
	case queda:
		if (
		   ((tabfase[plr1.posx][plr1.posy+1]==tijolo))||
		   ((tabfase[plr1.posx][plr1.posy+1]==concreto))||
		   ((tabfase[plr1.posx][plr1.posy+1]==escada))||
		   (plr1.posy>21)
		   )plr1.movimento=0;
		break;


	 } /* fim do SWITCH   */

}    /* verifica direcao */

int sorteia_direcao(i)
int i;
{
	int randomico;

	if((inimigo[i].posx<31)&&(inimigo[i].posx>0))
	{
	if(((tabfase[inimigo[i].posx+1][inimigo[i].posy]==tijolo)||
	   (tabfase[inimigo[i].posx+1][inimigo[i].posy]==concreto)||
	   (tabfase[inimigo[i].posx+1][inimigo[i].posy]==tijolofalso)
	   )&&
	   ((tabfase[inimigo[i].posx-1][inimigo[i].posy]==tijolo)||
	   (tabfase[inimigo[i].posx-1][inimigo[i].posy]==concreto)||
	   (tabfase[inimigo[i].posx-1][inimigo[i].posy]==tijolofalso)
	   ))return 0;
	else
	if((tabfase[inimigo[i].posx+1][inimigo[i].posy]==tijolo)||
	   (tabfase[inimigo[i].posx+1][inimigo[i].posy]==concreto)||
	   (tabfase[inimigo[i].posx+1][inimigo[i].posy]==tijolofalso)||
	   (inimigo[i].posx==31)
	   )return esq;
	else
	if((tabfase[inimigo[i].posx-1][inimigo[i].posy]==tijolo)||
	   (tabfase[inimigo[i].posx-1][inimigo[i].posy]==concreto)||
	   (tabfase[inimigo[i].posx-1][inimigo[i].posy]==tijolofalso)||
	   (inimigo[i].posx==0)
	   )return dir;

	else
	{

	randomico=random(5);
	if (randomico>3)                       /*********************/
		{                             /* verificar se nao ha */
		 if (randomico>4)return esq;  /* obstaculo na direcao */
		 else return dir;             /* escolhida !!! 	      */
		}			      /*     /               */
	else                                  /*    <=====           */
					      /*     \               */
		if (plr1.posx>inimigo[i].posx)return dir;
		else return esq;
	}
	}
	else {
		if(inimigo[i].posx>=31)
		{
		if((tabfase[inimigo[i].posx-1][inimigo[i].posy]==tijolo)||
		   (tabfase[inimigo[i].posx-1][inimigo[i].posy]==tijolofalso)||
		   (tabfase[inimigo[i].posx-1][inimigo[i].posy]==concreto)
		   )return 0;
		   else return esq;
		}
		else
		{
		if((tabfase[inimigo[i].posx+1][inimigo[i].posy]==tijolo)||
		   (tabfase[inimigo[i].posx+1][inimigo[i].posy]==tijolofalso)||
		   (tabfase[inimigo[i].posx+1][inimigo[i].posy]==concreto)
		   )return 0;
		   else return dir;
		}

	     }
}

void verifica_direcao_inimigo(i)
int i;
{

	if ((inimigo[i].posx==plr1.posx)&&
		    (inimigo[i].posy==plr1.posy))soma_vidas(-1);
	if(    ((tabfase[inimigo[i].posx][inimigo[i].posy+1]==0)||
		(tabfase[inimigo[i].posx][inimigo[i].posy+1]==escadapassar)||
		(tabfase[inimigo[i].posx][inimigo[i].posy+1]==tijolofalso)||
		(tabfase[inimigo[i].posx][inimigo[i].posy+1]==bolinha))&&
		(tabfase[inimigo[i].posx][inimigo[i].posy]!=escada)
	  ) inimigo[i].movimento=queda; /* queda */

	switch(inimigo[i].movimento)
	{
	case 0:
		if (plr1.posy>inimigo[i].posy)
		   {
		   if(tabfase[inimigo[i].posx][inimigo[i].posy+1]==escada)
				inimigo[i].movimento=baixo;
		   else inimigo[i].movimento=sorteia_direcao(i);
		   }
		else if(plr1.posy<inimigo[i].posy)
		   {
		   if(
		      /*(tabfase[inimigo[i].posx][inimigo[i].posy]==escada)*/

		      (inimigo[i].elementoant==escada)&&

		      (tabfase[inimigo[i].posx][inimigo[i].posy-1]!=tijolo)&&
		      (tabfase[inimigo[i].posx][inimigo[i].posy-1]!=tijolofalso)&&
		      (tabfase[inimigo[i].posx][inimigo[i].posy-1]!=concreto)
		      )	inimigo[i].movimento=cima;
		   else inimigo[i].movimento=sorteia_direcao(i);
		   }
		else inimigo[i].movimento=sorteia_direcao(i);
		break;
	case dir:
		if (
		   /*(tabfase[inimigo[i].posx][inimigo[i].posy]==escada)*/

		   (inimigo[i].elementoant==escada)||

		   (tabfase[inimigo[i].posx][inimigo[i].posy+1]==escada)||
		   (tabfase[inimigo[i].posx+1][inimigo[i].posy]==tijolo)||
		   (tabfase[inimigo[i].posx+1][inimigo[i].posy]==concreto)||
		   (tabfase[inimigo[i].posx+1][inimigo[i].posy]==tijolofalso)||
		   (inimigo[i].posx>=31)
		   )inimigo[i].movimento=0;

	/*	if (!((tabfase[inimigo[i].posx][inimigo[i].posy]==0)||
		    (tabfase[inimigo[i].posx][inimigo[i].posy]==bolinha))
		    )inimigo[i].movimento=0;
	*/
		break;
	case esq:
        	if (
		   /*(tabfase[inimigo[i].posx][inimigo[i].posy]==escada)*/
		   (inimigo[i].elementoant==escada)||

		   (tabfase[inimigo[i].posx][inimigo[i].posy+1]==escada)||
		   (tabfase[inimigo[i].posx-1][inimigo[i].posy]==tijolo)||
		   (tabfase[inimigo[i].posx-1][inimigo[i].posy]==concreto)||
		   (tabfase[inimigo[i].posx-1][inimigo[i].posy]==tijolofalso)||
		   (inimigo[i].posx<=0)
		   )inimigo[i].movimento=0;
	/*	if (!((tabfase[inimigo[i].posx][inimigo[i].posy]==0)||
		    (tabfase[inimigo[i].posx][inimigo[i].posy]==bolinha)))
		    inimigo[i].movimento=0;
	*/
		break;
	case baixo:
		if (
		    ((plr1.posy==inimigo[i].posy))||
		    (tabfase[inimigo[i].posx][inimigo[i].posy+1]!=escada))
		    inimigo[i].movimento=0;
		break;
	case cima:
		if ((plr1.posy==inimigo[i].posy)||

		    /*(tabfase[inimigo[i].posx][inimigo[i].posy]!=escada)*/
		    (inimigo[i].elementoant!=escada)||

		    (tabfase[inimigo[i].posx][inimigo[i].posy-1]==tijolo)||
		    (tabfase[inimigo[i].posx][inimigo[i].posy-1]==tijolofalso)||
		    (tabfase[inimigo[i].posx][inimigo[i].posy-1]==concreto)
		    )inimigo[i].movimento=0;
	case queda:
		if (((tabfase[inimigo[i].posx][inimigo[i].posy+1]!=0)&&
		    (tabfase[inimigo[i].posx][inimigo[i].posy+1]!=bolinha)&&
		    (tabfase[inimigo[i].posx][inimigo[i].posy+1]!=escadapassar)&&
		    (tabfase[inimigo[i].posx][inimigo[i].posy+1]!=tijolofalso))||
		    (inimigo[i].posy>=22)
		    )inimigo[i].movimento=0;
		break;
	} /* fim SWITCH */
}  /* verifica direcao inimigo */


/*--------------------------------------------------------*/
/*         faz os movimentos de todos os bonecos          */
/*--------------------------------------------------------*/

void efetua_movimento()
{
	struct time t;
	long int t_atu;
	int img=2,contador,i;
	for(contador=5;contador<=20;contador+=5)
	{
	setcolor(WHITE);
	switch (plr1.movimento)
	{
		case esq:
			putimage(plr1.posx*20-contador+5,plr1.posy*20,
				  pontimg[4],COPY_PUT);
			getimage(plr1.posx*20-contador,plr1.posy*20,
				 plr1.posx*20-contador+19,plr1.posy*20+19,
				 pontimg[4]);
			/*desenha(plr1.posx-1,plr1.posy,tabfase[plr1.posx-1][plr1.posy]);
			desenha(plr1.posx,plr1.posy,tabfase[plr1.posx][plr1.posy]);
			setcolor(WHITE);*/
			desenhajogador(plr1.posx*20-contador,plr1.posy*20,plr1.img);
			((plr1.img==3)?(plr1.img=1):(plr1.img++));
		     if (contador==20)
			{
			 putimage(plr1.posx*20-contador,plr1.posy*20,
				  pontimg[4],COPY_PUT);
			desenha(plr1.posx,plr1.posy,tabfase[plr1.posx][plr1.posy]);
			desenha(plr1.posx-1,plr1.posy,tabfase[plr1.posx-1][plr1.posy]);
			setcolor(WHITE);
			desenhajogador((--plr1.posx)*20,plr1.posy*20,img);
			 }
		     break;
		case dir:

		     putimage(plr1.posx*20+contador-5,plr1.posy*20,pontimg[4],COPY_PUT);
			getimage(plr1.posx*20+contador,plr1.posy*20,
				plr1.posx*20+contador+19,plr1.posy*20+19,
				pontimg[4]);
			/*desenha(plr1.posx+1,plr1.posy,tabfase[plr1.posx+1][plr1.posy]);
			desenha(plr1.posx,plr1.posy,tabfase[plr1.posx][plr1.posy]);
			setcolor(WHITE);*/

			(  (plr1.img==1)?(plr1.img=2):
			   ((plr1.img==2)?(plr1.img=3):(plr1.img=1)));
			desenhajogador(plr1.posx*20+contador,
					plr1.posy*20,plr1.img);

		     if(contador==20)
			{
			putimage(plr1.posx*20+contador,
				plr1.posy*20,pontimg[4],COPY_PUT);
			desenha(plr1.posx+1,plr1.posy,tabfase[plr1.posx+1][plr1.posy]);
			desenha(plr1.posx,plr1.posy,tabfase[plr1.posx][plr1.posy]);
			setcolor(WHITE);
			desenhajogador((++plr1.posx)*20,plr1.posy*20,img);
			}
		     break;
		case cima:
		     putimage(plr1.posx*20,plr1.posy*20-contador+5,
				pontimg[4],COPY_PUT);
			getimage(plr1.posx*20,plr1.posy*20-contador,
				plr1.posx*20+19,plr1.posy*20-contador+19,
				pontimg[4]);

			/*desenha(plr1.posx,plr1.posy-1,tabfase[plr1.posx][plr1.posy-1]);
			desenha(plr1.posx,plr1.posy,tabfase[plr1.posx][plr1.posy]);
			setcolor(WHITE);*/
			desenhajogador(plr1.posx*20,plr1.posy*20-contador,img);
			((img!=1)?(img=1):(img=2));
		     if (contador==20)
				{
				putimage(plr1.posx*20,plr1.posy*20-contador
				,pontimg[4],COPY_PUT);
				desenha(plr1.posx,plr1.posy-1,tabfase[plr1.posx][plr1.posy-1]);
				desenha(plr1.posx,plr1.posy,tabfase[plr1.posx][plr1.posy]);
				setcolor(WHITE);
				desenhajogador((plr1.posx)*20,
				(--plr1.posy)*20,img);
				}
		     break;
		case baixo:
		     putimage(plr1.posx*20,plr1.posy*20+contador-5,
				pontimg[4],COPY_PUT);
			getimage(plr1.posx*20,plr1.posy*20+contador,
				plr1.posx*20+19,plr1.posy*20+contador+19,
				pontimg[4]);
			/*desenha(plr1.posx,plr1.posy+1,tabfase[plr1.posx][plr1.posy+1]);
			desenha(plr1.posx,plr1.posy,tabfase[plr1.posx][plr1.posy]);
			setcolor(WHITE);*/
			desenhajogador(plr1.posx*20,plr1.posy*20+contador,img);
			((img!=1)?(img=1):(img=2));
		     if(contador==20)
			{
			putimage(plr1.posx*20,plr1.posy*20+contador-5,
				pontimg[4],COPY_PUT);
			desenha(plr1.posx,plr1.posy+1,tabfase[plr1.posx][plr1.posy+1]);
			desenha(plr1.posx,plr1.posy,tabfase[plr1.posx][plr1.posy]);
			setcolor(WHITE);
			desenhajogador((plr1.posx)*20,(++plr1.posy)*20,img);
			}

		     break;
/*fura dir   */ case furadir:
/*e fura esq */	     if (contador==5)
/*	     */	     {fura(dir);
/* gatilho !!*/	     setfillstyle(SOLID_FILL,BLACK);
/* acertar ..*/	     bar((plr1.posx+1)*20,(plr1.posy+1)*20,
			 (plr1.posx+1)*20+19,(plr1.posy+1)*20+19);
		     tabfase[plr1.posx+1][plr1.posy+1]=0;
		     }
		     break;
		case furaesq:
		     if (contador==5)
		     {
		     fura(esq);
		     setfillstyle(SOLID_FILL,BLACK);
		     bar((plr1.posx-1)*20,(plr1.posy+1)*20,
			 (plr1.posx-1)*20+19,(plr1.posy+1)*20+19);
		     tabfase[plr1.posx-1][plr1.posy+1]=0;
		     }
		     break;
		case queda:
		     putimage(plr1.posx*20,plr1.posy*20+contador-5,
				pontimg[4],COPY_PUT);
			getimage(plr1.posx*20,plr1.posy*20+contador,
				plr1.posx*20+19,plr1.posy*20+contador+19,
				pontimg[4]);
			/*desenha(plr1.posx,plr1.posy+1,tabfase[plr1.posx][plr1.posy+1]);
			desenha(plr1.posx,plr1.posy,tabfase[plr1.posx][plr1.posy]);
			setcolor(WHITE);*/
			desenhajogador(plr1.posx*20,plr1.posy*20+contador,img);
			sound(600+plr1.posy*10);
			delay(5);nosound();
			delay(5);
		     if(contador==20)
			{
			putimage(plr1.posx*20,plr1.posy*20+contador
					,pontimg[4],COPY_PUT);
			desenha(plr1.posx,plr1.posy+1,tabfase[plr1.posx][plr1.posy+1]);
			desenha(plr1.posx,plr1.posy,tabfase[plr1.posx][plr1.posy]);
			setcolor(WHITE);
			desenhajogador((plr1.posx)*20,(++plr1.posy)*20,img);
			}
		     break;

		case 0:
			break;

	 } /* fim do switch plr1.movimento*/
	 if ((plr1.movimento!=furadir)&&(plr1.movimento!=furaesq))
	 {
	  gettime(&t);
	  t_atu=t.ti_hour*60 + t.ti_min*60 + t.ti_sec*100 + t.ti_hund;
	  if ((pausa-(t_ant-t_atu)>0)&&(t_ant-t_atu<pausa))
		delay(pausa-(t_ant-t_atu));
	  t_ant=t_atu;
	  /* delay (pausa- [intervalo decorrido]) */
	 }


	 setcolor(LIGHTBLUE);

	 for(i=0;i<=numinimigos;i++)
		{	/* apaga todos os inimigos */
		if ((inimigo[i].posx==plr1.posx)&&
		    (inimigo[i].posy==plr1.posy))soma_vidas(-1);
		switch(inimigo[i].movimento)
		{
		case esq:
			putimage(inimigo[i].posx*20-contador+5,
				inimigo[i].posy*20,pontimg[i],COPY_PUT);
			break;
		case dir:
			putimage(inimigo[i].posx*20+contador-5,
				inimigo[i].posy*20,pontimg[i],COPY_PUT);
			break;
		case baixo:
			putimage(inimigo[i].posx*20,
				inimigo[i].posy*20+contador-5,
				pontimg[i],COPY_PUT);
			break;
		case cima:
                	putimage(inimigo[i].posx*20,
				inimigo[i].posy*20-contador+5,
				pontimg[i],COPY_PUT);
			break;
		case queda:
                	putimage(inimigo[i].posx*20,
				inimigo[i].posy*20+contador-5,
				pontimg[i],COPY_PUT);
			break;
		}}    /* apagou os inimigos */

	 for(i=0;i<=numinimigos;i++)
		     /* salva imagem sob inimigos */
	 switch(inimigo[i].movimento)
	 {
	 case esq:
/*adaptacao*/
		getimage(inimigo[i].posx*20-contador,inimigo[i].posy*20,
				 inimigo[i].posx*20-contador+19,
				 inimigo[i].posy*20+19,pontimg[i]);
		break;
	 case dir:
		getimage(inimigo[i].posx*20+contador,inimigo[i].posy*20,
				inimigo[i].posx*20+contador+19,
				inimigo[i].posy*20+19,pontimg[i]);
		break;
	 case cima:
		getimage(inimigo[i].posx*20,inimigo[i].posy*20-contador,
				inimigo[i].posx*20+19,
				inimigo[i].posy*20-contador+19,pontimg[i]);
		break;
	 case baixo:
		getimage(inimigo[i].posx*20,inimigo[i].posy*20+contador,
				inimigo[i].posx*20+19,
				inimigo[i].posy*20+contador+19,pontimg[i]);
		break;
	 case queda:
         	getimage(inimigo[i].posx*20,inimigo[i].posy*20+contador,
				inimigo[i].posx*20+19,
				inimigo[i].posy*20+contador+19,pontimg[i]);
		break;
	 case 0:
		break;

	 }/*fim for..switch(ini.i)*/
	  /* salvou imagens sob inimigos*/

	 for(i=0;i<=numinimigos;i++)
	 {
	 switch(inimigo[i].movimento)
	 {
	case esq:
/*adaptacao*/

		desenhajogador(inimigo[i].posx*20-contador,
				 inimigo[i].posy*20,inimigo[i].img);

		((inimigo[i].img!=1)?(inimigo[i].img=1):
					(inimigo[i].img=2));
		if (contador==20)inimigo[i].posx--;
		break;
	 case dir:
		desenhajogador(inimigo[i].posx*20+contador,
					inimigo[i].posy*20,inimigo[i].img);
		((inimigo[i].img!=1)?(inimigo[i].img=1):
					(inimigo[i].img=2));
		if (contador==20)++inimigo[i].posx;
		break;
	 case cima:
		desenhajogador(inimigo[i].posx*20,
				inimigo[i].posy*20-contador,inimigo[i].img);
                ((inimigo[i].img!=1)?(inimigo[i].img=1):
					(inimigo[i].img=2));
		if (contador==20)--inimigo[i].posy;
		break;
	 case baixo:
		desenhajogador(inimigo[i].posx*20,
				inimigo[i].posy*20+contador,inimigo[i].img);
                ((inimigo[i].img!=1)?(inimigo[i].img=1):
					(inimigo[i].img=2));
		if (contador==20)++inimigo[i].posy;
		break;
	 case queda:
         	desenhajogador(inimigo[i].posx*20,
				inimigo[i].posy*20+contador,inimigo[i].img);
		if (contador==20)++inimigo[i].posy;
		break;
	 case 0:
		break;
	 }  /* desenhou todos inimigos */

	 /* ------------------- */

	/* for(i=0;i<=numinimigos;i++)*/
	/* {*/

	if (contador==20)
	     {
	     if ((inimigo[i].elementoant==0)||
		 (inimigo[i].elementoant==bolinha)||
		(inimigo[i].elementoant==escada))
	     tabfase[inimigo[i].posxant][inimigo[i].posyant]=inimigo[i].
								elementoant;
	     inimigo[i].posxant=inimigo[i].posx;
	     inimigo[i].posyant=inimigo[i].posy;
	     inimigo[i].elementoant=tabfase[inimigo[i].posx]
					[inimigo[i].posy];
	     if ((tabfase[inimigo[i].posx][inimigo[i].posy]==0)||
		 (tabfase[inimigo[i].posx][inimigo[i].posy]==bolinha)||
		 (tabfase[inimigo[i].posx][inimigo[i].posy]==escada))
		  tabfase[inimigo[i].posx][inimigo[i].posy]=concreto;
	     }
	 /*}*/

	/* ----------------------- */

	 }



	 } /* fim do For contador*/
}

void inicializa_jogo()
{
	int i;
	struct time t;
	char caract=255;
	desenhafase();
	soma_pontos(0);
	setfillstyle(SOLID_FILL,BLACK);
	bar(564,462,638,478);
	setcolor(LIGHTGRAY);
	outtextxy(566,463,"1");
        setcolor(LIGHTCYAN);
	outtextxy(565,462,"1");
	gettime(&t);
	t_ant=t.ti_hour*60 + t.ti_min*60 + t.ti_sec*100 + t.ti_hund;
	do
	{
	if (kbhit())
		{
		 do {caract=getch();}
			while((kbhit())||(caract==0));
		 if (!caract) do{caract=getch();}
			while(kbhit());
		 switch(caract)
		 {
		  case 72:    /*  seta p. cima  */
			plr1.movimento=cima;
			break;
		  case 80:    /*  seta p. baixo */
			plr1.movimento=baixo;
			break;
		  case 75:    /*  seta p. esq   */
			plr1.movimento=esq;
			break;
		  case 77:    /*  seta p. dir   */
			plr1.movimento=dir;
			break;
		  case  'Z':    /* Z */
		  case  'z':    /* z */
			plr1.movimento=furaesq;
			break;
		  case  'X':    /* X */
		  case  'x':    /* x */
			plr1.movimento=furadir;
			break;
		  case  27:
			vidas=-1;  /*  ABRE MENU */
				   /* SAI DO JOGO,PERDE UMA VIDA, */
				   /* CONTINUA ...... */
			break;
		  case 'T':
		  case 't':
			while (!(kbhit()))delay(pausa);
			break;
		  case '-':
			pausa+=5;
			if (pausa==70)pausa=65;
			break;
		  case '+':
			pausa-=5;
			if (pausa==0)pausa=5;
			break;
		  case 'Y':
			numbolinhas=0;
			plr1.posx=0;
                        plr1.posy=0;
			break;
		 } /* fim do SWITCH   */
	      }  /* fim do IF KBHIT */

	if (ptrprimeiro!=NULL)buracos_abertos();
	verifica_direcao();

	for(i=0;i<=numinimigos;verifica_direcao_inimigo(i),i++);

	if (vidas>=0)efetua_movimento();
	}
	while (vidas>=0);
}

void fim()
{
	libera_memoria();
	closegraph();
}

char menu_apresentacao()
{
struct REGPACK reg;
	char ch=0;
	int i;
	while(kbhit()){ch=getch();}
	setfillstyle(SOLID_FILL,LIGHTGRAY);
	bar(100,50,539,389);
	setcolor(WHITE);
	line(100,50,100,389);line(100,50,539,50);
	line(101,50,101,389);line(100,51,539,51);
	line(110,379,529,379);line(529,60,529,379);
	setcolor(DARKGRAY);
	line(539,389,539,50);line(539,389,100,389);
	line(538,389,538,51);line(539,388,101,388);
	line(110,60,529,60);line(110,60,110,379);

	settextstyle(installuserfont("bold"),HORIZ_DIR,3);
	setusercharsize(3,2,1,1);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	setcolor(BROWN);
	for(i=0;i<=6;i++)
		{
		if (i==6)setcolor(GREEN);
		outtextxy(320+i,60+i,"Lode");
		outtextxy(320+i,120+i,"Runner");
		outtextxy(319+i,60+i,"Lode");
		outtextxy(319+i,120+i,"Runner");
		}
	setcolor(RED);
	settextjustify(LEFT_TEXT,TOP_TEXT);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	outtextxy(150,205,"    Cassiano Lustosa Froes");
	outtextxy(150,220,"              fevereiro/94");
	outtextxy(130,320,"<Esc> Sai p/ o DOS");
	outtextxy(130,330,"<+> <-> mudam a velocidade");
	outtextxy(130,340,"Qualquer outra tecla inicia");

	rectangle(438,303,451,369);
	circle(436,335,1);
	setfillstyle(SOLID_FILL,RED);
	bar(440,300+pausa,449,367);/*311+pausa/2);*/

	for(i=8;i<=18;i++,desenha(i,14,tijolo),desenha(i+6,12,tijolo));
	desenha(13,12,concreto);
	setcolor(WHITE);
	desenhajogador(13*20,13*20,2);
	setcolor(BLUE);
	desenhajogador(9*20,13*20,3);
	do
	  {
	  ch=getch();
	  setfillstyle(SOLID_FILL,LIGHTGRAY);
	  bar(440,300+pausa,449,367);/*311+pausa/2);*/
	  if (ch=='-')
			{
			pausa+=5;
			if (pausa==70)pausa=5;
			}

	  if ((ch=='+')||(ch=='='))
			{
			pausa-=5;
			if (pausa==0)pausa=65;
			}
	  setfillstyle(SOLID_FILL,RED);
	  bar(440,300+pausa,449,367);/*311+pausa/2);*/
	  }
	while((ch=='+')||(ch=='-')||(ch=='='));
/*	intr(0x5,&reg);*/
	return ch;
}


void jogo()
{
	char opcao=0;
	int i,j;
	do
	{
	vidas=5;
	opcao=menu_apresentacao();
	if (opcao!=27) {
			arquivo=fopen("lddef.fse","r");
			for(i=0;i<=31;i++)
			    for(j=0;j<=22;j++)
			    tabfase[i][j]=tabfaseclone[i][j]=getc(arquivo);
			pontos=0;
			numfase=1;
			inicializa_jogo();
			fclose(arquivo);
			}

	}
	while (opcao!=27);
}
void main()
{
struct REGPACK reg;
	inicializa_grafico();
	line(0,0,20,20);
	intr(0x5, &reg);
	inicializa_sistema();
/*	intr(5, &reg);*/

	jogo();
	fim();
}