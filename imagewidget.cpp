#include <QtWidgets>
#include "imagewidget.h"
#include "mainwindow.h"

ImageWidget::ImageWidget(QWidget* parent) : QWidget(parent) {
    setAutoFillBackground(false) ;
}

void ImageWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this) ;
    painter.fillRect(this->rect(), Qt::gray) ;

    QImage& image = reinterpret_cast<MainWindow*>(parentWidget())->image ;
    if (!image.isNull()) {
        // rcDest 계산
        QSize sz = this->rect().size() ;
        QSize szSrc = image.rect().size() ;

        float scale = float(sz.width()) / szSrc.width() ;
        int w, h ;

        h = int(scale * szSrc.height() + 0.5f) ;
        if (h > sz.height()) {
            scale = float(sz.height()) / szSrc.height() ;
        }

        w = int(scale * szSrc.width() + 0.5f) ;
        h = int(scale * szSrc.height() + 0.5f) ;
        int left = this->rect().left() + (sz.width() - w) / 2 ;
        int top = this->rect().top() + (sz.height() - h) / 2 ;
        QRect rcDest(left, top, w, h) ;

        painter.drawImage(rcDest, image, image.rect()) ;

        QImage& mask = reinterpret_cast<MainWindow*>(parentWidget())->mask ;
        if (!mask.isNull()) {
            painter.setOpacity(0.7) ;
            painter.drawImage(rcDest, mask, mask.rect()) ;
        }
    }
}
