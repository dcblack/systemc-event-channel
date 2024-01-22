#include "top.hpp"
#include <systemc>
using namespace sc_core;

int sc_main( [[maybe_unused]]int argc, [[maybe_unused]]const char* argv )
{
  static constexpr const char* msgType{"/Doulos/event_channel/main"};
  Top_module top{"top"};
  sc_core::sc_start();
  if ( not sc_end_of_simulation_invoked()) {
      SC_REPORT_ERROR( msgType, "No explicit sc_stop" );
      sc_stop(); // invokes end_of_simulation
  }

  //----------------------------------------------------------------------------
  // Summarize simulation results
  auto errorCount = sc_report_handler::get_count( SC_ERROR );
  auto fatalCount = sc_report_handler::get_count( SC_FATAL );
  auto success = 0 == (errorCount + fatalCount);
  auto text = success ? "Simulation PASSED" : "Simulation FAILED";
  SC_REPORT_INFO_VERB( msgType, text, SC_NONE );
  return success?0:1;
}
