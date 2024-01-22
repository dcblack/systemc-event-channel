#include "top.hpp"
#include "event.hpp"
#include "sc_time_literals.hpp"
#include <systemc>
#include <string>
using namespace std::literals;
using namespace sc_core::literals;
using namespace sc_core; // convenience in cpp

#define HORIZONTAL_LINE SC_REPORT_INFO( msgType, ( std::string(80,'-')).c_str() );
#define FSTR ( std::string{name()} + ":"s + std::string{__func__} )
#define TSTR ( sc_core::sc_time_stamp().to_string() )

// Backwards compatibility to SystemC 2.3
#ifndef SC_NAMED
#define SC_NAMED(name) name(#name)
#endif

//------------------------------------------------------------------------------
SC_MODULE( Producer_module ) {

  static constexpr const char* msgType{"Producer_module"};
  sc_port<Event_notify_if> SC_NAMED(event_port);
  sc_export<Event_observe_if> SC_NAMED(event_export);
  Event_channel SC_NAMED(local_event);

  explicit SC_CTOR( Producer_module ) {
    SC_METHOD(producer_method);
    sensitive << local_event();
    dont_initialize();
    SC_THREAD(producer_thread);
    event_export.bind( local_event );
  }

  void producer_thread() {
    //--------------------------------------------------------------------------
    // Test local usage exported
    for(auto repetitions=3; repetitions--; ) {
      HORIZONTAL_LINE
      local_event.notify(SC_ZERO_TIME);
      SC_REPORT_INFO( msgType, (FSTR +" sent local event at "s + TSTR).c_str() );
      wait( 1_ns );
      local_event.notify(1_ns);
      SC_REPORT_INFO( msgType, (FSTR +" sent local delayed event at "s + TSTR).c_str() );
    }
    wait( SC_ZERO_TIME );
    HORIZONTAL_LINE
    local_event.notify();
    SC_REPORT_INFO( msgType, (FSTR +" sent local immediate event at "s + TSTR).c_str() );
    wait( 100_ns );
    //--------------------------------------------------------------------------
    // Test port usage
    for(auto repetitions=3; repetitions--; ) {
      HORIZONTAL_LINE
      event_port->notify(SC_ZERO_TIME);
      SC_REPORT_INFO( msgType, (FSTR +" sent external event at "s + TSTR).c_str() );
      wait( 1_ns );
      event_port->notify(1_ns);
      SC_REPORT_INFO( msgType, (FSTR +" sent external delayed event at "s + TSTR).c_str() );
    }
    wait( SC_ZERO_TIME );
    HORIZONTAL_LINE
    event_port->notify();
    SC_REPORT_INFO( msgType, (FSTR +" sent external immediate event at "s + TSTR).c_str() );
    wait( 100_ns );
    //--------------------------------------------------------------------------
    HORIZONTAL_LINE
    SC_REPORT_INFO( msgType, (FSTR +" stopping at "s + TSTR).c_str() );
    sc_stop();
  }
  void producer_method() {
    SC_REPORT_INFO( msgType, (FSTR +" received local event at "s + TSTR).c_str() );
  }
};

//------------------------------------------------------------------------------
SC_MODULE( Consumer_module ) {
  static constexpr const char* msgType{"Consumer_module"};
  sc_port<Event_observe_if> SC_NAMED(external_port);
  sc_port<Event_observe_if> SC_NAMED(exported_port);

  [[noreturn]] void external_thread() {
    for(;;) {
      wait(external_port->event());//< Dynamic
      SC_REPORT_INFO( msgType, (FSTR +" received event at "s + TSTR).c_str() );
    }
  }
  [[noreturn]] void exported_thread() {
    for(;;) {
      wait(exported_port->event());//< Dynamic
      SC_REPORT_INFO( msgType, (FSTR +" received event at "s + TSTR).c_str() );
    }
  }
  void external_method() {
    SC_REPORT_INFO( msgType, (FSTR +" received external event at "s + TSTR).c_str() );
  }
  void exported_method() {
    SC_REPORT_INFO( msgType, (FSTR +" received exported event at "s + TSTR).c_str() );
  }
  explicit SC_CTOR( Consumer_module ) {
    SC_THREAD(external_thread);
    SC_THREAD(exported_thread);
    SC_METHOD(external_method);
    sensitive << external_port;
    dont_initialize();
    SC_METHOD(exported_method);
    sensitive << exported_port;
    dont_initialize();
  }
};

//------------------------------------------------------------------------------
Top_module::Top_module(const sc_module_name& instance)
  : sc_module(instance)
  , producer{new Producer_module{"producer"}}
  , consumer1{new Consumer_module{"consumer1"}}
  , consumer2{new Consumer_module{"consumer2"}}
  , event_chan{new Event_channel{"event_chan"}}
{
  // Connect
  producer->event_port.bind(*event_chan);
  consumer1->external_port.bind(*event_chan);
  consumer2->external_port.bind(*event_chan);
  consumer1->exported_port.bind(producer->event_export);
  consumer2->exported_port.bind(producer->event_export);
}

Top_module::~Top_module() {
  if( producer   != nullptr ) { delete producer;   producer   = nullptr; }
  if( consumer1  != nullptr ) { delete consumer1;  consumer1  = nullptr; }
  if( consumer2  != nullptr ) { delete consumer2;  consumer2  = nullptr; }
  if( event_chan != nullptr ) { delete event_chan; event_chan = nullptr; }
}
