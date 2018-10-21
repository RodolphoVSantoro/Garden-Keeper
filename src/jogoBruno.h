#ifndef JOGOBRUNO_H
#define JOGOBRUNO_H

#include "PIG.h"
#include "../src/tiposJogo.h"
#include "extension/ext.h"

static int max(int x, int y){
    if(x>y) return x;
    else return y;
}

static double max(double x, double y){
    if(x>y) return x;
    else return y;
}

static int min(int x, int y){
    if(x<y) return x;
    else return y;
}

static double min(double x, double y){
    if(x<y) return x;
    else return y;
}

Botao *botaoMenuCompra[NMAGIAS];
int FundoMenuCompra=-1;
int idFonteHUD;
int larJanela,altJanela;

//Inicializações
void carregaJogo(int &player, int &p_largura, int &p_altura, Botao* *ret, Botao* *fim);
void criaTimersJogo(int &timeTeclado, int &cooldown, int &timerInimigos, int &timerEndRound, int &timerPAUSE);
void setupPartida(Cena **C, int player, int &px, int &py, int larJanela, int altJanela, int &pontos, int &pontosGastos, int &nRound, int &toSpawn, int &spawned, double &tempoEspera, int &hpPlayer, bool comprados[NMAGIAS], int essencia[NMAGIAS], tipoMagia &tM, double &tCooldown, int startRound);
int calculaHPInimigos(int nRound);
int calculaQTDInimigos(int nRound);

//compra
void FCompra(int n_arg, va_list v);
static Botao* criaBotaoMenu(tipoMagia tM, int px, int py);
bool menuCompra(int &pontos, int &pontosGastos, bool comprada[NMAGIAS], int essencia[NMAGIAS], PIG_Evento &evento, PIG_Teclado meuTeclado);
//va_list={int &pontos, int &pontosGastos, bool comprada[NMAGIAS], int essencia[NMAGIAS],int n}
//n_arg=5

//Player
bool trocaMagia(Cena *C, tipoMagia &tipoM, bool comprados[NMAGIAS], PIG_Teclado meuTeclado, PIG_Evento &evento);
bool jogaMagia(Cena *C, int px, int py, int p_largura, int p_altura, PIG_Evento &evento, PIG_Teclado meuTeclado, int essencia[NMAGIAS], double &tCooldown, tipoMagia tipo=FAGULHA);
bool movimentacaoPlayer(Cena *C, int &px, int &py, int p_largura, int p_altura, PIG_Teclado meuTeclado);

//HUD
void escreveNumero(int numero, int px, int py, int idFonte=0);
void escreveNumero(double numero, int px, int py, int idFonte=0);
void FRetry(int n_arg,va_list v);
void FExit(int n_arg, va_list v);
void telaPause(Botao **botaoRetry, Botao **botaoSaida, PIG_Evento &evento);
void telaDerrota(int score, Botao* *botaoRetry, Botao* *botaoSaida, PIG_Evento &evento);
void telaEssencia(Cena *C, tipoMagia tM, int essencia[NMAGIAS]);

//Inimigos
void criaFormigas(Cena *C,int maxHp, int dano=1);


void carregaJogo(int &player, int &p_largura, int &p_altura, Botao* *ret, Botao* *fim){
    GetDimensoesJanela(&altJanela,&larJanela);
    relativeAltura=altJanela/ALTURAPADRAO;
    relativeLargura=larJanela/LARGURAPADRAO;
    //player
    player = CriaObjeto("../imagens/per.png");
    GetDimensoesObjeto(player,&p_altura,&p_largura);
    resizeObjeto(player,relativeAltura,relativeLargura);
    p_altura*=relativeAltura;
    p_largura*=relativeLargura;

    //HUD
    idFonteHUD=CriaFonteNormal("../fontes/arial.ttf",50*min(relativeAltura,relativeLargura),BRANCO,0,PRETO);

    //botoes
    int altBotao,larBotao;
    *ret = new Botao("../imagens/menuRetry/retry1.png","../imagens/menuRetry/retry2.png","../imagens/menuRetry/retry3.png",65,198,0,FRetry,larJanela/2-larJanela/11,altJanela/2-altJanela/15,"",idFonteHUD);
    (*ret)->getDimensoes(&altBotao,&larBotao);
    (*ret)->setDimensoes(altBotao*relativeAltura,larBotao*relativeLargura);
    *fim = new Botao("../imagens/menuRetry/sair1.png","../imagens/menuRetry/sair2.png","../imagens/menuRetry/sair3.png",65,90,0,FExit,larJanela/2+larJanela/27,altJanela/2-altJanela/15,"",idFonteHUD);
    (*fim)->getDimensoes(&altBotao,&larBotao);
    (*fim)->setDimensoes(altBotao*relativeAltura,larBotao*relativeLargura);

    for(int i=11;i<210;i++)
        enemyHealth[i]=-1;
    for(int i=1;i<210;i++)
        QTDenemy[i]=-1;
}

void criaTimersJogo(int &timerTeclado, int &cooldown, int &timerInimigos, int &timerEndRound, int &timerPAUSE){
    //teclado
    timerTeclado = CriaTimer();
    cooldown = CriaTimer();
    //Inimigos
    timerInimigos = CriaTimer();
    srand((unsigned int)time(NULL));
    timerEndRound = CriaTimer();
    timerPAUSE=CriaTimer();
}

int LUZFUNDO;
bool crescente;

void setupPartida(Cena **C, int player, int &px, int &py, int &pontos, int &pontosGastos, int &nRound, int &toSpawn, int &spawned, double &tempoEspera, int &hpPlayer, bool comprados[NMAGIAS], int essencia[NMAGIAS], tipoMagia &tM, double &tCooldown, int startRound=1){
    relativeAltura=altJanela/ALTURAPADRAO;
    relativeLargura=larJanela/LARGURAPADRAO;
    //player
    px=larJanela/2;
    py=altJanela/2;

    //Cena(fundo, imagens aparecendo e projeteis)
    if(*C!=NULL)
        liberaCena(C);
    criaCena(C);
    setBackGroundImage(*C,"../imagens/grass.png");
    int altBG,larBG;
    getDimensoesBackGroundImage(*C,&altBG,&larBG);
    setDimensoesBackGroundImage(*C,altBG*relativeAltura,larBG*relativeLargura);
    LUZFUNDO=20;
    crescente=true;
    setLuzFundo(LUZFUNDO);
    int poste = criaLuz(*C,"../imagens/Poste.png",3*larJanela/4,3*altJanela/5,0,255,706*max(relativeAltura,relativeLargura));
    int cx,cy;
    getCentroObjeto(poste,&cx,&cy);
    nodeobj *aux = busca(*C,poste);
    movePontoLuz(cx,cy,aux->idLuz);
    resizeObjeto(poste,relativeAltura,relativeLargura);

    adicionaPlayer(*C,player,1);
    movePlayer(*C,px,py);
    hpPlayer=PLAYERSTARTHP;
    setHPPlayer((*C)->F,hpPlayer);

    //magia
    tCooldown=0.0;
    pontos=0;
    pontosDeHits=0;
    pontosGastos=0;
    comprados[FAGULHA]=true;
    essencia[FAGULHA]=0;
    tM=FAGULHA;
    for(int i=1;i<NMAGIAS;i++){
        comprados[i]=false;
        essencia[i]=0;
    }

    //Inimigos
    nRound=startRound;
    toSpawn=calculaQTDInimigos(nRound);
    tempoEspera=7.5;
    spawned=vivos=criados=0;
    maxHPFormiga=calculaHPInimigos(nRound);

    //menu de compra
    int pxMenu=larJanela/5,pyMenu=altJanela/10;
    if(FundoMenuCompra==-1){
        FundoMenuCompra = CriaObjeto("../imagens/menuBuy/fundoMenu.png",1,127);
        resizeObjeto(FundoMenuCompra,relativeAltura,relativeLargura);
        MoveObjeto(FundoMenuCompra,pxMenu,pyMenu);
    }
    int altBotao,larBotao;

    int PX=pxMenu+larJanela/50,PY=pyMenu+altJanela/30;
    botaoMenuCompra[FAGULHA] = new Botao(IMG_MENU_COMPRA[FAGULHA],IMG_MENU_COMPRA[FAGULHA],IMG_MENU_COMPRA[FAGULHA],140,140,5,FCompra,PX,PY,"0 points(infinite essence)");
    botaoMenuCompra[FAGULHA]->getDimensoes(&altBotao,&larBotao);
    botaoMenuCompra[FAGULHA]->setDimensoes(altBotao*relativeAltura,larBotao*relativeLargura);

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(j==0 && i==0)j++;
            tipoMagia t = (tipoMagia)(j+i*3);
            botaoMenuCompra[t] = criaBotaoMenu(t,PX+j*larJanela/10,PY+i*altJanela/6);
            botaoMenuCompra[t]->getDimensoes(&altBotao,&larBotao);
            botaoMenuCompra[t]->setDimensoes(altBotao*relativeAltura,larBotao*relativeLargura);
        }
    }
}

static Botao* criaBotaoMenu(tipoMagia tM, int px, int py){
    char buff[200];
    sprintf(buff, "%d points", CUSTO_MAGIA[tM]);
    return new Botao(IMG_MENU_COMPRA[tM],IMG_MENU_COMPRA[tM],IMG_MENU_COMPRA[tM],140,140,6,FCompra,px,py,buff);
}

//va_list=int *pontos, int *pontosGastos, bool comprada[NMAGIAS], int essencia[NMAGIAS],int n
//n_arg=5
void FCompra(int n_arg, va_list v){
    int *pontos = va_arg(v,int*);
    int *pontosGastos = va_arg(v,int*);
    bool *comprada = va_arg(v,bool*);
    int *essencia = va_arg(v,int*);
    int tipo = va_arg(v,int);
    if(!comprada[tipo]){
        if(CUSTO_MAGIA[tipo] <= *pontos){
            if(n_arg==6){
                Botao *self = va_arg(v,Botao*);
                char txt[128];
                sprintf(txt,"%d points(%d)", CUSTO_ESSENCIA[tipo], QTD_ESSENCIA[tipo]);
                self->setTexto(txt);
            }
            comprada[tipo]=true;
            essencia[tipo]+=QTD_ESSENCIA[tipo];
            (*pontos)-=CUSTO_MAGIA[tipo];
            (*pontosGastos)+=CUSTO_MAGIA[tipo];
        }
    }
    else
        if(CUSTO_ESSENCIA[tipo] <= *pontos){
            essencia[tipo]+=QTD_ESSENCIA[tipo];
            (*pontos)-=CUSTO_ESSENCIA[tipo];
            (*pontosGastos)+=CUSTO_ESSENCIA[tipo];
        }
}

bool menuCompra(int &pontos, int &pontosGastos, bool comprada[NMAGIAS], int essencia[NMAGIAS], PIG_Evento &evento, PIG_Teclado meuTeclado){
    if(meuTeclado[TECLA_SHIFTESQUERDO]){
        DesenhaObjeto(FundoMenuCompra);
        botaoMenuCompra[FAGULHA]->DesenhaBotao(evento);
        botaoMenuCompra[FAGULHA]->Pressionado(evento,5,&pontos,&pontosGastos,comprada,essencia,FAGULHA);
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(i==0 && j==0)j++;
                tipoMagia t = (tipoMagia)(j+i*3);
                botaoMenuCompra[t]->DesenhaBotao(evento);
                botaoMenuCompra[t]->Pressionado(evento,6,&pontos,&pontosGastos,comprada,essencia,t,botaoMenuCompra[t]);
            }
        }
    }
}

bool movimentacaoPlayer(Cena *C, int &px, int &py, int p_largura, int p_altura, PIG_Teclado meuTeclado){
    static bool w=false, a=false, s=false, d=false;
    float k=(relativeAltura+relativeLargura)/2.0;
    if(meuTeclado[TECLA_d] && !a && px<=larJanela-p_largura){
        px=px+(7*k);
        d=true;
    }
    else d=false;
    if(meuTeclado[TECLA_a] && !d && px>=0){
        px=px-(7*k);
        a=true;
    }
    else a=false;
    if(meuTeclado[TECLA_w] && !s && py<=altJanela-p_altura){
        py=py+(7*k);
        w=true;
    }
    else w=false;
    if(meuTeclado[TECLA_s] && !w && py>=0){
        py=py-(7*k);
        s=true;
    }
    else s=false;
    if(a || d || s || w){
        movePlayer(C,px,py);
        return true;
    }
    return false;
}

/*
#define coseno(x) cos(M_PIx/180.0)
#define seno(x) sin(M_PIx/180.0)

ponto rotacionaPonto(ponto a, ponto b, double graus){
    ponto p;
    p.x = a.x + coseno(graus)*(b.x-a.x) + seno(graus)*(a.y-b.y);
    p.y = a.y + coseno(graus)*(b.y-a.y) + seno(graus)*(b.x-a.x);
    return p;
}
*/
bool aper=false;

void pressOrReleaseMouse(PIG_Evento &evento){
    if(evento.tipoEvento==EVENTO_MOUSE){
        if(evento.mouse.acao==MOUSE_PRESSIONADO){
            if(evento.mouse.botao==MOUSE_DIREITO)
                    aper=true;
        }
            else
                if(evento.mouse.acao==MOUSE_LIBERADO)
                    aper=false;
    }
}

bool jogaMagia(Cena *C, int px, int py, int p_largura, int p_altura, PIG_Evento &evento, PIG_Teclado meuTeclado, int essencia[NMAGIAS], double &tCooldown, tipoMagia tipo){
    if(meuTeclado[TECLA_BARRAESPACO] || aper){
        px+=p_largura/2;
        py+=p_altura/2;
        int mx=evento.mouse.posX, my=evento.mouse.posY;
        mx-=LARGURA_MAGIA[tipo]/2*relativeLargura;
        my-=ALTURA_MAGIA[tipo]/2*relativeAltura;
        if(tipo==FAGULHA || essencia[tipo]>0){
            double t = sqrt((double)(3125.0*SPEED_MAGIA[tipo]*relativeAltura*relativeLargura)/(double)((mx-px)*(mx-px)+(my-py)*(my-py)));
            double vx=(double)(mx-px)*t,vy=(double)(my-py)*t;
            if(tipo==TESLA){
                double x,y;
                for(double i=-11.25;i<=11.25;i+=7.5){
                    //seno e coseno em graus
                    x = px + coseno(i)*(mx-px) + seno(i)*(py-my);
                    vx = (x-px)*t;
                    y = py + coseno(i)*(my-py) + seno(i)*(mx-px);
                    vy = (y-py)*t;
                    for(int j=0;j<2;j++){
                        int rx=rand()%200;
                        int ry=rand()%200;
                        double auxX=vx+(rx)*sgn(vx);
                        double auxY=vy+(ry)*sgn(vy);
                        criaProjetil(C,IMG_MAGIA[LASIER],px,py,auxX,auxY,mx+auxX,my+auxY,DANO_MAGIA[LASIER],TESLA,0,2);
                    }
                }
            }
            else if(tipo==ICESTORM){
                criaProjetil(C,IMG_MAGIA[tipo],mx,my,0,0,mx,my,DANO_MAGIA[tipo],tipo,hpICESTORM,2);
            }
            else if(tipo==FAGULHA){
                criaProjetil(C, IMG_MAGIA[tipo], px, py, vx, vy, mx, my, DANO_MAGIA[tipo], tipo,hpFAGULHA,2);
            }
            else{
                criaProjetil(C, IMG_MAGIA[tipo], px, py, vx, vy, mx, my, DANO_MAGIA[tipo], tipo,0,2);
            }
            if(tipo!=FAGULHA) essencia[tipo]--;
            tCooldown=COOLDOWN_MAGIA[tipo];
            return true;
        }
        else return false;
    }
    else return false;
}

bool umVerdadeiro(bool vet[], int t){
    for(int i=0;i<t;i++)
        if(vet[i])
            return true;
    return false;
}

bool trocaMagia(Cena *C, tipoMagia &tipoM, bool comprados[NMAGIAS], PIG_Teclado meuTeclado, PIG_Evento &evento){
    static bool pressionado[NMAGIAS]={0};
    if(evento.tipoEvento==EVENTO_MOUSE){
        if(evento.mouse.acao==MOUSE_RODINHA){
            int r=-evento.mouse.relY;
            while(!comprados[mod(tipoM+r,NMAGIAS)]){
                r=r+sgn(r);
            }
            tipoM=(TIPOMAGIA)mod(((int)tipoM)+r,NMAGIAS);
        }
    }
    for(int i=TECLA_1,j=0;i<=TECLA_9;i++,j++){
        if(meuTeclado[i] && !umVerdadeiro(pressionado,NMAGIAS) && comprados[j]){
            tipoM=(tipoMagia)j;
            pressionado[j]=true;
        }
        else
            pressionado[j]=false;
    }
}

void criaFormigas(Cena *C,int maxHp, int dano){
    int rx,ry;
    char op;
    op=rand()%2;
    if(op){
        rx=rand()%(larJanela-100)+50;
        op=rand()%2;
        if(op)ry=altJanela-50;
        else  ry=0;
    }
    else{
        ry=rand()%(altJanela-100)+50;
        op=rand()%2;
        if(op)rx=larJanela-50;
        else  rx=0;
    }
    int id = criaInimigoSegue(C,"../imagens/Formiga.png",rx,ry,maxHp,dano);
    resizeObjeto(id,relativeAltura,relativeLargura);
}

void FRetry(int n_arg, va_list v){
    estadoJogo=RETRY;
}

void FExit(int n_arg, va_list v){
    estadoJogo=FIM;
}

void telaPause(Botao **botaoRetry, Botao **botaoSaida, PIG_Evento &evento){
    int altura=altJanela,largura=larJanela,idJanela=0;
    EscreverCentralizada("Pause",largura/2,altura/2,idFonteHUD);
    (*botaoRetry)->DesenhaBotao(evento);
    (*botaoRetry)->Pressionado(evento,0);
    (*botaoSaida)->DesenhaBotao(evento);
    (*botaoSaida)->Pressionado(evento,0);
}

void telaDerrota(int score, Botao **botaoRetry, Botao **botaoSaida, PIG_Evento &evento){
    char pontos[32];
    sprintf(pontos,"%d Kills",score);
    int altura=altJanela,largura=larJanela,idJanela=0;
    EscreverCentralizada("You're Ant Food",largura/2,altura/2+altura/15,idFonteHUD);
    EscreverCentralizada(pontos,largura/2,altura/2+altura/100,idFonteHUD);
    (*botaoRetry)->DesenhaBotao(evento);
    (*botaoRetry)->Pressionado(evento,0);
    (*botaoSaida)->DesenhaBotao(evento);
    (*botaoSaida)->Pressionado(evento,0);
}

void telaEssencia(Cena *C, tipoMagia tM, int essencia[NMAGIAS]){
    int x=larJanela,y=altJanela;
    x=x-x/13;
    y=y/7;
    EscreverCentralizada(NOME_MAGIA[tM],x,y/2,idFonteHUD);
    if(tM==FAGULHA || tM==NOTMAGIC){
        EscreverEsquerda("inf",x,y,idFonteHUD);
    }
    else{
        escreveNumero(essencia[tM],x,y,idFonteHUD);
    }
}

void escreveNumero(int numero, int px, int py, int idFonte){
    char str[128];
    sprintf(str,"%d", numero);
    EscreverEsquerda(str,px,py,idFonte);
}

void escreveNumero(double numero, int px, int py, int idFonte){
    char str[128];
    sprintf(str,"%.2f", numero);
    EscreverEsquerda(str,px,py,idFonte);
}

int calculaQTDInimigos(int nRound){
    if(nRound<210 && QTDenemy[nRound]!=-1)
        return QTDenemy[nRound];
    else
        return (nRound)*log(nRound+1)/2+calculaQTDInimigos(nRound-1);
}

int calculaHPInimigos(int nRound){
    if(nRound<210 && enemyHealth[nRound]!=-1)
        return enemyHealth[nRound];
    else
        return 2*(nRound)*log(nRound)*log(nRound)+calculaHPInimigos(nRound-1);
}

#endif // JOGOBRUNO_H
