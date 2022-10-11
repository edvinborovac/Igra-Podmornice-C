#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

//deklaracije funkcija

void newGame(char (*matricaIgrac)[11], char (*matricaNPC)[11], int (*gridIgrac)[10], int(*gridNPC)[10]);
int potezi (int prviPotez,char (*matricaIgrac)[11], char (*matricaNPC)[11], int (*gridIgrac)[10], int(*gridNPC)[10]);
int igracHit(int (*gridIgrac)[10], int (*gridNPC)[10], char (*matricaIgrac)[11], char (*matricaNPC)[11]);
int isPotopljena(int (*grid)[10], int vrijednost);
int isWin(int (*grid)[10]);
int brojPreostalihPolja(int (*grid)[10]);
int brojNeisprobanihPolja(int (*grid)[10]);
void ispisMatrice(char (*matrica)[11]);
void resetirajMatricu(char (*matrica)[11]);
void kreirajMatricu(int (*grid)[10],char (*matrica)[11]);
void kreirajMatricuProtivnik(int (*grid)[10],char (*matrica)[11]);
int izbornik();
int dodajPodmornicu(int (*grid)[10], char (*matrica)[11]);
void resetGrida(int (*grid)[10]);
void generiranjeFormacijeRacunalo(int (*grid)[10]);


//main 

int main(){
	int pokretanje;
	char mojIspis[10][11], enemyIspis[10][11];
	int mojGrid[10][10], enemyGrid[10][10];
	
	while(69){
		pokretanje=izbornik();
		if(pokretanje==2) return 0;
		else newGame(mojIspis, enemyIspis, mojGrid, enemyGrid);
	}
	
	getch();
	system("cls");
	
	return 0;
}

//definicije funkcija

//newGame funkcija ce biti centralna funkcija koja ce kreirati slijed pokretanja ostalih funkcija za vrijeme postavljanja 
//formacija na pocetku igre te onda pokrece iducu fazu same izmjene poteza
void newGame(char (*matricaIgrac)[11], char (*matricaNPC)[11], int (*gridIgrac)[10], int(*gridNPC)[10]){
	int x,prviPotez,pobjednik=-1;
	
	resetirajMatricu(matricaIgrac);
	resetGrida(gridIgrac);
	resetirajMatricu(matricaNPC);
	resetGrida(gridNPC);
	generiranjeFormacijeRacunalo(gridNPC);
	x=dodajPodmornicu(gridIgrac, matricaIgrac);
	ispisMatrice(matricaNPC);
	printf("\n%d/%d\n", brojPreostalihPolja(gridNPC), brojNeisprobanihPolja(gridNPC));
	ispisMatrice(matricaIgrac);
	printf("\n%d/%d\n", brojPreostalihPolja(gridIgrac), brojNeisprobanihPolja(gridIgrac));
		
	srand((unsigned)time(NULL));
	prviPotez=0+rand()%2;	
	
	
	pobjednik=potezi(prviPotez,matricaIgrac,matricaNPC,gridIgrac,gridNPC);
	if(pobjednik==0) {
			system("cls");
			ispisMatrice(matricaNPC);
			printf("\n%d/%d\n", brojPreostalihPolja(gridNPC), brojNeisprobanihPolja(gridNPC));
			ispisMatrice(matricaIgrac);
			printf("\n%d/%d\n", brojPreostalihPolja(gridIgrac), brojNeisprobanihPolja(gridIgrac));
			printf("\n\nCESTITAM POBJEDILI STE!\n\n");
	}
	if(pobjednik==1) {
			system("cls");
			ispisMatrice(matricaNPC);
			printf("\n%d/%d\n", brojPreostalihPolja(gridNPC), brojNeisprobanihPolja(gridNPC));
			ispisMatrice(matricaIgrac);
			printf("\n%d/%d\n", brojPreostalihPolja(gridIgrac), brojNeisprobanihPolja(gridIgrac));
			printf("\n\nRACUNALO JE POBJEDILO!\n\n");
	}
	getch();
	printf("\n\nPress any key to continue.\n\n");
	getch();
}

//potezi funkcija obavlja neprestanu izmjenu poteza izmedju igraca i racunala dok ne vrati informaciju tko je pobjedio
int potezi (int prviPotez, char (*matricaIgrac)[11], char (*matricaNPC)[11], int (*gridIgrac)[10], int(*gridNPC)[10]){
	int pokusaj,size;
	
	//varijable potrebne za potez racunala
	int mapiranje[100],i,j,x,y,map,brojacHitova=0,pogodak=0,smjer,smjer2,smjer3,npc2brojac=0,npc3brojac=0,testniBrojac=0; 
	int x1,y1,x2,y2,x3,y3;
	
	for(i=0;i<100;i++){ //mapiranje sluzi da u polje spremi sve vrijednosti od 0 do 99 jer igraca ploca ima 10 x 10 polja
		mapiranje[i]=i; //i da nakon svakog pokusaja se obrise odredjeni broj iz polja kako racunalo ne bi pokusavalo gadjat isto polje.
	}
	
	srand((unsigned)time(NULL));
	
	if(prviPotez==0) goto promjenaPotezaIgrac;
	else goto promjenaPotezaNPC;
	
	promjenaPotezaIgrac:
		while(1){
			pokusaj=igracHit(gridIgrac, gridNPC, matricaIgrac, matricaNPC);
			
			if(pokusaj==1) size=4;
			if(pokusaj==2 || pokusaj==3) size=3;
			if(pokusaj==4 || pokusaj==5 || pokusaj==6) size=2;
			if(pokusaj==7) size=1;
			
			system("cls");
			ispisMatrice(matricaNPC);
			printf("\n%d/%d\n", brojPreostalihPolja(gridNPC), brojNeisprobanihPolja(gridNPC));
			ispisMatrice(matricaIgrac);
			printf("\n%d/%d\n", brojPreostalihPolja(gridIgrac), brojNeisprobanihPolja(gridIgrac));
			
			if(isPotopljena(gridNPC,pokusaj)==1 && pokusaj!=0) {
				printf("\n\nPOGODAK! Potopljena je podmornica velicine %d\n\n", size);
				if(isWin(gridNPC)==1) return 0;
				getch();
			}
			else 
				if(pokusaj!=0){
					printf("\n\nPOGODAK! Pogodjena je podmornica, ali nije jos potopljena.\n\n");
					getch();
				}
				else{
					goto promjenaPotezaNPC;
				}
		}
	
	//promjenaPotezaNPC label se pokrece svaki put kada igrac izgubi svoj potez, unutar tog labela se osim ispisa odredjuje gdje je racunalo
	//stalo pri svojem zadnjem potezu, odnosno u koju fazu se treba vratiti.
	promjenaPotezaNPC:
			system("cls");
			ispisMatrice(matricaNPC);
			printf("\n%d/%d\n", brojPreostalihPolja(gridNPC), brojNeisprobanihPolja(gridNPC));
			ispisMatrice(matricaIgrac);
			printf("\n%d/%d\n", brojPreostalihPolja(gridIgrac), brojNeisprobanihPolja(gridIgrac));
			
			if(pogodak==0) goto NPC0;
			if(pogodak==1) goto NPC1;
			if(pogodak==2) goto NPC2;
			if(pogodak==3) goto NPC3;
			
	//NPC0 je label iz kojeg se pokrece potez racunala u slucaju prvog poteza u igri, u slucaju da prethodno nema nikakvih pogodaka
	//i u slucaju kada je uspjesno potopio neku od podmornica i treba krenuti u potragu za novom.	
	NPC0:
			system("cls");
			ispisMatrice(matricaNPC);
			printf("\n%d/%d\n", brojPreostalihPolja(gridNPC), brojNeisprobanihPolja(gridNPC));
			ispisMatrice(matricaIgrac);
			printf("\n%d/%d\n", brojPreostalihPolja(gridIgrac), brojNeisprobanihPolja(gridIgrac));
			
			pogodak=0;
			
			map=0+rand()%(100-brojacHitova);
			
			//pretvaranje elementa polja preko generiranog indeksa u x i y koordinatu
			x=mapiranje[map]/10;
			y=mapiranje[map]%10;
		
			
		
			//brojacHitova se dize pri svakom pokusaju koje racunalo ispuca
			brojacHitova++;
			
			//brisanje pokusaja iz fonda neisprobanih polja kako racunalo ne bi pokusavalo vise puta gadjati isto polje
			for(j=0;(j+map)<(100-brojacHitova);j++){
				mapiranje[map+j]=mapiranje[map+j+1];
			}
			

			
			if(gridIgrac[x][y]==-1 || gridIgrac[x][y]==-2 || gridIgrac[x][y]==-3) {
				if(gridIgrac[x][y]!=-3) gridIgrac[x][y]=-2;
				kreirajMatricu(gridIgrac,matricaIgrac);
				goto promjenaPotezaIgrac;
			}
			
			if(gridIgrac[x][y]!=-1 && gridIgrac[x][y]!=-2 && gridIgrac[x][y]!=-3) {
				pokusaj=gridIgrac[x][y];
				pogodak++;
				gridIgrac[x][y]=-3;
				kreirajMatricu(gridIgrac,matricaIgrac);
				if(isWin(gridIgrac)==1) return 1;
				if(isPotopljena(gridIgrac,pokusaj)==1) goto NPC0;
				if(isPotopljena(gridIgrac,pokusaj)!=1) goto NPC1; 
			}
		
	//NPC1 label se pokrece kada je racunalo pogodilo prvu tocku nove podmornice i sada treba odrediti smjer u kojem ce nastaviti dalje	
	NPC1:	
			system("cls");
			ispisMatrice(matricaNPC);
			printf("\n%d/%d\n", brojPreostalihPolja(gridNPC), brojNeisprobanihPolja(gridNPC));
			ispisMatrice(matricaIgrac);	
			printf("\n%d/%d\n", brojPreostalihPolja(gridIgrac), brojNeisprobanihPolja(gridIgrac));
			
			do{
				testniBrojac=0;
				smjer=0+rand()%4;
				if(smjer==0 && (gridIgrac[x][y-1]==-2 || (y-1)<0)) testniBrojac++;
				if(smjer==1 && (gridIgrac[x-1][y]==-2 || (x-1)<0)) testniBrojac++;
				if(smjer==2 && (gridIgrac[x][y+1]==-2 || (y+1)>9)) testniBrojac++; 
				if(smjer==3 && (gridIgrac[x+1][y]==-2 || (x+1)>9)) testniBrojac++;
			} while(testniBrojac!=0);
			
			if(smjer==0) {y1=y-1; x1=x;}
			if(smjer==1) {x1=x-1; y1=y;}		
			if(smjer==2) {y1=y+1; x1=x;}	
			if(smjer==3) {x1=x+1; y1=y;}	
			
			map=(x1*10)+y1;
			
			brojacHitova++;
			
			for(j=0;(j+map)<(100-brojacHitova);j++){
				mapiranje[map+j]=mapiranje[map+j+1];
			}
			
			
			if(gridIgrac[x1][y1]==-1 || gridIgrac[x1][y1]==-2 || gridIgrac[x1][y1]==-3) {
				if(gridIgrac[x1][y1]!=-3) gridIgrac[x1][y1]=-2;
				kreirajMatricu(gridIgrac,matricaIgrac);
				goto promjenaPotezaIgrac;
			}
			
			if(gridIgrac[x1][y1]!=-1 && gridIgrac[x1][y1]!=-2 && gridIgrac[x1][y1]!=-3) {
				pokusaj=gridIgrac[x1][y1];
				pogodak++;
				gridIgrac[x1][y1]=-3;
				kreirajMatricu(gridIgrac,matricaIgrac);
				if(isWin(gridIgrac)==1) return 1;
				if(isPotopljena(gridIgrac,pokusaj)==1) goto NPC0;
				if(isPotopljena(gridIgrac,pokusaj)!=1) goto NPC2; 
			}
	
	//NPC2 label se pokrece kada racunalo pogodi dva polja u nizu koja pripadaju istoj podmornici ali ju i dalje nije potopilo		
	NPC2:
			system("cls");
			ispisMatrice(matricaNPC);
			printf("\n%d/%d\n", brojPreostalihPolja(gridNPC), brojNeisprobanihPolja(gridNPC));
			ispisMatrice(matricaIgrac);	
			printf("\n%d/%d\n", brojPreostalihPolja(gridIgrac), brojNeisprobanihPolja(gridIgrac));
			
			npc2brojac=0;
			
			do{
				testniBrojac=0;
				if(npc2brojac==0) smjer2=smjer;
				if(npc2brojac==1) {
					if(smjer==1) smjer2=3;
					if(smjer!=1) smjer2=abs(smjer-2);
				}
				if(npc2brojac>1) goto NPC0;
				
				npc2brojac++;
				
				if(smjer2==smjer && smjer2==0 && (gridIgrac[x1][y1-1]==-2 || (y1-1)<0)) testniBrojac++;
				if(smjer2==smjer && smjer2==1 && (gridIgrac[x1-1][y1]==-2 || (x1-1)<0)) testniBrojac++;
				if(smjer2==smjer && smjer2==2 && (gridIgrac[x1][y1+1]==-2 || (y1+1)>9)) testniBrojac++;
				if(smjer2==smjer && smjer2==3 && (gridIgrac[x1+1][y1]==-2 || (x1+1)>9)) testniBrojac++;
				
				if(smjer2!=smjer && smjer2==0 && (gridIgrac[x][y-1]==-2 || (y-1)<0)) testniBrojac++;
				if(smjer2!=smjer && smjer2==1 && (gridIgrac[x-1][y]==-2 || (x-1)<0)) testniBrojac++;
				if(smjer2!=smjer && smjer2==2 && (gridIgrac[x][y+1]==-2 || (y+1)>9)) testniBrojac++;
				if(smjer2!=smjer && smjer2==3 && (gridIgrac[x+1][y]==-2 || (x+1)>9)) testniBrojac++;
				
				
			} while(testniBrojac!=0);
			
			if(smjer2==0){
				if(smjer2==smjer) {x2=x1; y2=y1-1; goto dalje;}
				if(smjer2!=smjer) {x2=x; y2=y-1; goto dalje;}
			} 
			if(smjer2==1){
				if(smjer2==smjer) {x2=x1-1; y2=y1; goto dalje;}
				if(smjer2!=smjer) {x2=x-1; y2=y; goto dalje;}
			} 
			if(smjer2==2){
				if(smjer2==smjer) {x2=x1; y2=y1+1; goto dalje;}
				if(smjer2!=smjer) {x2=x; y2=y+1; goto dalje;}
			} 
			if(smjer2==3){
				if(smjer2==smjer) {x2=x1+1; y2=y1; goto dalje;}
				if(smjer2!=smjer) {x2=x+1; y2=y; goto dalje;}
			} 
			
		dalje: 
			
			map=x2*10+y2;
			
			brojacHitova++;
			
			for(j=0;(j+map)<(100-brojacHitova);j++){
				mapiranje[map+j]=mapiranje[map+j+1];
			}
			
			
		if(gridIgrac[x2][y2]==-1 || gridIgrac[x2][y2]==-2 || gridIgrac[x2][y2]==-3) {
				if(gridIgrac[x2][y2]!=-3) gridIgrac[x2][y2]=-2;
				kreirajMatricu(gridIgrac,matricaIgrac);
				goto promjenaPotezaIgrac;
			}
			
		if(gridIgrac[x2][y2]!=-1 && gridIgrac[x2][y2]!=-2 && gridIgrac[x2][y2]!=-3) {
				pokusaj=gridIgrac[x2][y2];
				pogodak++;
				gridIgrac[x2][y2]=-3;
				kreirajMatricu(gridIgrac,matricaIgrac);
				if(isWin(gridIgrac)==1) return 1;
				if(isPotopljena(gridIgrac,pokusaj)==1) goto NPC0;
				if(isPotopljena(gridIgrac,pokusaj)!=1) goto NPC3; 
			}
	
	//NPC3 label se pokrece kada racunalo pogodi tri polja u nizu koja pripadaju istoj podmornici ali ju i dalje nije potopilo		
	NPC3:
			system("cls");
			ispisMatrice(matricaNPC);
			printf("\n%d/%d\n", brojPreostalihPolja(gridNPC), brojNeisprobanihPolja(gridNPC));
			ispisMatrice(matricaIgrac);	
			printf("\n%d/%d\n", brojPreostalihPolja(gridIgrac), brojNeisprobanihPolja(gridIgrac));
						
			npc3brojac=0;
			
			do{
				testniBrojac=0;
				if(npc3brojac==0 && smjer2==smjer) smjer3=smjer;
				if(npc3brojac==1 && smjer2==smjer) {
					if(smjer==1) smjer3=3;
					if(smjer!=1) smjer3=abs(smjer-2);
				}
				if(smjer2!=smjer) smjer3=smjer2;
				if(npc3brojac>1) goto NPC0;
				
				npc3brojac++;
				
				if(smjer3==smjer2 && smjer3==0 && (gridIgrac[x2][y2-1]==-2 || (y2-1)<0)) testniBrojac++;
				if(smjer3==smjer2 && smjer3==1 && (gridIgrac[x2-1][y2]==-2 || (x2-1)<0)) testniBrojac++;
				if(smjer3==smjer2 && smjer3==2 && (gridIgrac[x2][y2+1]==-2 || (y2+1)>9)) testniBrojac++;
				if(smjer3==smjer2 && smjer3==3 && (gridIgrac[x2+1][y2]==-2 || (x2+1)>9)) testniBrojac++;
				
				if(smjer3!=smjer2 && smjer3==0 && (gridIgrac[x][y-1]==-2 || (y-1)<0)) testniBrojac++;
				if(smjer3!=smjer2 && smjer3==1 && (gridIgrac[x-1][y]==-2 || (x-1)<0)) testniBrojac++;
				if(smjer3!=smjer2 && smjer3==2 && (gridIgrac[x][y+1]==-2 || (y+1)>9)) testniBrojac++;
				if(smjer3!=smjer2 && smjer3==3 && (gridIgrac[x+1][y]==-2 || (x+1)>9)) testniBrojac++;
				
				
			} while(testniBrojac!=0);
			
			if(smjer3==0){
				if(smjer3==smjer2) {x3=x2; y3=y2-1; goto dalje1;}
				if(smjer3!=smjer2) {x3=x; y3=y-1; goto dalje1;}
			} 
			if(smjer3==1){
				if(smjer3==smjer2) {x3=x2-1; y3=y2; goto dalje1;}
				if(smjer3!=smjer2) {x3=x-1; y3=y; goto dalje1;}
			} 
			if(smjer3==2){
				if(smjer3==smjer2) {x3=x2; y3=y2+1; goto dalje1;}
				if(smjer3!=smjer2) {x3=x; y3=y+1; goto dalje1;}
			} 
			if(smjer3==3){
				if(smjer3==smjer) {x3=x2+1; y3=y2; goto dalje1;}
				if(smjer3!=smjer) {x3=x+1; y3=y; goto dalje1;}
			} 
			
		dalje1: 
			
			map=x3*10+y3;
			
			brojacHitova++;
			
			for(j=0;(j+map)<(100-brojacHitova);j++){
				mapiranje[map+j]=mapiranje[map+j+1];
			}
			
			
			
		if(gridIgrac[x3][y3]==-1 || gridIgrac[x3][y3]==-2 || gridIgrac[x3][y3]==-3) {
				if(gridIgrac[x3][y3]!=-3) gridIgrac[x3][y3]=-2;
				kreirajMatricu(gridIgrac,matricaIgrac);
				goto promjenaPotezaIgrac;
			}
			
		if(gridIgrac[x3][y3]!=-1 && gridIgrac[x3][y3]!=-2 && gridIgrac[x3][y3]!=-3) {
				pokusaj=gridIgrac[x3][y3];
				gridIgrac[x3][y3]=-3;
				kreirajMatricu(gridIgrac,matricaIgrac);
				if(isWin(gridIgrac)==1) return 1;
				else goto NPC0; 
			}
}

//igracHit funkcija se pokrece kada je igrac na redu da pogadja lokaciju protivnikovih podmornica
int igracHit(int (*gridIgrac)[10],int(*gridNPC)[10], char (*matricaIgrac)[11], char (*matricaNPC)[11]){
	system("cls");
	ispisMatrice(matricaNPC);
	printf("\n%d/%d\n", brojPreostalihPolja(gridNPC), brojNeisprobanihPolja(gridNPC));
	ispisMatrice(matricaIgrac);
	printf("\n%d/%d\n", brojPreostalihPolja(gridIgrac), brojNeisprobanihPolja(gridIgrac));
	
	int brojacPogreski=0,y,pogodak,x;
	char slovo;
	printf("\n\nUnesite koordinatu na koju zelite gadjati protivnika! (u formatu npr. C5)\n\n");
	do{
				
			if(brojacPogreski>0) printf("Pogresan unos! Pokusajte ponovno.\n");
			scanf(" %c", &slovo);
			scanf("%d", &y);
		
			brojacPogreski++;
		} while( (!(slovo>=65 && slovo<=74) && !(slovo>=97 && slovo<=106)) || !(y>0 && y<=10) );
		
	if(slovo>=97 && slovo<=106) x=slovo-97;
	if(slovo>=65 && slovo<=74) x=slovo-65;
	y=y-1;
	
	if(gridNPC[x][y]==-1 || gridNPC[x][y]==-2 || gridNPC[x][y]==-3) {
		if(gridNPC[x][y]==-3) gridNPC[x][y]=-3;
		else gridNPC[x][y]=-2;
		kreirajMatricuProtivnik(gridNPC,matricaNPC);
		return 0;
	}
	
	if(gridNPC[x][y]!=-1 && gridNPC[x][y]!=-2 && gridNPC[x][y]!=-3) {
		pogodak=gridNPC[x][y];
		gridNPC[x][y]=-3;
		kreirajMatricuProtivnik(gridNPC,matricaNPC);
		return pogodak;
	}
}

//isPotopljena funkcija prima grid jednog od igraca i vrijednost polja na kojoj je neki od igraca pogodio podmornicu. Provjerava postoji li 
//jos polja s tom vrijednoscu ili je potopljena cijela podmornica koja je imala tu vrijednost.

int isPotopljena(int (*grid)[10], int vrijednost){
	if(vrijednost==7) return 1;
	
	int i,j,brojac=0;
	
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(grid[i][j]==vrijednost) brojac++;
		}
	}
	
	if(brojac==0) return 1;
	else return 0;	
}

//isWin funkcija prima grid jednog od igraca i provjerava je li taj igrac izgubio, odnosno je li drugi igrac pobjedio
int isWin(int (*grid)[10]){
	int i,j,brojac=0;
	
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(grid[i][j]!=-1 && grid[i][j]!=-2 && grid[i][j]!=-3) brojac++;
		}
	}
	
	if(brojac==0) return 1;
	else return 0;
}

//brojPreostalihPolja je informacija koja je stalno vidljiva za vrijeme igre i govori koliko je kojem igracu preostalo nepogodjenih polja
int brojPreostalihPolja(int (*grid)[10]){
	int i,j,brojac=0;
	
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(grid[i][j]>=1 && grid[i][j]<=7) brojac++;
		}
	}
	
	return brojac;
}

//brojNeisprobanihPolja je informacija koja je stalno vidljiva za vrijeme igre i govori koliko je kojem igracu preostalo polja koja nisu jos gadjali
int brojNeisprobanihPolja(int (*grid)[10]){
	int i,j,brojac=0;
	
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(grid[i][j]==-2 || grid[i][j]==-3) brojac++;
		}
	}
	
	return 100-brojac;
}

//ispisMatrice funkcija se bavi pravilnim ispisom ploce koja ce se koristiti u igri
void ispisMatrice(char (*matrica)[11]){
	int *header,i,j,k;
	header=(int*)malloc(10*sizeof(int));
	
	
	for(k=0; k<10; k++){
		header[k]=k+1;
	}
	
	printf("\n");
	printf("\t  ");
	for(k=0; k<10; k++){
		printf("%d ", header[k]);
	}
	
	printf("\n\t");
	for(i=0;i<10;i++){
		for(j=0;j<11;j++){
			printf("%c ", matrica[i][j]);
		}
		printf("\n\t");
	}
	
	free(header);
}

//resetirajMatricu funkcija sluzi za postavljanje ploce na pocetno stanje, na primjer pri pokretanju nove igre
void resetirajMatricu(char (*matrica)[11]){
	int i,j;
	for(i=0;i<10;i++){
		for(j=0;j<11;j++){
			if(j==0) matrica[i][j]=65+i;
				else matrica[i][j]='.';
		}
	}
}

//kreirajMatricu funkcija sluzi za kreiranje prikaza polja koje ce sluziti kao user interface za vrijeme igranja
void kreirajMatricu(int (*grid)[10],char (*matrica)[11]){
	int i,j;
	for(i=0;i<10;i++){
		for(j=1;j<11;j++){
			if(grid[i][j-1]!=-1 && grid[i][j-1]!=-2 && grid[i][j-1]!=-3) matrica[i][j]='O';
			if(grid[i][j-1]==-2) matrica[i][j]='*';
			if(grid[i][j-1]==-3) matrica[i][j]='X';
		}
	}
}

//kreirajMatricuProtivnik sluzi za kreiranje prikaza polja koje predstavlja protivnikovu plocu, ali su prikazani samo pogotci i promasaji
void kreirajMatricuProtivnik(int (*grid)[10],char (*matrica)[11]){
	int i,j;
	for(i=0;i<10;i++){
		for(j=1;j<11;j++){
			if(grid[i][j-1]==-2) matrica[i][j]='*';
			if(grid[i][j-1]==-3) matrica[i][j]='X';
		}
	}
}

//izbornik funkcija ce se pokretati odma na pocetku programa i sluzi da se odabere opcija nove igre ili kraja rada
int izbornik(){
	int x;
	printf("\n\n\tOdaberite broj za nastavak:\n\t\t1. Nova igra\n\t\t2. Kraj rada\n\n");
	do{scanf("%d", &x);} while (x!=1 && x!=2);
	system("cls");
	return x;
}

//dodajPodmornicu funkcija se bavi rucnim unošenjem formacije podmornica za ljudskog igraca

int dodajPodmornicu(int (*grid)[10], char (*matrica)[11]){

		int i,j,x,y,smjer,size,testniBrojac[4]={0,0,0,0}; 
		int z,q;
		int br=1, brojacPogreski=0;
		char slovo;
		int odluka;
		
		srand((unsigned)time(NULL));
		
		printf("\n\n\tOdaberite broj za nastavak:\n\t1. Rucno unosenje formacije podmornica\n\t2. Nasumicno generiranje formacije podmornica\n\n");
		do{scanf("%d", &odluka);} while (odluka!=1 && odluka!=2);
		
		if(odluka==2) {
			system("cls");
			generiranjeFormacijeRacunalo(grid);
			kreirajMatricu(grid,matrica);
			return 1;
		}
		
		for(i=0;i<10;i++){ 
			for(j=0;j<10;j++){
				grid[i][j]=-1;
			}
		}
		
	while(br<=10){
		
		if(br==1) size = 4;
		if(br==2 || br==3) size = 3;
		if(br==4 || br==5 || br==6) size = 2;
		if(br==7 || br==8 || br==9 || br==10) size = 1;
	
		if(size!=1)
		{	
			brojacPogreski=0;
			system("cls");
			kreirajMatricu(grid,matrica);
			ispisMatrice(matrica);
			
			printf("\n Unosenje podmornice velicine %d!\n", size);
			printf("\n Unesite pocetnu tocku podmornice i smjer rasprostiranja podmornice! \n (npr. B4 3) (0-lijevo, 1-gore, 2-desno, 3-dolje)\n");
			do{
				do{
				
					if(brojacPogreski>0) printf("Pogresan unos! Pokusajte ponovno.\n");
					brojacPogreski=0;
					scanf(" %c", &slovo);
					scanf("%d", &y);
					
					scanf(" %d", &smjer);
					brojacPogreski++;
				} while((!(slovo>=65 && slovo<=74) && !(slovo>=97 && slovo<=106)) || !(y>0 && y<=10) || !(smjer>=0 && smjer<=3) );
				
				if(slovo>=97 && slovo<=106) x=slovo-97;
				if(slovo>=65 && slovo<=74) x=slovo-65;
				y=y-1;
				
				for(i=0;i<4;i++){
					testniBrojac[i]=0;
				}
				
				for(i=0;i<size;i++){
					
					if( grid[x][y-size+1+i]!=-1 || (grid[x+1][y-size+1+i]>=1 && grid[x+1][y-size+1+i]<=7 && (x+1)<=9) || 
						(grid[x-1][y-size+1+i]>=1 && grid[x-1][y-size+1+i]<=7 && (x-1)>=0) || (grid[x][y+1]>=1 && grid[x][y+1]<=7 && (y+1)<=9) || 
						(grid[x][y-size]>=1 && grid[x][y-size]<=7 && (y-size)>=0) || (y-size+1+i)<0 || (grid[x-1][y+1]>=1 && grid[x-1][y+1]<=7 && (x-1)>=0 && (y+1)<=9) ||
						(grid[x+1][y+1]>=1 && grid[x+1][y+1]<=7 && (y+1)<=9 && (x+1)<=9) || (grid[x-1][y-size]>=1 && grid[x-1][y-size]<=7 && (x-1)>=0 && (y-size)>=0) || 
						(grid[x+1][y-size]>=1 && grid[x+1][y-size]<=7 && (x+1)<=9 && (y-size)>=0) )
							testniBrojac[0]++;
					
					if( grid[x-size+1+i][y]!=-1 || (grid[x-size+1+i][y+1]>=1 && grid[x-size+1+i][y+1]<=7 && (y+1)<=9)  || 
						(grid[x-size+1+i][y-1]>=1 && grid[x-size+1+i][y-1]<=7 && (y-1)>=0) || (grid[x+1][y]>=1 && grid[x+1][y]<=7 && (x+1)<=9 )  || 
						(grid[x-size][y]>=1 && grid[x-size][y]<=7 && (x-size)>=0) || (x-size+1+i)<0 || (grid[x+1][y-1]>=1 && grid[x+1][y-1]<=7 && (x+1)<=9 && (y-1)>=0 ) || 
						(grid[x+1][y+1]>=1 && grid[x+1][y+1]<=7 && (x+1)<=9 && (y+1)<=9) || (grid[x-size][y-1]>=1 && grid[x-size][y-1]<=7 && (x-size)>=0 && (y-1)>=0) || 
						(grid[x-size][y+1]>=1 && grid[x-size][y+1]<=7 && (x-size)>=0 && (y+1)<=9) ) 
							testniBrojac[1]++;
					
					if( grid[x][y+i]!=-1 || (grid[x+1][y+i]>=1 && grid[x+1][y+i]<=7 && (x+1)<=9) || 
						(grid[x-1][y+i]>=1 && grid[x-1][y+i]<=7 && (x-1)>=0) || (grid[x][y-1]>=1 && grid[x][y-1]<=7 && (y-1)>=0) || 
						(grid[x][y+size]>=1 && grid[x][y+size]<=7 && (y+size)<=9) || (y+i)>9 || (grid[x-1][y-1]>=1 && grid[x-1][y-1]<=7 && (x-1)>=0 && (y-1)>=0) || 
						(grid[x+1][y-1]>=1 && grid[x+1][y-1]<=7 && (x+1)<=9 && (y-1)>=0) || (grid[x-1][y+size]>=1 && grid[x-1][y+size]<=7 && (x-1)>=0 && (y+size)<=9) ||
						(grid[x+1][y+size]>=1 && grid[x+1][y+size]<=7 && (x+1)<=9 && (y+size)<=9) ) 
							testniBrojac[2]++;
					
					if( grid[x+i][y]!=-1 || (grid[x+i][y+1]>=1 && grid[x+i][y+1]<=7 && (y+1)<=9) || 
						(grid[x+i][y-1]>=1 && grid[x+i][y-1]<=7 && (y-1)>=0) || (grid[x-1][y]>=1 && grid[x-1][y]<=7 && (x-1)>=0) || 
						(grid[x+size][y]>=1 && grid[x+size][y]<=7 && (x+size)<=9) || (x+i)>9 || (grid[x-1][y-1]>=1 && grid[x-1][y-1]<=7 && (x-1)>=0 && (y-1)>=0) ||
						(grid[x-1][y+1]>=1 && grid[x-1][y+1]<=7 && (x-1)>=0 && (y+1)<=9) || (grid[x+size][y-1]>=1 && grid[x+size][y-1]<=7 && (y-1)>=0 && (x+size)<=9) || 
						(grid[x+size][y+1]>=1 && grid[x+size][y+1]<=7 && (y+1)<=9 && (x+size)<=9) )
							testniBrojac[3]++;
					
				}	
				
			} while(grid[x][y]!=-1 || testniBrojac[smjer]!=0);
		
			if(smjer==0) {
				q=y-size+1;
				for(i=0;i<size;i++){
					grid[x][q+i]=br;
				}
				
			}
		
			if(smjer==1) {
				z=x-size+1;
				for(i=0;i<size;i++){
					grid[z+i][y]=br;
				}
			}
		
			if(smjer==2) {
				for(i=0;i<size;i++){
					grid[x][y+i]=br;
				}
			}
		
			if(smjer==3) {
				for(i=0;i<size;i++){
					grid[x+i][y]=br;
				}
			}
		}
		else
		{	
			brojacPogreski=0;
			system("cls");
			kreirajMatricu(grid,matrica);
			ispisMatrice(matrica);
			
			printf("\nUnosenje podmornice velicine %d!\n", size);
			printf("\nUnesite koordinatu podmornice! (npr. B4)\n");
			
			do
			{
				do{
					if(brojacPogreski>0) printf("Pogresan unos! Pokusajte ponovno.\n");
					brojacPogreski=0;
					scanf(" %c%d", &slovo,&y);
				//	scanf("%d", &y);
					brojacPogreski++;
				} while((!(slovo>=65 && slovo<=74) && !(slovo>=97 && slovo<=106)) || !(y>0 && y<=10) );	
				
			if(slovo>=97 && slovo<=106) x=slovo-97; //dodano novo
			if(slovo>=65 && slovo<=74) x=slovo-65;
			y=y-1;
				
			} while(grid[x][y]!=-1 || (grid[x+1][y]>=1 && grid[x+1][y]<=7 && (x+1)<=9) || (grid[x-1][y]>=1 && grid[x-1][y]<=7 && (x-1)>=0) ||
				   (grid[x][y+1]>=1 && grid[x][y+1]<=7 && (y+1)<=9) || (grid[x][y-1]>=1 && grid[x][y-1]<=7 && (y-1)>=0) || 
				   (grid[x-1][y-1]>=1 && grid[x-1][y-1]<=7 && (x-1)>=0 && (y-1)>=0) || (grid[x-1][y+1]>=1 && grid[x-1][y+1]<=7 && (x-1)>=0 && (y+1)<=9) || 
				   (grid[x+1][y-1]>=1 && grid[x+1][y-1]<=7 && (x+1)<=9 && (y-1)>=0) || (grid[x+1][y+1]>=1 && grid[x+1][y+1]<=7 && (x+1)<=9 && (y+1)<=9));
		
		grid[x][y]=7;
			
		}
		
		br++;
	}
	return 0;	
}


//resetGrida sluzi za resetiranje matrice koja radi u pozadini iza vidljive ploce pri igri
void resetGrida(int (*grid)[10]){
	int i,j;
	
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			grid[i][j]=0;
		}
	}
}

//generiranjeFormacijeRacunalo funkcija sluzi da racunalo prilikom pocetka svake nove igre moze samostalno, tocno 
//i svaki puta drugacije rasporediti brodove po ploci za igru, odnosno po "gridu"
void generiranjeFormacijeRacunalo(int (*grid)[10]){
	
	int i,j,x,y,smjer,size,testniBrojac[4]={0,0,0,0}; 
	int z,q; //modificirane koordinate s obzirom na velicinu
	int br=1; //brojac koji odredjuje koliko brodova je uneseno
	
	srand((unsigned)time(NULL));
	
	for(i=0;i<10;i++){ 
		for(j=0;j<10;j++){
			grid[i][j]=-1;
		}
	}
	
while(br<=10){
	
		if(br==1) size = 4;
		if(br==2 || br==3) size = 3;
		if(br>=4 && br<=6) size = 2;
		if(br>=7 && br<=10) size = 1;

	if(size!=1)
	{
		do{
			x=0+rand()%10;
			y=0+rand()%10;
			smjer=0+rand()%4; //smjer -> 0 lijevo, 1 gore, 2 desno, 3 dolje
			
			for(i=0;i<4;i++){
				testniBrojac[i]=0;
			}
			
			for(i=0;i<size;i++){ //provjerava u kojem smjeru ne moze ici s obzirom na velicinu i ostale podmornice
				
				if( 	grid[x][y-size+1+i]!=-1 || (grid[x+1][y-size+1+i]>=1 && grid[x+1][y-size+1+i]<=7 && (x+1)<=9) || 
						(grid[x-1][y-size+1+i]>=1 && grid[x-1][y-size+1+i]<=7 && (x-1)>=0) || (grid[x][y+1]>=1 && grid[x][y+1]<=7 && (y+1)<=9) || 
						(grid[x][y-size]>=1 && grid[x][y-size]<=7 && (y-size)>=0) || (y-size+1+i)<0 || (grid[x-1][y+1]>=1 && grid[x-1][y+1]<=7 && (x-1)>=0 && (y+1)<=9) ||
						(grid[x+1][y+1]>=1 && grid[x+1][y+1]<=7 && (y+1)<=9 && (x+1)<=9) || (grid[x-1][y-size]>=1 && grid[x-1][y-size]<=7 && (x-1)>=0 && (y-size)>=0) || 
						(grid[x+1][y-size]>=1 && grid[x+1][y-size]<=7 && (x+1)<=9 && (y-size)>=0) )
							testniBrojac[0]++;
					
					if( grid[x-size+1+i][y]!=-1 || (grid[x-size+1+i][y+1]>=1 && grid[x-size+1+i][y+1]<=7 && (y+1)<=9)  || 
						(grid[x-size+1+i][y-1]>=1 && grid[x-size+1+i][y-1]<=7 && (y-1)>=0) || (grid[x+1][y]>=1 && grid[x+1][y]<=7 && (x+1)<=9 )  || 
						(grid[x-size][y]>=1 && grid[x-size][y]<=7 && (x-size)>=0) || (x-size+1+i)<0 || (grid[x+1][y-1]>=1 && grid[x+1][y-1]<=7 && (x+1)<=9 && (y-1)>=0 ) || 
						(grid[x+1][y+1]>=1 && grid[x+1][y+1]<=7 && (x+1)<=9 && (y+1)<=9) || (grid[x-size][y-1]>=1 && grid[x-size][y-1]<=7 && (x-size)>=0 && (y-1)>=0) || 
						(grid[x-size][y+1]>=1 && grid[x-size][y+1]<=7 && (x-size)>=0 && (y+1)<=9) ) 
							testniBrojac[1]++;
					
					if( grid[x][y+i]!=-1 || (grid[x+1][y+i]>=1 && grid[x+1][y+i]<=7 && (x+1)<=9) || 
						(grid[x-1][y+i]>=1 && grid[x-1][y+i]<=7 && (x-1)>=0) || (grid[x][y-1]>=1 && grid[x][y-1]<=7 && (y-1)>=0) || 
						(grid[x][y+size]>=1 && grid[x][y+size]<=7 && (y+size)<=9) || (y+i)>9 || (grid[x-1][y-1]>=1 && grid[x-1][y-1]<=7 && (x-1)>=0 && (y-1)>=0) || 
						(grid[x+1][y-1]>=1 && grid[x+1][y-1]<=7 && (x+1)<=9 && (y-1)>=0) || (grid[x-1][y+size]>=1 && grid[x-1][y+size]<=7 && (x-1)>=0 && (y+size)<=9) ||
						(grid[x+1][y+size]>=1 && grid[x+1][y+size]<=7 && (x+1)<=9 && (y+size)<=9) ) 
							testniBrojac[2]++;
					
					if( grid[x+i][y]!=-1 || (grid[x+i][y+1]>=1 && grid[x+i][y+1]<=7 && (y+1)<=9) || 
						(grid[x+i][y-1]>=1 && grid[x+i][y-1]<=7 && (y-1)>=0) || (grid[x-1][y]>=1 && grid[x-1][y]<=7 && (x-1)>=0) || 
						(grid[x+size][y]>=1 && grid[x+size][y]<=7 && (x+size)<=9) || (x+i)>9 || (grid[x-1][y-1]>=1 && grid[x-1][y-1]<=7 && (x-1)>=0 && (y-1)>=0) ||
						(grid[x-1][y+1]>=1 && grid[x-1][y+1]<=7 && (x-1)>=0 && (y+1)<=9) || (grid[x+size][y-1]>=1 && grid[x+size][y-1]<=7 && (y-1)>=0 && (x+size)<=9) || 
						(grid[x+size][y+1]>=1 && grid[x+size][y+1]<=7 && (y+1)<=9 && (x+size)<=9) )
							testniBrojac[3]++;
				
			}	
		
		} while(grid[x][y]!=-1 || testniBrojac[smjer]!=0);
	
		if(smjer==0) {
			q=y-size+1;
			for(i=0;i<size;i++){
				grid[x][q+i]=br;
			}
		}
	
		if(smjer==1) {
			z=x-size+1;
			for(i=0;i<size;i++){
				grid[z+i][y]=br;
			}
		}
	
		if(smjer==2) {
			for(i=0;i<size;i++){
				grid[x][y+i]=br;
			}
		}
	
		if(smjer==3) {
			for(i=0;i<size;i++){
				grid[x+i][y]=br;
			}
		}
	}
	else
	{	
		do
		{
			x=0+rand()%10;
			y=0+rand()%10;
		}  while(grid[x][y]!=-1 || (grid[x+1][y]>=1 && grid[x+1][y]<=7 && (x+1)<=9) || (grid[x-1][y]>=1 && grid[x-1][y]<=7 && (x-1)>=0) ||
				(grid[x][y+1]>=1 && grid[x][y+1]<=7 && (y+1)<=9) || (grid[x][y-1]>=1 && grid[x][y-1]<=7 && (y-1)>=0) || 
				(grid[x-1][y-1]>=1 && grid[x-1][y-1]<=7 && (x-1)>=0 && (y-1)>=0) || (grid[x-1][y+1]>=1 && grid[x-1][y+1]<=7 && (x-1)>=0 && (y+1)<=9) || 
				(grid[x+1][y-1]>=1 && grid[x+1][y-1]<=7 && (x+1)<=9 && (y-1)>=0) || (grid[x+1][y+1]>=1 && grid[x+1][y+1]<=7 && (x+1)<=9 && (y+1)<=9));
		
		grid[x][y]=7;
		
	}
	
	br++;
}

	
}


