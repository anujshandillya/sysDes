#include "../include/HashGenerator.hpp"
#include <functional>
#include <cctype>

const std::string HashGenerator::CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const int HashGenerator::DEFAULT_LENGTH = 6;

HashGenerator::HashGenerator() : gen(rd()), dis(0, CHARACTERS.length() - 1) {
}

std::string HashGenerator::generateShortCode(int length) {
    std::string code;
    code.reserve(length);
    
    do {
        code.clear();
        for (int i = 0; i < length; ++i) {
            code += CHARACTERS[dis(gen)];
        }
    } while (isCodeUsed(code));
    
    addUsedCode(code);
    return code;
}

std::string HashGenerator::generateFromUrl(const std::string& url, int length) {
    // Simple hash-based generation
    std::hash<std::string> hasher;
    size_t hash = hasher(url);
    
    std::string code;
    code.reserve(length);
    
    for (int i = 0; i < length; ++i) {
        size_t index = (hash >> (i * 8)) % CHARACTERS.length();
        code += CHARACTERS[index];
    }
    
    // If code is already used, generate a random one
    if (isCodeUsed(code)) {
        return generateShortCode(length);
    }
    
    addUsedCode(code);
    return code;
}

bool HashGenerator::isCodeUsed(const std::string& code) const {
    return usedCodes.find(code) != usedCodes.end();
}

void HashGenerator::addUsedCode(const std::string& code) {
    usedCodes.insert(code);
}

void HashGenerator::removeUsedCode(const std::string& code) {
    usedCodes.erase(code);
}

void HashGenerator::clearUsedCodes() {
    usedCodes.clear();
}

size_t HashGenerator::getUsedCodesCount() const {
    return usedCodes.size();
}

bool HashGenerator::generateCustomCode(const std::string& customCode) {
    // Validate custom code
    if (customCode.empty() || customCode.length() > 20) {
        return false;
    }
    
    // Check if code contains only valid characters
    for (char c : customCode) {
        if (CHARACTERS.find(c) == std::string::npos) {
            return false;
        }
    }
    
    // Check if code is already used
    if (isCodeUsed(customCode)) {
        return false;
    }
    
    addUsedCode(customCode);
    return true;
} 