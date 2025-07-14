#pragma once

#include "Vehicle.hpp"

class PricingStrategy {
public:
    virtual ~PricingStrategy() = default;
    virtual double calculatePrice(const Vehicle& vehicle, double hours) const = 0;
};

class StandardPricing : public PricingStrategy {
public:
    double calculatePrice(const Vehicle& vehicle, double hours) const override;
};

class PremiumPricing : public PricingStrategy {
public:
    double calculatePrice(const Vehicle& vehicle, double hours) const override;
};