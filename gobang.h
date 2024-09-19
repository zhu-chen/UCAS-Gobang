#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

// #define DEBUG
//非调试模式下删除
#define MAXDEPTH 6
//自定义搜索深度
#define MAXLENGTH 15
//自定义可选落点长度


#define BLACK 1
#define WHITE 2
#define SIZE 15
#define ll long long
#define INTINF 0x3f3f3f3f
#define LLINF 0x3fffffffffffffff
#define MAXSTEP 300
typedef struct node{
    int x,y,color;
}node;


//gobang.c
void draw();
int play(int x,int y,int color);
void drawEmpty(int x,int y);
int isWin(int x,int y,int color);
int isBan(int x,int y,int color);
void begin();
void end();
void replay();
int regret();
void humanAiPlay();
void humanHumanPlay();
int humanPlay(int color);
int aiPlay(int color);
char num2char(int x);
int char2num(char x);
    
extern int chessboard[15][15];//1:黑棋 2:白棋
extern int CurrentStep;

//score.c
typedef struct STATUS{
    ll Statusnum[10];
}STATUS;
ll getWholeScore(int (*board)[15],int color);
ll getSingleScore(int (*board)[15],node p,int isopp);
STATUS getStatus(int (*board)[15],node p);
int isOutBound(int x,int y);

//ai.c
typedef struct SEEKPOINTS{
    node pos[MAXLENGTH];
    ll score[MAXLENGTH];
}SEEKPOINTS;
ll analysis(int (*board)[15],int color,int depth,ll alpha,ll beta);
SEEKPOINTS getPoints(int (*board)[15],int color);
typedef struct DECISION{
    node pos;
    ll score;
}DECISION;
extern DECISION decision;