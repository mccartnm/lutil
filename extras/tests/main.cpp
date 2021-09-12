
#include <iostream>

#include "lutil.h"
#include "lu_memory/managed_ptr.h"
#include "lu_storage/vector.h"
#include "lu_memory/abstract_memory.h"

#include <vector>

class Foo
{
public:
    char x;
};


class Bar : public Foo
{
};

using Vector = lutil::Vec<lutil::managed_ptr<Foo>>;

class my_mem : public lutil::uint16_memory
{
public:
    using lutil::uint16_memory::uint16_memory;

protected:
    virtual void _write(uint16_t address,
                        uint8_t *data,
                        uint16_t size) override {}


    /*
    Execute a read operation with the given address, data,
    and size.
    */
    virtual void _read(uint16_t address,
                       uint8_t *into,
                       uint16_t size) override {}

};

Vector make_it(Vector vv)
{
    lutil::managed_ptr<Foo> f(new Bar());
    f->x = 'a';
    vv.push(f);
    return vv;
}

void print_it(Vector vv)
{
    std::cout << vv[0]->x << std::endl;
}

int main(int argc, char const *argv[])
{
    Vector v;

    v = make_it(v);
    print_it(v);

    my_mem *mem = new my_mem(0x10);
    Foo foo;
    foo.x = '%';
    mem->write(0, foo);

    // To keep the console
    throw std::runtime_error("foo");

    return 0;
}