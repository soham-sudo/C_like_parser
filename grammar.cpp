#include "grammar.h"
#include <algorithm>
#include <stdexcept>

Grammar::Grammar() 
    : epsilon("ε", SymbolType::TERMINAL), 
      endMarker("$", SymbolType::TERMINAL) {
    // Add special symbols
    terminals.push_back(epsilon);
    terminals.push_back(endMarker);
}

void Grammar::addTerminal(const std::string& name) {
    // Check if terminal already exists
    for (const auto& term : terminals) {
        if (term.name == name) return;
    }
    terminals.push_back(GrammarSymbol(name, SymbolType::TERMINAL));
}

void Grammar::addNonTerminal(const std::string& name) {
    // Check if non-terminal already exists
    for (const auto& nonTerm : nonTerminals) {
        if (nonTerm.name == name) return;
    }
    nonTerminals.push_back(GrammarSymbol(name, SymbolType::NON_TERMINAL));
}

void Grammar::addProduction(const std::string& lhs, const std::vector<std::string>& rhs) {
    // Find or create left-hand side non-terminal
    GrammarSymbol leftSymbol = findSymbol(lhs);
    if (leftSymbol.type != SymbolType::NON_TERMINAL) {
        throw std::runtime_error("Left-hand side must be a non-terminal: " + lhs);
    }
    
    // Create right-hand side symbols
    std::vector<GrammarSymbol> rightSymbols;
    for (const auto& symName : rhs) {
        if (symName == "ε") {
            rightSymbols.push_back(epsilon);
        } else {
            rightSymbols.push_back(findSymbol(symName));
        }
    }
    
    // Add the production
    productions.push_back(Production(leftSymbol, rightSymbols));
}

void Grammar::setStartSymbol(const std::string& name) {
    startSymbol = findSymbol(name);
    if (startSymbol.type != SymbolType::NON_TERMINAL) {
        throw std::runtime_error("Start symbol must be a non-terminal: " + name);
    }
}

GrammarSymbol Grammar::findSymbol(const std::string& name) const {
    // Check if it's epsilon
    if (name == "ε") return epsilon;
    
    // Check if it's the end marker
    if (name == "$") return endMarker;
    
    // Check non-terminals
    for (const auto& nonTerm : nonTerminals) {
        if (nonTerm.name == name) return nonTerm;
    }
    
    // Check terminals
    for (const auto& term : terminals) {
        if (term.name == name) return term;
    }
    
    // Not found
    throw std::runtime_error("Symbol not found: " + name);
}

std::vector<Production> Grammar::getProductionsFor(const GrammarSymbol& nonTerminal) const {
    std::vector<Production> result;
    for (const auto& prod : productions) {
        if (prod.leftSide.name == nonTerminal.name) {
            result.push_back(prod);
        }
    }
    return result;
}

void Grammar::printGrammar() const {
    std::cout << "Grammar:" << std::endl;
    std::cout << "Start Symbol: " << startSymbol.name << std::endl;
    
    std::cout << "\nNon-Terminals: ";
    for (const auto& nonTerm : nonTerminals) {
        std::cout << nonTerm.name << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\nTerminals: ";
    for (const auto& term : terminals) {
        if (term.name != "ε" && term.name != "$") {
            std::cout << term.name << " ";
        }
    }
    std::cout << std::endl;
    
    std::cout << "\nProductions:" << std::endl;
    for (const auto& prod : productions) {
        std::cout << prod.leftSide.name << " → ";
        for (const auto& symbol : prod.rightSide) {
            std::cout << symbol.name << " ";
        }
        std::cout << std::endl;
    }
}

void Grammar::initializeGrammar() {
    // Add terminals
    addTerminal(";");
    addTerminal(",");
    addTerminal("(");
    addTerminal(")");
    addTerminal("{");
    addTerminal("}");
    addTerminal("int");
    addTerminal("float");
    addTerminal("main");
    addTerminal("=");
    addTerminal("while");
    addTerminal("<");
    addTerminal(">");
    addTerminal("++");
    addTerminal("--");
    addTerminal("+");
    addTerminal("-");
    addTerminal("*");
    addTerminal("/");
    addTerminal("ID");
    addTerminal("CONST");
    
    // Add non-terminals
    addNonTerminal("program");
    addNonTerminal("stmts");
    addNonTerminal("stmt");
    addNonTerminal("decl");
    addNonTerminal("type");
    addNonTerminal("id_list");
    addNonTerminal("id_tail");
    addNonTerminal("init_opt");
    addNonTerminal("while_stmt");
    addNonTerminal("expr_stmt");
    addNonTerminal("expr_stmt_tail");
    addNonTerminal("expr");
    addNonTerminal("expr_tail");
    addNonTerminal("term");
    addNonTerminal("term_tail");
    addNonTerminal("factor");
    addNonTerminal("unary_op");
    addNonTerminal("cond");
    addNonTerminal("rel_op");
    addNonTerminal("add_op");
    addNonTerminal("mul_op");
    
    // Set the start symbol
    setStartSymbol("program");
    
    // Add productions
    // Program structure
    addProduction("program", {"int", "main", "(", ")", "{", "stmts", "}"});
    
    // Statements
    addProduction("stmts", {"stmt", "stmts"});
    addProduction("stmts", {"ε"});
    
    // Statement types
    addProduction("stmt", {"decl"});
    addProduction("stmt", {"expr_stmt"});
    addProduction("stmt", {"while_stmt"});
    
    // Declaration
    addProduction("decl", {"type", "id_list", ";"});
    
    // Type
    addProduction("type", {"int"});
    addProduction("type", {"float"});
    
    // Identifier list
    addProduction("id_list", {"ID", "init_opt", "id_tail"});
    
    // Identifier list tail
    addProduction("id_tail", {",", "ID", "init_opt", "id_tail"});
    addProduction("id_tail", {"ε"});
    
    // Initialization option
    addProduction("init_opt", {"=", "expr"});
    addProduction("init_opt", {"ε"});
    
    // Expression statements
    addProduction("expr_stmt", {"ID", "expr_stmt_tail"});
    addProduction("expr_stmt", {"unary_op", "ID", ";"});
    
    // Expression statement tail - to disambiguate the ID cases
    addProduction("expr_stmt_tail", {"=", "expr", ";"});
    addProduction("expr_stmt_tail", {"++", ";"});
    addProduction("expr_stmt_tail", {"--", ";"});
    
    // While statement
    addProduction("while_stmt", {"while", "(", "cond", ")", "{", "stmts", "}"});
    
    // Condition
    addProduction("cond", {"expr", "rel_op", "expr"});
    
    // Relational operators
    addProduction("rel_op", {"<"});
    addProduction("rel_op", {">"});
    
    // Expression
    addProduction("expr", {"term", "expr_tail"});
    
    // Expression tail
    addProduction("expr_tail", {"add_op", "term", "expr_tail"});
    addProduction("expr_tail", {"ε"});
    
    // Term
    addProduction("term", {"factor", "term_tail"});
    
    // Term tail
    addProduction("term_tail", {"mul_op", "factor", "term_tail"});
    addProduction("term_tail", {"ε"});
    
    // Factor
    addProduction("factor", {"ID"});
    addProduction("factor", {"CONST"});
    addProduction("factor", {"(", "expr", ")"});
    
    // Unary operators
    addProduction("unary_op", {"++"});
    addProduction("unary_op", {"--"});
    
    // Additive operators
    addProduction("add_op", {"+"});
    addProduction("add_op", {"-"});
    
    // Multiplicative operators
    addProduction("mul_op", {"*"});
    addProduction("mul_op", {"/"});
    
    // Compute FIRST and FOLLOW sets right away
    computeFirstSets();
    computeFollowSets();
    
    // Print the FIRST set of program for debugging
    std::cout << "DEBUG: FIRST(program) = { ";
    for (const auto& symbol : getFirstSet(findSymbol("program"))) {
        std::cout << symbol.name << " ";
    }
    std::cout << "}" << std::endl;
}

// FIRST set computation
void Grammar::computeFirstSets() {
    // Initialize FIRST sets
    for (const auto& terminal : terminals) {
        firstSets[terminal].insert(terminal);
    }
    
    for (const auto& nonTerminal : nonTerminals) {
        firstSets[nonTerminal] = {};
    }
    
    // Keep computing until no changes
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (const auto& production : productions) {
            const GrammarSymbol& X = production.leftSide;
            const std::vector<GrammarSymbol>& rhs = production.rightSide;
            
            size_t size_before = firstSets[X].size();
            
            // Empty production
            if (rhs.empty() || rhs[0].name == "ε") {
                if (firstSets[X].insert(epsilon).second) {
                    changed = true;
                }
                continue;
            }
            
            // Add FIRST(Y₁) to FIRST(X) if Y₁ is the first symbol in the production
            std::set<GrammarSymbol> firstOfY1 = firstSets[rhs[0]];
            firstOfY1.erase(epsilon);
            for (const auto& terminal : firstOfY1) {
                if (firstSets[X].insert(terminal).second) {
                    changed = true;
                }
            }
            
            // If Y₁, Y₂, ..., Yᵢ₋₁ all have ε in their FIRST sets
            bool allHaveEpsilon = firstSets[rhs[0]].count(epsilon) > 0;
            for (size_t i = 1; i < rhs.size() && allHaveEpsilon; ++i) {
                const GrammarSymbol& Yi = rhs[i];
                
                // Add FIRST(Yᵢ) - {ε} to FIRST(X)
                std::set<GrammarSymbol> firstOfYi = firstSets[Yi];
                firstOfYi.erase(epsilon);
                for (const auto& terminal : firstOfYi) {
                    if (firstSets[X].insert(terminal).second) {
                        changed = true;
                    }
                }
                
                // Check if Yᵢ has ε
                allHaveEpsilon = firstSets[Yi].count(epsilon) > 0;
            }
            
            // If all symbols in rhs have ε, add ε to FIRST(X)
            if (allHaveEpsilon) {
                if (firstSets[X].insert(epsilon).second) {
                    changed = true;
                }
            }
            
            if (firstSets[X].size() > size_before) {
                changed = true;
            }
        }
    }
}

// FOLLOW set computation
void Grammar::computeFollowSets() {
    // Initialize FOLLOW sets
    for (const auto& nonTerminal : nonTerminals) {
        followSets[nonTerminal] = {};
    }
    
    // Add $ to FOLLOW(StartSymbol)
    followSets[startSymbol].insert(endMarker);
    
    // Keep computing until no changes
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (const auto& production : productions) {
            const GrammarSymbol& A = production.leftSide;
            const std::vector<GrammarSymbol>& rhs = production.rightSide;
            
            for (size_t i = 0; i < rhs.size(); ++i) {
                const GrammarSymbol& B = rhs[i];
                
                // Only interested in non-terminals
                if (B.type != SymbolType::NON_TERMINAL) continue;
                
                size_t size_before = followSets[B].size();
                
                // If B is followed by another symbol in the production
                if (i + 1 < rhs.size()) {
                    const GrammarSymbol& next = rhs[i + 1];
                    
                    // Add FIRST(next) - {ε} to FOLLOW(B)
                    std::set<GrammarSymbol> firstOfNext = firstSets[next];
                    bool hasEpsilon = firstOfNext.count(epsilon) > 0;
                    firstOfNext.erase(epsilon);
                    
                    for (const auto& symbol : firstOfNext) {
                        followSets[B].insert(symbol);
                    }
                    
                    // If FIRST(next) contains ε, add FOLLOW(A) to FOLLOW(B)
                    if (hasEpsilon) {
                        for (const auto& symbol : followSets[A]) {
                            followSets[B].insert(symbol);
                        }
                    }
                } 
                // If B is the last symbol in the production or all symbols to the right derive ε
                else {
                    // Add FOLLOW(A) to FOLLOW(B)
                    for (const auto& symbol : followSets[A]) {
                        followSets[B].insert(symbol);
                    }
                }
                
                if (followSets[B].size() > size_before) {
                    changed = true;
                }
            }
            
            // Handle the case where multiple symbols after B can derive ε
            for (int i = rhs.size() - 1; i >= 0; --i) {
                const GrammarSymbol& B = rhs[i];
                
                if (B.type != SymbolType::NON_TERMINAL) break;
                
                bool allHaveEpsilon = true;
                for (size_t j = i + 1; j < rhs.size(); ++j) {
                    if (firstSets[rhs[j]].count(epsilon) == 0) {
                        allHaveEpsilon = false;
                        break;
                    }
                }
                
                if (allHaveEpsilon) {
                    size_t size_before = followSets[B].size();
                    
                    // Add FOLLOW(A) to FOLLOW(B)
                    for (const auto& symbol : followSets[A]) {
                        followSets[B].insert(symbol);
                    }
                    
                    if (followSets[B].size() > size_before) {
                        changed = true;
                    }
                } else {
                    break;
                }
            }
        }
    }
}

std::set<GrammarSymbol> Grammar::getFirstSetOfSequence(const std::vector<GrammarSymbol>& symbols) const {
    std::set<GrammarSymbol> result;
    
    if (symbols.empty()) {
        result.insert(epsilon);
        return result;
    }
    
    bool allHaveEpsilon = true;
    for (const auto& symbol : symbols) {
        // Get FIRST set for this symbol
        const std::set<GrammarSymbol>& firstSet = getFirstSet(symbol);
        
        // Add all terminals except epsilon
        for (const auto& term : firstSet) {
            if (term.name != "ε") {
                result.insert(term);
            }
        }
        
        // Check if this symbol can derive epsilon
        if (firstSet.count(epsilon) == 0) {
            allHaveEpsilon = false;
            break;
        }
    }
    
    // If all symbols can derive epsilon, add epsilon to result
    if (allHaveEpsilon) {
        result.insert(epsilon);
    }
    
    return result;
}

const std::set<GrammarSymbol>& Grammar::getFirstSet(const GrammarSymbol& symbol) const {
    auto it = firstSets.find(symbol);
    if (it == firstSets.end()) {
        static std::set<GrammarSymbol> emptySet;
        return emptySet;
    }
    return it->second;
}

const std::set<GrammarSymbol>& Grammar::getFollowSet(const GrammarSymbol& symbol) const {
    auto it = followSets.find(symbol);
    if (it == followSets.end()) {
        static std::set<GrammarSymbol> emptySet;
        return emptySet;
    }
    return it->second;
}

void Grammar::printFirstSets() const {
    std::cout << "\nFIRST Sets:" << std::endl;
    for (const auto& nonTerm : nonTerminals) {
        std::cout << "FIRST(" << nonTerm.name << ") = { ";
        for (const auto& symbol : getFirstSet(nonTerm)) {
            std::cout << symbol.name << " ";
        }
        std::cout << "}" << std::endl;
    }
}

void Grammar::printFollowSets() const {
    std::cout << "\nFOLLOW Sets:" << std::endl;
    for (const auto& nonTerm : nonTerminals) {
        std::cout << "FOLLOW(" << nonTerm.name << ") = { ";
        for (const auto& symbol : getFollowSet(nonTerm)) {
            std::cout << symbol.name << " ";
        }
        std::cout << "}" << std::endl;
    }
}