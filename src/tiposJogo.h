#ifndef TIPOS_JOGO_H
#define TIPOS_JOGO_H

#define PLAYERSTARTHP 2
double playerSpeedCooldown=0.016;

#define NMAGIAS 9
#define ICE_PELLET 4
#define TEXT_FAGULHA "No cost(Infinite ammo)"
#define IMG_FAGULHA "../imagens/menuBuy/fagulha.png"
typedef enum TIPOMAGIA{NOTMAGIC=-1,FAGULHA=0, LASIER=1, BRISA=2, FIREBALL=3, TESLA=4, GLACIAL=5, SUPERNOVA=6, TROVAO=7, ICESTORM=8,ICEPELLET=9}tipoMagia;

int CUSTO_MAGIA[NMAGIAS] = {0,2000,3000,7000,7500,8000,15000,12000,18000};
int CUSTO_ESSENCIA[NMAGIAS] = {0,500,500,1500,1750,2000,5000,4000,6000};
int QTD_ESSENCIA[NMAGIAS] = {0, 100, 70, 50, 50, 50, 30, 80, 30};
//terceiro 800 e da ice pellet
int DANO_MAGIA[NMAGIAS] = {150,800,800,6000,800,10000,160000,40000,10000};
//12 da ice pellet
int SPEED_MAGIA[NMAGIAS] = {20,100,6,30,100,40,2,300,0};
//o segundo 0.06 e do tesla mesmo
double COOLDOWN_MAGIA[NMAGIAS+1] = {0.15,0.06,0.9,0.45,0.3,0.3,3,0.045,12,3};
//cooldown de ice pellets na icestorm
#define COOLDOWN_PONTOS 0.2
double COOLDOWN_ICE=0.1;
//define quanto vai durar
int hpICESTORM = 150;
int hpFAGULHA = 60;
int hpICEPELLET = 30;

int LARGURA_MAGIA[NMAGIAS] = {14,39,56,70,27,132,141,141,141};

int ALTURA_MAGIA[NMAGIAS] = {14,11,56,70,27,98,141,70,101};

float COOLDOWN_HPMAGIA[NMAGIAS+1] = {0.01,0,0,0,0,0,0,0,0,0.01};

char IMG_MENU_COMPRA[NMAGIAS][100] =
                                    {
                                        "../imagens/menuBuy/fagulha.png",
                                        "../imagens/menuBuy/lasier.png",
                                        "../imagens/menuBuy/brisa.png",
                                        "../imagens/menuBuy/fireball.png",
                                        "../imagens/menuBuy/tesla.png",
                                        "../imagens/menuBuy/glacial.png",
                                        "../imagens/menuBuy/supernova.png",
                                        "../imagens/menuBuy/trovao.png",
                                        "../imagens/menuBuy/icestorm.png",
                                    };

char IMG_MAGIA[NMAGIAS][100] = {
                                    "../imagens/magias/fagulha.png",
                                    "../imagens/magias/lasier.png",
                                    "../imagens/magias/brisa.png",
                                    "../imagens/magias/fireball.png",
                                    "../imagens/magias/icePellet.png",
                                    "../imagens/magias/glacial.png",
                                    "../imagens/magias/supernova.png",
                                    "../imagens/magias/trovao.png",
                                    "../imagens/magias/icestorm.png"
                                };


char NOME_MAGIA[NMAGIAS][100] = {
                                    "Fagulha",
                                    "Lasier",
                                    "Brisa",
                                    "Fireball",
                                    "Tesla",
                                    "Glacial",
                                    "Supernova",
                                    "Thundera",
                                    "Ice Storm"
                                };

int enemyHealth[210] = {0,100,150,250,350,500,650,750,850,950,1100};
int QTDenemy[210] = {6};
int maxHPFormiga;

#endif
