#ifndef CENA_H
#define CENA_H
#include <stdlib.h>
#include <math.h>
#include "tiposExt.h"
#include "fisica.h"
#define SEMBG -1

void criaCena(Cena **C, int idJanela=0){
    Cena *c = (Cena*)malloc(sizeof(Cena));
    c->primeiro=NULL;
    c->idJanela=idJanela;
    c->bgImage=-1;
    c->bgUHandle=true;
    c->F=criaSistemaFisico();
    (*C)=c;
    IniciaShadowMap(idJanela);
}

void libera(nodeobj *n){
    if(n->userHandled==false){
        if(n->tipo==ANIMACAO)
            DestroiAnimacao(n->id);
        else DestroiObjeto(n->id);
        if(n->tipo==LUZ)
            destroiPontoLuz(n->idLuz);
    }
    free(n);
}

void liberaCena(Cena **C){
    nodeobj *aux;
    while((*C)->primeiro!=NULL){
        aux=(*C)->primeiro->prox;
        libera((*C)->primeiro);
        (*C)->primeiro=aux;
    }
    if(!((*C)->bgUHandle))
        DestroiObjeto((*C)->bgImage);
    finalizaShadowMap((*C)->idJanela);
    libera((*C)->F);
    free(*C);
    (*C)=NULL;
}

int adicionaNode(Cena *C, int id, int layer, tipoOBJ tO, bool userHandled, int px=0, int py=0, double vx=0.0, double vy=0.0, tipoPonto tP=NONE, int idLuz=-1, int hp=0, int dano=0, tipoMagia tM=NOTMAGIC){
    nodeobj *n = (nodeobj*)malloc(sizeof(nodeobj)),*aux,*ant;
    n->id=id;
    MoveObjeto(id,px,py);
    n->tipo=tO;
    n->layer=layer;
    n->userHandled=userHandled;
    if(tO==LUZ)
        n->idLuz=idLuz;
    adicionaPontoMaterial(C->F,n->id,hp,dano,px,py,vx,vy,tP,tM);
    ant=NULL;
    aux=C->primeiro;
    while(aux!=NULL){
        if(aux->layer > n->layer)break;
        ant=aux;
        aux=aux->prox;
    }
    if(aux==C->primeiro){
        n->prox=C->primeiro;
        C->primeiro=n;
    }
    else{
        n->prox=aux;
        ant->prox=n;
    }
    return n->id;
}

void rmvNode(Cena *C, int id){
    nodeobj *percorre=C->primeiro,*ant=NULL,*tmp;
    while(percorre!=NULL){
        if(percorre->id==id){
            tmp=percorre;
            percorre=tmp->prox;
            if(ant!=NULL) ant->prox=percorre;
            else C->primeiro=percorre;
            rmvPontoMaterial(C->F,tmp->id);
            libera(tmp);
            break;
        }
        ant=percorre;
        percorre=percorre->prox;
    }
}

void setBackGroundImage(Cena *C, int idImg){
    C->bgImage=idImg;
    C->bgUHandle=true;
}

void setBackGroundImage(Cena *C, char *fname){
    int id = CriaObjeto(fname);
    C->bgImage=id;
    C->bgUHandle=false;
}

void getDimensoesBackGroundImage(Cena *C, int *altura, int *largura){
    GetDimensoesObjeto(C->bgImage,altura,largura);
}

void setDimensoesBackGroundImage(Cena *C, int altura, int largura){
    SetDimensoesObjeto(C->bgImage,altura,largura);
}

void adicionaObjeto(Cena *C, int idObj, int layer=0, tipoOBJ t=OBJETO){
    adicionaNode(C,idObj,layer,t,true,0,0,0,0,NONE);
}

void adicionaPlayer(Cena *C, int idObj, int layer=0, tipoOBJ t=OBJETO, int hp=1){
    adicionaNode(C,idObj,layer,t,true,0,0,0,0,PLAYER,-1,hp);
}

void adicionaPlayer(Cena *C, char name[128], int layer=0, tipoOBJ t=OBJETO, int hp=1){
    int idObj = CriaObjeto(name);
    adicionaNode(C,idObj,layer,t,false,0,0,0,0,PLAYER,-1,hp);
}

void adicionaObjeto(Cena *C, char name[128], int layer=0, tipoOBJ t=OBJETO){
    int id;
    if(t==OBJETO)
        id=CriaObjeto(name);
    else if(t==ANIMACAO)
        id=CriaAnimacao(name);
    adicionaNode(C,id,layer,t,false,0,0,0,0,NONE);
}

static int produtoEscalar(int p1x, int p1y, int p2x, int p2y){
    return p1x*p2x+p1y*p2y;
}

static double tamanho(double px, double py){
    return sqrt(px*px+py*py);
}

static double GetAngulo(int ox, int oy, int px, int py){
    px-=ox;
    py-=oy;
    ox-=10;
    oy=0;
    return acos(produtoEscalar(px,py,ox,oy)/(tamanho(ox,oy)*tamanho(px,py)))*sgn(py);
}

int criaProjetil(Cena *C, char* fname, int ox, int oy, double vx, double vy, int mx, int my, int dano, tipoMagia tM, int hp, int layer){
    int id = CriaObjeto(fname);
    resizeObjeto(id,relativeAltura,relativeLargura);
    if(tM!=NOTMAGIC && tM!=ICESTORM){
        int tLar,tAlt;
        GetDimensoesObjeto(id,&tAlt,&tLar);
        SetPivoObjeto(id,tLar/2,tAlt/2);
        double theta=RAD2GRAUS(GetAngulo(ox,oy,mx,my));
        SetAnguloObjeto(id,theta);
    }
    return adicionaNode(C,id,layer,OBJETO,false,ox,oy,vx,vy,PROJETIL,-1,hp,dano,tM);
}

int criaInimigoSegue(Cena *C, char* fname, int px, int py, int hp=1, int dano=1, int layer=0){
    int id = CriaObjeto(fname);
    adicionaNode(C,id,layer,OBJETO,false,px,py,0,0,NPC,-1,hp,dano);
    return id;
}

int criaLuz(Cena *C, char *fname, int x, int y, int layer=0, unsigned char intensidade=255, int raio=-1, int idJanela=0){
    int id=CriaObjeto(fname);
    int largura,altura;
    GetDimensoesObjeto(id,&altura,&largura);
    if(raio==-1){
        raio = std::max(largura,altura);
    }
    int luz = criaPontoLuz(x+largura/2,y+altura/2,intensidade,raio,idJanela);
    //retorna id pro obj
    return adicionaNode(C,id,layer,LUZ,false,x,y,0,0,NONE,luz);
}

void moveLuz(Cena *C, int idLuz, int x, int y){
    nodeobj *aux=C->primeiro;
    while(aux!=NULL && aux->id==idLuz)
        aux=aux->prox;
    if(aux!=NULL){
        movePontoLuz(x,y,aux->idLuz);
        MoveObjeto(aux->id,x,y);
    }
}

void destroiLuz(Cena *C, int idLuz){
    nodeobj *aux=C->primeiro;
    while(aux!=NULL && aux->id==idLuz)
        aux=aux->prox;
    if(aux!=NULL){
        destroiPontoLuz(aux->idLuz,C->idJanela);
        DestroiObjeto(aux->id);
    }
}

void moveObjetos(Cena *C){
    moveObjetos(C->F,C);
}

void DesenhaCena(Cena *C, bool moveObj=true, bool desIluminacao=true){
    if(moveObj)
        moveObjetos(C);
    nodeobj *aux=C->primeiro;
    if(C->bgImage!=SEMBG)
        DesenhaObjeto(C->bgImage);
    while(aux!=NULL){
        switch(aux->tipo){
        case LUZ:
        case OBJETO:
            DesenhaObjeto(aux->id);
        break;
        break;
        case ANIMACAO:
            DesenhaAnimacao(aux->id);
        break;
        }
        if(aux!=NULL)
            aux=aux->prox;
    }
    if(desIluminacao)
        DesenhaIluminacao(C->idJanela);
}

nodeobj* busca(Cena *C, int id){
    nodeobj *aux=C->primeiro;
    while(aux!=NULL && aux->id!=id)
        aux=aux->prox;
    return aux;
}

#endif // CENAH
