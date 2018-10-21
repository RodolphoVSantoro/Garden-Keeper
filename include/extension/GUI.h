#ifndef GUI_H
#define GUI_H
#include <string.h>

class Botao{
    private:
        int idImagem, idImagemMouseSobre, idImagemPressionado;
        int idFonte;
        int x,y,altura,largura,n_args;
        void (*funcPress)(int, va_list);
        char textoMouse[128];
        bool pressionado=false;

        void Desenha(int idImg){
            MoveObjeto(idImg,x,y);
            SetDimensoesObjeto(idImg,altura,largura);
            DesenhaObjeto(idImg);
        }
    public:
        Botao(int idImg, int idImgMouse, int idImgPressionado, int alt, int larg, int n_arg=0, void (*f)(int, va_list)=NULL, int px=0, int py=0, char text[128]="",int idfonte=-1){
            idImagem=idImg;
            idImagemMouseSobre=idImgMouse;
            idImagemPressionado=idImgPressionado;
            altura=alt;
            largura=larg;
            x=px;
            y=py;
            n_args=n_arg;
            funcPress=f;
            strcpy(textoMouse,text);
            if(idfonte==-1)
                idFonte = CriaFonteNormal("../fontes/arial.ttf",20,BRANCO,0,PRETO);
            else
                idFonte=idfonte;
        }
        Botao(char nomeImg[128], char nomeImgMouseSobre[128], char nomeImgPressionado[128], int alt, int larg, int n_arg=0, void (*f)(int, va_list)=NULL, int px=0, int py=0, char text[128]="",int idfonte=-1){
            int larBotao,altBotao;
            idImagem=CriaObjeto(nomeImg);
            GetDimensoesObjeto(idImagem,&altBotao,&larBotao);
            idImagemMouseSobre=CriaObjeto(nomeImgMouseSobre);
            SetDimensoesObjeto(idImagemMouseSobre,altBotao,larBotao);
            idImagemPressionado=CriaObjeto(nomeImgPressionado);
            SetDimensoesObjeto(idImagemPressionado,altBotao,larBotao);
            altura=alt;
            largura=larg;
            x=px;
            y=py;
            n_args=n_arg;
            funcPress=f;
            strcpy(textoMouse,text);
            if(idfonte==-1)
                idFonte = CriaFonteNormal("../fontes/arial.ttf",20,BRANCO,0,PRETO);
            else
                idFonte=idfonte;
        }
        void Pressionado(PIG_Evento &evento,int n_args,...){
            int mx,my;
            if(evento.tipoEvento==EVENTO_MOUSE){
                if(evento.mouse.acao==MOUSE_PRESSIONADO){
                    if(evento.mouse.botao==MOUSE_ESQUERDO){
                        mx=evento.mouse.posX;
                        my=evento.mouse.posY;
                        if(mx>=x && mx<=x+largura && my>=y && my<=y+altura){
                            if(pressionado==false){
                                pressionado=true;
                            }
                        }
                    }
                }
                else
                if(evento.mouse.acao==MOUSE_LIBERADO && pressionado==true){
                    if(funcPress!=NULL){
                        va_list args;
                        va_start(args,n_args);
                        funcPress(n_args,args);
                        va_end(args);
                    }
                    pressionado=false;
                }
                else
                if(evento.mouse.acao==MOUSE_MOVIDO && pressionado==true){
                    mx=evento.mouse.posX;
                    my=evento.mouse.posY;
                    if(mx<x || mx>x+largura || my<y || my>y+altura){
                        pressionado=false;
                    }
                }
            }
        }
        void DesenhaBotao(PIG_Evento evento){
            int i,j;
            i=evento.mouse.posX;
            j=evento.mouse.posY;
            //se esta Sobre do botao
            if(pressionado==true){
                Desenha(idImagemPressionado);
            }
            else{
                if(i>=x && i<=x+largura && j>=y && j<=y+altura){
                    Desenha(idImagemMouseSobre);
                    if(textoMouse[0]!=0){
                        EscreverEsquerda(textoMouse,i,j,idFonte);
                    }
                }
                else{
                    Desenha(idImagem);
                }
            }
        }
        void setDimensoes(int alt, int larg){
            SetDimensoesObjeto(idImagem,alt,larg);
            SetDimensoesObjeto(idImagemMouseSobre,alt,larg);
            SetDimensoesObjeto(idImagemPressionado,alt,larg);
            altura=alt;
            largura=larg;
        }
        void getDimensoes(int *alt, int *larg){
            *alt=altura;
            *larg=largura;
        }
        void setTexto(char str[128]){
            strcpy(textoMouse,str);
        }
};

#endif //GUI_H
