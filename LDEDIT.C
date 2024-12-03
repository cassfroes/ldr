/*
  Lode Runner
  Editor de Fases
  por Cassiano Lustosa Froes
  em fevereiro/94
*/

#include <graphics.h>
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
#define pausa 50


char tabfase[32][23];
char sai=0;
int numfase=0;
int posx,posy;
FILE *arquivo;

void inicializa_grafico()
{
	int dr,mo;
	detectgraph(&dr,&mo);
	initgraph(&dr,&mo,'');
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

	line(horiz+8,vert+12,horiz+5,vert+19); /*perna esq*/
	line(horiz+8,vert+13,horiz+6,vert+19); /*perna esq*/

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
	line(horiz+6,vert+14,horiz+6,vert+14);
	line(horiz+6,vert+15,horiz+5,vert+14);

	line(horiz+11,vert+12,horiz+14,vert+19); /*perna dir*/
	line(horiz+12,vert+13,horiz+15,vert+19);

	line(horiz+8,vert+6,horiz+5,vert+10); /*braco esq*/
	line(horiz+9,vert+6,horiz+6,vert+10); /*braco esq*/
	line(horiz+10,vert+6,horiz+13,vert+11);/*braco dir*/
	line(horiz+11,vert+6,horiz+14,vert+12);/*braco dir*/
	break;
	}
}


void desenha(horiz,vert,elemento)
int horiz,vert,elemento;
{
	tabfase[horiz][vert]=elemento;

	setfillstyle(SOLID_FILL,BLACK);
	bar(horiz*20,vert*20,horiz*20+19,vert*20+19);
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
	   case tijolofalso:
		setfillstyle(SOLID_FILL,RED);
		bar(horiz*20,vert*20,horiz*20+19,vert*20+19);
		setcolor(LIGHTMAGENTA);
		line(horiz*20,vert*20,horiz*20+19,vert*20);
		line(horiz*20,vert*20,horiz*20,vert*20+9);
		line(horiz*20,vert*20+10,horiz*20+9,vert*20+10);
		line(horiz*20+11,vert*20+10,horiz*20+19,vert*20+10);
		line(horiz*20+11,vert*20+10,horiz*20+11,vert*20+19);
		setcolor(MAGENTA);
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
	   case escadapassar:
		setcolor(LIGHTMAGENTA);
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
	   case plr:
		setcolor(WHITE);
		desenhajogador(horiz*20,vert*20,3);
		break;
	   case ini:
		setcolor(BLUE);
		desenhajogador(horiz*20,vert*20,3);
		break;

	   default:tabfase[horiz][vert]=0;
	}


}

void desenhafase()
{
	int i,j;
	for(i=0;i<=31;i++)
	{
	 for(j=0;j<=22;j++)
	 desenha(i,j,tabfase[i][j]);
	}


}   /* fim desenhafase */
void salva()
{
	int i,j;

	if (arquivo!=NULL)
	{
	for(i=0;i<=31;i++)
		for(j=0;j<=22;j++)
			putc(tabfase[i][j],arquivo);
	}

}

void le()
{
	int i,j;
	for(i=0;i<=31;i++)
		for(j=0;j<=22;j++)
			tabfase[i][j]=getc(arquivo);
	desenhafase();
}



void inicializa_sistema()
{
	int i,j,x;
	setcolor(WHITE);

	for(i=1;i<=6;i++)
		desenha(i*2,23,i);
	rectangle(0,460,639,479);
	outtextxy(320,465,"<L>er Fase / <S>alvar Fase / <Esc>");
	for(i=0;i<=31;i++)
		for(j=0;j<=22;j++)
			tabfase[i][j]=0;
	arquivo=fopen("lddef.fse","a+");
}
void edita()
{
	char ch;

	setwritemode(XOR_PUT);
	setcolor(YELLOW);
	rectangle(posx*20,posy*20,posx*20+19,posy*20+19);
	do {} while(!kbhit());
		rectangle(posx*20,posy*20,posx*20+19,posy*20+19);
		setwritemode(COPY_PUT);
		ch=getch();
		if(!ch)ch=getch();
		switch (ch)
		{
		case 75:(posx>0)?(posx--):(posx=31);
			break;
		case 77:(posx<31)?(posx++):(posx=0);
			break;
		case 72:(posy>0)?(posy--):(posy=22);
			break;
		case 80:(posy<22)?(posy++):(posy=0);
			break;
		case 27:sai=1;
			break;
		case 115:
		case 83:
			salva();
			break;
		case 76:
		case 108:
			le();
			break;
		default:if ((ch>=49)&&(ch<=57))desenha(posx,posy,ch-48);
		}
}

void fim()
{
	fclose(arquivo);
	closegraph();
}

void main()
{
	inicializa_grafico();
	inicializa_sistema();
	do
	edita();
	while (!sai);
	fim();
}