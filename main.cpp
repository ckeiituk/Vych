#include <iostream>
#include <math.h>
#define _USE_MATH_DEFINES

typedef double p_type;

p_type f(p_type x){
    return cos(x);
}

class Integral{
private:
    p_type s, e;
    struct weights{
        p_type r;
        p_type w;
    };
    weights w2[2] = {{-0.577350269189626,1.000000000000000}, {0.577350269189626,1.000000000000000}};
public:
    Integral(p_type s, p_type e): s(s), e(e){
        // std::cout << "Введите левый конец: ";
        // std::cin >> a;
        // std::cout << "\nВведите правый конец: ";
        // std::cin >> b;
        // std::cout << '\n';
    }
    p_type Trapezoid(int n){
        p_type h = (e - s) / (n - 1);
        p_type tmp = (f(s) + f(e)) / 2;
        for(int i = 1; i < n-1; i++){
            tmp += f(s + i*h);
        }
        return h*tmp;
    }
    p_type Gauss(p_type a, p_type b){
        p_type sum = 0;
        p_type p = (b - a) / 2;
        p_type q = (b + a) / 2;
        for (int i = 0; i < 2; i++){
            sum += p * w2[i].w * f(p * w2[i].r + q);
        }
    }
    p_type Runge(char in, int n){
        p_type int1 = 0;
        p_type int2 = 0;
        p_type int4 = 0;
        p_type step = (e - s) / (n - 1);
        p_type b, m, m1, m2;
        switch(in){
            case 't':
                int1 = Trapezoid(n);
                int2 = Trapezoid(2*n);
                int4 = Trapezoid(4*n);
                break;
            case 'g':
                for(p_type a = s; a < e; a += step){
                    b = a + step;
                    m = a + step / 2;
                    m1 = a + step / 4;
                    m2 = a + 3 * step / 4;
                    int1 += Gauss(a, b);
                    int2 += Gauss(a, m);
                    int2 += Gauss(m, b);
                    int4 += Gauss(a, m1);
                    int4 += Gauss(m1, m);
                    int4 += Gauss(m, m2);
                    int4 += Gauss(m2, b);
                }
                break;
        }
        return log(abs(int1 - int2)/abs(int2 - int4))/log(2);
    }
    p_type integrate() {
        return sin(e) - sin(s);
    }
};

int main(){
    Integral fin(0, M_PI/2);
    std::cout << "I = " << fin.integrate() << '\n';
    std::cout << "Gauss: " << fin.Gauss(0, M_PI/2) << ",\nTrapezoid: " << fin.Trapezoid(4) << " (h) | " << fin.Trapezoid(40) << " (10*h)\n";
    std::cout << "Gauss: |I-I_n| = " << abs(fin.integrate() - fin.Gauss(0, M_PI/2)) << '\n';
    std::cout << "Trapezoid: |I-I_n| = " << abs(fin.integrate() - fin.Trapezoid(4)) << '\n';     
    std::cout << "Порядок точности для метода трапеций(h) по правилу Рунге: \n" <<fin.Runge('t', 4) << '\n';
    std::cout << "Порядок точности для метода трапеций(10*h) по правилу Рунге: \n" <<fin.Runge('t', 40) << '\n';  
    std::cout << "Порядок точности для метода Гаусса по правилу Рунге: \n" << fin.Runge('g', 4) << '\n';
    return 0;
}
