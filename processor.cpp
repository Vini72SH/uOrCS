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
    this->predictors = new predictors_t;
    this->btb->allocate();
    this->predictors->allocate();
};

// =============================================================================
void processor_t::clock() {
    // Get the next instruction from the trace.
    opcode_package_t new_instruction;
    if (!orcs_engine.trace_reader->trace_fetch(&new_instruction)) {
        // If EOF.
        orcs_engine.simulator_alive = false;
    }

    // Checks if the data is up to date
    uint64_t address = new_instruction.opcode_address;
    bool update = predictors->updatePredictors(address);
    if (update) {
        // BTB-Hit | Conditional and Unconditional Cases
        if (btb->getPrevious() == HIT) {
            // If UNCOND, 0 latency
            if (btb->getTypeBranch() != BRANCH_COND) {
                this->global_cycle++;
            } else {
                // If COND
                    // HIT - 0 latency
                    // MISS - 512 latency
                    if (predictors->getHit())
                        this->global_cycle++;
                    else
                        this->global_cycle += 512;
            }
        } else {
            // BTB-Miss | Conditional and Unconditional Cases
            if (btb->getTypeBranch() != BRANCH_COND) {
                this->global_cycle += 12;
            } else {
                // If Taken, 512 latency
                // If Not Taken, 0 latency
                if (predictors->getBranchResult() == TAKEN)
                    this->global_cycle += 512;
                else 
                    this->global_cycle++;
            }
        }
        
    } else {
        global_cycle++;
    }

    if (new_instruction.opcode_operation == INSTRUCTION_OPERATION_BRANCH) {
        address = new_instruction.opcode_address;
        uint64_t opcode_size = new_instruction.opcode_size;
        uint8_t branch = new_instruction.branch_type;

        predictors->predictBranch(address, opcode_size, branch);
        if (btb->btb_search_update(address, global_cycle)) {
            btb->setPrevious(HIT);
        } else {
            btb->btb_insert(address, branch, global_cycle);
            btb->setPrevious(MISS);
        }
    }
};

// =============================================================================
void processor_t::statistics() {;
    ORCS_PRINTF("######################################################\n");
    ORCS_PRINTF("processor_t\n");
    btb->statistics();
    predictors->statistics();
    ORCS_PRINTF("Simulation Time: %ld\n", global_cycle);
};

// =============================================================================
processor_t::~processor_t(){
    this->global_cycle = 0;
    delete this->btb;
    delete this->predictors;
    this->btb = nullptr;
    this->predictors = nullptr;
};