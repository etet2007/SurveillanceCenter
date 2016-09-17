#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool loadFile(const QString &);


private slots:


private:
    void createActions();
    void createMenus();

    void scaleImage(double factor);

    double scaleFactor;

    QImage imageDisplay;

    QLabel *imageLabel;


    QAction *openAct;
    QAction *zoomInAct;
    QAction *rotateAct;
    QMenu *viewMenu;
    QMenu *fileMenu;


};

#endif
