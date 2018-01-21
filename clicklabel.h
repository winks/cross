#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>

class ClickLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickLabel(QWidget * parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~ClickLabel();

signals:
    void clicked(QMouseEvent* ev);

protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // CLICKLABEL_H
