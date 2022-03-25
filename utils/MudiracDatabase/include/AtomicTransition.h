//
// Created by carlosvigo on 24/03/2022.
//

#ifndef MUDIRAC_ATOMICTRANSITION_H
#define MUDIRAC_ATOMICTRANSITION_H

#include <string>
#include <utility>
#include <vector>
#include <stdexcept>

class AtomicTransition {
    friend class MuonicAtomData;
    friend class MudiracDatabase;
private:
    std::string Name{};
    double Energy{};
    double Frequency{};

public:
    AtomicTransition(std::string name, const double &energy, const double &frequency) :
    Name(std::move(name)),
    Energy(energy),
    Frequency(frequency)
    {
        if (Energy <= 0)
            throw std::invalid_argument("Invalid transition energy E = " + std::to_string(Energy) + " eV");
        if (Frequency <= 0)
            throw std::invalid_argument("Invalid transition frequency f = " + std::to_string(Frequency) + " s^-1");
    }

public:
    [[nodiscard]] const std::string &getName() const {
        return Name;
    }
    [[nodiscard]] double getEnergy() const {
        return Energy;
    }
    [[nodiscard]] double getFrequency() const {
        return Frequency;
    }
};


#endif //MUDIRAC_ATOMICTRANSITION_H
