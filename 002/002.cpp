#include <iostream>
#include <cmath>
#include <ostream>
using std::cout, std::endl, std::ostream;


class Complex {
    public:
    virtual double real() const = 0;
    virtual double imag() const = 0;
    virtual double arg() const = 0;
    virtual double abs() const = 0;

    virtual ~Complex() = default;
};

class AlgComplex : public Complex {
    private:
    double re, im;
    public:
    AlgComplex(double r, double i) : re(r), im(i) {}

    double real() const override {
        return re;
    }
    double imag() const override {
        return im;
    }
    double arg() const override {
        return std::atan2(im, re);
    }
    double abs() const override {
        return std::sqrt(re * re + im * im);
    }
};

class TrigComplex : public Complex {
    private:
    double r, phi;
    public:
    TrigComplex(double r, double phi) : r(r), phi(phi) {}

    double real() const override {
        return r*std::cos(phi);
    }
    double imag() const override {
        return r*std::sin(phi);
    }
    double arg() const override {
        return phi;
    }
    double abs() const override {
        return r;
    }
};

AlgComplex operator+(const Complex& num1, const Complex& num2) {
    return AlgComplex(num1.real() + num2.real(), num1.imag() + num2.imag());
};

AlgComplex operator-(const Complex& num1, const Complex& num2){
    return AlgComplex(num1.real() - num2.real(), num1.imag() - num2.imag());
}

AlgComplex operator*(const Complex& num1, const Complex& num2){
    return AlgComplex(num1.real() * num2.real() - num1.imag() * num2.imag(), num1.real() * num2.imag() + num1.imag() * num2.real());
}

AlgComplex operator/(const Complex& num1, const Complex& num2){
    double delim = num2.real() * num2.real() + num2.imag() * num2.imag();
    return AlgComplex((num1.real() * num2.real() + num1.imag() * num2.imag())/ delim, 
    (num1.real() * (-1) * num2.imag() + num1.imag() * num2.real()) / delim);
}

AlgComplex operator*(double k, const Complex& num) {
    return AlgComplex(k * num.real(), k * num.imag());
};

AlgComplex operator-(const Complex& num) {
    return AlgComplex(-num.real(), -num.imag());
}

TrigComplex sqrt_complex(const Complex& num) {
    return TrigComplex(std::sqrt(num.abs()), num.arg() / 2.);
}

struct Roots {
    AlgComplex x1, x2;
};

Roots solve(const Complex& A, const Complex& B, const Complex& C) {
    AlgComplex D = B * B - 4. * A * C;
    TrigComplex sqrt_D = sqrt_complex(D);

    AlgComplex A_2 = 2. *  A;
    AlgComplex x1 = (-B + sqrt_D) / A_2;
    AlgComplex x2 = (-B - sqrt_D) / A_2;

    return {x1, x2};
};

ostream& operator<<(ostream& os, const Complex& num) {
    return os << num.real() << (num.imag() >= 0 ? " + " : " - ") << std::abs(num.imag()) << "i";;
};

int main() {
    AlgComplex a(3, 11);
    AlgComplex b(24, 2);
    TrigComplex c(5., M_PI/8.);
    TrigComplex d(2., M_PI/7.);

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;
    cout << "d = " << d << endl;

    cout << "|a| = " << a.abs() << endl;
    cout << "|c| = " << c.abs() << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a - c = " << a - c << endl;
    cout << "c * d = " << c * d << endl;
    cout << "b / c = " << b / c << endl;
    cout << "arg(b) = " << b.arg() << endl;
    cout << "arg(d) = " << d.arg() << endl;
    cout << endl;

    AlgComplex A1(1, 0), B1(1, 0), C1(1, 0);
    Roots r1 = solve(A1, B1, C1);
    cout << "x^2 + x + 1 = 0" << endl;
    cout << "x1 = " << r1.x1 << ", x2 = " << r1.x2 << endl;

    return 0;
}
