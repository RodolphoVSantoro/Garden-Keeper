class CParticula: public CObjeto{

private:
Timer timer;
int idTimer;
int xini,yini;
int dx,dy;
int menorX,maiorX,menorY,maiorY;
int hp;
int fading;
float limTempo;
float angRot;
float escalaInicial,escalaFinal;

public:
    int viva;

    CParticula(int x,int y,int deslocX,int deslocY,int vida, float anguloRot, float escalaIni, float escalaFim, int fadeOut, char *nomeArq,int usaGerenciadorTimer=1,int idJanela=0):CObjeto(x,y,nomeArq,idJanela){
        xini = x;
        yini = y;
        dx = deslocX;
        dy = deslocY;
        hp = vida;
        angRot = anguloRot;
        escalaInicial = escalaIni;
        escalaFinal = escalaFim;
        fading = fadeOut;

        if (usaGerenciadorTimer){
            timer = NULL;
            idTimer = CGerenciadorTimers::CriaTimer();
        }else{
            timer = new CTimer(false);
            idTimer = -1;
        }

        viva = 1;
    }

    void DefineLimites(int xMenor,int xMaior,int yMenor,int yMaior, float tempoMax){
        menorX = xMenor;
        maiorX = xMaior;
        menorY = yMenor;
        maiorY = yMaior;
        limTempo = tempoMax;
    }

    ~CParticula(){
        if (timer){
            delete timer;
        }else{
            CGerenciadorTimers::DestroiTimer(idTimer);
        }

    }

    void Move(){
        if (!viva) return;
        float decorrido;
        if (timer){
            decorrido = timer->GetTempoDecorrido();
        }else{
            decorrido = CGerenciadorTimers::GetTempoDecorrido(idTimer);
        }
        float porcTempoVida = decorrido/limTempo;
        if (porcTempoVida>1.0f){
            viva = false;
            return;
        }

        if (fading)
            CObjeto::SetOpacidade(255*(1-porcTempoVida));

        int alt,larg;
        CObjeto::GetDimensoesOriginais(alt,larg);

        float fatorDim = escalaInicial*(1-porcTempoVida)+escalaFinal*(porcTempoVida);
        CObjeto::SetDimensoes(alt*fatorDim,larg*fatorDim);
        CObjeto::SetPivo(0.5f,0.5f);
        CObjeto::SetAngulo(decorrido*angRot);
        x = xini + dx*decorrido;
        y = yini + dy*decorrido;

        CObjeto::Move(x,y);
        viva = (x>menorX)&&(x<maiorX)&&(y>menorY)&&(y<maiorY);
    }

    void MudaDirecao(int deslocX,int deslocY){
        dx = deslocX;
        dy = deslocY;
    }

    int Colisao(Objeto outro){
        int resp = CObjeto::Colisao(outro);
        if (resp){
            hp--;
            viva = hp>0;
        }
        return resp;
    }

    int Colisao(CParticula *outra){
        int resp = CObjeto::Colisao(outra);
        if (resp){
            hp--;
            viva = hp>0;

            outra->hp--;
            outra->viva = hp> 0;
        }
        return resp;
    }
};

typedef CParticula* Particula;
