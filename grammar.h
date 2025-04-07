#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iostream>

// Symbol types
enum class SymbolType {
    TERMINAL,
    NON_TERMINAL
};

// Symbol structure
struct GrammarSymbol {
    std::string name;
    SymbolType type;
    
    // Default constructor
    GrammarSymbol() : name(""), type(SymbolType::TERMINAL) {}
    
    // Constructor
    GrammarSymbol(const std::string& n, SymbolType t) : name(n), type(t) {}
    
    // For comparisons (needed for sets and maps)
    bool operator==(const GrammarSymbol& other) const {
        return name == other.name && type == other.type;
    }
    
    bool operator<(const GrammarSymbol& other) const {
        return name < other.name || (name == other.name && type < other.type);
    }
};

// Production rule
struct Production {
    GrammarSymbol leftSide;
    std::vector<GrammarSymbol> rightSide;
    
    // Constructor
    Production(const GrammarSymbol& lhs, const std::vector<GrammarSymbol>& rhs) 
        : leftSide(lhs), rightSide(rhs) {}
    
    // For debugging
    void print() const {
        std::cout << leftSide.name << " → ";
        for (const auto& symbol : rightSide) {
            std::cout << symbol.name << " ";
        }
        std::cout << std::endl;
    }
};

// Grammar class to store and manage grammar rules
class Grammar {
private:
    std::vector<GrammarSymbol> terminals;
    std::vector<GrammarSymbol> nonTerminals;
    std::vector<Production> productions;
    GrammarSymbol startSymbol;
    
    // Maps for FIRST and FOLLOW sets
    std::map<GrammarSymbol, std::set<GrammarSymbol>> firstSets;
    std::map<GrammarSymbol, std::set<GrammarSymbol>> followSets;
    
    // Special symbols
    GrammarSymbol epsilon;
    GrammarSymbol endMarker;
    
public:
    // Constructor
    Grammar();
    
    // Initialize the grammar for our simplified C-like language
    void initializeGrammar();
    
    // Add symbols
    void addTerminal(const std::string& name);
    void addNonTerminal(const std::string& name);
    
    // Add a production rule
    void addProduction(const std::string& lhs, const std::vector<std::string>& rhs);
    
    // Set the start symbol
    void setStartSymbol(const std::string& name);
    
    // Get methods
    const std::vector<GrammarSymbol>& getTerminals() const { return terminals; }
    const std::vector<GrammarSymbol>& getNonTerminals() const { return nonTerminals; }
    const std::vector<Production>& getProductions() const { return productions; }
    const GrammarSymbol& getStartSymbol() const { return startSymbol; }
    const GrammarSymbol& getEpsilon() const { return epsilon; }
    const GrammarSymbol& getEndMarker() const { return endMarker; }
    
    // Find symbol by name
    GrammarSymbol findSymbol(const std::string& name) const;
    
    // Get productions for a non-terminal
    std::vector<Production> getProductionsFor(const GrammarSymbol& nonTerminal) const;
    
    // Print the grammar
    void printGrammar() const;
    
    // Compute FIRST and FOLLOW sets
    void computeFirstSets();
    void computeFollowSets();
    
    // Get FIRST and FOLLOW sets
    const std::set<GrammarSymbol>& getFirstSet(const GrammarSymbol& symbol) const;
    const std::set<GrammarSymbol>& getFollowSet(const GrammarSymbol& symbol) const;
    std::set<GrammarSymbol> getFirstSetOfSequence(const std::vector<GrammarSymbol>& symbols) const;
    
    // Print FIRST and FOLLOW sets
    void printFirstSets() const;
    void printFollowSets() const;
};

#endif // GRAMMAR_H