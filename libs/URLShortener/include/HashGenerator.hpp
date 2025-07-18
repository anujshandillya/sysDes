#ifndef HASH_GENERATOR_HPP
#define HASH_GENERATOR_HPP

#include <string>
#include <random>
#include <unordered_set>

class HashGenerator {
private:
    static const std::string CHARACTERS;
    static const int DEFAULT_LENGTH;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;
    std::unordered_set<std::string> usedCodes;

public:
    HashGenerator();
    
    // Generate a unique short code
    std::string generateShortCode(int length = DEFAULT_LENGTH);
    
    // Generate a short code from a URL (hash-based)
    std::string generateFromUrl(const std::string& url, int length = DEFAULT_LENGTH);
    
    // Check if a code is already used
    bool isCodeUsed(const std::string& code) const;
    
    // Add a code to the used set
    void addUsedCode(const std::string& code);
    
    // Remove a code from the used set
    void removeUsedCode(const std::string& code);
    
    // Clear all used codes
    void clearUsedCodes();
    
    // Get the number of used codes
    size_t getUsedCodesCount() const;
    
    // Generate a custom code (user-defined)
    bool generateCustomCode(const std::string& customCode);
};

#endif // HASH_GENERATOR_HPP 