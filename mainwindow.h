#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <iostream>
using namespace std;
#include "gamedata.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Game* game;                            //代表游戏的指针
    GameModel type;                        //游戏类型
    int clickhang,clicklie;                 //存储将要点击位置的坐标
    bool selectPos=false;                   //是否选中
    void paintEvent(QPaintEvent *event);        //绘制棋盘的函数
    void initiapvp();                       //人人对战的初始化棋盘
    void initiaeve();                       //机机对战初始化函数
    void initiapve();                       //人机对战的初始化
    void mouseMoveEvent(QMouseEvent *event);//鼠标监听函数
    void mouseReleaseEvent(QMouseEvent *event);//鼠标释放
    void peoinput();  //人下棋
public slots:                               //槽函数，加slots
    void aiinput();   //ai下棋
};

#endif // MAINWINDOW_H
