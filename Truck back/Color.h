#include <GL/glut.h>
#include <iostream>

using namespace std;

enum ColorName
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    MAGENTA,
    CYAN,
    BLACK,
    WHITE,
    PINK,
    TURQUOISE,
    GREY,
    SILVER,
    SLATEGRAY,
    VIOLET,
    BROWN,
    ORANGE
};

class Color
{
    private:
        GLfloat red, green, blue;

    public:
        Color(GLfloat red, GLfloat green, GLfloat blue)
        {
           setColor(red, green, blue);
        }

        Color(ColorName color)
        {
            setColor(color);
        }

        Color()
        {
            setColor(BLACK);
        }

        void setColor(ColorName color)
        {
            switch(color)
            {
                case RED: setColor(1.0f, 0.0f, 0.0f);
                    break;
                case GREEN: setColor(0.0f, 1.0f, 0.0f);
                    break;
                case BLUE: setColor(0.0f, 0.0f, 1.0f);
                    break;
                case YELLOW: setColor(1.0f, 1.0f, 0.0f);
                    break;
                case MAGENTA: setColor(1.0f, 0.0f, 1.0f);
                    break;
                case CYAN: setColor(0.0f, 1.0f, 1.0f);
                    break;
                case BLACK: setColor(0.0f, 0.0f, 0.0f);
                    break;
                case WHITE: setColor(1.0f, 1.0f, 1.0f);
                    break;
                case PINK: setColor(1.0f, 0.5f, 1.0f);
                    break;
                case TURQUOISE: setColor(0.5f, 1.0f, 1.0f);
                    break;
                case GREY: setColor(0.33f, 0.33f, 0.33f);
                    break;
                case SILVER: setColor(0.75f, 0.75f, 0.75f);
                    break;
                case SLATEGRAY: setColor(0.44f, 0.5f, 0.56f);
                    break;
                case VIOLET: setColor(0.54f, 0.17f, 0.88f);
                    break;
                case BROWN: setColor(0.54f, 0.27f, 0.08f);
                    break;
                case ORANGE: setColor(1.0f, 0.55f, 0.0f);
                    break;

            }
        }

        void setColor(GLfloat red, GLfloat green, GLfloat blue);

        GLfloat getRed()
        {
            return this->red;
        }

        GLfloat getGreen()
        {
            return this->green;
        }

        GLfloat getBlue()
        {
            return this->blue;
        }

        GLfloat operator[](int index)
        {
            switch(index)
            {
                case 0: return this->red;
                    break;
                case 1: return this->green;
                    break;
                case 2: return this->blue;
                    break;
                default:
                        cout << endl << "EXCEPTION: Color index " << index << " out of bounds, acceptable (0 = red, 1 = green, 2 = blue)" << endl;
                        return 0.0f;
                    break;
            }
        }

        void apply()
        {
            glColor3f(this->red, this->green, this->blue);
        }

        static void reset()
        {
            glColor3f(0.0f, 0.0f, 0.0f);
        }
};
