#include "gamedata.h"
#include <mainwindow.h>
#include <iostream>
#include <time.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
gameData::gameData()
{

}

bool Game::peace(){                               //判断和棋函数
    for(int i=2;i<msize-2;i++){
        for(int j=2;j<msize-2;j++){         //遍历棋盘每一个位置
            if(map[i][j]==0) return false;      //若为空则返回false
            else continue;                  //否则继续遍历
        }
    }
    return true;                            //全不为空说明和棋了
}

bool Game::nohand(int x,int y,int qi){            //判断禁手
    int qin=1,four=0,five=0;		//four和five分别代表下了该棋后出现的活/冲四和五的数量，超过一个则禁手
    for(int i=1;i<6;i++){       //左
        if((x-i)<2) break;
        if(map[x-i][y]==qi) qin++;
        else break;
    }
    for(int i=1;i<6;i++){       //右
        if((x+i)>=msize-2) break;
        if(map[x+i][y]==qi) qin++;
        else break;
    }
    if(qin==4) four++;
    else if(qin==5) five++;
    qin=1;
    for(int i=1;i<6;i++){       //上
        if((y-i)<2) break;
        if(map[x][y-i]==qi) qin++;
        else break;
    }
    for(int i=1;i<6;i++){       //下
        if((y+i)>=msize-2) break;
        if(map[x][y+i]==qi) qin++;
        else break;
    }
    if(qin==4) four++;
    else if(qin==5) five++;
    qin=1;
    for(int i=1;i<6;i++){       //45度角
        if((x-i)<2||(y-i)<2) break;
        if(map[x-i][y-i]==qi) qin++;
        else break;
    }
    for(int i=1;i<6;i++){
        if((x+i)>=msize-2||(y+i)>=msize-2) break;
        if(map[x+i][y+i]==qi) qin++;
        else break;
    }
    if(qin==4) four++;
    else if(qin==5) five++;
    qin=1;
    for(int i=1;i<6;i++){       //135度角
        if((x-i)<2||(y+i)>=msize-2) break;
        if(map[x-i][y+i]==qi) qin++;
        else break;
    }
    for(int i=1;i<6;i++){
        if((x+i)>=msize-2||(y-i)<2) break;
        if(map[x+i][y-i]==qi) qin++;
        else break;
    }
    if(qin==4) four++;
    else if(qin==5) five++;
    if(four>=2||five>=2) return true;
    else return false;
}

bool Game::win(int x,int y){                            //判断落子是否胜利
    int qi=map[x][y]; //当前棋色
    int qin=1;          //当前棋色的数量
    for(int i=1;i<6;i++){       //左
        if((x-i)<2) break;
        if(map[x-i][y]==qi) qin++;
        else break;
    }
    for(int i=1;i<6;i++){       //右
        if((x+i)>=msize-2) break;
        if(map[x+i][y]==qi) qin++;
        else break;
    }
    if(qin>=6) return true;
    qin=1;
    for(int i=1;i<6;i++){       //上
        if((y-i)<2) break;
        if(map[x][y-i]==qi) qin++;
        else break;
    }
    for(int i=1;i<6;i++){       //下
        if((y+i)>=msize-2) break;
        if(map[x][y+i]==qi) qin++;
        else break;
    }
    if(qin>=6) return true;
    qin=1;
    for(int i=1;i<6;i++){       //45度角
        if((x-i)<2||(y-i)<2) break;
        if(map[x-i][y-i]==qi) qin++;
        else break;
    }
    for(int i=1;i<6;i++){
        if((x+i)>=msize-2||(y+i)>=msize-2) break;
        if(map[x+i][y+i]==qi) qin++;
        else break;
    }
    if(qin>=6) return true;
    qin=1;
    for(int i=1;i<6;i++){       //135度角
        if((x-i)<2||(y+i)>=msize-2) break;
        if(map[x-i][y+i]==qi) qin++;
        else break;
    }
    for(int i=1;i<6;i++){
        if((x+i)>=msize-2||(y-i)<2) break;
        if(map[x+i][y-i]==qi) qin++;
        else break;
    }
    if(qin>=6) return true;
    return false;               //各个方向都不满足条件，返回false
}

void Game::valuemap(int qi){                     //计算每个位置的价值，qi==1 黑棋，qi==-1白棋
    int eni;
    if(qi==1) eni=-1;
    else eni=1;
    int enin=0,ain=0,blankn=0; 	//代表一个方向上敌人的棋子数，电脑的棋子数，空白棋位
    scomap.clear();
    for(int i=0;i<msize;i++)                        //价值棋盘清空
    {
        std::vector<int>lineboard;
        for(int j=0;j<msize;j++)
            lineboard.push_back(0);
        scomap.push_back(lineboard);
    }
    for(int i=2; i<msize-2; i++) {		//开始计算价值，采用网上比较流行的加权法，分为进攻、防守两种情况
        for(int j=2; j<msize-2; j++) {
            if(map[i][j]==0) {			//只有合法位置的空位才考虑
                if(qi==1&&nohand(i,j,1)&&jin) {
                    scomap[i][j]=-1;
                    continue;
                }
                if(qi==1&&i==11&&j==11){
                    scomap[i][j]=100000;
                    continue;
                }
                for(int k=1;k<6;k++){       //左
                    if(i-k<2) break;
                    if(map[i-k][j]==qi) ain++;
                    else if(map[i-k][j]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                for(int k=1;k<6;k++){       //右
                    if(i+k>=msize-2) break;
                    if(map[i+k][j]==qi) ain++;
                    else if(map[i+k][j]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                if(ain>=5) scomap[i][j]+=810000;
                if(ain==4){
                    if(blankn==2) scomap[i][j]+=15000;
                    else if(blankn==1) scomap[i][j]+=10000;
                }
                if(ain==3){
                    if(blankn==2) scomap[i][j]+=3000;
                    else if(blankn==1) scomap[i][j]+=1000;
                }
                if(ain==2){
                    if(blankn==2) scomap[i][j]+=400;
                    else if(blankn==1) scomap[i][j]+=100;
                }
                if(ain==1){
                    if(blankn==2) scomap[i][j]+=50;
                    else if(blankn==1) scomap[i][j]+=20;
                }
                ain=0;
                blankn=0;
                for(int k=1;k<6;k++){       //上
                    if(j-k<2) break;
                    if(map[i][j-k]==qi) ain++;
                    else if(map[i][j-k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                for(int k=1;k<6;k++){       //下
                    if(j+k>=msize-2) break;
                    if(map[i][j+k]==qi) ain++;
                    else if(map[i][j+k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                if(ain>=5) scomap[i][j]+=810000;
                if(ain==4){
                    if(blankn==2) scomap[i][j]+=15000;
                    else if(blankn==1) scomap[i][j]+=10000;
                }
                if(ain==3){
                    if(blankn==2) scomap[i][j]+=3000;
                    else if(blankn==1) scomap[i][j]+=1000;
                }
                if(ain==2){
                    if(blankn==2) scomap[i][j]+=400;
                    else if(blankn==1) scomap[i][j]+=100;
                }
                if(ain==1){
                    if(blankn==2) scomap[i][j]+=50;
                    else if(blankn==1) scomap[i][j]+=20;
                }
                ain=0;
                blankn=0;
                for(int k=1;k<6;k++){       //75度
                    if((i-k<2)||(j-k<2)) break;
                    if(map[i-k][j-k]==qi) ain++;
                    else if(map[i-k][j-k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                for(int k=1;k<6;k++){       //255度
                    if((i+k>=msize-2)||(j+k>=msize-2)) break;
                    if(map[i+k][j+k]==qi) ain++;
                    else if(map[i+k][j+k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                if(ain>=5) scomap[i][j]+=810000;
                if(ain==4){
                    if(blankn==2) scomap[i][j]+=15000;
                    else if(blankn==1) scomap[i][j]+=10000;
                }
                if(ain==3){
                    if(blankn==2) scomap[i][j]+=3000;
                    else if(blankn==1) scomap[i][j]+=1000;
                }
                if(ain==2){
                    if(blankn==2) scomap[i][j]+=400;
                    else if(blankn==1) scomap[i][j]+=100;
                }
                if(ain==1){
                    if(blankn==2) scomap[i][j]+=50;
                    else if(blankn==1) scomap[i][j]+=20;
                }
                ain=0;
                blankn=0;
                for(int k=1;k<6;k++){       //45度
                    if((i+k>=msize-2)||(j-k<2)) break;
                    if(map[i+k][j-k]==qi) ain++;
                    else if(map[i+k][j-k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                for(int k=1;k<6;k++){       //225度
                    if((i-k<2)||(j+k>=msize-2)) break;
                    if(map[i-k][j+k]==qi) ain++;
                    else if(map[i-k][j+k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                if(ain>=5) scomap[i][j]+=810000;
                if(ain==4){
                    if(blankn==2) scomap[i][j]+=15000;
                    else if(blankn==1) scomap[i][j]+=10000;
                }
                if(ain==3){
                    if(blankn==2) scomap[i][j]+=3000;
                    else if(blankn==1) scomap[i][j]+=1000;
                }
                if(ain==2){
                    if(blankn==2) scomap[i][j]+=400;
                    else if(blankn==1) scomap[i][j]+=100;
                }
                if(ain==1){
                    if(blankn==2) scomap[i][j]+=50;
                    else if(blankn==1) scomap[i][j]+=20;
                }
                ain=0;
                blankn=0;
                for(int k=1;k<6;k++){       //左
                    if(i-k<2) break;
                    if(map[i-k][j]==eni) enin++;
                    else if(map[i-k][j]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                for(int k=1;k<6;k++){       //右
                    if(i+k>=msize-2) break;
                    if(map[i+k][j]==eni) enin++;
                    else if(map[i+k][j]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                if(enin>=5) scomap[i][j]+=200000;
                if(enin==4){
                    if(blankn==2) scomap[i][j]+=20000;
                    else if(blankn==1) scomap[i][j]+=12500;
                }
                if(enin==3){
                    if(blankn==2) scomap[i][j]+=6000;
                    else if(blankn==1) scomap[i][j]+=2000;
                }
                if(enin==2){
                    if(blankn==2) scomap[i][j]+=300;
                    else if(blankn==1) scomap[i][j]+=200;
                }
                if(enin==1){
                    if(blankn==2) scomap[i][j]+=25;
                    else if(blankn==1) scomap[i][j]+=10;
                }
                enin=0;
                blankn=0;
                for(int k=1;k<6;k++){       //上
                    if(j-k<2) break;
                    if(map[i][j-k]==eni) enin++;
                    else if(map[i][j-k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                for(int k=1;k<6;k++){       //下
                    if(j+k>=msize-2) break;
                    if(map[i][j+k]==eni) enin++;
                    else if(map[i][j+k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                if(enin>=5) scomap[i][j]+=200000;
                if(enin==4){
                    if(blankn==2) scomap[i][j]+=20000;
                    else if(blankn==1) scomap[i][j]+=12500;
                }
                if(enin==3){
                    if(blankn==2) scomap[i][j]+=6000;
                    else if(blankn==1) scomap[i][j]+=2000;
                }
                if(enin==2){
                    if(blankn==2) scomap[i][j]+=300;
                    else if(blankn==1) scomap[i][j]+=200;
                }
                if(enin==1){
                    if(blankn==2) scomap[i][j]+=25;
                    else if(blankn==1) scomap[i][j]+=10;
                }
                enin=0;
                blankn=0;
                for(int k=1;k<6;k++){       //75度
                    if((i-k<2)||(j-k<2)) break;
                    if(map[i-k][j-k]==eni) enin++;
                    else if(map[i-k][j-k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                for(int k=1;k<6;k++){       //255度
                    if((i+k>=msize-2)||(j+k>=msize-2)) break;
                    if(map[i+k][j+k]==eni) enin++;
                    else if(map[i+k][j+k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                if(enin>=5) scomap[i][j]+=200000;
                if(enin==4){
                    if(blankn==2) scomap[i][j]+=20000;
                    else if(blankn==1) scomap[i][j]+=12500;
                }
                if(enin==3){
                    if(blankn==2) scomap[i][j]+=6000;
                    else if(blankn==1) scomap[i][j]+=2000;
                }
                if(enin==2){
                    if(blankn==2) scomap[i][j]+=300;
                    else if(blankn==1) scomap[i][j]+=200;
                }
                if(enin==1){
                    if(blankn==2) scomap[i][j]+=25;
                    else if(blankn==1) scomap[i][j]+=10;
                }
                enin=0;
                blankn=0;
                for(int k=1;k<6;k++){       //45度
                    if((i+k>=msize-2)||(j-k<2)) break;
                    if(map[i+k][j-k]==eni) enin++;
                    else if(map[i+k][j-k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                for(int k=1;k<6;k++){       //225度
                    if((i-k<2)||(j+k>=msize-2)) break;
                    if(map[i-k][j+k]==eni) enin++;
                    else if(map[i-k][j+k]==0){
                        blankn++;
                        break;
                    }
                    else break;
                }
                if(enin>=5) scomap[i][j]+=200000;
                if(enin==4){
                    if(blankn==2) scomap[i][j]+=20000;
                    else if(blankn==1) scomap[i][j]+=12500;
                }
                if(enin==3){
                    if(blankn==2) scomap[i][j]+=6000;
                    else if(blankn==1) scomap[i][j]+=2000;
                }
                if(enin==2){
                    if(blankn==2) scomap[i][j]+=300;
                    else if(blankn==1) scomap[i][j]+=200;
                }
                if(enin==1){
                    if(blankn==2) scomap[i][j]+=25;
                    else if(blankn==1) scomap[i][j]+=10;
                }
                enin=0;
                blankn=0;
            }
        }
    }
}
void Game::gameStart(GameModel t){                              //初始化棋盘和价值棋盘(valuemap)
    gameModel=t;
    map.clear();
    for(int i=0;i<msize;i++)                        //棋盘清空
    {
        std::vector<int>lineboard;
        for(int j=0;j<msize;j++)
            lineboard.push_back(0);
        map.push_back(lineboard);
    }
    for(int i=0;i<msize;i++){
        for(int j=0;j<msize;j++){
            if(i==0||i==1||i==msize-1||i==msize-2||j==0||j==1||j==msize-1||j==msize-2) map[i][j]=3;
        }
    }
    for(int i=0;i<msize;i++)                        //价值棋盘清空
    {
        std::vector<int>lineboard;
        for(int j=0;j<msize;j++)
            lineboard.push_back(0);
        scomap.push_back(lineboard);
        for(int j=0;j<msize;j++){
            if(i==0||i==1||i==msize-1||i==msize-2||j==0||j==1||j==msize-1||j==msize-2) scomap[i][j]=-100;
        }
    }
    player=true;                                //将要下棋的重新设定为黑棋
}

void Game::peodo(int hang,int lie){
    if(player) map[hang][lie]=1;        //player==true，黑棋
    else map[hang][lie]=-1;             //白棋
    if(player) player=false;
    else player=true;                   //换下棋方
}

void Game::aido(int &clicklie,int &clickhang){
    int qi;
    if(player) qi=1;
    else qi=-1;
    valuemap(qi);
    int m=0;
    std::vector<std::pair<int,int>> mDian;      //存储包含最大价值的点的坐标
    for(int i=2;i<msize-2;i++){
        for(int j=2;j<msize-2;j++){
            if(map[i][j]==0){                   //假设可以下棋
                if(scomap[i][j]>m){             //当前价值大于记录的最大价值
                    m=scomap[i][j];             //更新最大价值
                    mDian.clear();              //清空之前的最大价值数组
                    mDian.push_back(std::make_pair(i,j));
                }
                else if(scomap[i][j]==m){       //假如等于最大价值
                    mDian.push_back(std::make_pair(i,j));
                }
            }
        }
    }
    int gjr;                //随机数
    srand((unsigned int)time(0));               //产生随机种子
    gjr=rand()%mDian.size();            //在所有价值最大的点中随机选择一个
    std::pair<int,int> dianP=mDian.at(gjr);
    clicklie=dianP.first;
    clickhang=dianP.second;
    if(clicklie<2||clicklie>=msize-2||clickhang<2||clickhang>=msize-2) Game::aido(clicklie,clickhang);
    upload(clicklie,clickhang);
}
void Game::upload(int l, int h){
    if(player) map[l][h]=1;         //黑棋
    else map[l][h]=-1;              //白棋
    if(player) player=false;        //交换下棋方
    else player=true;
}
void Game::setjin(){
    jin=true;                   //禁手开启
}
void Game::closejin(){
    jin=false;                  //禁手关闭
}
bool Game::aiwin(int qi){       //判断ai是否胜利
    for(int i=2;i<msize-1;i++){
        for(int j=2;j<msize-1;j++){         //棋盘上每一个位置都判断一遍
            if(map[i][j]==qi){
                if(win(i,j)) return true;
            }
        }
    }
    return false;
}
