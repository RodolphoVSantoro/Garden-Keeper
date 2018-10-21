#include <iostream>
#include <time.h>
#include <math.h>
#define STARTINGPOINTS 300
#define STARTINGROUND 1
#include "../src/jogoBruno.h"

using namespace std;

PIG_Evento evento;
PIG_Teclado meuTeclado;

int main( int argc, char* args[] ) {
    CriaJogo("Meu Jogo",0,300,500);

    Cena *C=NULL;
    int larJanela,altJanela;
    int player=-1,px,py,p_largura,p_altura,hpPlayer=2;
    tipoMagia tM;
    int essencia[NMAGIAS];
    bool comprados[NMAGIAS];
    int pontos,pontosGastos;
    int nRound, toSpawn, spawned;
    double tempoEspera,tCooldown;
    int timerTeclado=-1,cooldown=-1,timerInimigos=-1,timerEndRound=-1,timerPAUSE=-1;
    //int timerLimFPS=CriaTimer();
    Botao *botaoRetry,*botaoSaida;

    carregaJogo(player,p_largura,p_altura,&botaoRetry,&botaoSaida,larJanela,altJanela);
    criaTimersJogo(timerTeclado,cooldown,timerInimigos,timerEndRound,timerPAUSE);
    setupPartida(&C,player,px,py,larJanela,altJanela,pontos,pontosGastos,nRound,toSpawn,spawned,tempoEspera,hpPlayer,comprados,essencia,tM,tCooldown,STARTINGROUND);
    meuTeclado = GetTeclado();
    estadoJogo=MIDROUND;

    while(JogoRodando() && estadoJogo!=FIM){
        /*if(TempoDecorrido(timerLimFPS)>0.005){
            ReiniciaTimer(timerLimFPS);
        }
        else continue;*/
        evento = GetEvento();
        pressOrReleaseMouse(evento);
        if(evento.tipoEvento==MOUSE_RODINHA){
            printf("%d\n",evento.mouse.relY);
        }
        IniciaDesenho();
        if(estadoJogo==MIDROUND || estadoJogo==ENDROUND){
            //Movimentacao
            if(TempoDecorrido(timerTeclado)>playerSpeedCooldown){
                bool moveu = movimentacaoPlayer(C, px, py, larJanela, altJanela, p_largura, p_altura,meuTeclado);
                if(moveu)
                    ReiniciaTimer(timerTeclado);
            }
            //Magias
            trocaMagia(C,tM,comprados,meuTeclado,evento);
            if(TempoDecorrido(cooldown)>tCooldown){
                tCooldown=0.0;
                bool jogou = jogaMagia(C,px,py,larJanela,altJanela,p_largura,p_altura,evento,meuTeclado,essencia,tCooldown,tM);
                if(jogou){
                    ReiniciaTimer(cooldown);
                }
            }

            //PAUSE
            if(meuTeclado[TECLA_ESC] && TempoDecorrido(timerPAUSE)>0.2){
                ReiniciaTimer(timerPAUSE);
                estadoAnterior=estadoJogo;
                estadoJogo=PAUSE;
            }
        }
        if(estadoJogo==MIDROUND){
            //inimigos
            if(TempoDecorrido(timerInimigos)>3.0/((nRound)*log((double)nRound+1.0)) && vivos<50 && toSpawn>0){
                criaFormigas(C,larJanela,altJanela,maxHPFormiga);
                ReiniciaTimer(timerInimigos);
                criados++;
                vivos++;
                toSpawn--;
                spawned++;
            }
            if(toSpawn<=0 && vivos==0){
                toSpawn=calculaQTDInimigos(nRound+1);
                maxHPFormiga=calculaHPInimigos(nRound+1);
                estadoJogo=ENDROUND;
                ReiniciaTimer(timerEndRound);
            }
        }

        DesenhaCena(C,estadoJogo!=PAUSE);
        pontos=(criados-vivos)*60+pontosDeHits-pontosGastos+STARTINGPOINTS;
        escreveNumero(pontos,50,200);
        escreveNumero(nRound,50,100);
        escreveNumero(C->F->hpPlayer,larJanela/2,50);
        EscreveFPS();

        if(estadoJogo==ENDROUND){
            double t = TempoDecorrido(timerEndRound);
            float L;
            if(crescente)L=LUZFUNDO+(20*t/tempoEspera);
            else L=LUZFUNDO-(20*t/tempoEspera);
            if(L==200)crescente=false;
            else if(L==20)crescente=true;
            setLuzFundo(L);
            escreveNumero(t,700,700);
            if(t>tempoEspera){
                if(LUZFUNDO==200)crescente=false;
                else if(LUZFUNDO==20)crescente=true;
                if(crescente)LUZFUNDO+=20;
                else LUZFUNDO-=20;
                setLuzFundo(LUZFUNDO);
                nRound++;
                estadoJogo=MIDROUND;
            }
        }

        if(estadoJogo==PAUSE){
            telaPause(&botaoRetry, &botaoSaida, evento);
            PausaTimer(timerEndRound);
            if(meuTeclado[TECLA_ESC] && TempoDecorrido(timerPAUSE)>0.2){
                DespausaTimer(timerEndRound);
                estadoJogo=estadoAnterior;
                estadoAnterior=PAUSE;
                ReiniciaTimer(C->F->timerAT);
                ReiniciaTimer(timerPAUSE);
            }
        }

        if(estadoJogo==PERDEU){
            telaDerrota(criados-vivos,&botaoRetry,&botaoSaida,evento);
        }
        else{
            telaEssencia(C,tM,essencia);
        }

        if(estadoJogo==MIDROUND || estadoJogo==ENDROUND){
            menuCompra(pontos,pontosGastos,comprados,essencia,evento,meuTeclado);
        }

        if(estadoJogo==RETRY){
            setupPartida(&C,player,px,py,larJanela,altJanela,pontos,pontosGastos,nRound,toSpawn,spawned,tempoEspera,hpPlayer,comprados,essencia,tM,tCooldown,STARTINGROUND);
            estadoJogo=MIDROUND;
        }

        EncerraDesenho();
    }

    liberaCena(&C);
    FinalizaJogo();

    return 0;
}
