#pragma once

class ImpactStrategy {
public:
    virtual ~ImpactStrategy() = default;
    virtual float calculate(float ovr) = 0;
};

class SetterStrategy : public ImpactStrategy {
public:
    float calculate(float ovr) override { return ovr * 1.2f; }
};

class LiberoStrategy : public ImpactStrategy {
public:
    float calculate(float ovr) override { return ovr * 0.9f; }
};