#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initializeRotateAxisBox();
    void initializeAddDetailBox();

protected:
    void keyPressEvent(QKeyEvent *pe);

private slots:
    void on_addDetail_clicked();
    void on_removeDetail_clicked();

    void on_rotateDetail_clicked();
    void on_rotateScene_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
