#ifndef ED
#define ED
#include "tiposExt.h"
#include <stdlib.h>

//recebe referência para um ponteiro para uma matriz de char
void CriaMatriz2d(unsigned char ***m, int altura, int largura){
    (*m) = (unsigned char**)malloc(sizeof(unsigned char*)*altura);
    for(int i=0;i<altura;i++){
        (*m)[i]=(unsigned char*)malloc(sizeof(unsigned char)*largura);
        for(int j=0;j<largura;j++)
            (*m)[i][j]=0;
    }
}

void LiberaMatriz2d(unsigned char ***m, int altura, int largura){
    for(int i=0;i<altura;i++)
        free((*m)[i]);
    free(*m);
    (*m)=NULL;
}

void initMatriz(unsigned char **m, int altura, int largura, unsigned char valor){
    for(int i=0;i<altura;i++)
        for(int j=0;j<largura;j++)
            m[i][j]=valor;
}

pilhaInt *criaPilhaInt(){
    pilhaInt *p = (pilhaInt*)malloc(sizeof(pilhaInt));
    p->top=NULL;
    return p;
}

void push(pilhaInt *p, int v){
    nodeInt *n = (nodeInt*)malloc(sizeof(nodeInt));
    n->v=v;
    n->prox=p->top;
    p->top=n;
}

int pop(pilhaInt *p){
    int v=p->top->v;
    nodeInt *aux=p->top->prox;
    free(p->top);
    p->top=aux;
    return v;
}

void rmv(pilhaInt *p, int v){
    nodeInt *aux=p->top,*ant=NULL;
    while(aux!=NULL){
        if(aux->v==v)
            break;
        ant=aux;
        aux=aux->prox;
    }
    if(ant==NULL){
        p->top=aux->prox;
        free(aux);
    }
    else{
        if(aux!=NULL){
            ant->prox=aux->prox;
            free(aux);
        }
    }
}

bool vazia(pilhaInt *p){
    return p->top==NULL;
}

void libera(pilhaInt *p){
    nodeInt *aux;
    while(p->top!=NULL){
        aux=p->top->prox;
        free(p->top);
        p->top=aux;
    }
    free(p);
}
#endif
