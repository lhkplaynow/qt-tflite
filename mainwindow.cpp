#include <QtCore>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    imagewidget = new ImageWidget() ;
    setCentralWidget(imagewidget) ;

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openImage) ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QImage deepseg(const QImage& img) ;

void MainWindow::openImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "/home/lhk/temp/TestImages/people", "All Files (*.*);;Image Files (*.png *.jpg)") ;
    if (!fileName.isEmpty()) {
        QImage image1(fileName) ;
        qDebug() << "format=" << image1.format() << "width=" << image1.width() << "height=" << image1.height() << "depth=" << image1.depth() ;

        if (!image1.isNull()) {
            image = image1 ;
            on_actionNone_triggered() ;
        }
    }
}

void MainWindow::on_actionNone_triggered()
{
    mask = QImage() ;
    imagewidget->repaint() ;
}

void MainWindow::on_actionDeeplabV3_triggered()
{
    if (!image.isNull()) {
        mask = deepseg(image) ;
        imagewidget->repaint() ;
    }
}
