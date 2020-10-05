#include <GL/glut.h>
#include <cstdio>
#include <cassert>

void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
        glVertex3f(0.5, 0.0, 0.5);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
        glVertex3f(0.0, 0.0, 0.5);
    glEnd();
    // glFlush();
    glutSwapBuffers();
    int imgW = 400, imgH = 300;
    auto *pixels = new unsigned char[3 * imgW * imgH];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    // glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, imgW, imgH, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    for (int i = 0; i < 3 * imgW * imgH; i++) {
        if (pixels[i] != 0) {
            printf("nonzero %d %d %d\n", (i / 3) % imgW, (i / 3) / imgW, i % 3);
            exit(0);
        }
    }
}
 
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world!");
    glutDisplayFunc(drawScene);
    glutMainLoop();
    return 0;
}
