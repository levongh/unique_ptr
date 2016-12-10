#pragma once

struct Foo {
    Foo() { std::cout << "Foo...\n"; }
    ~Foo() { std::cout << "~Foo...\n\n"; }
};

struct D{

    D(){};

    D(const D& other){
        std::cout << "call D const copy constructor... \n";
    }

    D(D& other){
        std::cout << "call D copy constructor... \n";
    }

    D(D&& other){
        std::cout << "call D move constructor... \n";
    }

    void operator () (Foo* p) const {
        std::cout << "Call delete for Foo object...\n";
        delete p;
    };
};

void test_constructors()
{
    //constructor (1)
    std::cout << "Example constructor(1)...\n\n";
    eda::unique_ptr<Foo> up;

    //constructor (2)
    std::cout << "Example constructor(2)...\n";
    Foo* f = new Foo();
    eda::unique_ptr<Foo> up2(f); //up2 now owns f
    up2.reset();

    //constructor (3&4)
    std::cout << "Example constructor(3&4)...\n";

    //D is not an lvalue-reference - d is a non-const rvalue
    eda::unique_ptr<Foo, D> up3(new Foo(), D()); //D must be MoveConstructible
    up3.reset();

    //D is not an lvalue-refernce - d is a left value
    D d2;
    eda::unique_ptr<Foo, D> up4(new Foo(), d2); //D must be Copyconstructible
    up4.reset();

    //D is a left value reference type
    D d3;
    eda::unique_ptr<Foo, D&> up5(new Foo(), d3); //up3 holds a reference to d3
    up5.reset();

    //D is a const left value reference type
    const D d4;
    eda::unique_ptr<Foo, const D&> up6(new Foo(), d4);
    up6.reset();

    //constructor (5)
    std::cout << "Example constructor(5)...\n";
    eda::unique_ptr<Foo> up7(new Foo());
    eda::unique_ptr<Foo> up8(std::move(up7)); //ownership is transfered to up8
    up8.reset();

    //constructor 6
    std::cout << "Example constructor(6)...\n\n";
    eda::unique_ptr<Foo> up9(nullptr);

    //constructor 7 - D is move constructed
    D d;
    std::cout << "Example constructor(7)...\n";
    eda::unique_ptr<Foo, D> up10(new Foo(), d);     //D is not a reference
    eda::unique_ptr<Foo, D> up11(std::move(up10));       //D is move constructed
    up11.reset();

    //constructor 7 - D is copy constructed
    eda::unique_ptr<Foo, D&> up12(new Foo(), d);    //D is a reference
    eda::unique_ptr<Foo, D> up13(std::move(up12));       //D is copy constructed
    up13.reset();

}
