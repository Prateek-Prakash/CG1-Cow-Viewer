#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setDefaults();
}

GLWidget::~GLWidget()
{
    glDeleteTextures(1, &tex);
}

void GLWidget::setDefaults()
{
    Mx = My = Mz = 0.0;
    mAngleX = mAngleY = mAngleZ = 0.0;
    nearClipping = 1.0f;
    farClipping = 1000.0f;
    fillMode = true;
    zoomFactor = 1.0;
    Cx = Cy = Cz = 0.0;
    mouseModel = true;
    colorR = colorG = colorB = colorA = 1.0;
    matR = matG = matB = matA = 0.0;
    vLength = hLength = 0.5;
    lightMode = true;
    smoothShading = false;
    checker = true;
    textureOn = true;
}

void GLWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClearDepth(1.0f);
    //Anti-Aliasing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //Enable Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);

    //Light Position
    GLfloat lightPos[] = { 0.5, 0.5, 0.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    tex = loadTexture("Checker.jpg");
    tex2 = loadTexture("Cow.jpg");
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(textureOn)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    if(checker)
        glBindTexture(GL_TEXTURE_2D, tex);
    else
        glBindTexture(GL_TEXTURE_2D, tex2);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Color
    GLfloat ambientLight[]	= {colorR, colorG, colorB, colorA};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    GLfloat materialSpec[] = { matR, matG, matB, matA };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpec);
    //Polygon Mode
    toggleFillMode();
    toggleLighting();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-vLength * zoomFactor, vLength * zoomFactor, -hLength * zoomFactor, hLength * zoomFactor, nearClipping, farClipping);;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Camera Positioning
    gluLookAt(Cx, Cy, (3 * objRad) + Cz, 0, 0, 0, 0, 1, 0);
    //Model Transformations (Reverse Order)
    glTranslatef(Mx, My, Mz);
    glRotatef(mAngleX, 1.0, 0.0, 0.0);
    glRotatef(mAngleY, 0.0, 1.0, 0.0);
    glRotatef(mAngleZ, 0.0, 0.0, 1.0);
    glTranslatef(-centerX, -centerY, -centerZ);
    //glColor3f(colorR, colorG, colorB);

    //Model Creation
    glBegin(GL_TRIANGLES);
        for(int i = 0; i < face.length(); i++)
        {
            if(smoothShading == true)
                glNormal3f(Vx[face[i][0] -1], Vy[face[i][0] -1], Vz[face[i][0] -1]);
            else
                glNormal3f(Nx[i], Ny[i], Nz[i]);
            glTexCoord2f( 0.0f, 0.0f ); glVertex3f(vertex[face[i][0] - 1][0], vertex[face[i][0] - 1][1], vertex[face[i][0] - 1][2]);
            if(smoothShading == true)
                glNormal3f(Vx[face[i][1] -1], Vy[face[i][1] -1], Vz[face[i][1] -1]);
            glTexCoord2f( 1.0f, 0.0f ); glVertex3f(vertex[face[i][1] - 1][0], vertex[face[i][1] - 1][1], vertex[face[i][1] - 1][2]);
            if(smoothShading == true)
                glNormal3f(Vx[face[i][2] -1], Vy[face[i][2] -1], Vz[face[i][2] -1]);
            glTexCoord2f( 0.0f, 1.0f ); glVertex3f(vertex[face[i][2] - 1][0], vertex[face[i][2] - 1][1], vertex[face[i][2] - 1][2]);
        }
    glEnd();
    glFlush();
}

void GLWidget::updateGL()
{
    update();
}

void GLWidget::toggleFillMode()
{
    if(fillMode == true)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPosition = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dX = event->x() - lastPosition.x();
    int dY = event->y() - lastPosition.y();
    if(event->buttons() & Qt::LeftButton)
    {
        if(mouseModel)
        {
            mAngleX = setAngle(mAngleX + 1 * dY);
            mAngleY = setAngle(mAngleY + 1 * dX);
        }
        else
        {
            Cx = Cx + dX;
            Cy = Cy + dY;
        }
    }
    else if(event->buttons() & Qt::RightButton)
    {
        if(mouseModel)
        {
            mAngleX = setAngle(mAngleX + 1 * dY);
            mAngleZ = setAngle(mAngleZ - 1 * dX);
        }
        else
        {
            Cx = Cx  + dX;
            Cz = Cz + dY;
        }
    }
    lastPosition = event->pos();
    emit sendAngles();
    emit sendCameraT();
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
     emit sendZoom(event->delta() / 25);
}

GLfloat GLWidget::setAngle(GLfloat angle)
{
    if(angle > 360)
        return angle - 360;
    else if (angle < -360)
        return angle + 360;
    else
        return angle;
}

void GLWidget::toggleLighting()
{
    if(lightMode == true)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);
}

GLuint GLWidget::loadTexture(const char *source)
{
    return SOIL_load_OGL_texture
                (
                    source,
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
                );
}
