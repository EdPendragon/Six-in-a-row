#ifndef GAMEDATA_H
#define GAMEDATA_H
#include <vector>
#include <iostream>
using namespace std;

class gameData
{
public:
    gameData();
};

//以下数据用来表示棋盘数据
const int msize=24;        //网格数为20*20
const int r_chess=26;       //棋子的半径
const int l_block=40;       //棋盘每格的边长
const int d_side=40;        //棋盘与窗口边缘的距离
//以下数据常量用于其它用处
const int l_mark=8;         //标记的落下棋子边长
const int mouse_catch=l_block/2;   //鼠标捕捉距离,为每个格子的一半边长
enum GameModel{                     //游戏模式
    pvp,
    pve,
    eve
};
enum GameStatus{                    //游戏状态
    Running,
    Over
};
//下方的类用于存放下棋需要的各种函数与变量
class Game{
    public:
        Game(){};       //构造函数
        int num;		//num回合数
        bool player;                        //标记下棋方，true为黑棋，false为白棋
        bool jin;                           //true表示开启禁手模式，false表示关闭禁手模式
        GameModel gameModel;                //游戏模式
        GameStatus status;                  //游戏状态
    public:
        std::vector<std::vector<int>> scomap;       //储存地图上每一个位置对于ai的价值
        std::vector<std::vector<int>> map;         //char类型地图，其中1代表黑棋，-1代表白棋,0代表空位
          void gameStart(GameModel m);              //开始游戏，m=pvp则为人人，m==pve则为人机，m==eve则为观看机机对战
          void valuemap(int qi);              //计算每个位置的价值，qi==1 黑棋，qi==-1白棋
          bool nohand(int x,int y,int qi);    //判断是否触发禁手，x、y为坐标，qi为棋子种类
          bool win(int x,int y);              //判断是否胜利
          void peodo(int hang,int lie);   //实际落子
          void aido(int &clicklie,int &clickhang);    //ai实际落子
          void upload(int l,int h);         //上传更新每次的数据
          void setjin();                    //开启禁手
          void closejin();                  //关闭禁手
          bool aiwin(int qi);               //判断ai是否胜利
          bool peace();                     //判断和棋
};
#endif // GAMEDATA_H
