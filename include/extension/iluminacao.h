#ifndef ILUMINACAO_H
#define ILUMINACAO_H
#define MAXPONTOSLUZ 5000
#define MAXRAIO 4096
#define MAX_ALPHA 255
#define SIZEOF_ARGB 4
#include <stdio.h>
#include <math.h>

#include "ED.h"

static int modulo(int x){
    if(x<0)
        return -x;
    else
        return x;
}

static SDL_Rect* criaRect(int x, int y, int w, int h){
    SDL_Rect *r = (SDL_Rect*)malloc(sizeof(SDL_Rect));
    r->h=h;
    r->w=w;
    r->x=x;
    r->y=y;
    return r;
}

class ShadowMap{
private:
    int lastPontoluz;

public:
    pilhaInt *indPontosEmJanela[MAX_JANELAS];
    SDL_Texture *Mapa[MAX_JANELAS];
    SDL_Renderer *renderer[MAX_JANELAS];
    int altura[MAX_JANELAS];
    int largura[MAX_JANELAS];

    unsigned char luzFundo[MAX_JANELAS];
    PONTOLUZ *pontosLuz[MAXPONTOSLUZ];
    bool IluminacaoModificada[MAX_JANELAS];

    ShadowMap(){
        lastPontoluz=0;
        memset(altura,0,sizeof(altura));
        memset(largura,0,sizeof(largura));
        memset(luzFundo,0,sizeof(luzFundo));
        for(int i=0;i<MAX_JANELAS;i++)IluminacaoModificada[MAX_JANELAS]=false;
    }
    void iniciaShadowMap(int idJanela=0){
        indPontosEmJanela[idJanela]=criaPilhaInt();
        for(int i=0;i<MAXPONTOSLUZ;i++)pontosLuz[i]=NULL;

        SDL_Window* J = CGerenciadorJanelas::GetWindow(idJanela);
        SDL_GetWindowSize(J,&largura[idJanela],&altura[idJanela]);

        renderer[idJanela] = CGerenciadorJanelas::GetJanela(idJanela)->GetRenderer();
        Mapa[idJanela] = SDL_CreateTexture(renderer[idJanela],SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET
                                             ,largura[idJanela],altura[idJanela]);
    }

    void FinalizaShadowMap(int idJanela=0){
        libera(indPontosEmJanela[idJanela]);
        for(int i=0;i<MAXPONTOSLUZ;i++){
            if(pontosLuz[i]!=NULL){
                SDL_DestroyTexture(pontosLuz[i]->texture);
                free(pontosLuz[i]);
            }
        }

        SDL_DestroyTexture(Mapa[idJanela]);
    }

    void criaTexturaLuz(PONTOLUZ *p){
        unsigned char luz;
        int aux;
        int centroX=(int)ceil(p->raio/2.0);
        int centroY=centroX;
        const int fadeOut=255;//p->intensidade;
        unsigned char *pixels = (unsigned char*)malloc(sizeof(unsigned char)*(p->raio*p->raio*4)*SIZEOF_ARGB);
        for(int i=0;i<(p->raio*p->raio*4)*SIZEOF_ARGB;i++)pixels[i]=0;
        for(int i=-p->raio;i<p->raio;i++){
            const int h=sqrt(p->raio*p->raio - i*i);
            for(int j=-h;j<=h;j++){
                const int x=i+p->raio,y=j+p->raio;
                aux=p->intensidade-((fadeOut*sqrt(i*i+j*j))/(p->raio));

                if(aux>255.0)luz=255;
                else if(aux<0)luz=0;
                else  luz=(unsigned char)aux;

                const unsigned int offset = ( (2*p->raio) * SIZEOF_ARGB * x ) + y * 4;
                pixels[offset+0]=luz;
                pixels[offset+1]=luz;
                pixels[offset+2]=luz;
                pixels[offset+3]=MAX_ALPHA;
            }
        }
        SDL_UpdateTexture(p->texture, NULL, pixels, (2*p->raio) * SIZEOF_ARGB);
        free(pixels);
    }

    int CriaPontoLuz(int x, int y, unsigned char intensidade,int raio,int idJanela){
        int i=0;
        //busca ponto de luz n usado no vetor
        for(i=0;i<lastPontoluz && pontosLuz[i]!=NULL;i++);
        //if nao encontrou
        if(i==MAXPONTOSLUZ)return -1;
        //pontos de luz ja usados
        if(i==lastPontoluz)lastPontoluz++;

        push(indPontosEmJanela[idJanela],i);

        pontosLuz[i]=(PONTOLUZ*)malloc(sizeof(PONTOLUZ));
        pontosLuz[i]->posX=x;
        pontosLuz[i]->posY=altura[idJanela]-y-1;
        pontosLuz[i]->idJanela=idJanela;
        pontosLuz[i]->intensidade=intensidade;
        if(raio>MAXRAIO) pontosLuz[i]->raio=MAXRAIO;
        else if(raio<0)pontosLuz[i]->raio=0;
        else pontosLuz[i]->raio=raio;
        int w=2*raio;
        int h=2*raio;
        pontosLuz[i]->texture=SDL_CreateTexture(renderer[idJanela],SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,w,h);
        criaTexturaLuz(pontosLuz[i]);
        return i;
    }

    void destroiPontoLuz(int idPonto){
        rmv(indPontosEmJanela[pontosLuz[idPonto]->idJanela],idPonto);
        free(pontosLuz[idPonto]);
        pontosLuz[idPonto]=NULL;
    }

    void desenhaTexturaLuz(PONTOLUZ *p, int idJanela){
        SDL_SetRenderTarget(renderer[idJanela],Mapa[idJanela]);
        int x = p->posX-p->raio;
        int y = p->posY-p->raio;
        int w = 2*p->raio;
        int h = 2*p->raio;
        SDL_Rect *rect = criaRect(x,y,w,h);
        SDL_SetTextureBlendMode(p->texture,SDL_BLENDMODE_ADD);
        SDL_RenderCopy(renderer[idJanela],p->texture,NULL,rect);
        free(rect);
        SDL_SetRenderTarget(renderer[idJanela],NULL);
    }

    void resetLuzFundo(int idJanela=0){
        SDL_SetRenderTarget(renderer[idJanela],Mapa[idJanela]);
        SDL_SetRenderDrawColor(renderer[idJanela],luzFundo[idJanela],luzFundo[idJanela],luzFundo[idJanela],MAX_ALPHA);
        SDL_RenderClear(renderer[idJanela]);
        SDL_SetRenderTarget(renderer[idJanela],NULL);
    }

    void desenhaLuzes(int idJanela=0){
        resetLuzFundo(idJanela);

        pilhaInt *aux = criaPilhaInt();
        int ind;

        while(!vazia(indPontosEmJanela[idJanela])){
            ind=pop(indPontosEmJanela[idJanela]);
            push(aux,ind);
            desenhaTexturaLuz(pontosLuz[ind],idJanela);
        }

        while(!vazia(aux)){
            ind=pop(aux);
            push(indPontosEmJanela[idJanela],ind);
        }

        libera(aux);
    }
};

ShadowMap shadowmap;

void IniciaShadowMap(int idJanela=0){
    shadowmap.iniciaShadowMap(idJanela);
}

void finalizaShadowMap(int idJanela=0){
    shadowmap.FinalizaShadowMap(idJanela);
}

int criaPontoLuz(int x, int y, unsigned char intensidade, int raio, int idJanela=0){
    int idPontoLuz = shadowmap.CriaPontoLuz(x,y,intensidade,raio,idJanela);
    shadowmap.IluminacaoModificada[idJanela]=true;
    return idPontoLuz;
}

void destroiPontoLuz(int idPontoLuz, int idJanela=0){
    shadowmap.destroiPontoLuz(idPontoLuz);
    shadowmap.IluminacaoModificada[idJanela]=true;
}

void movePontoLuz(int x,int y, int idPonto){
    int idJanela=shadowmap.pontosLuz[idPonto]->idJanela;
    shadowmap.pontosLuz[idPonto]->posX=x;
    shadowmap.pontosLuz[idPonto]->posY=shadowmap.altura[idJanela]-y-1;
    shadowmap.IluminacaoModificada[shadowmap.pontosLuz[idPonto]->idJanela]=true;
}

void setPontoLuzintensidade(unsigned char intensidade, int idPonto){
    shadowmap.pontosLuz[idPonto]->intensidade=intensidade;
    shadowmap.IluminacaoModificada[shadowmap.pontosLuz[idPonto]->idJanela]=true;
}

void setLuzFundo(unsigned char intensidade, int idJanela=0){
    shadowmap.luzFundo[idJanela]=intensidade;
    shadowmap.IluminacaoModificada[idJanela]=true;
}

void DesenhaIluminacao(int idJanela=0){
    if(shadowmap.IluminacaoModificada){
        shadowmap.desenhaLuzes();
    }
    SDL_SetTextureBlendMode(shadowmap.Mapa[idJanela],SDL_BLENDMODE_MOD);
    SDL_RenderCopy(shadowmap.renderer[idJanela],shadowmap.Mapa[idJanela],NULL,NULL);
}

#endif
