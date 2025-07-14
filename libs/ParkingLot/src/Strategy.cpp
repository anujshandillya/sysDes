#include "../include/Strategy.hpp"
#include "../include/Vehicle.hpp"

double StandardPricing::calculatePrice(const Vehicle& v, double hours) const {
    double baseRate = 5.0; // $5 per hour
    double minCharge = 2.0; // Minimum $2 charge
    
    double total = baseRate * hours;
    return (total < minCharge) ? minCharge : total;
}

double PremiumPricing::calculatePrice(const Vehicle& v, double hours) const {
    double baseRate = 8.0; // $8 per hour for premium
    double minCharge = 3.0; // Minimum $3 charge
    
    double total = baseRate * hours;
    return (total < minCharge) ? minCharge : total;
}