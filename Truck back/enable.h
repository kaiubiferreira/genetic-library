#include <GL/glut.h>
#include "Color.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

class Settings
{
    private:
        GLuint windowsWidth;
        GLuint windowsHeight;
        GLuint windowsPositionX;
        GLuint windowsPositionY;
        Color backgroundColor;
        const char * title;

    public:
        Settings(){}

        Settings(void (GLUTCALLBACK * func)(void), const char * title = "Untitled", Color backgroundColor = *new Color(YELLOW), int windowsWidth = 640, int windowsHeight = 480, int windowsPositionX = 0, int windowsPositionY = 0)
        {
            this->windowsWidth = windowsWidth;
            this->windowsHeight = windowsHeight;
            this->windowsPositionX = windowsPositionX;
            this->windowsPositionY = windowsPositionY;
            this->backgroundColor = backgroundColor;
            this->title = title;

            glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

            glutInitWindowSize(windowsWidth, windowsHeight);
            glutInitWindowPosition(windowsPositionX,windowsPositionY);
            glutCreateWindow(title);

            glEnable(GL_DEPTH_TEST);
            glEnable(GL_COLOR_MATERIAL);
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_NORMALIZE);
            glShadeModel(GL_SMOOTH);

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            //setBackgroundColor(backgroundColor);

            glutReshapeFunc(resize);
            glutKeyboardFunc(key);
            glutDisplayFunc(func);
        }

        void setBackgroundColor(Color backgroundColor);

        void setWindowsSize(int width, int height);

        void setTitle(const char * title);

        static void resize(int width, int height);
        static void key(unsigned char key, int x, int y);
};
