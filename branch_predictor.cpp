#include "branch_predictor.hpp"

// Bimodal predictor Methods
// =============================================================================
predictorBimodal_t::predictorBimodal_t() {
    counterTable = nullptr;
};

// =============================================================================
void predictorBimodal_t::allocate() {
    maxBranch = 0;
    truePredicts = 0;
    int size = (1 << BITS);
    counterTable = new short[size];

    if (counterTable == nullptr) 
        return;

    // Sets 50% as likely Taken
    // and 50% as likely Not Taken
    for (int i = 0; i < size; ++i)
        counterTable[i] = 1 + (i % 2);
};

// =============================================================================
uint64_t predictorBimodal_t::getIndex(uint64_t address) {
    // Get the least significant m BITS
    uint32_t index = (1 << BITS) - 1;
    index = index & address;

    return index;
};

// =============================================================================
short predictorBimodal_t::predictBimodal(uint64_t address) {
    uint64_t index = getIndex(address);
    maxBranch++;

    return counterTable[index] >> 1;
};

// =============================================================================
void predictorBimodal_t::updateBimodal(uint64_t address, bool branch_result) {
    short predictBranch;
    uint64_t index = getIndex(address);

    if (counterTable == nullptr) 
        return;

    // If the prediction was correct, increase
    // If the prediction is wrong, it decreases
    predictBranch = counterTable[index] >> 1;
    if (branch_result) {
        if (counterTable[index] < 3) 
            counterTable[index]++;
    } else { 
        if (counterTable[index] > 0) 
            counterTable[index]--;
    }

    // Increases total hits
    truePredicts += (predictBranch == branch_result);
};

// =============================================================================
predictorBimodal_t::~predictorBimodal_t() {
    maxBranch = 0;
    truePredicts = 0;
    delete[] counterTable;
    counterTable = nullptr;
};
// =============================================================================

// Gshare predictor Methods
// =============================================================================
predictorGshare_t::predictorGshare_t() {
    counterTable = nullptr;
};

// =============================================================================
void predictorGshare_t::allocate() {
    maxBranch = 0;
    truePredicts = 0;
    int size = (1 << BITS);
    counterTable = new short[size];

    if (counterTable == nullptr) 
        return;

    // Sets 50% as likely Taken
    // and 50% as likely Not Taken
    for (int i = 0; i < size; ++i)
        counterTable[i] = 1 + (i % 2);
    
    // Sets the GHR to zero
    for (int i = 0; i < BITS; ++i)
        ghr[i] = false;
};

// =============================================================================
uint64_t predictorGshare_t::getIndex(uint64_t address) {
    int tam = BITS;
    uint64_t index = 0;

    // Get the decimal value of the GHR
    for (int i = tam - 1; i >= 0; --i) {
        if (ghr[i]) {
            index = index + (1 << (tam - (i + 1)));
        }
    }

    // Sets the shift size to the right
    int shift = 0;
    uint64_t div = address;
    while (div > 0) {
        div = div / 2;
        shift++;
    }

    // Make an XOR with the GHR values ​​
    // and the most significant m BITS
    if (shift <= BITS) {
        index = (index ^ (address & ((1 << BITS) - 1)));
    } else {
        shift = shift - BITS;
        index = (index ^ (address >> shift));
    }

    return index;
};

// =============================================================================
bool predictorGshare_t::predictGshare(uint64_t address) {
    uint64_t index = getIndex(address);
    maxBranch++;

    return counterTable[index] >> 1;
};

// =============================================================================
void predictorGshare_t::updateGshare(uint64_t address, bool branch_result) {
    short predictBranch;
    uint64_t index = getIndex(address);

    if (counterTable == nullptr) 
        return;

    // If the prediction was correct, increase
    // If the prediction is wrong, it decreases
    predictBranch = counterTable[index] >> 1;
    if (branch_result) {
        if (counterTable[index] < 3) 
            counterTable[index]++;
    } else { 
        if (counterTable[index] > 0) 
            counterTable[index]--;
    }

    // Shifts GHR bits to the left
    for (int i = 0; i < BITS - 1 ; ++i) {
        ghr[i] = ghr[i + 1];
    }

    // Register the most recent result in GHR
    ghr[BITS - 1] = branch_result;
    truePredicts += (predictBranch == branch_result);

};

// =============================================================================
predictorGshare_t::~predictorGshare_t() {
    maxBranch = 0;
    truePredicts = 0;
    delete[] counterTable;
    counterTable = nullptr;
};

// =============================================================================

// Combining predictors Methods
// =============================================================================
predictors_t::predictors_t() {
    bimodal = nullptr;
    gshare = nullptr;
    select = nullptr;
};

// =============================================================================
void predictors_t::allocate() {
    int size = (1 << BITS);
    valid = false;
    maxBranch = 0;
    truePredicts = 0;
    addressSize = 0;
    hitPredict = false;
    gsharePredict = false;
    bimodalPredict = false;
    currentAddress = 0;
    bimodal = new predictorBimodal_t;
    gshare = new predictorGshare_t;
    select = new short[size];
    bimodal->allocate();
    gshare->allocate();

    // Sets 50% as likely Bimodal
    // and 50% as likely Gshare
    for (int i = 0; i < size; ++i) 
        select[i] = 1 + (i % 2);
};

// =============================================================================
uint64_t predictors_t::getIndex(uint64_t address) {
    uint64_t index = ((1 << BITS) - 1);
    index = index & address;

    return index;
};

// =============================================================================
bool predictors_t::predictBranch(uint64_t address, uint64_t add_size) {
    if ((bimodal == nullptr) || (gshare == nullptr) || (select == nullptr))
        return false;

    uint64_t index = getIndex(address);
    bool bPredict = bimodal->predictBimodal(address);
    bool gsPredict = gshare->predictGshare(address);
    
    // Stores current branch instruction data
    // To be used in the next cycle
    currentAddress = address;
    addressSize = add_size;
    gsharePredict = gsPredict;
    bimodalPredict = bPredict;
    valid = true;
    maxBranch++;

    // Returns the most likely prediction
    if (select[index] >> 1) {
        return gsPredict;
    } else {
        return bPredict;
    }
};

// =============================================================================
bool predictors_t::updatePredictors(uint64_t next_address) {
    if ((bimodal == nullptr) || (gshare == nullptr) || (select == nullptr))
        return false;
    
    if (valid) {
        // Acquire results and update predictors individually
        uint64_t index = getIndex(currentAddress);
        branchResult = (currentAddress + addressSize != next_address);
        bimodal->updateBimodal(currentAddress, branchResult);
        gshare->updateGshare(currentAddress, branchResult);

        // Find out which predictor was used
        bool predict;
        if (select[index] >> 1) {
            predict = gsharePredict;
        } else {
            predict = bimodalPredict;
        }

        // Updates the selector based on hits 
        // and misses from the previous prediction
        bool hit = ((predict) == (branchResult));
        if (bimodalPredict != gsharePredict) {
            if (bimodalPredict == branchResult) {
                if (select[index] > 0) select[index]--;
            } else {
                if (select[index] < 3) select[index]++;
            }
        }

        // Updates the global hit counter
        if (hit) truePredicts++;
        hitPredict = hit;
        valid = false;

        return true;
    }
    return false;
};

// =============================================================================
bool predictors_t::getHit() {
    return hitPredict;
};

// =============================================================================
bool predictors_t::getBranchResult() {
    return branchResult;
};

// =============================================================================
void predictors_t::statistics() {
    double bimodalPercent = ((double)bimodal->truePredicts / 
                             (double)bimodal->maxBranch) * 100;
    double gsharePercent = ((double)gshare->truePredicts / 
                           (double)gshare->maxBranch) * 100;
    double accuracy = ((double)truePredicts / (double)maxBranch) * 100;

    printf("Bimodal Accuracy: %f\n", bimodalPercent);
    printf("Gshare Accuracy: %f\n", gsharePercent);
    printf("Combining predictors Accuracy: %f\n", accuracy);
};

// =============================================================================
predictors_t::~predictors_t() {
    valid = 0;
    maxBranch = 0;
    truePredicts = 0;
    addressSize = 0;
    hitPredict = false;
    gsharePredict = false;
    bimodalPredict = false;
    currentAddress = 0;
    delete bimodal;
    delete gshare;
    delete[] select;
    bimodal = nullptr;
    gshare = nullptr;
    select = nullptr;
};

// =============================================================================