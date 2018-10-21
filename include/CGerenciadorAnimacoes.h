#include "CAnimacao.h"
class CGerenciadorAnimacoes{

private:
    static int totalAnimacoes;
    static PoolNumeros numAnimacoes;
    static Animacao animacoes[MAX_ANIMACOES];

public:

    static Animacao GetAnimacao(int id_animacao){
        return animacoes[id_animacao];
    }

    static void Inicia(SDL_Renderer *renderer=NULL){
        totalAnimacoes = 0;
        for (int i=0;i<MAX_ANIMACOES;i++)
            animacoes[i] = NULL;
        numAnimacoes = new CPoolNumeros(MAX_ANIMACOES);
    }

    static void Encerra(){
        for (int i=0;i<MAX_ANIMACOES;i++){
            if (animacoes[i]) delete animacoes[i];
        }
        delete numAnimacoes;
    }

    static int CriaAnimacao(char* nomeArquivoBMP,int retiraFundo=1,int opacidade=255,int idJanela=0){
        int resp = numAnimacoes->RetiraLivre();
        animacoes[resp] = new CAnimacao(0,0,nomeArquivoBMP,retiraFundo,opacidade,idJanela);
        totalAnimacoes++;
        return resp;
    }

    static int CriaAnimacao(int id_animacaoBase){
        int resp = numAnimacoes->RetiraLivre();
        animacoes[resp] = new CAnimacao(0,0,animacoes[id_animacaoBase]);
        totalAnimacoes++;
        return resp;
    }

    static void DestroiAnimacao(int id_animacao){
        numAnimacoes->DevolveUsado(id_animacao);
        delete animacoes[id_animacao];
        totalAnimacoes--;
        animacoes[id_animacao] = NULL;
    }

    inline static void Move(int id_animacao,int px,int py){
        animacoes[id_animacao]->Move(px,py);
    }

    inline static void GetPosicaoXY(int id_animacao, int *x,int *y){
        animacoes[id_animacao]->GetXY(*x,*y);
    }

    inline static int Desenha(int id_animacao){
        return animacoes[id_animacao]->Desenha();
    }

    inline static void CriaFrame(int id_animacao,int codFrame,int x,int y,int altura,int largura){
        animacoes[id_animacao]->CriaFrame(codFrame,x,y,altura,largura);
    }

    inline static void CriaModo(int id_animacao,int idModo, int loop){
        animacoes[id_animacao]->CriaModo(idModo,loop);
    }

    inline static void InsereFrame(int id_animacao, int idModo,int idFrame, float delayFrame){
        animacoes[id_animacao]->InsereFrame(idModo,idFrame,delayFrame);
    }

    inline static void MudaModo(int id_animacao,int idModo,int inicia){
        animacoes[id_animacao]->MudaModo(idModo,inicia);
    }

    inline static void SetOpacidade(int id_animacao,int valor){
        animacoes[id_animacao]->SetOpacidade(valor);
    }

    inline static void SetColoracao(int id_animacao,PIG_Cor cor){
        animacoes[id_animacao]->SetColoracao(cor);
    }

    inline static void SetPivo(int id_animacao,int x,int y){
        animacoes[id_animacao]->SetPivo(x,y);
    }

    inline static void GetPivo(int id_animacao,int *x,int *y){
        animacoes[id_animacao]->GetPivo(*x,*y);
    }

    inline static void SetAngulo(int id_animacao,float valor){
        animacoes[id_animacao]->SetAngulo(valor);
    }

    inline static float GetAngulo(int id_animacao){
        return animacoes[id_animacao]->GetAngulo();
    }

    inline static int ColisaoAnimacoes(int id_animacao1,int id_animacao2){
        return animacoes[id_animacao1]->Colisao(animacoes[id_animacao2]);
    }

    inline static int ColisaoObjeto(int id_animacao,Objeto obj){
        return animacoes[id_animacao]->Colisao(obj);
    }

    inline static void SetDimensoes(int id_animacao,int altura,int largura){
        animacoes[id_animacao]->SetDimensoes(altura,largura);
    }

    inline static void GetDimensoes(int id_animacao,int *altura,int *largura){
        animacoes[id_animacao]->GetDimensoes(*altura,*largura);
    }

    inline static void SetValorIntAnimacao(int idAnimacao, int chave, int valor){
        animacoes[idAnimacao]->SetValoresInt(chave,valor);
    }

    inline static void SetValorIntAnimacao(int idAnimacao, std::string chave, int valor){
        animacoes[idAnimacao]->SetValoresInt(chave,valor);
    }

    inline static void SetValorFloatAnimacao(int idAnimacao, int chave, float valor){
        animacoes[idAnimacao]->SetValoresFloat(chave,valor);
    }

    inline static void SetValorFloatAnimacao(int idAnimacao, std::string chave, float valor){
        animacoes[idAnimacao]->SetValoresFloat(chave,valor);
    }

    inline static void SetValorStringAnimacao(int idAnimacao, int chave, std::string valor){
        animacoes[idAnimacao]->SetValoresString(chave,valor);
    }

    inline static void SetValorStringAnimacao(int idAnimacao, std::string chave, std::string valor){
        animacoes[idAnimacao]->SetValoresString(chave,valor);
    }

    inline static bool GetValorIntAnimacao(int idAnimacao, int indice, int *valor){
        return animacoes[idAnimacao]->GetValoresInt(indice,*valor);
    }

    inline static bool GetValorFloatAnimacao(int idAnimacao, int indice, float *valor){
        return animacoes[idAnimacao]->GetValoresFloat(indice,*valor);
    }

    inline static bool GetValorStringAnimacao(int idAnimacao, int indice, char *valor){
        std::string str;
        bool resp = animacoes[idAnimacao]->GetValoresString(indice,str);
        strcpy(valor,str.c_str());
        return resp;
    }

    inline static bool GetValorIntAnimacao(int idAnimacao, std::string indice, int *valor){
        return animacoes[idAnimacao]->GetValoresInt(indice,*valor);
    }

    inline static bool GetValorFloatAnimacao(int idAnimacao, std::string indice, float *valor){
        return animacoes[idAnimacao]->GetValoresFloat(indice,*valor);
    }

    inline static bool GetValorStringAnimacao(int idAnimacao, std::string indice, char *valor){
        std::string str;
        bool resp = animacoes[idAnimacao]->GetValoresString(indice,str);
        strcpy(valor,str.c_str());
        return resp;
    }

};

PoolNumeros CGerenciadorAnimacoes::numAnimacoes;
int CGerenciadorAnimacoes::totalAnimacoes;
Animacao CGerenciadorAnimacoes::animacoes[MAX_ANIMACOES];

