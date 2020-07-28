LUTIL
=====

Tempalte utilities for embedded systems. Built for _extremely_ lightweight applications. USeful for when the STL is too heavy a cost on particular devices. Includes a number of utilities such as:

Storage:
- vector (`Vec`)
- map (`Map`)
- matrix (`Matrix`)

Memory:
- Smart Pointer (`managed_ptr`)
- Smart String (`managed_string`)
- Smart Data (`managed_data`)

Embedded Utils:
- Multi-element Printer (`Printer::print`)
- Input processor (`Processor`) and processable elements (e.g. `Button`) with event callbacks

