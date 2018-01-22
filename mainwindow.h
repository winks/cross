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
    void randomizeSolution();
    void checkStatus();
    void resetField();
    void setupLabels();
    void repaint();
    void change(int x, int y, State z);

private:
    Ui::MainWindow *ui;
    QPicture pic;
    QPainter painter;
    QFont hintsFont;

    const static int font_size = 26;
    const static int width_ = 7;
    const static int height_ = 5;
    const static int border_left = 0;
    const static int border_top = 0;
    const static int padding_text = 25;
    const static int size = 100;

    bool wonGame = false;
    int field[width_][height_];
    int solution[width_][height_] = { {1,1,1,1,1}, {1,1,0,1,1}, {}, {}, {}, {}, {} };
    QVector<QString> hints_w[width_];
    QString hints_h[height_];
};

#endif // MAINWINDOW_H
