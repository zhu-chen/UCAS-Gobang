#include "gobang.h"

#define ALIVE5 0
#define ALIVE4 1
#define BLOCK4 2
#define ALIVE3 3
#define BLOCK3 4
#define ALIVE2 5
#define BLOCK2 6
#define ALIVE1 7

const ll score[2][8]={{100000000,500000,400,400,20,20,1,1},//己方分数
// {100000000,100000,100000,8000,50,50,3,3}};//对方分数
{100000000,500000,400,400,20,20,1,1}};

ll getWholeScore(int (*board)[15],int color){//计算整个棋盘的分数
    int oppColor=3-color;
    ll re=0;
    for(int i=0;i<SIZE;++i){
        for(int j=0;j<SIZE;++j){
            node p={i,j,board[i][j]};
            
            if(board[i][j]==color){
                ll score=getSingleScore(board,p,0);
                // #ifdef DEBUG
                //     printf("getWholeScore():color:%d x:%d y:%d score:%d\n",color,i,j,score);
                // #endif
                re+=score;
            }else if(board[i][j]==oppColor){
                ll score=getSingleScore(board,p,1);
                // #ifdef DEBUG
                //     printf("getWholeScore():color:%d x:%d y:%d\n",oppColor,i,j);
                // #endif
                re-=score;
            }
        }
    }
    return re;
}

ll getSingleScore(int (*board)[15], node p,int isopp){//计算单点分数
    STATUS nowStatus=getStatus(board,p);
    ll re=0;
    for(int i=0;i<8;++i){
        re+=score[isopp][i]*nowStatus.Statusnum[i];
    }
    if(nowStatus.Statusnum[ALIVE3]>=2){
        re+=10000;
    }
    else if(nowStatus.Statusnum[BLOCK4]>=2){
        re+=30000;
    }else if(nowStatus.Statusnum[BLOCK4]>=1&&nowStatus.Statusnum[ALIVE3]>=1){
        re+=20000;
    }
    // #ifdef DEBUG
    //     printf("getSingleScore():color:%d x:%d y:%d score:%lld\n",p.color,p.x,p.y,re);
    // #endif
    return re;  
}

STATUS getStatus(int (*board)[15], node p){//统计p点周围棋形数
    int dir[4][2]={{1,0},{0,1},{1,1},{1,-1}};
    int px=p.x,py=p.y,color=p.color,oppColor=3-p.color;
    STATUS re;
    memset(re.Statusnum,0,sizeof(re.Statusnum));
    for(int d=0;d<4;++d){
        int cnt=1;
        int block=0;
        int space=0;
        for(int i=1;i<5;++i){
            int nx=px+i*dir[d][0],ny=py+i*dir[d][1];
            if(isOutBound(nx,ny)|| board[nx][ny]==oppColor){
                int lastx=px+(i-1)*dir[d][0],lasty=py+(i-1)*dir[d][1];
                if(board[lastx][lasty]!=0)block++;
                break;
            }
            if(board[nx][ny]==0){
                if(space>=1)break;
                int nextx=px+(i+1)*dir[d][0],nexty=py+(i+1)*dir[d][1];
                if(!isOutBound(nextx,nexty)&&board[nextx][nexty]==color)
                    space++;
                else 
                    break;
            }
            if(board[nx][ny]==color){
                cnt++;
                if(i==4){
                    int nextx=px+(i+1)*dir[d][0],nexty=py+(i+1)*dir[d][1];
                    if(isOutBound(nextx,nexty)||board[nextx][nexty]==oppColor)block++;
                }
            }
        }
        for(int i=1;i<5;++i){
            int nx=px-i*dir[d][0],ny=py-i*dir[d][1];
            if(isOutBound(nx,ny)|| board[nx][ny]==oppColor){
                int lastx=px-(i-1)*dir[d][0],lasty=py-(i-1)*dir[d][1];
                if(board[lastx][lasty]!=0)block++;
                break;
            }
            if(board[nx][ny]==0){
                if(space>=1)break;
                int nextx=px-(i+1)*dir[d][0],nexty=py-(i+1)*dir[d][1];
                if(!isOutBound(nextx,nexty)&&board[nextx][nexty]==color)
                    space++;
                else 
                    break;
            }
            if(board[nx][ny]==color){
                cnt++;
                if(i==4){
                    int nextx=px-(i+1)*dir[d][0],nexty=py-(i+1)*dir[d][1];
                    if(isOutBound(nextx,nexty)||board[nextx][nexty]==oppColor)block++;
                }
            }
        }
        switch ( cnt ){
        case 1:
            if(block==0)re.Statusnum[ALIVE1]++;
            break;
        case 2:
            if(block==0)re.Statusnum[ALIVE2]++;
            else if(block==1)re.Statusnum[BLOCK2]++;
            break;
        case 3:
            if(block==0)re.Statusnum[ALIVE3]++;
            else if(block==1)re.Statusnum[BLOCK3]++;
            break;
        case 4:
            if(block==0){
                if(space==0)re.Statusnum[ALIVE4]++;
                else re.Statusnum[BLOCK4]++;
            }
            else if(block==1)re.Statusnum[BLOCK4]++;
            break;
        case 5:
            if(space==0)
                re.Statusnum[ALIVE5]++;
            else 
                re.Statusnum[ALIVE4]++;
            break;
        default:
            if(cnt>=6){
                if(space==0){
                    re.Statusnum[ALIVE5]++;
                    break;
                }else{
                    int ncnt=0;
                    for(int i=1;i<5;++i){
                        int nx=px+i*dir[d][0],ny=py+i*dir[d][1];
                        if(!isOutBound(nx,ny)&&board[nx][ny]==color)ncnt++;
                        else break;
                    }
                    for(int i=1;i<5;++i){
                        int nx=px-i*dir[d][0],ny=py-i*dir[d][1];
                        if(!isOutBound(nx,ny)&&board[nx][ny]==color)ncnt++;
                        else break;
                    }
                    if(ncnt>=6)re.Statusnum[ALIVE5]++;
                    else re.Statusnum[ALIVE4]++;
                }
            }
            
            break;
        }
    }
    return re;
}

int isOutBound(int x,int y){
    return (x<0)||(x>=SIZE)||(y<0)||(y>=SIZE);
}