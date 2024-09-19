/*
五子棋核心代码
*/
#include "gobang.h"

int chessboard[15][15];
int isAi[3]={0,0,0};
int CurrentStep=0;

node Steps[MAXSTEP];



char num2char(int num){
    if(num<10)  return num+'0';
    else return num-10+'a';
}

int char2num(char c){
    if(c>='0'&&c<='9')  return c-'0';
    else return c-'a'+10;
}

char readchar(){
    char c=getchar();
    while(!((c>='0'&&c<='9')||(c>='a'&&c<='z'))){
        c=getchar();
    }
    return c;
}

void drawEmpty(int x,int y){
    if(x==0){
        if(y==0) printf("%s","┏");
        else if(y==14) printf("┓");
        else printf("┯");
    }else if(x==14){
        if(y==0) printf("┗");
        else if(y==14) printf("┛");
        else printf("┷");
    }else{
        if(y==0) printf("┠");
        else if(y==14) printf("┨");
        else printf("┼");
    }
}

void draw(){
    //system("cls");
    printf(" 0123456789abcde\n");
    for(int i=0;i<15;i++){
        printf("%c",num2char(i));
        for(int j=0;j<15;j++){
            if(chessboard[i][j]==0){
                drawEmpty(i,j);
            }else if(chessboard[i][j]==BLACK){
                printf("●");
            }else if(chessboard[i][j]==WHITE){
                printf("○");
            } 
        }
        printf("\n");
    }
}

int play(int x,int y,int color){
    //实现落子操作
    #ifdef DEBUG
        printf("play():x:%d y:%d color:%d\n",x,y,color);
    #endif
    if(x<0||x>=15||y<0||y>=15){
        printf("超出棋盘范围\n");
        return -1;
    }
    if(chessboard[x][y]!=0){
        printf("该位置已有棋子\n");
        return -1;
    }
    if(color!=1&&color!=2){
        printf("棋子颜色错误\n");
        return -1;
    }
    Steps[++CurrentStep].x=x,Steps[CurrentStep].y=y,Steps[CurrentStep].color=color;
    chessboard[x][y]=color;
    if(isWin(x,y,color)){
        if(color==BLACK){
            printf("黑棋胜利");
        }else{
            printf("白棋胜利");
        }
        draw();
        return 2;
    }
    return 0;
}

int regret(){
    //实现悔棋操作
    if(CurrentStep==0){
        printf("无棋可悔\n");
        return -1;
    }else if(CurrentStep==1){
        chessboard[Steps[CurrentStep].x][Steps[CurrentStep].y]=0;
        CurrentStep--;
        return 0;
    }
    chessboard[Steps[CurrentStep].x][Steps[CurrentStep].y]=0;
    CurrentStep--;
    chessboard[Steps[CurrentStep].x][Steps[CurrentStep].y]=0;
    CurrentStep--;
    return 0;
}

int isWin(int x,int y,int color){
    int dir[4][2]={{1,0},{0,1},{1,1},{1,-1}};
    for(int i=0;i<4;i++){
        int count=1;
        for(int j=1;j<5;j++){
            int nx=x+dir[i][0]*j;
            int ny=y+dir[i][1]*j;
            if(nx<0||nx>15||ny<0||ny>15) break;
            if(chessboard[nx][ny]==color) count++;
            else break;
        }
        for(int j=1;j<5;j++){
            int nx=x-dir[i][0]*j;
            int ny=y-dir[i][1]*j;
            if(nx<0||nx>15||ny<0||ny>15) break;
            if(chessboard[nx][ny]==color) count++;
            else break;
        }
        if(count>=5) return 1;
    }
    return 0;
}
    
void begin(){
    
    memset(chessboard,0,sizeof(chessboard));
    printf("欢迎来到五子棋游戏\n");
    //判断是人机对战还是人人对战
    printf("请选择游戏模式：\n");
    printf("1.人人对战\n");
    printf("2.人机对战\n");
    int mode;
    scanf("%d",&mode);
    //判断机器执黑还是执白
    if(mode==2){
        printf("请选择机器执棋颜色：\n");
        printf("1.黑棋\n");
        printf("2.白棋\n");
        int aiColor;
        scanf("%d",&aiColor);
        if(aiColor==1) isAi[BLACK]=1;
        else isAi[WHITE]=1;
    }
    switch (mode){
    case 1:
        humanHumanPlay();
        break;

    case 2:
        humanAiPlay();
        break;
    }
}

void humanHumanPlay(){
    int color=BLACK;
    while(1){
        draw();
        if(color==BLACK) printf("黑棋下,");
        else printf("白棋下,");
        printf("请输入坐标：");
        char x,y;
        x=readchar();
        if(x=='r'){
            int status=regret();
            if(status==-1) continue;
            color=3-color;
            continue;
        }
        y=readchar();
        getchar();
        int res=play(char2num(x),char2num(y),color);
        if(res==-1){
            printf("请重下\n");
            continue;
        }else if(res==2){
            break;
        }else if(res==1){
            continue;
        }
        
        color=3-color;
    }
    end();
}

void humanAiPlay(){
    int color=BLACK;
    while(1){
        int res=0;
        if(isAi[color]){
            res=aiPlay(color);
        }else{
            res=humanPlay(color);
        }
        if(res==-1){
            #ifdef DEBUG
                printf("humanAiPlay():exit with code %d\n",res);
            #endif
            printf("请重下\n");
            continue;
        }else if(res==2){
            break;
        }else if(res==1){
            continue;
        }
        color=3-color;
    }
    #ifdef DEBUG
       printf("humanAiPlay():exit\n");
    #endif
    end();
}

int aiPlay(int color){
    printf("AI思考中...\n");
    ll alpha=-LLINF,beta=LLINF;
    ll score=analysis(chessboard,color,MAXDEPTH,alpha,beta);
    #ifdef DEBUG
        printf("aiPlay():x=%d,y=%d,score=%lld\n",decision.pos.x,decision.pos.y,score);
    #endif
    int res=play(decision.pos.x,decision.pos.y,color);
    printf("AI下在%c,%c\n",num2char(decision.pos.x),num2char(decision.pos.y));
    return res;
}

int humanPlay(int color){
    draw();
    printf("请输入坐标：");
    char x,y;
    x=readchar();
    if(x=='r'||y=='r'){
        int status=regret();
        if(status==-1) return 0;
        return 1;
    }
    y=readchar();
    getchar();
    int res=play(char2num(x),char2num(y),color);
    return res;
}

int main(){
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif    //解决Windows下中文乱码问题

    begin();
    return 0;
}

void end(){
    printf("游戏结束\n");
    printf("是否复盘?\n");
    printf("1.是\n");
    printf("2.否\n");
    int choice;
    scanf("%d",&choice);
    if(choice==1){
        replay();
    }

}

void replay(){
    printf("复盘中...\n");
    memset(chessboard,0,sizeof(chessboard));
    for(int i=1;i<=CurrentStep;i++){
        printf("第%d步:",i);
        printf("%c,%c\n",num2char(Steps[i].x),num2char(Steps[i].y));
        printf("棋盘状态：\n");
        chessboard[Steps[i].x][Steps[i].y]=Steps[i].color;
        draw();
        system("pause");
    }
    printf("复盘结束\n");
}