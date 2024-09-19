#include "gobang.h"

DECISION decision;

#ifdef DEBUG
int k6flag=0;
void drawboard(int (*board)[15]){
    printf(" 0123456789abcde\n");
    for(int i=0;i<15;i++){
        printf("%c",num2char(i));
        for(int j=0;j<15;j++){
            if(board[i][j]==0){
                printf("┼");
            }else if(board[i][j]==BLACK){
                printf("●");
            }else if(board[i][j]==WHITE){
                printf("○");
            } 
        }
        printf("\n");
    }
}

void printP(SEEKPOINTS P){
    for(int i=0;i<MAXLENGTH;++i){
        if(P.score[i]==-LLINF)break;
        printf("PrintP,i=%d x:%d y:%d score:%lld\n",i,P.pos[i].x,P.pos[i].y,P.score[i]);
    }
}
#endif


ll analysis(int (*board)[15],int color,int depth,ll alpha,ll beta){
    ll result=getWholeScore(board,color);
//     #ifdef DEBUG
//         printf("analysis()in:depth:%d color:%d result:%lld\n",depth,color,result);
// //        drawboard(board);
//     #endif
    
    if(result>=5000000||result<=-5000000){
        if(depth==MAXDEPTH){
            SEEKPOINTS re=getPoints(board,color);
            decision.pos=re.pos[0];
            decision.score=result;
        }
        // #ifdef DEBUG
        //     printf("analysis()return:depth:%d color:%d result:%lld\n",depth,color,result);
        // #endif
        return result;
    }
    if(depth==0){
        // SEEKPOINTS re=getPoints(board,color);
        // return re.score[0];
        return result;
    }
    
    // if(depth%2 == 0){//max层，我方决策
    //     int sameboard[15][15];
    //     memcpy(sameboard,board,sizeof(sameboard));

    //     SEEKPOINTS P=getPoints(sameboard,color);
    //     for(int i=0;i<MAXLENGTH;++i){
    //         if(P.score[i]==-LLINF)break;
    //         sameboard[P.pos[i].x][P.pos[i].y]=color;
    //         ll tmp=analysis(sameboard,3-color,depth-1,alpha,beta);
    //         sameboard[P.pos[i].x][P.pos[i].y]=0;
    //         // #ifdef DEBUG
    //         //     printf("analysis()a:depth:%d color:%d alpha:%lld,x:%d,y:%d\n",depth,color,alpha,P.pos[i].x,P.pos[i].y);
    //         // #endif
    //         if(tmp>alpha){
    //             alpha=tmp;
                
    //             if(depth==MAXDEPTH){
    //                 decision.pos=P.pos[i];
    //                 decision.score=alpha;
    //             }
    //         }
    //         if(alpha>=beta)break;
    //     }
    //     // #ifdef DEBUG
    //     //     printf("analysis()returna:depth:%d color:%d alpha:%lld\n",depth,color,alpha);
    //     // #endif
    //     return alpha;
    // }else{//min层，敌方决策
    //     int sameboard[15][15];
    //     memcpy(sameboard,board,sizeof(sameboard));
    //     SEEKPOINTS P=getPoints(sameboard,3-color);
    //     for(int i=0;i<MAXLENGTH;++i){
    //         if(P.score[i]==-LLINF)break;
    //         sameboard[P.pos[i].x][P.pos[i].y]=color;
    //         ll tmp=analysis(sameboard,3-color,depth-1,alpha,beta);
    //         sameboard[P.pos[i].x][P.pos[i].y]=0;
    //         // #ifdef DEBUG
    //         //     printf("analysis()b:depth:%d color:%d beta:%lld,x:%d,y:%d\n",depth,color,beta,P.pos[i].x,P.pos[i].y);
    //         // #endif
    //         if(tmp<beta){
    //             beta=tmp;
    //         }
    //         if(alpha>=beta)break;
    //     }
    //     // #ifdef DEBUG
    //     //     printf("analysis()returnb:depth:%d color:%d beta:%lld\n",depth,color,beta);
    //     // #endif
    //     return beta;
    // }
    int sameboard[15][15];
    memcpy(sameboard,board,sizeof(sameboard));
    SEEKPOINTS P;
    P=getPoints(sameboard,color);
    for(int k=0;k<MAXLENGTH;++k){
        
        
        // if(isOutBound(P.pos[k].x,P.pos[k].y)){
        //     #ifdef DEBUG
        //         printf("analysis():outbound:x:%d y:%d,score:%lld k:%d \n",P.pos[k].x,P.pos[k].y,P.score[k],k);
        //         printP(P);
        //         drawboard(sameboard);
        //         SEEKPOINTS NP;
        //         k6flag=1;
        //         NP=getPoints(sameboard,color);
        //         k6flag=0;
        //     #endif
        //     continue;
        // }

        
        if(P.score[k]==-LLINF)break;
        sameboard[P.pos[k].x][P.pos[k].y]=color;
        ll tmp=-analysis(sameboard,3-color,depth-1,-beta,-alpha);
        sameboard[P.pos[k].x][P.pos[k].y]=0;
        #ifdef DEBUG
            if(depth==MAXDEPTH){
                printf("analysismaxdepth,x=%d,y=%d,score=%lld,tmp=%lld,k=%d\n",P.pos[k].x,P.pos[k].y,P.score[k],tmp,k);
            }
        #endif
        // #ifdef DEBUG
        //     printf("analysis()b:depth:%d color:%d beta:%lld,x:%d,y:%d\n",depth,color,beta,P.pos[i].x,P.pos[i].y);
        // #endif
        if(tmp>=beta){
            return beta;
        }
        if(tmp>alpha){
            alpha=tmp;
            
            if(depth==MAXDEPTH){
                decision.pos=P.pos[k];
                decision.score=alpha;
            }
        }
    }
    return alpha;
}



SEEKPOINTS getPoints(int (*board)[15],int color){
    // #ifdef DEBUG
    //     printf("getpoints():color:%d\n",color);
    // #endif
    int vis[15][15]; 
    memset(vis,0,sizeof(vis));
    ll val[15][15];
    SEEKPOINTS re;
    //局部搜索以改善效率
    for(int i=0;i<MAXLENGTH;++i){
        re.pos[i].x=-1;
        re.pos[i].y=-1;
        re.score[i]=-LLINF;
    }
    for(int i=0;i<SIZE;++i){
        for(int j=0;j<SIZE;++j){
            if(board[i][j]==0)continue;
            for(int k=-3;k<=3;++k){
                if(!isOutBound(i+k,j))vis[i+k][j]=1;    
                if(!isOutBound(i,j+k))vis[i][j+k]=1;
                if(!isOutBound(i+k,j+k))vis[i+k][j+k]=1;
                if(!isOutBound(i+k,j-k))vis[i+k][j-k]=1;
            }
        }
    }
    if(CurrentStep==0){
        re.pos[0].x=7;
        re.pos[0].y=7;
        re.score[0]=0;
        return re;
    }
    for(int i=0;i<SIZE;++i){
        for(int j=0;j<SIZE;++j){
            val[i][j]=-LLINF;
            if(!vis[i][j])continue;
            if(board[i][j]!=0)continue;
            board[i][j]=color;
            val[i][j]=getWholeScore(board,color);
            board[i][j]=0;

            board[i][j]=3-color;   
            val[i][j]+=getWholeScore(board,3-color);
            board[i][j]=0;
        }
    }

    ll maxScore=-LLINF;
    for(int k=0;k<MAXLENGTH;++k){
        maxScore=-LLINF;
        for(int i=0;i<SIZE;++i){
            for(int j=0;j<SIZE;++j){
                if(val[i][j]>maxScore){
                    maxScore=val[i][j];
                    re.pos[k].x=i;
                    re.pos[k].y=j;
                    re.score[k]=maxScore;
                    #ifdef DEBUG
                        if(k6flag){
                            printf("tryk6,i=%d,j=%d,score=%d\n",i,j,maxScore);
                        }
                    #endif
                }
                
            }
        }
        val[re.pos[k].x][re.pos[k].y]=-LLINF;
    }
    #ifdef DEBUG
        if(k6flag){
//                            drawboard(board);
            printf("k6flag,outbound=%d\n",isOutBound(re.pos[6].x,re.pos[6].y));
            printP(re);
        }
        
    #endif
    
    return re;
}
/*
int searchKill(int (*board)[15],int color,int depth){

}


int getKillPoints(int (*board)[15],int color){
    int vis[15][15]; 
    memset(vis,0,sizeof(vis));
    ll val[15][15];
    SEEKPOINTS re;
    //局部搜索以改善效率
    for(int i=0;i<SIZE;++i){
        for(int j=0;j<SIZE;++j){
            if(board[i][j]==0)continue;
            for(int k=-3;k<=3;++k){
                if(!isOutBound(i+k,j))vis[i+k][j]=1;    
                if(!isOutBound(i,j+k))vis[i][j+k]=1;
                if(!isOutBound(i+k,j+k))vis[i+k][j+k]=1;
                if(!isOutBound(i+k,j-k))vis[i+k][j-k]=1;
            }
        }
    }

    for(int i=0;i<SIZE;++i){
        for(int j=0;j<SIZE;++j){
            val[i][j]=-LLINF;
            if(!vis[i][j])continue;
            int score=getWholeScore(board,color);
            if(score>=100000)
        }
    }
}
*/