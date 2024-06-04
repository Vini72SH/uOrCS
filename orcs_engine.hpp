#ifndef ORCS_ENGINE_HPP_
#define ORCS_ENGINE_HPP_

#include <cstdint>  // uint64_t

class processor_t;
class trace_reader_t;

// =============================================================================
class orcs_engine_t {
   private:
   public:
    char *arg_trace_file_name;
    trace_reader_t *trace_reader;
    processor_t *processor;
    bool simulator_alive;
    uint64_t global_cycle;

    // =========================================================================
    // Methods.
    // =========================================================================
    orcs_engine_t();
    void allocate();
    uint64_t get_global_cycle();
};

#endif  // ORCS_ENGINE_HPP_
