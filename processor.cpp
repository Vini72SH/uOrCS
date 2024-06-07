#include "processor.hpp"  // allocate, clock, processor_t, statistics

// If you use clangd, it'll give you a warning on this include, but it's used by
// ORCS_PRINTF. Try removing it to see what happens.
// ~ Gabriel
#include <cstdio>

#include "opcode_package.hpp"  // opcode_package_t
#include "orcs_engine.hpp"     // orcs_engine_t
#include "simulator.hpp"       // ORCS_PRINTF
#include "trace_reader.hpp"

// =============================================================================
processor_t::processor_t(){};

// =============================================================================
void processor_t::allocate() {
    this->global_cycle = 0;
    this->btb = new btb_t;
    this->btb->allocate();
};

// =============================================================================
void processor_t::clock() {
    // Get the next instruction from the trace.
    opcode_package_t new_instruction;
    if (!orcs_engine.trace_reader->trace_fetch(&new_instruction)) {
        // If EOF.
        orcs_engine.simulator_alive = false;
    }

    if (new_instruction.opcode_operation == INSTRUCTION_OPERATION_BRANCH) {
        if (btb->btb_search_update(new_instruction.opcode_address, global_cycle)) {
            this->global_cycle++;
        } else {
            btb->btb_insert(new_instruction.opcode_address,
                            new_instruction.branch_type,
                            global_cycle);
            this->global_cycle += 12;
        }
    } else {
        this->global_cycle++;
    }
};

// =============================================================================
void processor_t::statistics() {
    double accuracy = ((double)btb->btb_hit / (double)btb->total_branch) * 100;
    ORCS_PRINTF("######################################################\n");
    ORCS_PRINTF("processor_t\n");
    ORCS_PRINTF("Total Cyles: %ld\n", global_cycle);
    ORCS_PRINTF("Total Branch's: %ld\n", btb->total_branch);
    ORCS_PRINTF("BTB-Hit: %ld\n", btb->btb_hit);
    ORCS_PRINTF("BTB Accuracy: %f\n", accuracy);
    btb->imprime();
};

// =============================================================================
processor_t::~processor_t(){
    this->global_cycle = 0;
    delete this->btb;
    this->btb = nullptr;
};