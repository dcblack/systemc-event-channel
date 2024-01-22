#include "top.hpp"
#include "event.hpp"
#include "sc_time_literals.hpp"
#include <systemc>
#include <string>
using namespace std::literals;
using namespace sc_core::literals;
using namespace sc_core; // convenience in cpp

#define FSTR ( std::string{name()} + ":"s + std::string{__func__} )
#define TSTR ( sc_core::sc_time_stamp().to_string() )

SC_MODULE( Producer_module ) {
  static constexpr const char* msgType{"Producer_module"};
  sc_port<Event_notify_if> event_port{"event_port"};
  void producer_thread() {
    for(auto n=5; n--; ) {
      wait(2_ns);
      event_port->notify(SC_ZERO_TIME);
      event_port->notify(1_ns);
      SC_REPORT_INFO_VERB( msgType
                         , (FSTR +" sent event at "s + TSTR).c_str()
                         , SC_NONE
                         );
    }
    wait(100_ns);
    SC_REPORT_INFO_VERB( msgType
                       , (FSTR +" stopping at "s + TSTR).c_str()
                       , SC_NONE
                       );
    sc_stop();
  }
  explicit SC_CTOR( Producer_module ) {
    SC_THREAD(producer_thread);
  }
};

SC_MODULE( Consumer_module ) {
  static constexpr const char* msgType{"Consumer_module"};
  sc_port<Event_view_if> incoming_port;

  [[noreturn]] void consumer_thread() {
    for(;;) {
      wait(incoming_port->event());//< Dynamic
      SC_REPORT_INFO_VERB( msgType
                         , (FSTR +" received event at "s + TSTR).c_str()
                         , SC_NONE
                         );
    }
  }
  void consumer_method() {
    SC_REPORT_INFO_VERB( msgType
                       , (FSTR +" received event at "s + TSTR).c_str()
                       , SC_NONE
                       );
  }
  explicit SC_CTOR( Consumer_module ) {
    SC_THREAD(consumer_thread);
    SC_METHOD(consumer_method);
    sensitive << incoming_port;
  }
};

Top_module::Top_module(const sc_module_name& instance)
  : sc_module(instance)
  , producer{new Producer_module{"producer"}}
  , consumer1{new Consumer_module{"consumer1"}}
  , consumer2{new Consumer_module{"consumer2"}}
  , event_chan{new Event_channel{"event_chan"}}
{
  // Connect
  producer->event_port.bind(*event_chan);
  consumer1->incoming_port.bind(*event_chan);
  consumer2->incoming_port.bind(*event_chan);
}

Top_module::~Top_module() {
  if( producer   != nullptr ) { delete producer;   producer   = nullptr; }
  if( consumer1  != nullptr ) { delete consumer1;  consumer1  = nullptr; }
  if( consumer2  != nullptr ) { delete consumer2;  consumer2  = nullptr; }
  if( event_chan != nullptr ) { delete event_chan; event_chan = nullptr; }
}
