#include "Vector.h"

Vector::Vector(){}
Vector::Vector(GLfloat x, GLfloat y, GLfloat z)
{
    setCoordinates(x, y, z);
}

void Vector::setCoordinates(GLfloat x, GLfloat y, GLfloat z)
{
    this->v[0] = x;
    this->v[1] = y;
    this->v[2] = z;
}

Vector Vector::operator+(Vector other)
{
    return Vector(v[0] + other[0], v[1] + other[1], v[2] + other[2]);
}

Vector Vector::operator-(Vector other)
{
    return Vector(v[0] - other[0], v[1] - other[1], v[2] - other[2]);
}

Vector Vector::operator*(Vector other)
{
    return Vector(v[1] * other[2] - v[2] * other[1],
                  v[2] * other[0] - v[0] * other[2],
                  v[0] * other[1] - v[1] * other[0]);
}

Vector Vector::operator/(float scalar)
{
    if(scalar != 0)
        return Vector(v[0]/scalar, v[1]/scalar, v[2]/scalar);
    else
        cout << endl << "Exception: attempt to divide a Vector by 0" << endl;
}


Vector Vector::operator*(float scalar)
{
    return Vector(v[0]*scalar, v[1]*scalar, v[2]*scalar);
}

Vector Vector::operator-()
{
    return Vector(-v[0], -v[1], -v[2]);
}

bool Vector::operator==(Vector other)
{
    if(v[0] == other[0] && v[1] == other[1] && v[2] == other[2])
        return true;
    else
        return false;
}

GLfloat Vector::operator[](int index) const
{
    if (index < 3)
        return v[index];
    else
    {
        cout << endl << "Exception: Vector index " << index << " out of bounds";
        return 0;
    }
}

GLfloat Vector::getMagnitude()
{
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

void Vector::normalize()
{
    float m = this->getMagnitude();
    v[0] = v[0]/m;
    v[1] = v[1]/m;
    v[2] = v[2]/m;
}

Vector Vector::getNormalized()
{
    float m = this->getMagnitude();
    return Vector(v[0]/m, v[1]/m, v[2]/m);
}

Vector Vector::getNormal(Vector q, Vector r)
{
    Vector pq = q - *this;
    Vector pr = r - *this;
    Vector normal = pq * pr;
    normal.normalize();

    return normal;
}


ostream &operator<<(ostream &output, const Vector &v) {
	cout << '[' << v[0] << ", " << v[1] << ", " << v[2] << ']';
	return output;
}
