#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pic(),
    painter(&pic)
{
    ui->setupUi(this);
    ui->label->setPicture(pic);
    connect(ui->label, &ClickLabel::clicked, this, &MainWindow::onClicked);
    reset();
    repaint();
}

void MainWindow::change(int x, int y, State state) {
    field[x][y] = state;
}

void MainWindow::repaint() {
    QColor color;
    painter.begin(&pic);
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num; j++) {


    switch (field[i][j]) {
    case State::Yes:
        color = Qt::red;
        break;
    case State::No:
        color = Qt::gray;
        break;
    default:
        color = Qt::white;
        break;
    }

    painter.setPen(Qt::blue);
    painter.fillRect((i*size),   (j*size),   size,   size,   Qt::blue);
    painter.fillRect((i*size)+1, (j*size+1), size-2, size-2, color);
    qDebug() << "c "  << " | " << color;

        }
    }
    painter.end();
    ui->label->setPicture(pic);
    checkStatus();
}

void MainWindow::reset() {
    //painter.setPen(Qt::blue);

    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num; j++) {
            field[i][j] = 0;
            //qDebug() << "  " << i+1 << "/" << num << " |" << j << "/" << num;
            //painter.fillRect((i*size),   (j*size),   size,  size,   Qt::blue);
            //painter.fillRect((i*size)+1, (j*size)+1, size-2,size-2, Qt::white);
        }
    }
    //painter.end();
    //ui->label->setPicture(pic);
}

void MainWindow::onClicked(QMouseEvent* ev) {
    qDebug() << ev;
    QPointF p = ev->localPos();
    int x = p.rx() / size;
    int y = p.ry() / size;
    qDebug() << x << " " << y << " | " << (ev->button() == Qt::LeftButton);
    if (ev->button() == Qt::LeftButton) {
        if (field[x][y] == State::No) {
            return;
        }
        if (field[x][y] == State::Yes) {
            change(x, y, State::Empty);
        } else {
            change(x, y, State::Yes);
        }
    } else if (ev->button() == Qt::RightButton) {
        //field[x][y] = State::No;
        //change(x,y, State::No);
        if (field[x][y] == State::Yes) {
            return;
        }
        if (field[x][y] == State::No) {
            change(x, y, State::Empty);
        } else {
            change(x, y, State::No);
        }
    }
    repaint();
}

void MainWindow::checkStatus() {
    bool finished = false;
    int matches = 0;
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num; j++) {
            if (field[i][j] == solution[i][j]) {
                matches++;
            }
        }
    }

    qDebug() << "RESULT: " << (matches == (num*num)) << " |" << matches << "/";
}

MainWindow::~MainWindow()
{
    delete ui;
}
