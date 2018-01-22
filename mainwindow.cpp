#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cstdlib>
#include <math.h>
#include <random>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pic(),
    painter(&pic)
{
    ui->setupUi(this);
    //ui->label->setPicture(pic);
    connect(ui->label, &ClickLabel::clicked, this, &MainWindow::onClicked);
    ui->label->setGeometry(border_left, border_top,
                           ((width_+1)  * (size) +0),
                           ((height_+1) * (size))+0);

    ui->label->setStyleSheet("QLabel { background-color : white; }");
    ui->label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    hintsFont.setPixelSize(font_size);

    qDebug() << "w h " << (width_+1) * (size) << "/" << (height_+1) * (size) << endl;
    randomizeSolution();
    setupLabels();
    resetField();
    repaint();
}

void MainWindow::change(int x, int y, State state) {
    field[x][y] = state;
}

void MainWindow::repaint() {
    QColor color;
    painter.begin(&pic);
    for (int i = 0; i < width_; i++) {
        for (int j = 0; j < height_; j++) {
            switch (field[i][j]) {
            case State::Yes:
                color = Qt::darkGreen;
                break;
            case State::No:
                color = Qt::gray;
                break;
            default:
                color = Qt::white;
                break;
            }
            painter.setPen(Qt::blue);
            painter.fillRect(size+(i*size),   size+(j*size),   size,   size,   Qt::darkCyan);
            painter.fillRect(size+2+(i*size), size+2+(j*size), size-2, size-1, color);
            //qDebug() << "c "  << " | " << i << "/" << j << " :: " << color;
        }
    }
    painter.setPen(Qt::black);
    painter.setFont(hintsFont);
    // vertical hints
    for (int j = 0; j < height_; j++) {
        painter.drawText(10, (1.5*size)+(size*j), hints_h[j]);
    }
    // horizontal hints
    for (int i = 0; i < width_; i++) {
        for (int ii = 0; ii < hints_w[i].size(); ii++) {
            painter.drawText((1.5*size)+(i*size), padding_text+(ii*font_size), hints_w[i].at(ii));
        }
    }
    if (wonGame) {
        painter.setPen(Qt::red);
        painter.drawText(0, padding_text, "You won!");
    }
    painter.end();
    ui->label->setPicture(pic);
}

void MainWindow::resetField() {
    for (int i = 0; i < width_; i++) {
        for (int j = 0; j < height_; j++) {
            field[i][j] = 0;
        }
    }
}

void MainWindow::randomizeSolution() {
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < width_; i++) {
        for (int j = 0; j < height_; j++) {
            solution[i][j] = dis(gen);
        }
    }
}

void MainWindow::onClicked(QMouseEvent* ev) {
    QPointF p = ev->localPos();
    int x = floor((p.rx() - size) / size);
    int y = floor((p.ry() - size) / size);
    if (x < 0 || y < 0) {
        if (wonGame && x < 1 && y < 0) {
            wonGame = false;
            randomizeSolution();
            setupLabels();
            resetField();
            repaint();
        }
        return;
    }
    qDebug() << ev;
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
        if (field[x][y] == State::Yes) {
            return;
        }
        if (field[x][y] == State::No) {
            change(x, y, State::Empty);
        } else {
            change(x, y, State::No);
        }
    }
    checkStatus();
    repaint();
}

void MainWindow::checkStatus() {
    int matches = 0;
    for (int i = 0; i < width_; i++) {
        for (int j = 0; j < height_; j++) {
            if (field[i][j] == solution[i][j] || field[i][j] == State::No) {
                matches++;
            }
        }
    }
    wonGame = (matches == (width_*height_));
    qDebug() << "RESULT: " << wonGame << " / " << matches;
}

void MainWindow::setupLabels() {
    for (int i = 0; i < width_; i++) {
        QVector<QString> hints;
        int match = 0;
        for (int j = 0; j < height_; j++) {
            if (solution[i][j] == State::Yes) {
                match++;
            } else {
                if (match > 0 && j+1 < height_) {
                    hints.append(QString::number(match));
                    match = 0;
                    continue;
                }
            }
            if (j+1 == height_) {
                if (match > 0) {
                    hints.append(QString::number(match));
                }
                match = 0;
            }
        }
        if (hints.size() < 1) {
            hints.append("0");
        }
        hints_w[i] = hints;
    }
    // vertical labels
    for (int j = 0; j < height_; j++) {
        QString lbl;
        int match = 0;
        for (int i = 0; i < width_; i++) {
            if (solution[i][j] == State::Yes) {
                match++;
            } else {
                if (match > 0 && i+1 < width_) {
                    lbl.append(" ");
                    lbl.append(QString::number(match));
                    match = 0;
                    continue;
                }
            }
            if (i+1 == width_) {
                if (match > 0) {
                    lbl.append(" ");
                    lbl.append(QString::number(match));
                }
                match = 0;
            }
            hints_h[j] = lbl.trimmed().size() > 0 ? lbl.trimmed() : "0";
        }
        //qDebug() << hints_h[j] << endl;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
