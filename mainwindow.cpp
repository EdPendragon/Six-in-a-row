#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamedata.h"
#include <QTimer>
#include <QMouseEvent>
#include <cmath>
#include <Qpainter>
#include <iostream>
#include <QMessageBox>
#include <QPushButton>
#include <time.h>
#include <QMediaPlayer>
#include <QDebug>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
    setFixedSize(d_side*2+l_block*msize,d_side*2+l_block*msize);        //设置窗口的大小
    initiapvp();                            //游戏开始默认初始化人人对战
    game->setjin();                       //游戏开始默认关闭禁手
    QMediaPlayer *player=new QMediaPlayer(this);//设置背景音乐
    QPushButton* pvpb=new QPushButton;      //创建初始化pvp模式的按钮
    pvpb->setParent(this);
    pvpb->setText("人人对战模式");
    pvpb->move(150,12);
    pvpb->resize(150,75);
    connect(pvpb,&QPushButton::clicked,[=](){
       initiapvp();
       QMessageBox::StandardButton btnValue = QMessageBox::information(this,"模式启动", "人人模式已启动");
       player->stop();
       player->setMedia(QUrl::fromLocalFile("D:/CloudMusic/bgm1.mp3"));
       player->setVolume(7);
       player->play();
    });
    QPushButton* pveb=new QPushButton;      //创建初始化pve模式的按钮，人先手
    pveb->setParent(this);
    pveb->setText("人机对战模式");
    pveb->move(450,12);
    pveb->resize(150,75);
    connect(pveb,&QPushButton::clicked,[=](){
       initiapve();
       QMessageBox::StandardButton btnValue = QMessageBox::information(this,"模式启动", "人机模式已启动");
       player->stop();
       player->setMedia(QUrl::fromLocalFile("D:/CloudMusic/bgm2.flac"));
       player->setVolume(20);
       player->play();
    });
    QPushButton* eveb=new QPushButton;      //创建初始化eve模式的按钮
    eveb->setParent(this);
    eveb->setText("机机对战模式");
    eveb->move(750,12);
    eveb->resize(150,75);
    connect(eveb,&QPushButton::clicked,[=](){
       initiaeve();
       QMessageBox::StandardButton btnValue = QMessageBox::information(this, "模式启动","机机模式已启动");
       player->stop();
       player->setMedia(QUrl::fromLocalFile("D:/CloudMusic/bgm3.mp3"));
       player->setVolume(20);
       player->play();
    });
    QPushButton* kaijin=new QPushButton;      //打开禁手模式
    kaijin->setParent(this);
    kaijin->setText("开启禁手");
    kaijin->move(900,300);
    kaijin->resize(75,37);
    connect(kaijin,&QPushButton::clicked,[=](){
       game->setjin();
       QMessageBox::StandardButton btnValue = QMessageBox::information(this, "禁手","黑棋禁手开启");
    });
    QPushButton* guanjin=new QPushButton;      //关闭禁手模式
    guanjin->setParent(this);
    guanjin->setText("关闭禁手");
    guanjin->move(900,450);
    guanjin->resize(75,37);
    connect(guanjin,&QPushButton::clicked,[=](){
       game->closejin();
       QMessageBox::StandardButton btnValue = QMessageBox::information(this,"禁手","黑棋禁手关闭");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){              //绘制棋盘函数的实现
    QPainter painter(this);         //画笔
    //加载图片
    QPixmap img(":/new/prefix1/bjt.jpg");
    QPalette pal;
    pal.setBrush(this->backgroundRole(),QBrush(img));
    this->setPalette(pal);
    painter.setRenderHint(QPainter::Antialiasing,true);        //开启抗锯齿功能
    for(int i=2; i<msize-2; i++){                     //一共画24-4条竖线和横线
        painter.drawLine(d_side+l_block*i,d_side+l_block*2,
                         d_side+l_block*i,size().height()-d_side-l_block*3);       //第i+1条竖线
        painter.drawLine(d_side+l_block*2,d_side+l_block*i,
                         size().width()-d_side-l_block*3,d_side+l_block*i);      //第i+1条横线
    }
    //绘制鼠标选中的点
    QBrush brush;       //绘图刷子
    brush.setStyle(Qt::SolidPattern);       //定义绘图模式
    if(clickhang>1&&clicklie>1&&clickhang<msize-1&&clicklie<msize-1&&game->map[clicklie][clickhang]==0){
        if(game->player) brush.setColor(Qt::black);
        else brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawEllipse(d_side+l_block*clickhang-l_mark/2,d_side+l_block*clicklie-l_mark/2,l_mark,l_mark);
    }
    //点击之后绘画棋子
    for(int i=2;i<msize-2;i++){
        for(int j=2;j<msize-2;j++){
            if(game->map[j][i]==1){     //黑棋
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(d_side+l_block*j-r_chess/2,d_side+l_block*i-r_chess/2,r_chess,r_chess);
            }
            else if(game->map[j][i]==-1){       //白棋
                    brush.setColor(Qt::white);
                    painter.setBrush(brush);
                    painter.drawEllipse(d_side+l_block*j-r_chess/2,d_side+l_block*i-r_chess/2,r_chess,r_chess);
            }
        }
    }
    //判断是否触发禁手
    if(clickhang>0&&clicklie>0&&clickhang<msize&&clicklie<msize&&
            (game->map[clickhang][clicklie]==1)&&(type==pvp||type==pve)&&game->nohand(clickhang,clicklie,1)&&game->jin){
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "The result is ", "White Chess has won.");
        if(btnValue==QMessageBox::Ok){                              //点击对话框
            game->gameStart(type);                                  //重新开始游戏
            if(btnValue==QMessageBox::Ok){                              //点击对话框
                game->gameStart(type);                                  //重新开始游戏
            }
        }
    }
    //判断是否胜利
    if(clickhang>=0&&clicklie>=0&&clickhang<=msize&&clicklie<=msize&&
            (game->map[clickhang][clicklie]==1||game->map[clickhang][clicklie]==-1)){       //落子合法
        if(game->win(clickhang,clicklie)&&game->status==Running){       //假如判断胜利
            game->status=Over;
            QString shenglifang;
            if(game->map[clickhang][clicklie]==1) shenglifang="Black Chess";            //黑棋获胜
            else if(game->map[clickhang][clicklie]==-1) shenglifang="White Chess";      //白棋获胜
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "The result is ", shenglifang + " has won.");
            if(btnValue==QMessageBox::Ok){                              //点击对话框
                if(type==pvp) initiapvp();
                else if(type==pve) initiapve();
                else if(type==eve) initiaeve();                 //初始化游戏
            }
        }
    }
    if(game->peace()){                 //和棋
        QString shenglifang;
        shenglifang="No one";
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "The result is ", shenglifang + " has won.");
        if(btnValue==QMessageBox::Ok){                              //点击对话框
            if(type==pvp) initiapvp();
            else if(type==pve) initiapve();
            else if(type==eve) initiaeve();                 //初始化游戏
        }
    }
}
void MainWindow::initiapvp(){                       //人人对战的初始化
    game=new Game;
    game->status=Running;
    type=pvp;
    game->num=0;
    game->gameStart(type);
    update();
}
void MainWindow::initiapve(){                       //人机对战的初始化
    game=new Game;
    game->status=Running;
    type=pve;
    game->num=0;
    game->gameStart(type);
    update();
}
void MainWindow::initiaeve(){                       //机机对战的初始化
    game=new Game;
    game->status=Running;
    type=eve;
    game->num=0;
    game->gameStart(type);
    update();
}
void MainWindow::mouseMoveEvent(QMouseEvent* event){        //鼠标监听
    int x=event->x(),y=event->y();                  //鼠标坐标
    if(x>d_side+l_block*2&&x<size().width()-d_side-l_block*3&&y>d_side+l_block*2&&y<=size().height()-d_side-l_block*3){
        int heng=(x-d_side)/l_block;        //获得最近的左上角点
        int zong=(y-d_side)/l_block;
        int zuoTopX=d_side+l_block*heng;
        int zuoTopY=d_side+l_block*zong;       //周围一共四个点，选择最近的
        clickhang=-1;
        clicklie=-1;               //初始化结果坐标
        int l=0;      //计算距离，随后取整
        l=sqrt((x-zuoTopX)*(x-zuoTopX)+(y-zuoTopY)*(y-zuoTopY));
        if(l<mouse_catch){
            clickhang=heng;
            clicklie=zong;
            if(game->map[clickhang][clicklie]==0) selectPos=true;
        }
        l=sqrt((x-zuoTopX-l_block)*(x-zuoTopX-l_block)+(y-zuoTopY)*(y-zuoTopY));
        if(l<mouse_catch){
            clickhang=heng+1;
            clicklie=zong;
            if(game->map[clickhang][clicklie]==0) selectPos=true;
        }
        l=sqrt((x-zuoTopX)*(x-zuoTopX)+(y-zuoTopY-l_block)*(y-zuoTopY-l_block));
        if(l<mouse_catch){
            clickhang=heng;
            clicklie=zong+1;
            if(game->map[clickhang][clicklie]==0) selectPos=true;
        }
        l=sqrt((x-zuoTopX-l_block)*(x-zuoTopX-l_block)+(y-zuoTopY-l_block)*(y-zuoTopY-l_block));
        if(l<mouse_catch){
            clickhang=heng+1;
            clicklie=zong+1;
            if(game->map[clickhang][clicklie]==0) selectPos=true;
        }
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(type==pvp||type==pve){
        if(selectPos==false) return;
        else selectPos=false;
        peoinput();
        if(type==pve){
            if(game->peace()){                 //和棋
                QString shenglifang;
                shenglifang="No one";
                QMessageBox::StandardButton btnValue = QMessageBox::information(this, "The result is ", shenglifang + " has won.");
                if(btnValue==QMessageBox::Ok){                              //点击对话框
                    if(type==pvp) initiapvp();
                    else if(type==pve) initiapve();
                    else if(type==eve) initiaeve();                 //初始化游戏
                }
            }
            QTimer::singleShot(40, this, SLOT(aiinput()));
        }
    }
    if(type==eve){
        if(game->peace()){                 //和棋
            QString shenglifang;
            shenglifang="No one";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "The result is ", shenglifang + " has won.");
            if(btnValue==QMessageBox::Ok){                              //点击对话框
                if(type==pvp) initiapvp();
                else if(type==pve) initiapve();
                else if(type==eve) initiaeve();                 //初始化游戏
            }
        }
            aiinput();
            QTimer::singleShot(400, this, SLOT(aiinput()));
        }
}
void MainWindow::peoinput(){
    if(clickhang!=-1&&clicklie!=-1&&game->map[clickhang][clicklie]==0){    //根据鼠标选择位置下棋
        game->peodo(clickhang,clicklie);
        game->num++;
        update();
    }
    else return;
}

void MainWindow::aiinput(){
    game->aido(clicklie,clickhang);           //根据价值地图下棋
    game->num++;
    update();
    if(game->aiwin(1)){       //假如判断胜利
    QString shenglifang;
    shenglifang="Black Chess";      //黑棋获胜
    QMessageBox::StandardButton btnValue = QMessageBox::information(this, "The result is ", shenglifang + " has won.");
    if(btnValue==QMessageBox::Ok){                              //点击对话框
        if(type==pve) initiapve();
        else if(type==pvp) initiapvp();
        else if(type==eve) initiaeve();
    }}
    if(game->aiwin(-1)){       //假如判断胜利
    QString shenglifang;
    shenglifang="White Chess";      //白棋获胜
    QMessageBox::StandardButton btnValue = QMessageBox::information(this, "The result is ", shenglifang + " has won.");
    if(btnValue==QMessageBox::Ok){                              //点击对话框
        if(type==pve) initiapve();
        if(type==pvp) initiapvp();
        else if(type==eve) initiaeve();
    }}
}
