#ifndef TRACE_READER_HPP
#define TRACE_READER_HPP

#include <zlib.h>  // gzFile

#include <cstdint>  // uint32_t

#include "opcode_package.hpp"  // opcode_package_t

// =============================================================================
class trace_reader_t {
   private:
    gzFile gzStaticTraceFile;
    gzFile gzDynamicTraceFile;
    gzFile gzMemoryTraceFile;

    /// Controls the trace reading.
    bool is_inside_bbl;
    uint32_t currect_bbl;
    uint32_t currect_opcode;

    /// Controls the static dictionary.
    /// Total of BBLs for the static file.
    /// Total of instructions for each BBL.
    uint32_t binary_total_bbls;
    uint32_t *binary_bbl_size;
    /// Complete dictionary of BBLs and instructions.
    opcode_package_t **binary_dict;

    uint64_t fetch_instructions;

   public:
    // =========================================================================
    /// Methods.
    // =========================================================================
    trace_reader_t();
    ~trace_reader_t();
    void allocate(char *trace_file_name);
    void statistics();

    /// Generate the static dictionary.
    void get_total_bbls();
    void define_binary_bbl_size();
    void generate_binary_dict();

    bool trace_string_to_opcode(char *input_string, opcode_package_t *opcode);
    bool trace_next_dynamic(uint32_t *next_bbl);
    bool trace_next_memory(uint64_t *next_address, uint32_t *operation_size,
                           bool *is_read);
    bool trace_fetch(opcode_package_t *m);
};

#endif  // TRACE_READER_HPP
