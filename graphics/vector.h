#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>

template <class T>
class Vector
{
    public:
    /*
        Declared in public, since putting in private
        and writing getter-setters is an exercise
        in inefficiency
    */
        T x,y,z;
    
    //  Null vector constructor
        Vector<T> ()
            {x = 0, y = 0, z = 0;}
        
    //  Vector constructor
        Vector<T> (T a,T b,T c)
        {
            x = a;
            y = b;
            z = c;    
        }
        
    //  Returns magnitude
        double magnitude()
            { return sqrt(magnitude2()); }
        
    //  Returns square of magnitude
        T magnitude2()
            { return x*x+y*y+z*z; }
            
    //  Operator overloading for adding two vectors (component addition)
        Vector<T> operator+ (const Vector<T>& other)
        {
            Vector<T> v;
            v.x = other.x + this->x;
            v.y = other.y + this->y;  
            v.z = other.z + this->z;
            return v;
        }
    
    //  Operator overloading for adding two vectors (component subtraction)  
        Vector<T> operator- (const Vector<T>& other)
        {
            Vector<T> v;
            v.x = other.x - this->x;
            v.y = other.y - this->y;  
            v.z = other.z - this->z;
            return v;
        } 
        
    //  Scalar product of two vectors
        T operator* (const Vector<T>& other)
        {
            T sum=0;
            sum += other.x * this->x;
            sum += other.y * this->y;  
            sum += other.z * this->z;
            return sum;
        }
        
    //  Scalar multiplication by vector
        Vector<T> operator* (double k)
        {
            Vector<T> v;
            v.x *= k; v.y *= k, v.z *= k;
            return v;
        }
        
    //  Cross product of two vectors
        Vector<T> operator^ (const Vector<T>& other)
        {
            Vector<T> v;
            v.x =    this->y * other.z - this->z * other.y ;
            v.y = - (this->x * other.z - this->z * other.x);
            v.z =    this->x * other.y - this->y * other.x ;
            return v;
        }
    
    //  Assignment of vector
        Vector<T> operator=(const Vector<T>& other)
        {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
        }
    
    //  Cross product of two vectors
        Vector<T> operator==(const Vector<T>& other)
        {
            return this->x==other.x && this->y==other.y
                   && this->z==other.z;
        }
           
    //  Angle between two vectors
    //  Returns angle in radians
        static double angleBetween(Vector<T> v1, Vector<T> v2)
            { return acos(v1*v2/sqrt(v1.magnitude2()*v2.magnitude2())); }
        
    //  Normalize vector
        double normalize()
        {
            Vector v;
            double mag = magnitude(v);
            v.x /= mag , v.y /= mag, v.z /= mag;
            return v;
        }
        
    //  Check if vector is null i.e magnitude=0
        bool isNull()
            { return x=y=z=0; } 
};

//  Allow vector components to be chained in ostream
    template <class T>
    std::ostream& operator<<(std::ostream &os, Vector<T> const &v)
        { return os << v.x << " " << v.y << " " << v.z; }

/*

int main()
{
    Vector<float> v(1,2,3);
    Vector<float> p;
    p.x = 5, p.y = 2, p.z = 3;
    std::cout<<p;
    Vector<float> q = v^p;
    double r;

    r = v*p;
    std::cout<<q<<r;
    std::cout<<Vector<float>::angleBetween(v,v)<<" "<<Vector<float>::angleBetween(v,p);
    return 0;   
}
*/

#endif