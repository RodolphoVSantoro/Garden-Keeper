#ifndef EXT_H
#define EXT_H

#define GRAUS2RAD(x) M_PI*x/180.0
#define RAD2GRAUS(x) 180.0*x/M_PI
#define coseno(x) cos(GRAUS2RAD(x))
#define seno(x) sin(GRAUS2RAD(x))

int sgn(double n){
    if(n>0)return 1;
    if(n<0) return -1;
    return 0;
}

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

#include "tiposExt.h"

//*******Prototipos de funcoes*********//

//******janela.h******//
void GetDimensoesTela(int *altura, int *largura);
void SetDimensoesJanela(int altura, int largura, int indjanela);
void GetDimensoesJanela(int *altura, int *largura, int indjanela);
bool foraJanela(int x, int y, int idJanela);
void EscreveFPS(int idFonte,int idJanela);

//******fisica.h******//
sistemaFisico* criaSistemaFisico();
void adicionaPontoMaterial(sistemaFisico *F, int id,int hp, int dano, double px, double py, double vx, double vy, tipoPonto t, double mass);
void rmvPontoMaterial(sistemaFisico *F, int id);
void libera(sistemaFisico *F);
int colide(sistemaFisico *F, int id, tipoPonto t);
bool foraJanela(sistemaFisico *F, int id, int idJanela);
void moveObjetos(sistemaFisico *F, Cena *C);

//*****Cena.h*********//
void criaCena(Cena **C, int idJanela);
void libera(nodeobj *n);
void liberaCena(Cena **C);
int adicionaNode(Cena *C, int id, int layer, tipoOBJ tO, bool userHandled, int px, int py, double vx, double vy, tipoPonto tP, int idLuz, int hp, int dano, tipoMagia tM);
void rmvNode(Cena *C, int id);

void setBackGroundImage(Cena *C, int idImg);
void setBackGroundImage(Cena *C, char *fname);
void adicionaObjeto(Cena *C, int idObj, int layer, tipoOBJ t);
void adicionaObjeto(Cena *C, char name[128], int layer, tipoOBJ t);
int criaProjetil(Cena *C, char* fname, int ox, int oy, double vx, double vy, int mx, int my, int dano, tipoMagia tM, int hp=0, int layer=0);
int criaInimigoSegue(Cena *C, char* fname, int px, int py, int hp, int dano, int layer);
void moveObjetos(Cena *C);
void DesenhaCena(Cena *C, bool moveObj, bool desIluminacao);

#define FIM 0
#define PERDEU 1
#define RETRY 2
#define PAUSE 3
#define MIDROUND 4
#define ENDROUND 5
int estadoAnterior=FIM;
int estadoJogo=MIDROUND;

int criados=0;
int vivos=0;

#include "janela.h"
#include "iluminacao.h"
#include "GUI.h"
#include "fisica.h"
#include "cena.h"

#endif // EXT_H
