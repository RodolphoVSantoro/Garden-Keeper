#ifndef FIS_H
#define FIS_H
#include "tiposJogo.h"
#include <math.h>

sistemaFisico* criaSistemaFisico(){
    sistemaFisico *F = (sistemaFisico*)malloc(sizeof(sistemaFisico));
    F->primeiro=NULL;
    F->absoluteTime=0.0;
    F->timerAT=CriaTimer();
    F->timerHPPlayer=CriaTimer();
    return F;
}

void setHPPlayer(sistemaFisico *F, int hp){
    F->hpPlayer=F->maxhpPlayer=hp;
}

void adicionaPontoMaterial(sistemaFisico *F, int id, int hp, int dano, double px, double py, double vx, double vy, tipoPonto t, tipoMagia tm=NOTMAGIC, double mass=0){
    if(t==PLAYER){
        F->xPlayer=px;
        F->yPlayer=py;
    }
    nodePontoMaterial *n = (nodePontoMaterial*)malloc(sizeof(nodePontoMaterial));

    n->posX=px;
    n->posY=py;
    n->velX=vx;
    n->velY=vy;
    n->id=id;
    n->tipo=t;
    n->massa=mass;
    n->hp=hp;
    n->dano=dano;
    n->tM=tm;
    n->status=NOSTATUS;
    if(t==NPC)
        n->timer=CriaTimer();
    if(t==PROJETIL && (tm!=BRISA && tm!=GLACIAL && tm!=ICEPELLET && tm!=ICESTORM)){
        int largura,altura,r;
        GetDimensoesObjeto(id,&altura,&largura);
        r=std::max(largura,altura);
        n->idPLuz=criaPontoLuz(px,py,255,r);
    }
    else n->idPLuz=-1;
    if(tm==ICESTORM || tm==ICEPELLET || tm==BRISA || tm==FAGULHA){
        n->timer=CriaTimer();
    }

    n->prox=F->primeiro;
    F->primeiro=n;
}

void rmvPontoMaterial(sistemaFisico *F, int id){
    nodePontoMaterial *aux=F->primeiro,*ant=NULL;
    while(aux!=NULL){
        if(aux->id==id){
            if(ant==NULL)
                F->primeiro=aux->prox;
            else
                ant->prox=aux->prox;
            if(aux->tipo==NPC)
                vivos--;
            if(aux->tipo==PROJETIL && aux->idPLuz!=-1)
                destroiPontoLuz(aux->idPLuz);
            free(aux);
            break;
        }
        ant=aux;
        aux=aux->prox;
    }
}

void libera(sistemaFisico *F){
    nodePontoMaterial *aux=F->primeiro;
    while(aux!=NULL){
        F->primeiro=aux->prox;
        free(aux);
        aux=F->primeiro;
    }
    DestroiTimer(F->timerAT);
    free(F);
}

// retorna -1 se o ponto material n colide, ou o id do ponto que colidiu
int colide(sistemaFisico *F, int id, tipoPonto t){
    nodePontoMaterial *aux=F->primeiro,*ant=NULL;
    while(aux!=NULL){
        //nao teste colisao de objeto com ele mesmo!!!
        if(aux->id!=id){
            if((aux->tipo==NPC || aux->tipo==OBSTACULO) && t==PROJETIL){
                if(TestaColisaoObjetos(aux->id,id)){
                    return aux->id;
                }
            }
            if(aux->tipo==PLAYER && t==NPC){
                if(TestaColisaoObjetos(aux->id,id)){
                    return aux->id;
                }
            }
        }
        aux=aux->prox;
    }
    return -1;
}

bool foraJanela(sistemaFisico *F, int id, int idJanela=0){
    nodePontoMaterial *aux=F->primeiro;
    while(aux!=NULL){
        if(id==aux->id)
            return foraJanela((int)aux->posX,(int)aux->posY,idJanela);
        aux=aux->prox;
    }
    return false;
}

void movePlayer(Cena *C, int x, int y){
    nodePontoMaterial *aux=C->F->primeiro;
    int id=-1;
    while(aux!=NULL){
        if(aux->tipo==PLAYER){
            C->F->xPlayer=x;
            C->F->yPlayer=y;
            MoveObjeto(aux->id,x,y);
        }
        aux=aux->prox;
    }
}

nodePontoMaterial* busca(sistemaFisico *F, int id){
    nodePontoMaterial *aux=F->primeiro;
    while(aux!=NULL && aux->id!=id)
        aux=aux->prox;
    return aux;
}

/*void imprime(sistemaFisico *F){
    nodePontoMaterial *per=F->primeiro;
    while(per!=NULL){
        printf("%d, ", per->id);
        per=per->prox;
    }
    printf("\n");
}*/

void danoEmArea(sistemaFisico *F, double px, double py, int dano){
    nodePontoMaterial *aux=F->primeiro;
    while(aux!=NULL){
        if(aux->tipo==NPC){
            aux->hp-=(int)(dano/sqrt(((px-aux->posX)*(px-aux->posX)+(py-aux->posY)*(py-aux->posY))));
        }
        aux=aux->prox;
    }
}

int pontosDeHits=0;

void moveObjetos(sistemaFisico *F, Cena *C=NULL){
    if(estadoJogo==PERDEU)return;
    nodePontoMaterial *aux=F->primeiro,*ant=NULL,*tmp=NULL;
    double t=TempoDecorrido(F->timerAT),lambda;
    ReiniciaTimer(F->timerAT);
    F->absoluteTime+=t;
    int hit,largura,altura,playerhit=0;
    bool fJanela;
    int larObj,altObjt,randX;
    bool removeu=false;
    while(aux!=NULL){
        removeu=false;
        switch(aux->tipo){
        case PLAYER:
            if(F->hpPlayer<F->maxhpPlayer){
                if(TempoDecorrido(F->timerHPPlayer)>2.0){
                    F->hpPlayer++;
                    ReiniciaTimer(F->timerHPPlayer);
                }
            }
        break;
        case PROJETIL:
            tmp=aux;
            if((aux->tM==ICESTORM && TempoDecorrido(aux->timer)>COOLDOWN_ICE)){
                    GetDimensoesObjeto(aux->id,&altObjt,&larObj);
                    randX=rand()%larObj+aux->posX;
                    criaProjetil(C,IMG_MAGIA[ICE_PELLET],randX,aux->posY+altObjt/3,0,-400,aux->posX,aux->posY-10,DANO_MAGIA[ICE_PELLET],ICEPELLET,hpICEPELLET);
                    aux->hp--;
                    ReiniciaTimer(aux->timer);
            }
            if((aux->tM==ICEPELLET || aux->tM==FAGULHA) && TempoDecorrido(aux->timer)>COOLDOWN_HPMAGIA[aux->tM]){
                aux->hp-=1;
                ReiniciaTimer(aux->timer);
            }
            if((tmp->tM==ICEPELLET || tmp->tM==ICESTORM || tmp->tM==FAGULHA) && tmp->hp<=0){
                    aux=aux->prox;
                    rmvNode(C,tmp->id);
            }
            else{
                hit=colide(F,aux->id,aux->tipo);
                fJanela=foraJanela(aux->posX,aux->posY);
                if(fJanela || hit!=-1){
                    if(C!=NULL){
                        if(hit!=-1){
                            nodePontoMaterial *In=busca(F,hit);
                            if(In!=NULL){
                                if(In->hp>0){
                                    if(tmp->tM!=BRISA && tmp->tM!=ICESTORM){
                                        pontosDeHits+=10;
                                        In->hp-=tmp->dano;
                                    }
                                    else{
                                        if(tmp->tM==BRISA && TempoDecorrido(tmp->timer)>COOLDOWN_PONTOS){
                                            pontosDeHits+=10;
                                            ReiniciaTimer(tmp->timer);
                                        }
                                        In->hp-=tmp->dano*t;
                                    }
                                }
                                double vx=100,vy=100;
                                switch(tmp->tM){
                                    case BRISA:
                                        In->status=In->status|SLOW;
                                    break;
                                    case FIREBALL:
                                        In->status+=ONFIRE;
                                    break;
                                    case TROVAO:
                                        In->status+=PARALISIS;
                                    break;
                                    case GLACIAL:
                                        criaProjetil(C,IMG_MAGIA[ICE_PELLET],tmp->posX,tmp->posY,-vx,0,tmp->posX-10,tmp->posY,DANO_MAGIA[ICE_PELLET],ICEPELLET,hpICEPELLET);
                                        criaProjetil(C,IMG_MAGIA[ICE_PELLET],tmp->posX,tmp->posY,-vx,vy,tmp->posX-10,tmp->posY+10,DANO_MAGIA[ICE_PELLET],ICEPELLET,hpICEPELLET);
                                        criaProjetil(C,IMG_MAGIA[ICE_PELLET],tmp->posX,tmp->posY,0,vy,tmp->posX,tmp->posY+10,DANO_MAGIA[ICE_PELLET],ICEPELLET,hpICEPELLET);
                                        criaProjetil(C,IMG_MAGIA[ICE_PELLET],tmp->posX,tmp->posY,vx,vy,tmp->posX+10,tmp->posY+10,DANO_MAGIA[ICE_PELLET],ICEPELLET,hpICEPELLET);
                                        criaProjetil(C,IMG_MAGIA[ICE_PELLET],tmp->posX,tmp->posY,vx,0,tmp->posX+10,tmp->posY,DANO_MAGIA[ICE_PELLET],ICEPELLET,hpICEPELLET);
                                        criaProjetil(C,IMG_MAGIA[ICE_PELLET],tmp->posX,tmp->posY,vx,-vy,tmp->posX+10,tmp->posY-10,DANO_MAGIA[ICE_PELLET],ICEPELLET,hpICEPELLET);
                                        criaProjetil(C,IMG_MAGIA[ICE_PELLET],tmp->posX,tmp->posY,0,-vy,tmp->posX,tmp->posY-10,DANO_MAGIA[ICE_PELLET],ICEPELLET,hpICEPELLET);
                                        criaProjetil(C,IMG_MAGIA[ICE_PELLET],tmp->posX,tmp->posY,-vx,-vy,tmp->posX-10,tmp->posY-10,DANO_MAGIA[ICE_PELLET],ICEPELLET,hpICEPELLET);
                                    break;
                                    case SUPERNOVA:
                                        danoEmArea(F, tmp->posX, tmp->posY, DANO_MAGIA[tmp->tM]);
                                    break;
                                }
                            }
                        }
                        if((tmp->tM!=BRISA && tmp->tM!=ICESTORM) ||
                           (tmp->tM==BRISA && fJanela)){
                            removeu=true;
                            aux=aux->prox;
                            rmvNode(C,tmp->id);
                        }
                    }
                    else{
                        removeu=true;
                        aux=aux->prox;
                        rmvPontoMaterial(F,tmp->id);
                        rmvPontoMaterial(F,hit);
                    }
                }
                if(!removeu){
                    aux->posX+=aux->velX*t;
                    aux->posY+=aux->velY*t;
                    MoveObjeto(aux->id,(int)aux->posX,(int)aux->posY);
                    if(aux->idPLuz!=-1){
                        GetDimensoesObjeto(aux->id,&altura,&largura);
                        movePontoLuz((int)aux->posX+largura/2,(int)aux->posY+altura/2,aux->idPLuz);
                    }
                }
            }
        break;
        case NPC:
            if(aux->hp<=0){
                rmvNode(C,aux->id);
            }
            else{
                if(aux->status&PARALISIS)
                    aux->velX=aux->velY=0;
                if(aux->status&ONFIRE)
                    aux->hp-=FIREDAMAGE;
                if(TempoDecorrido(aux->timer)>0.5){
                    if(colide(F,aux->id,aux->tipo)!=-1){
                        F->hpPlayer-=aux->dano;
                        ReiniciaTimer(F->timerHPPlayer);
                        playerhit=1;
                        ReiniciaTimer(aux->timer);
                        if(F->hpPlayer<=0){
                            estadoJogo=PERDEU;
                        }
                    }
                }
                else if(colide(F,aux->id,aux->tipo)!=-1) playerhit=1;
                if(playerhit==0){
                    float k=(relativeAltura+relativeLargura)/2.0;
                    lambda = 1.5*sqrt((double)6000.0/(double)((F->xPlayer-aux->posX)*(F->xPlayer-aux->posX)+(F->yPlayer-aux->posY)*(F->yPlayer-aux->posY)));
                    aux->velX=(F->xPlayer-aux->posX)*lambda*t*k;
                    aux->velY=(F->yPlayer-aux->posY)*lambda*t*k;
                    if(aux->status&SLOW){
                        aux->velX/=10;
                        aux->velY/=10;
                    }
                    aux->posX+=aux->velX;
                    aux->posY+=aux->velY;
                    playerhit=0;
                    MoveObjeto(aux->id,(int)aux->posX,(int)aux->posY);
                }
                playerhit=0;
            }
        break;
        }
        ant=aux;
        if(aux!=NULL)
            aux=aux->prox;
    }
}

#endif // FIS_H
