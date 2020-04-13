#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openGLWidget, SIGNAL(sendAngles()), this, SLOT(receiveRotation()));
    connect(ui->openGLWidget, SIGNAL(sendZoom(int)), this, SLOT(receiveZoom(int)));
    connect(ui->openGLWidget, SIGNAL(sendCameraT()), this, SLOT(receiveCameraT()));
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::openFile(QString path)
{
    QFile inputFile(path);
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        //File Error
    }
    else
    {
        ui->openGLWidget->vertex.clear();
        ui->openGLWidget->face.clear();
        ui->openGLWidget->Nx.clear();
        ui->openGLWidget->Ny.clear();
        ui->openGLWidget->Nz.clear();
        ui->openGLWidget->Vx.clear();
        ui->openGLWidget->Vy.clear();
        ui->openGLWidget->Vz.clear();
        int tempCount = 0;
        GLfloat xMin, xMax, yMin, yMax, zMin, zMax;
        while (!inputFile.atEnd())
        {
            QRegExp regEx("(\\ |\\t)");
            QString line = inputFile.readLine();
            QStringList list = line.split(regEx);
            if(list[0] == "v")
            {
                if(tempCount++ == 0)
                {
                    xMin = xMax = list[1].toFloat();
                    yMin = yMax = list[2].toFloat();
                    zMin = zMax = list[3].toFloat();
                }
                else
                {
                    if(list[1].toFloat() < xMin)
                        xMin = list[1].toFloat();
                    if(list[1].toFloat() > xMax)
                        xMax = list[1].toFloat();
                    if(list[2].toFloat() < yMin)
                        yMin = list[2].toFloat();
                    if(list[2].toFloat() > yMax)
                        yMax = list[2].toFloat();
                    if(list[3].toFloat() < zMin)
                        zMin = list[3].toFloat();
                    if(list[3].toFloat() > zMax)
                        zMax = list[3].toFloat();
                }
                QVector<GLfloat> points;
                points.push_back(list[1].toFloat());
                points.push_back(list[2].toFloat());
                points.push_back(list[3].toFloat());
                ui->openGLWidget->vertex.push_back(points);

            }
            if(list[0] == "f")
            {
                QVector<int> points;
                points.push_back(list[1].toInt());
                points.push_back(list[2].toInt());
                points.push_back(list[3].toInt());
                ui->openGLWidget->face.push_back(points);
            }
        }
        inputFile.close();
        on_resetButton_clicked();
        ui->openGLWidget->centerX = (xMin + xMax) / 2.0f;
        ui->openGLWidget->centerY = (yMin + yMax) / 2.0f;
        ui->openGLWidget->centerZ = (zMin + zMax) / 2.0f;
        ui->openGLWidget->objRad = qMax(qMax(xMax - xMin, yMax - yMin), zMax - zMin) / 2.0f;
        ui->openGLWidget->updateGL();

        float * Verx = new float[ui->openGLWidget->vertex.length()];
        float * Very = new float[ui->openGLWidget->vertex.length()];
        float * Verz = new float[ui->openGLWidget->vertex.length()];
        for(int i = 0; i < ui->openGLWidget->vertex.length(); i++)
        {
            Verx[i] = Very[i] = Verz[i] = 0.0;
        }

        for(int i = 0; i < ui->openGLWidget->face.length(); i++)
        {
            float Ux = ui->openGLWidget->vertex[ui->openGLWidget->face[i][1] - 1][0] - ui->openGLWidget->vertex[ui->openGLWidget->face[i][0] - 1][0];
            float Uy = ui->openGLWidget->vertex[ui->openGLWidget->face[i][1] - 1][1] - ui->openGLWidget->vertex[ui->openGLWidget->face[i][0] - 1][1];
            float Uz = ui->openGLWidget->vertex[ui->openGLWidget->face[i][1] - 1][2] - ui->openGLWidget->vertex[ui->openGLWidget->face[i][0] - 1][2];

            float Vx = ui->openGLWidget->vertex[ui->openGLWidget->face[i][2] - 1][0] - ui->openGLWidget->vertex[ui->openGLWidget->face[i][0] - 1][0];
            float Vy = ui->openGLWidget->vertex[ui->openGLWidget->face[i][2] - 1][1] - ui->openGLWidget->vertex[ui->openGLWidget->face[i][0] - 1][1];
            float Vz = ui->openGLWidget->vertex[ui->openGLWidget->face[i][2] - 1][2] - ui->openGLWidget->vertex[ui->openGLWidget->face[i][0] - 1][2];

            float Nx = (Uy*Vz) - (Uz*Vy);
            float Ny = (Uz*Vx) - (Ux*Vz);
            float Nz = (Ux*Vy) - (Uy*Vx);

            float mag = qSqrt((Nx*Nx) + (Ny*Ny) + (Nz*Nz));
            ui->openGLWidget->Nx.push_back(Nx / mag);
            ui->openGLWidget->Ny.push_back(Ny / mag);
            ui->openGLWidget->Nz.push_back(Nz / mag);

            Verx[ui->openGLWidget->face[i][0] - 1] += Nx / mag;
            Very[ui->openGLWidget->face[i][1] - 1] += Ny / mag;
            Verz[ui->openGLWidget->face[i][2] - 1] += Nz / mag;
        }

        for(int i = 0; i < ui->openGLWidget->vertex.length(); i++)
        {
            ui->openGLWidget->Vx.push_back(Verx[i]);
            ui->openGLWidget->Vy.push_back(Very[i]);
            ui->openGLWidget->Vz.push_back(Verz[i]);
        }

        delete [] Verx;
        delete [] Very;
        delete [] Verz;
    }
}

void MainWindow::on_sliderMx_sliderMoved(int position)
{
    ui->openGLWidget->Mx = position;
    ui->editMx->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderMx_valueChanged(int value)
{
    on_sliderMx_sliderMoved(value);
}

void MainWindow::on_sliderMy_sliderMoved(int position)
{
    ui->openGLWidget->My = position;
    ui->editMy->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderMy_valueChanged(int value)
{
    on_sliderMy_sliderMoved(value);
}

void MainWindow::on_sliderMz_sliderMoved(int position)
{
    ui->openGLWidget->Mz = position;
    ui->editMz->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderMz_valueChanged(int value)
{
    on_sliderMz_sliderMoved(value);
}

void MainWindow::on_fillModeToggle_clicked()
{
    if(ui->openGLWidget->fillMode == true)
        ui->openGLWidget->fillMode = false;
    else
        ui->openGLWidget->fillMode = true;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderRotate_sliderMoved(int position)
{
    if(ui->radioX->isChecked())
    {
        ui->openGLWidget->mAngleX = ui->openGLWidget->setAngle(position);
        ui->editRx->setText(QString::number(position));
    }
    else if(ui->radioY->isChecked())
    {
        ui->openGLWidget->mAngleY = ui->openGLWidget->setAngle(position);
        ui->editRy->setText(QString::number(position));
    }
    else if(ui->radioZ->isChecked())
    {
        ui->openGLWidget->mAngleZ = ui->openGLWidget->setAngle(position);
        ui->editRz->setText(QString::number(position));
    }
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderRotate_valueChanged(int value)
{
    on_sliderRotate_sliderMoved(value);
}

void MainWindow::on_radioX_clicked()
{
    ui->sliderRotate->setValue(ui->openGLWidget->mAngleX);
}

void MainWindow::on_radioY_clicked()
{
    ui->sliderRotate->setValue(ui->openGLWidget->mAngleY);
}

void MainWindow::on_radioZ_clicked()
{
    ui->sliderRotate->setValue(ui->openGLWidget->mAngleZ);
}

void MainWindow::on_resetButton_clicked()
{
    ui->openGLWidget->setDefaults();
    ui->sliderMx->setValue(0);
    ui->sliderMy->setValue(0);
    ui->sliderMz->setValue(0);
    ui->radioX->setChecked(true);
    ui->radioX->setChecked(false);
    ui->radioX->setChecked(false);
    ui->sliderRotate->setValue(0);
    ui->nearSpinBox->setValue(0.1);
    ui->farSpinBox->setValue(1000.0);
    ui->sliderZoom->setValue(100.0);
    ui->editRy->setText("0");
    ui->editRz->setText("0");
    ui->editCx->setText("0");
    ui->editCy->setText("0");
    ui->editCz->setText("0");
    ui->mouseModel->setChecked(true);
    ui->mouseCamera->setChecked(false);
    ui->sliderCx->setValue(0);
    ui->sliderCy->setValue(0);
    ui->sliderCz->setValue(0);
    ui->spinBoxR->setValue(1.0);
    ui->spinBoxG->setValue(1.0);
    ui->spinBoxB->setValue(1.0);
    ui->spinBoxA->setValue(1.0);
    ui->vLength->setValue(0.5);
    ui->hLength->setValue(0.5);
    ui->spinBoxR_2->setValue(0.0);
    ui->spinBoxG_2->setValue(0.0);
    ui->spinBoxB_2->setValue(0.0);
    ui->spinBoxA_2->setValue(0.0);
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderZoom_sliderMoved(int position)
{
    if(position == 0.0)
        position = 1.0;
    ui->openGLWidget->zoomFactor = 100.0 / position;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderZoom_valueChanged(int value)
{
    on_sliderZoom_sliderMoved(value);
}

void MainWindow::receiveRotation()
{
    if(ui->radioX->isChecked())
        ui->sliderRotate->setValue(ui->openGLWidget->mAngleX);
    else if(ui->radioY->isChecked())
        ui->sliderRotate->setValue(ui->openGLWidget->mAngleY);
    else if(ui->radioZ->isChecked())
        ui->sliderRotate->setValue(ui->openGLWidget->mAngleZ);
    ui->editRx->setText(QString::number(ui->openGLWidget->mAngleX));
    ui->editRy->setText(QString::number(ui->openGLWidget->mAngleY));
    ui->editRz->setText(QString::number(ui->openGLWidget->mAngleZ));
}

void MainWindow::receiveZoom(int X)
{
    ui->sliderZoom->setValue(ui->sliderZoom->value() + X);
}

void MainWindow::on_farSpinBox_valueChanged(double arg1)
{
    ui->openGLWidget->farClipping = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_nearSpinBox_valueChanged(double arg1)
{
    ui->openGLWidget->nearClipping = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_openFileButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath(), tr("Object Files (*.obj)"));
    if(!path.isEmpty() && !path.isNull())
    {
        openFile(path);
    }
}

void MainWindow::on_sliderCx_sliderMoved(int position)
{
    ui->openGLWidget->Cx = position;
    ui->editCx->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderCx_valueChanged(int value)
{
    on_sliderCx_sliderMoved(value);
}

void MainWindow::on_sliderCy_sliderMoved(int position)
{
    ui->openGLWidget->Cy = position;
    ui->editCy->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderCy_valueChanged(int value)
{
    on_sliderCy_sliderMoved(value);
}

void MainWindow::on_sliderCz_sliderMoved(int position)
{
    ui->openGLWidget->Cz = position;
    ui->editCz->setText(QString::number(position));
    ui->openGLWidget->updateGL();
}

void MainWindow::on_sliderCz_valueChanged(int value)
{
    on_sliderCz_sliderMoved(value);
}

void MainWindow::on_mouseModel_clicked()
{
    ui->openGLWidget->mouseModel = true;
}

void MainWindow::on_mouseCamera_clicked()
{
    ui->openGLWidget->mouseModel = false;
}

void MainWindow::receiveCameraT()
{
    ui->sliderCx->setValue(ui->openGLWidget->Cx);
    ui->sliderCy->setValue(ui->openGLWidget->Cy);
    ui->sliderCz->setValue(ui->openGLWidget->Cz);
}

void MainWindow::on_spinBoxR_valueChanged(double arg1)
{
    ui->openGLWidget->colorR = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_spinBoxG_valueChanged(double arg1)
{
    ui->openGLWidget->colorG = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_spinBoxB_valueChanged(double arg1)
{
    ui->openGLWidget->colorB = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_spinBoxA_valueChanged(double arg1)
{
    ui->openGLWidget->colorA = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_vLength_valueChanged(double arg1)
{
    ui->openGLWidget->vLength = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_hLength_valueChanged(double arg1)
{
    ui->openGLWidget->hLength = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_lightToggle_clicked()
{
    if(ui->openGLWidget->lightMode == true)
        ui->openGLWidget->lightMode = false;
    else
        ui->openGLWidget->lightMode = true;
    ui->openGLWidget->updateGL();
}


void MainWindow::on_spinBoxR_2_valueChanged(double arg1)
{
    ui->openGLWidget->matR = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_spinBoxG_2_valueChanged(double arg1)
{
    ui->openGLWidget->matG = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_spinBoxB_2_valueChanged(double arg1)
{
    ui->openGLWidget->matB = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_spinBoxA_2_valueChanged(double arg1)
{
    ui->openGLWidget->matA = arg1;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_shadingToggle_clicked()
{
    if(ui->openGLWidget->smoothShading == true)
        ui->openGLWidget->smoothShading = false;
    else
        ui->openGLWidget->smoothShading = true;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_textureSource_clicked()
{
    if(ui->openGLWidget->checker == true)
        ui->openGLWidget->checker = false;
    else
        ui->openGLWidget->checker = true;
    ui->openGLWidget->updateGL();
}

void MainWindow::on_textureToggle_clicked()
{
    if(ui->openGLWidget->textureOn == true)
        ui->openGLWidget->textureOn = false;
    else
        ui->openGLWidget->textureOn = true;
    ui->openGLWidget->updateGL();
}
