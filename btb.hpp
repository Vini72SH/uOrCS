#ifndef __BTB__
#define __BTB__

// BTB Defines
#define INPUT 12
#define ASSOCIATIVE_SET 1024
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete;

#include <iostream> 
#include <cstdint>

class btb_input_t{
   public:
    uint64_t instruction_address;
    uint64_t last_access_cycle;
    short typeBranch;
    bool valityBit;

    // =========================================================================
    // Methods.
    // =========================================================================
    btb_input_t();
    ~btb_input_t();

   private:
    DISALLOW_COPY_AND_ASSIGN(btb_input_t)
};

class btb_set_t{
   public:
    btb_input_t **inputs;

    // =========================================================================
    // Methods.
    // =========================================================================
    btb_set_t();
    void allocate_set();
    ~btb_set_t();

   private:
    DISALLOW_COPY_AND_ASSIGN(btb_set_t)
};

class btb_t{
   public:
    btb_set_t **sets;
    long int btbHit;
    long int totalBranch;

    // =========================================================================
    // Methods.
    // =========================================================================
    btb_t();
    void allocate();
    void btb_insert(uint64_t address, short br_type, uint64_t current_cycle);
    int btb_search_update(uint64_t address, uint64_t current_cycle);
    void statistics();
    ~btb_t();

   private:
    DISALLOW_COPY_AND_ASSIGN(btb_t)
};

#endif