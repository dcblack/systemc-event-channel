# About this project

Demonstrates a simple SystemC channel to send and receive events.

## API

```c++
Event_channel(const std::string& instance)
void notify()
void notify(const sc_core::sc_time& delay)
const sc_core::sc_event& event() const
const sc_core::sc_event& default_event() const 
const sc_core::sc_event& operator()() const 
```

## Example

See file `top.hpp` and `top.cpp` for example code using this channel.

## Future

The following ideas will require a header-only definition

- Add option to limit the number of readers/writers
- Add option to use `sc_event_quue` instead of `sc_event`
- Add a `triggered()` method.
- Add debug interface to count events

### The end

