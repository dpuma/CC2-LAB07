#include<iostream>
using namespace std;

class Holder
{
    int* m_data;
    int m_size;

public:
    Holder(int size)         // Constructor
    {
        cout << "ctor. normal" << endl;
        m_data = new int[size];
        m_size = size;
    }
    ~Holder()                // Destructor
    {
        delete[] m_data;
    }

    Holder(const Holder& other)
    {
        cout << "ctor. copia normal" << endl;

        m_data = new int[other.m_size];  // (1)
        std::copy(other.m_data, other.m_data + other.m_size, m_data);  // (2)
        m_size = other.m_size;
    }

    Holder(Holder&& other)     // <-- usando referencia rvalue
    {
        cout << "ctor. usando rvalue reference" << endl;

        m_data = other.m_data;   // (1)
        m_size = other.m_size;
        other.m_data = nullptr;  // (2)
        other.m_size = 0;
    }

    Holder& operator=(const Holder& other)
    {
        cout << "operador '=' normal" << endl;

        if(this == &other)
            return *this;
        delete[] m_data;
        m_data = new int[other.m_size];
        std::copy(other.m_data, other.m_data + other.m_size, m_data);
        m_size = other.m_size;
        return *this;
    }

    Holder& operator=(Holder&& other)     // <-- usando rvalue reference
    {
        cout << "operador rvalue reference '='" << endl;

        if (this == &other)
            return *this;

        delete[] m_data;

        m_data = other.m_data;
        m_size = other.m_size;

        other.m_data = nullptr;
        other.m_size = 0;

        return *this;
    }
};

Holder createHolder(int size)
{
    return Holder(size);
}

template <class T>
intercambio(T& a, T& b) {
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
}

string getString() {
    return "hello world";
}


int main()
{
    cout << "== LVALUE Y RVALUE ==" << endl;
    int x = 666;                  // 666 es rvalue, x es lvalue
    int y = x + 5;

    string s1 = "hello ";
    string s2 = "world";
    string s3 = s1 + s2;      	  // s3 lvalue que recibe un rvalue


    string s4 = getString();   	  // s4 es un lvalue y getString devuelve el rvalue
    cout << x << endl;
    cout << y << endl;
    cout << s4 << endl;

    cout << "\n== REFERENCIA RVALUE ==" << endl;

    string&& s_rref = s1 + s2;    // el resultado s1 + s2 es un rvalue que se asigna a un lvalue(s_rref)
    s_rref += ", my friend";      // aqui se puede cambiar el string temporal
    cout << s_rref << '\n';       // "Hello world, my friend"

    cout << "\n== STD::MOVE() ==" << endl;

    int a = 2, b = 3;

    cout << "a = " << a << ", b = " << b << endl;

    cout << "swap" << endl;
    intercambio<int>(a,b);
    cout << "a = " << a << ", b = " << b << endl;

    cout << "\n== CONSTRUCTOR MOVE Y OPERADOR DE ASIGNACION MOVE==" << endl;

    Holder h1(1000);                // ctor. normal
    Holder h2(h1);                  // copy constructor (lvalue in input)
    Holder h3 = createHolder(2000); // move constructor (rvalue in input)

    h2 = h3;                        // assignment operator (lvalue in input)
    h2 = createHolder(500);         // move assignment operator (rvalue in input)

    return 0;
}
