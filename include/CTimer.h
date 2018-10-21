class CTimer{

private:
    long int inicio;
    long int pausa;
    bool pausado;

public:
    CTimer(bool congelado){
        Reinicia(congelado);
    }

    ~CTimer(){
    }

    float GetTempoDecorrido(){
        if (pausado) return (pausa-inicio)/1000.0;
        else         return (clock()-inicio)/1000.0;
    }

    void Pausa(){
        if (!pausado){
            pausado = true;
            pausa = clock();
        }
    }

    void Despausa(){
        if (pausado){
            long int decorrido = pausa - inicio;
            inicio = clock() - decorrido;
            pausado = false;
        }
    }

    void Reinicia(bool congelado){
        inicio = clock();
        pausado = congelado;
        if (congelado)
            pausa = inicio;
    }

    CTimer* Copia(){
        CTimer* outro = new CTimer(pausado);
        outro->inicio = inicio;
        outro->pausa = pausa;
        return outro;
    }
};

typedef CTimer* Timer;
