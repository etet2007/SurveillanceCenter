#include "mainwindow.h"

#include <QtWidgets>

const int QlabelWidth= 16*100;
const int QlabelHeight= 9*100;


//MainWindow类继承QMainWindow
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    scaleFactor=1.0;

    //create imageLabel object
    imageLabel = new QLabel;
    //imageLabel->setBackgroundRole(QPalette::Base);//Sets the background role of the widget to role.
    imageLabel->setFixedSize(QlabelWidth,QlabelHeight);//Sets both the minimum and maximum sizes of the widget to s, thereby preventing it from ever growing or shrinking.
    imageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    imageLabel->setScaledContents(false);//When enabled and the label shows a pixmap, it will scale the pixmap to fill the available space.

    //frame of imageLabel
    imageLabel->setFrameShape(QFrame::Box);
    imageLabel->setLineWidth(3);


    setCentralWidget(imageLabel);

    createActions();
    createMenus();

    loadFile("C:\\Users\\Administrator\\Desktop\\视屏监控全景融合与场景再现系统\backGround.jpg");

    //MainWindow init
    setWindowFilePath("监控中心");
    resize(QGuiApplication::primaryScreen()->availableSize() );//QWidget类的方法 void	resize(const QSize &)

}


bool MainWindow::loadFile(const QString &fileName)
{
    QImage imageRead(fileName);//QImage对象 image

    //fail to load the image
    if (imageRead.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));//Returns a copy of this string with the lowest numbered place marker replaced by string a, i.e., %1, %2, ..., %99.
        setWindowFilePath(QString());//QString() Constructs a null string. Null strings are also empty.
        imageLabel->setPixmap(QPixmap());//QPixmap() Constructs a null pixmap.
        imageLabel->adjustSize();//Adjusts the size of the widget to fit its contents.
        return false;
    }

    imageDisplay=imageRead.scaled(QlabelWidth,QlabelHeight,Qt::KeepAspectRatioByExpanding);

    imageLabel->setPixmap(QPixmap::fromImage(imageDisplay));  //QLabel's method:setPixmap


    return true;
}


void MainWindow::open()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);//append each mimeTypeName to mimeTypeFilters

    mimeTypeFilters.sort();

    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);

    //Constructs a file dialog with the given parent and caption that initially displays the contents of the specified directory.
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);//Sets the filters used in the file dialog, from a list of MIME types.
    dialog.selectMimeTypeFilter("image/jpeg");//Sets the current MIME type filter.

    while (
           dialog.exec() == //OK, to the accept() slot and a Cancel button to the reject() slot.
           QDialog::Accepted && ! loadFile(dialog.selectedFiles().first()) //Click Cancel  jump out while ,Clike Ok and loadFile returns fale restart the dialog
           )
    {}         //loadFile return false ,restart the dialog
}

void MainWindow::zoomIn()
{
    scaleImage(1.25);
}
void MainWindow::rotate()
{

    QMatrix matrix;
    matrix.rotate(10);
    imageDisplay= imageDisplay.transformed(matrix);
    imageLabel->setPixmap(QPixmap::fromImage(imageDisplay));
}

void MainWindow::scaleImage(double factor)
{
//    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageDisplay=imageDisplay.scaled(QlabelWidth*scaleFactor,QlabelHeight*scaleFactor,Qt::KeepAspectRatioByExpanding);
    imageLabel->setPixmap(QPixmap::fromImage(imageDisplay));
}


void MainWindow::createActions()
{
    //in the header file ,QAction *openAct; now give it an object and connect QAction object to SIGNAL SLOT
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    rotateAct=new QAction(tr("Rotate 270"),this);
    connect(rotateAct, SIGNAL(triggered()),this, SLOT(rotate()));

}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(rotateAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
}
