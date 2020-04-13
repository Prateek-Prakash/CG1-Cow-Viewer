#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QtMath>
#include <QMouseEvent>
#include "gl/GLU.h"
#include "SOIL.h"

#pragma comment(lib, "glu32")

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLuint tex;
    GLuint tex2;

    bool checker;

    bool fillMode;
    bool mouseModel;

    bool lightMode;
    bool smoothShading;
    bool textureOn;

    QVector<GLfloat> Nx, Ny, Nz;
    QVector<float> Vx, Vy, Vz;

    GLdouble Mx, My, Mz;
    GLdouble zoomFactor;

    GLfloat centerX, centerY, centerZ;    
    GLfloat mAngleX, mAngleY, mAngleZ;
    GLfloat nearClipping;
    GLfloat farClipping;
    GLfloat objRad;
    GLfloat Cx, Cy, Cz;
    GLfloat colorR, colorG, colorB, colorA;
    GLfloat matR, matG, matB, matA;
    GLfloat vLength, hLength;

    QPoint lastPosition;

    QVector<QVector<GLfloat>> vertex;
    QVector<QVector<int>> face;

    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void setDefaults();
    void initializeGL();
    void paintGL();
    void updateGL();
    void toggleFillMode();
    void toggleLighting();

    GLuint loadTexture(const char* source);

    GLfloat setAngle(GLfloat angle);

private slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

signals:
    void sendAngles();
    void sendZoom(int X);
    void sendCameraT();
};

#endif // GLWIDGET_H
