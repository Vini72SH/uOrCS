#ifndef __BRANCH__
#define __BRANCH__

#define BITS 25
#define TAKEN 1
#define NTAKEN 0
#define SYSCALL 0
#define CALL 1
#define RETURN 2
#define UNCOND 3
#define COND 4
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete;

#include <iostream>
#include <cstdint>

class predictorBimodal_t{
   public:
    long int maxBranch;
    long int truePredicts;
    short *counterTable;

    // =========================================================================
    // Methods.
    // =========================================================================
    predictorBimodal_t();
    void allocate();
    uint64_t getIndex(uint64_t address);
    short predictBimodal(uint64_t address);
    void updateBimodal(uint64_t address, bool branch_result);
    ~predictorBimodal_t();
   private:
    DISALLOW_COPY_AND_ASSIGN(predictorBimodal_t)
};

class predictorGshare_t{
   public:
    long int maxBranch;
    long int truePredicts;
    bool ghr[BITS];
    short *counterTable;

    // =========================================================================
    // Methods.
    // =========================================================================
    predictorGshare_t();
    void allocate();
    uint64_t getIndex(uint64_t address);
    bool predictGshare(uint64_t address);
    void updateGshare(uint64_t address, bool branch_result);
    ~predictorGshare_t();

   private:
    DISALLOW_COPY_AND_ASSIGN(predictorGshare_t)
};

class predictors_t{
   public:
    short *select;
    predictorBimodal_t *bimodal;
    predictorGshare_t *gshare;
    long int maxBranch;
    long int truePredicts;
    uint64_t currentAddress;
    uint64_t addressSize;
    uint8_t currentBranch;
    bool gsharePredict;
    bool bimodalPredict;
    bool hitPredict;
    bool branchResult;
    bool valid;

    // =========================================================================
    // Methods.
    // =========================================================================
    predictors_t();
    void allocate();
    uint64_t getIndex(uint64_t address);
    bool predictBranch(uint64_t address, uint64_t add_size, uint8_t branch);
    bool updatePredictors(uint64_t next_address);
    bool getHit();
    bool getBranchResult();
    void statistics();
    ~predictors_t();

   private:
    DISALLOW_COPY_AND_ASSIGN(predictors_t)
};
 
#endif
