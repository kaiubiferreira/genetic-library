#include <GL/glut.h>
#include <iostream>
#include "math.h"

using namespace std;

class Vector
{
    private:
        GLfloat v[3];

    public:
        Vector();
        Vector(GLfloat x, GLfloat y, GLfloat z);

        void setCoordinates(GLfloat x, GLfloat y, GLfloat z);
        Vector operator+(Vector other);
        Vector operator-(Vector other);
        Vector operator*(Vector other);
        Vector operator/(float scalar);
        Vector operator*(float scalar);
        Vector operator-();

        bool   operator==(Vector);

        GLfloat operator[](int index) const;
        GLfloat getMagnitude();

        void normalize();
        Vector getNormalized();
        Vector getNormal(Vector q, Vector r);
};

std::ostream &operator << (std::ostream &output, const Vector &v);
