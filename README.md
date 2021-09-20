# LUTIL

Template utilities for embedded systems. Built for _extremely_ lightweight applications. Useful for when the STL is too heavy a cost on particular devices. Includes a number of utilities such as:


## Embedded Utils:

### XBee Series 3 Interface
Simple to use interface for API mode XBee Series 3 modules.

```cpp
lutil::XBee3 xbee;
lutil::XBee3Adress addr{ 0x01234567,0x01234567 };

xbee.setStream(&Serial2);

uint8_t msg[] = "Send this!"
xbee.send(msg, sizeof(msg));
```

Full example:
- [XBee3 Sender](./examples/XBee3/XBee3_Sender.ino)
- [XBee3 Receiver](./examples/XBee3/XBee3_Receiver.ino)

### Multi-element Printer (`Printer::print`)
A quick-and-dirty means of printing multiple values without needing so many `Serial.println` commands

```cpp
// Basic
util::Printer::print("Output");

// An otherwise bunch-of-statements.
util::Printer::print("x: % y: % z: %", xval, yval, zval);
```

### Input processor (`Processor`)
An event loop utility class for handling input and custom behaviour. Combined with `Processable` subclasses (e.g. `Button`) this is an interface to avoid polluting your `setup()` and `loop()` statement with tons of logic.

```cpp
namespace util = lutil::v_1;

util::Button<5> my_button; // PIN 5

void pressed_callback() {
    Serial.println("The button was pressed!");
}

void setup() {
    Serial.begin(115200);
    auto &proc = util::Processor::get();
    proc.add_processable(&my_button); // Register button
    proc.init(); // Initialize elements

    // Register a callback when the button get's pressed. The
    // button instance will handle debounce filtering and state
    // management without clogging the loop()
    my_button.when((int)util::ButtonState::Pressed, pressed_callback);
}

void loop() {
    util::Processor::get().process(); // Event-style loop
}
```
You can create your own `Processable` classes with callbacks right from the `lutil` api.


### State Machine (`StateDriver`)
A state machine defintion class. This helps create a clear understanding of a systems state and runtime. This is a more complex topic so refer to the [example](./examples/StateMachine/StateMachine.ino).

This is meant to be combined with the `Processor` for maximum control over a machine.

## Storage

### vector (`Vec`)
A simple vector that will auto size with it's contents.

```cpp
util::Vec<float> vector;
vector.push(12.0);
vector.push(1.0);
vector.push(4.0);

Serial.println(vector[2]); // 4.0

vector.pop(2); // == 4.0
vector.pop(-1); // == 1.0
```

### map (`Map`)
A mapping object for basic key:value handling

```cpp
struct Sensor { /*...*/ };

util::Map<String, Sensor *> id_map;
Sensor gryo;
Sensor accel;
Sensor magno;

// Insert some values
id_map["gryo"] = &gryo;
id_map["accel"] = &accel;
id_map["magno"] = &mango;

Serial.println(id_map["gryo"]->sensor_name); // e.g. LSM9DS1
```

### matrix (`Matrix`)
A matrix utility for NxN sized tables

```cpp
util::Matrix<4, 4> transform;
transform[0][1] = 44.23
```

## Memory:

### Smart Pointer (`managed_ptr`)
A pointer that auto de-references objects and cleans up otherwise dangling pointers

```cpp
{
    util::managed_ptr<int> obj(new int(123));
    Serial.println(obj.get()); // .get() obtains the pointer
}
// The data is now deleted as there are no more references to it
```

### Smart String (`managed_string`)
A `managed_ptr<char>` that handles string comparisons in a common-sense way.

```cpp

util::managed_string foo = "My cool string!"; // Or foo("some string");

if (foo == "My Cool String") {
    // Do the thing
}
```

### Smart Data (`managed_data`)
A `managed_ptr<uint8_t>` for general purpose, self managed data that can be of variable size. This is useful for things that benefit from variadic nature


Examples
--------
There are number of examples to look at for the various micro utils. Each is built for simplicity and quick-setup.
