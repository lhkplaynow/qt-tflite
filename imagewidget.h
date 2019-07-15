#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>


class ImageWidget : public QWidget {
    Q_OBJECT
protected:
public:
    explicit ImageWidget(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent*);
};

#endif // IMAGEWIDGET_H
