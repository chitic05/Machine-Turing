#include "Turing.h"
#include "parser.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

Turing::Turing(const char* fileName){

    Parser parser(fileName);
    sectionsMap& sm = parser.getSections();

    auto statesIt = sm.find(".states");
    if (statesIt != sm.end() && !statesIt->second.empty()) {
        for (const auto& l : statesIt->second) {
            std::istringstream line(l);
            std::string word;
            if (!(line >> word)) {
                continue;
            }
            
            bool F = false, S = false;
            do {
                if(word == "F") F = true;
                else if(word == "S") S = true;
                else break;  // word is the state name, exit loop
                line >> word;
            } while(line);
            if(F) addFinalState(word);
            if(S) setStartingState(word);
            addState(word);
            
        }
    } else {
        throw std::runtime_error("States este gol!\n");
    }

    if (this->startingState.empty()) {
        throw std::runtime_error("Nu ai definit starea initiala (S <stare>).\n");
    }

    /*====ALPHABET======*/

    auto alphabetIt = sm.find(".alphabet");
    if (alphabetIt != sm.end() && !alphabetIt->second.empty()) {
        for (const auto& l : alphabetIt->second) {
            std::istringstream line(l);
            char c;
            while (line >> c) {
                this->addSymbol(c);
            }
        }
    } else {
        throw std::runtime_error("Alphabet este gol!\n");
    }

    /*====TRANSITIONS======*/
    auto transitionsIt = sm.find(".transitions");
    if (transitionsIt != sm.end() && !transitionsIt->second.empty()) {
        for (const auto& l : transitionsIt->second) {
            std::istringstream line(l);
            std::string from, sym, to, bit, dir;

            if (!(line >> from >> sym >> to >> bit >> dir)) {
                continue;
            }

            if(sym.length()>1){
                throw std::runtime_error("Symbol should be a char! " + sym + "\n");
            }
            if(sym[0] == '\0'){
                std::cerr << "Warning: Skipping transition with null byte (\\0) - reserved as blank symbol!\n";
                continue;
            }
            if(bit.length()>1){
                throw std::runtime_error("Overwrite bit should be a char! " + bit + "\n");
            }
            int8_t d;
            if(toupper(dir[0]) == 'L') d = -1;
            else if(toupper(dir[0]) == 'R') d = 1;
            else throw std::runtime_error("Direction is invalid: " + dir + "!\n");
            try {
                this->addTransition(from, sym[0], {
                                                    to,
                                                    static_cast<uint8_t>(bit[0]),
                                                    d
                                                  });
            } catch (const std::exception& e) {
                std::cerr << "Eroarea cand s-au parsat tranzitiile: " << e.what() << '\n';
            }
        }
    } else {
        throw std::runtime_error("Transitions este gol!\n");
    }

}

void Turing::initializeBand(const char* input){
    size_t i;
    for(i = 0; input[i] != '\0' && i < BAND_SIZE; i++) {
        band[i] = static_cast<uint8_t>(input[i]);
    }
    inputLength = i;  // Store the input length
    //Fiil with '\0' blank
    while(i < BAND_SIZE) {
        band[i] = '\0';
        i++;
    }
}

void Turing::setStartingState(const std::string& state){
    this->startingState = state;
}
void Turing::addState(const std::string& state){
    this->states.insert(state);
}
void Turing::addFinalState(const std::string& state){
    this->finalStates.insert(state);

}
        
void Turing::addSymbol(const uint8_t sym){
    this->alphabet.insert(sym);
}

void Turing::addTransition(const std::string& state, const uint8_t bit, const destination& dest){
    if(this->states.count(state) && this->alphabet.count(bit)){
        if(this->states.count(dest.destState) && this->alphabet.count(dest.bandByte) && abs(dest.direction)<=1){
            this->transition[state][bit] = dest;
            return;
        }
    }
    throw std::runtime_error("Transition is impossible!\n");
}


bool Turing::run(){
    currentState = startingState;
    bandPtr = 0;
    
    while(true) {
        //If in final state AND reached end of input, accept
        if(finalStates.count(currentState) > 0 && bandPtr >= inputLength) {
            return true;
        }
        
        //Current symbol from band
        uint8_t currentSymbol = band[bandPtr];
        
        auto stateIt = transition.find(currentState);
        if(stateIt == transition.end()) {
            return false;  //No transition from this state
        }

        auto symbolIt = stateIt->second.find(currentSymbol);
        if(symbolIt == stateIt->second.end()) {
            return false;  //No transition for this symbol
        }
        
        const destination& dest = symbolIt->second;
        
        band[bandPtr] = dest.bandByte;
        
        int newPtr = static_cast<int>(bandPtr) + dest.direction;
        if(newPtr < 0 || newPtr >= static_cast<int>(BAND_SIZE)) {
            return false;  // Pointer out of bounds
        }
        bandPtr = static_cast<size_t>(newPtr);
        
        currentState = dest.destState;
    }
    
    return false;  // Should not reach here
}