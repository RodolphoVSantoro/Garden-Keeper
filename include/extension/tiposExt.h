#ifndef TIPOSEXT_H
#define TIPOSEXT_H

typedef unsigned long long int ull;

//----------------iluminacao.h--------------------

typedef struct{
    int posX,posY;
    unsigned char intensidade;
    int raio;
    int idJanela;
    SDL_Texture *texture;
}PONTOLUZ;

//-----------------ED.h---------------------

typedef struct NODEINT{
    int v;
    struct NODEINT *prox;
}nodeInt;

typedef struct PILHAINT{
    nodeInt *top;
}pilhaInt;

//-------------------------fisica.h---------------------------------------

typedef enum tipoPontoMaterial{PROJETIL=0,PLAYER=1,NPC=2,OBSTACULO=3,NONE=4} tipoPonto;

//defines pro status
#define NOSTATUS 0
#define ONFIRE 1
#define FIREDAMAGE 1000
#define PARALISIS 2
#define SLOW 4

typedef struct NODEPONTOMATERIAL{
    int id;
    int idPLuz;
    int timer;
    double posX, posY; // em pixels
    double velX, velY; //pixels/s
    double massa; //ainda nao implementado
    double hp,dano;
    int status;
    tipoPonto tipo; //define como vai ser calculado movimento, colisoes, etc.
    tipoMagia tM;
    struct NODEPONTOMATERIAL *prox;
}nodePontoMaterial;

typedef struct PILHAPARTICULA{
    nodePontoMaterial *primeiro;
    double absoluteTime; //segundos que se passaram
    int xPlayer,yPlayer,hpPlayer,maxhpPlayer,timerHPPlayer;
    int timerAT;
}sistemaFisico;

//------------------cena.h--------------------------------

typedef enum tipoOBJ{OBJETO,ANIMACAO,LUZ}tipoobj;

typedef struct NODEOBJ{
    int id;
    int layer;
    bool userHandled;
    int idLuz;
    tipoOBJ tipo;
    struct NODEOBJ *prox;
}nodeobj;

typedef struct CENA{
    nodeobj *primeiro;
    int idJanela;
    int bgImage;
    bool bgUHandle;
    sistemaFisico *F;
}Cena;

#endif // TIPOSEXT_H
