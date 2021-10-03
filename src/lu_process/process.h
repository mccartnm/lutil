
#pragma once
#include "lutil.h"
#include "lu_storage/vector.h"
#include "lu_storage/map.h"

namespace lutil {

class Processable;

class Processor {
public:
    static Processor &get(); // Global Instance
    Processor();

    void add_processable(Processable *);

    void init();
    void process();

private:
    Vec<Processable *> _processables;
};

typedef void (*ProcessCallback)(void *);

class Processable {
public:

    explicit Processable();
    virtual void init() = 0;
    virtual void process() = 0;

    void when(
        int trigger,
        ProcessCallback callback,
        void *data = nullptr
    );

protected:
    // Called when an event occurs which will in turn fire any
    // registered callbacks
    void event(int trigger);

private:
    struct Callback
    {
        ProcessCallback callback;
        void *data;
    };
    Map<int, Vec<Callback>> _callbacks;
};

}
