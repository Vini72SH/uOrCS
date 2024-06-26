#include "btb.hpp" // Methods for all BTB classes

// Suport Function // 
// =============================================================================
uint64_t set_ind(uint64_t address) {
    return address % ASSOCIATIVE_SET;
};

// =============================================================================

// BTB_INPUT_T METHODS //
// =============================================================================
btb_input_t::btb_input_t() {
    instruction_address = 0;
    last_access_cycle = 0;
    typeBranch = 0;
    valityBit = false;
};

// =============================================================================
btb_input_t::~btb_input_t() {};

// =============================================================================

// BTB_SET_T METHODS //
// =============================================================================
btb_set_t::btb_set_t() {
    inputs = nullptr;
};

// =============================================================================
void btb_set_t::allocate_set() {
    // Assigns nullptr to all pointers
    this->inputs = new btb_input_t*[INPUT];
    for (int i = 0; i < INPUT; ++i)
        this->inputs[i] = nullptr;
};

// =============================================================================
btb_set_t::~btb_set_t() {
    // Frees all allocated memory
    if (this->inputs != nullptr) {
        for (int i = 0; i < INPUT; ++i)
            delete this->inputs[i];
        delete[] this->inputs;
    }
};

// =============================================================================

// BTB_T METHODS //
// =============================================================================
btb_t::btb_t() {};

// =============================================================================
void btb_t::allocate() {
    this->typeBranch = 0;
    this->previousHitMiss = false;
    this->btbHit = 0;
    this->totalBranch = 0;
    this->sets = new btb_set_t*[ASSOCIATIVE_SET];
    // Assigns nullptr to all pointers
    for (int i = 0; i < ASSOCIATIVE_SET; ++i) {
        this->sets[i] = nullptr;
    }
};

// =============================================================================
bool btb_t::getPrevious() {
    return previousHitMiss;
};

// =============================================================================
short btb_t::getTypeBranch() {
    return typeBranch;
};

// =============================================================================
void btb_t::setPrevious(bool hit_miss) {
    previousHitMiss = hit_miss;
};

// =============================================================================
void btb_t::btb_insert(uint64_t address, short br_type, uint64_t current_cycle) {
    // If the set does not exist, create it
    uint32_t address_set = set_ind(address);
    if (this->sets[address_set] == nullptr) {
        this->sets[address_set] = new btb_set_t();
        this->sets[address_set]->allocate_set();
    }
    btb_set_t *current_set = this->sets[address_set];

    /* Search for empty space or oldest address */
    int index = 0;
    uint64_t cycle = current_cycle;
    for (int i = 0; i < INPUT; i++) {
        if ((current_set->inputs[i] == nullptr)) {
            current_set->inputs[i] = new btb_input_t;
            index = i;
            break;
        }

        if (current_set->inputs[i]->last_access_cycle < cycle) {
            index = i;
            cycle = current_set->inputs[i]->last_access_cycle;
        }
    }

    /* Set the input */
    current_set->inputs[index]->instruction_address = address;
    current_set->inputs[index]->typeBranch = br_type;
    current_set->inputs[index]->last_access_cycle = current_cycle;
    current_set->inputs[index]->valityBit = true;
    this->typeBranch = br_type;
    totalBranch++;
};

// =============================================================================
int btb_t::btb_search_update(uint64_t address, uint64_t current_cycle) {
    int i;
    int index = 0;
    uint32_t address_set = set_ind(address);
    btb_set_t *current_set = this->sets[address_set];


    if (current_set == nullptr)
        return 0;

    /* Search the address */
    btb_input_t *current_input;
    for (i = 0; i < INPUT; ++i) {
        current_input = current_set->inputs[i];
        if (current_input != nullptr) {
            if ((current_input->instruction_address == address) && 
               (current_input->valityBit == true)) {
                index = i;
                break;
            }
        }
    }

    if(index != i) return 0;

    // Update input
    current_input = current_set->inputs[index];
    current_input->last_access_cycle = current_cycle;
    this->typeBranch = current_input->typeBranch;
    totalBranch++;
    btbHit++;

    return 1;
};

// =============================================================================
void btb_t::statistics() {
    int i, j, total, inputs;
    btb_set_t *set_atual;
    btb_input_t *entrada_atual;

    total = 0;
    inputs = 0;
    for (i = 0; i < ASSOCIATIVE_SET; ++i) {
        set_atual = this->sets[i];
        if (set_atual != nullptr) {
            ++total;
            //std::cout << "Conjunto N° " << i << "\n";
            //std::cout << "==================================\n";
            for (j = 0; j < INPUT; ++j) {
                entrada_atual = set_atual->inputs[j];
                if (entrada_atual != nullptr) {
                    ++inputs;
                    //printf("Add: %ld BR: %d Cycle: %ld\n",
                    //       entrada_atual->instruction_address,
                    //       entrada_atual->type_branch, 
                    //       entrada_atual->last_access_cycle);
                }
            }
            //std::cout << "==================================\n";
        }
    }

    double result = ((double)btbHit / (double)totalBranch) * 100;
    printf("\n=== Branch Target Buffer Statistics === \n");
    printf("----------------------------------------\n");
    printf("Used/Total Inputs: %d / %d\n", inputs, INPUT * ASSOCIATIVE_SET);
    printf("Used/Total Sets: %d / %d\n", total, ASSOCIATIVE_SET);
    printf("BTB Accuracy: %f\n", result);
    printf("Hits / Total Branch: %ld / %ld\n", btbHit, totalBranch);
    printf("========================================\n");
};

// =============================================================================
btb_t::~btb_t() {
    if (this->sets != nullptr) {
        for (int i = 0; i < ASSOCIATIVE_SET; ++i)
            delete sets[i];
    }
    delete[] sets;
};
// =============================================================================
