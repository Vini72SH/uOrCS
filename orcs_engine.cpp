#include "orcs_engine.hpp"

#include "processor.hpp"     // processor_t
#include "trace_reader.hpp"  // trace_reader_t

// =============================================================================
orcs_engine_t::orcs_engine_t(){};

// =============================================================================
void orcs_engine_t::allocate() {
    this->trace_reader = new trace_reader_t;
    this->processor = new processor_t;
};

uint64_t orcs_engine_t::get_global_cycle() { return this->global_cycle; };
