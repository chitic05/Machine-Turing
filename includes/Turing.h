#pragma once
#include <cstdint>
#include <cstddef> // for size_t

#include <unordered_set>
#include <unordered_map>
#include <string> 

constexpr size_t BAND_SIZE = 1024;

struct destination{
    std::string destState;
    uint8_t bandByte;
    int8_t direction;  
};

class Turing{
    private:
        uint8_t band[BAND_SIZE]{};
        size_t bandPtr{0};
        size_t inputLength{0};  // Track input length

        std::unordered_set<std::string> states;
        std::string startingState;
        std::string currentState;
        std::unordered_set<std::string> finalStates;

        std::unordered_set<uint8_t> alphabet;
                            
        std::unordered_map<std::string, // state
                        std::unordered_map<uint8_t, // bandBit
                                        destination> // dest triple(state, overwrite value, direction)
                        > transition;
        

        void setStartingState(const std::string& state);
        void addState(const std::string& state);
        void addFinalState(const std::string& state);
        
        void addSymbol(const uint8_t sym);

        void addTransition(const std::string& state, const uint8_t bit, const destination& dest);
    public:
        Turing(const char* fileName);
        void initializeBand(const char* input);
        bool run();
};