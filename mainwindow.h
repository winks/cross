#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPicture>

namespace Ui {
class MainWindow;
}
enum State {Empty, Yes, No};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onClicked(QMouseEvent* ev);

private:
    void checkStatus();
    void reset();
    void repaint();
    void change(int x, int y, State z);

private:
    Ui::MainWindow *ui;
    QPicture pic;
    QPainter painter;
    const static int num = 5;
    int size = 100;
    int field[num][num];
    int solution[num][num] = { {1,1,1,1,1},{}, {}, {}, {}};
};

#endif // MAINWINDOW_H
