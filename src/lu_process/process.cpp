#include "process.h"

namespace lutil {

Processor &Processor::get() {
    static Processor *instance = nullptr;
    if (!instance) {
        instance = new Processor();
    }
    return *instance;
}

Processor::Processor()
{
}

void Processor::add_processable(Processable *proc)
{
    _processables.push(proc);
}

void Processor::init()
{
    for (size_t i = 0; i < _processables.count(); i++) {
        _processables[i]->init();
    }
}

void Processor::process()
{
    for (size_t i = 0; i < _processables.count(); i++) {
        _processables[i]->process();
    }
}

Processable::Processable()
{
    Processor::get().add_processable(this);
}

void Processable::when(int trigger, ProcessCallback callback, void *data)
{
    if (!_callbacks.contains(trigger)) {
        _callbacks.insert(trigger, Vec<Callback>());
    }
    _callbacks[trigger].push({callback, data});
}

void Processable::event(int trigger) {
    if (_callbacks.contains(trigger)) {
        auto &cb_vec = _callbacks[trigger];
        for (size_t i = 0; i < cb_vec.count(); i++) {
            cb_vec[i].callback(cb_vec[i].data);
        }
    }
}

}
