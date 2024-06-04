#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "btb.hpp"             // btb_t

class processor_t {
   public:
    btb_t *btb;
    uint64_t global_cycle;

    // =========================================================================
    // Methods.
    // =========================================================================
    processor_t();
    void allocate();
    void clock();
    void statistics();
    ~processor_t();
};

#endif  // PROCESSOR_HPP
