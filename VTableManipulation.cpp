#include <iostream>
#include <cstring>

class Base
{
public:
    int a = 5;
    virtual void func1() { std::cout << "Base::func1" << std::endl; }
    virtual void func2() { std::cout << "Base::func2" << std::endl; }
    void func3() { std::cout << "Base::func3" << std::endl; }
    virtual ~Base() { std::cout << "Base::~Base: " << a << std::endl; }
};

class Derived : public Base
{
public:
    void func1()  { std::cout << "Derived::func1" << std::endl; }
    void func2()  { std::cout << "Derived::func2" << std::endl; }
    void func3() { std::cout << "Derived::func3" << std::endl; }
    ~Derived() { std::cout << "Derived::~Derived: " << a << std::endl; }
};

void manipulatedFunc1()
{
    std::cout << "manipulatedFunc1" << std::endl;
}

void Manipulate(void* b)
{
    void** originalVTable = *(void***)b;
    void*** originalValue = (((void***)b) + 1);

    void** newVTable = new void*[4];
    std::memcpy(newVTable, originalVTable, sizeof(void*) * 4);

    void (*funcPtr)() = (void (*)())newVTable[1];
    void (*funcPtr2)(Derived *) = (void (*)(Derived *))newVTable[2];

    newVTable[1] = (void*)&manipulatedFunc1;

    *originalValue = (void**)0x12;
    funcPtr2((Derived*)b);
    funcPtr();

    *(void***)b = newVTable;
}

int main()
{
    Base* b = new Derived();
    Manipulate(b);
    b->func1();
    b->func2();
    b->func3();

    std::cout << b->a << std::endl;

    delete b;

    return 0;
}
