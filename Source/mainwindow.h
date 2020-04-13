#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openFile(QString path);

private slots:
    void on_sliderMx_sliderMoved(int position);
    void on_sliderMx_valueChanged(int value);
    void on_sliderMy_sliderMoved(int position);
    void on_sliderMy_valueChanged(int value);
    void on_sliderMz_sliderMoved(int position);
    void on_sliderMz_valueChanged(int value);
    void on_fillModeToggle_clicked();
    void on_sliderRotate_sliderMoved(int position);
    void on_sliderRotate_valueChanged(int value);
    void on_radioX_clicked();
    void on_radioY_clicked();
    void on_radioZ_clicked();
    void on_resetButton_clicked();
    void on_sliderZoom_sliderMoved(int position);
    void on_sliderZoom_valueChanged(int value);
    void receiveRotation();
    void receiveZoom(int X);
    void on_farSpinBox_valueChanged(double arg1);
    void on_nearSpinBox_valueChanged(double arg1);
    void on_openFileButton_clicked();
    void on_sliderCx_sliderMoved(int position);
    void on_sliderCx_valueChanged(int value);
    void on_sliderCy_sliderMoved(int position);
    void on_sliderCy_valueChanged(int value);
    void on_sliderCz_sliderMoved(int position);
    void on_sliderCz_valueChanged(int value);
    void on_mouseModel_clicked();
    void on_mouseCamera_clicked();
    void receiveCameraT();
    void on_spinBoxR_valueChanged(double arg1);
    void on_spinBoxG_valueChanged(double arg1);
    void on_spinBoxB_valueChanged(double arg1);
    void on_spinBoxA_valueChanged(double arg1);
    void on_vLength_valueChanged(double arg1);
    void on_hLength_valueChanged(double arg1);

    void on_lightToggle_clicked();

    void on_spinBoxR_2_valueChanged(double arg1);

    void on_spinBoxG_2_valueChanged(double arg1);

    void on_spinBoxB_2_valueChanged(double arg1);

    void on_spinBoxA_2_valueChanged(double arg1);

    void on_shadingToggle_clicked();

    void on_textureSource_clicked();

    void on_textureToggle_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
