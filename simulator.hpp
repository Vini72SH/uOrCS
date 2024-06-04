#ifndef _ORCS_ORCS_HPP_
#define _ORCS_ORCS_HPP_

// =============================================================================
/// This is the main header file, it contains all the prototypes and
/// describes the relations between classes.
// =============================================================================

/// C Includes
#include <getopt.h>    // getopt_long; POSIX standard getopt is in unistd.h
#include <inttypes.h>  // uint32_t
#include <unistd.h>    // getopt
#include <zlib.h>

/// C++ Includes
#include <cstdlib>  // EXIT_FAILURE

#include "orcs_engine.hpp"

// =============================================================================
/// Global SINUCA_ENGINE instantiation.
extern orcs_engine_t orcs_engine;

// =============================================================================
/// Definitions for Log, Debug, Warning, Error and Statistics.
#define FAIL 0  /// FAIL when return is int32_t or uint32_t.
#define OK 1    /// OK when return is int32_t or uint32_t.

#define TRACE_LINE_SIZE 512

/// DETAIL DESCRIPTION: Almost all errors and messages use this definition.
/// It will DEACTIVATE all the other messages below.
#define ORCS_PRINTF(...) printf(__VA_ARGS__);

#ifdef ORCS_DEBUG
#define DEBUG_PRINTF(...)         \
    {                             \
        ORCS_PRINTF("DEBUG: ");   \
        ORCS_PRINTF(__VA_ARGS__); \
    }
#else
#define DEBUG_PRINTF(...)
#endif

#define ERROR_INFORMATION()                                               \
    {                                                                     \
        ORCS_PRINTF("ERROR INFORMATION\n");                               \
        ORCS_PRINTF("ERROR: File: %s at Line: %u\n", __FILE__, __LINE__); \
        ORCS_PRINTF("ERROR: Function: %s\n", __PRETTY_FUNCTION__);        \
        ORCS_PRINTF("ERROR: Cycle: %" PRIu64 "\n",                        \
                    orcs_engine.get_global_cycle());                      \
    }

#define ERROR_ASSERT_PRINTF(v, ...)            \
    if (!(v)) {                                \
        ERROR_INFORMATION();                   \
        ORCS_PRINTF("ERROR_ASSERT: %s\n", #v); \
        ORCS_PRINTF("\nERROR: ");              \
        ORCS_PRINTF(__VA_ARGS__);              \
        exit(EXIT_FAILURE);                    \
    }

#define ERROR_PRINTF(...)         \
    {                             \
        ERROR_INFORMATION();      \
        ORCS_PRINTF("\nERROR: "); \
        ORCS_PRINTF(__VA_ARGS__); \
        exit(EXIT_FAILURE);       \
    }

// =============================================================================
/// Enumerations.
// =============================================================================

// =============================================================================
/// Enumerates the INSTRUCTION (Opcode and Uop) operation type.
enum instruction_operation_t {
    /// NOP
    INSTRUCTION_OPERATION_NOP,
    /// INTEGERS
    INSTRUCTION_OPERATION_INT_ALU,
    INSTRUCTION_OPERATION_INT_MUL,
    INSTRUCTION_OPERATION_INT_DIV,
    /// FLOAT POINT
    INSTRUCTION_OPERATION_FP_ALU,
    INSTRUCTION_OPERATION_FP_MUL,
    INSTRUCTION_OPERATION_FP_DIV,
    /// BRANCHES
    INSTRUCTION_OPERATION_BRANCH,
    /// MEMORY OPERATIONS
    INSTRUCTION_OPERATION_MEM_LOAD,
    INSTRUCTION_OPERATION_MEM_STORE,
    /// NOT IDENTIFIED
    INSTRUCTION_OPERATION_OTHER,
    /// SYNCHRONIZATION
    INSTRUCTION_OPERATION_BARRIER,
    /// HMC
    INSTRUCTION_OPERATION_HMC_ROA,  // #12 READ+OP +Answer
    INSTRUCTION_OPERATION_HMC_ROWA  // #13 READ+OP+WRITE +Answer
};

// =============================================================================
/// Enumerates the types of branches.
enum branch_t {
    BRANCH_SYSCALL,
    BRANCH_CALL,
    BRANCH_RETURN,
    BRANCH_UNCOND,
    BRANCH_COND
};

#endif  // _ORCS_ORCS_HPP_
