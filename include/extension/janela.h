#ifndef JAN_H
#define JAN_H

void GetDimensoesTela(int *altura, int *largura){
    SDL_DisplayMode DM;
    if(SDL_GetDesktopDisplayMode(0, &DM)==0){
        *largura = DM.w;
        *altura = DM.h;
    }
    else
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
}

void SetDimensoesJanela(int altura, int largura, int indjanela=0){
    SDL_Window* J = CGerenciadorJanelas::GetWindow(indjanela);
    SDL_SetWindowSize(J,largura,altura);
}

void GetDimensoesJanela(int *altura, int *largura, int indjanela=0){
    SDL_Window* J = CGerenciadorJanelas::GetWindow(indjanela);
    SDL_GetWindowSize(J,largura,altura);
}

bool foraJanela(int x, int y, int idJanela=0){
    int a,l;
    GetDimensoesJanela(&a,&l,idJanela);
    if(x<0)  return true;
    if(x>=l) return true;
    if(y<0)  return true;
    if(y>=a) return true;
    return false;
}

void EscreveFPS(int idFonte=0, int idJanela=0){
    char fps[32];
    sprintf(fps,"%.2f",GetFPS());
    int altura,largura;
    GetDimensoesJanela(&altura,&largura,idJanela);
    EscreverEsquerda(fps,largura/140,altura-altura/20,idFonte);
}

//recebe numeros para a altura e largura, e multiplica dimensoes do objeto por esses dois numeros
void resizeObjeto(int id, float multiplyHeight, float multiplyWidth){
    int alturaObjeto,larguraObjeto;
    GetDimensoesObjeto(id,&alturaObjeto,&larguraObjeto);
    SetDimensoesObjeto(id,alturaObjeto*multiplyHeight,larguraObjeto*multiplyWidth);
}

void getCentroObjeto(int id, int *cx, int *cy){
    int largura,altura;
    GetDimensoesObjeto(id,&altura,&largura);
    int px,py;
    GetXYObjeto(id,&px,&py);
    (*cx)=px+largura/2;
    (*cy)=py+altura/2;
}

#define ALTURAPADRAO 1080.0
#define LARGURAPADRAO 1920.0

float relativeAltura,relativeLargura;

#endif // JAN_H
