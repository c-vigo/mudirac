//
// Created by carlosvigo on 24/03/2022.
//

#ifndef MUDIRAC_MUONICATOMDATA_H
#define MUDIRAC_MUONICATOMDATA_H

#include "AtomicTransition.h"

#include <string>
#include <vector>

class MuonicAtomData {
    friend class MudiracDatabase;
private:
    unsigned short Z{}, A{};
    std::vector<AtomicTransition> Transitions{};

public:
    MuonicAtomData(const unsigned short &z, const unsigned short &a) : Z(z), A(a) {
        if (Z < 1 || Z > 112)
            throw std::invalid_argument("Invalid Z = " + std::to_string(z));
        if (A < 1 || A > 300)
            throw std::invalid_argument("Invalid A = " + std::to_string(a));
    }

    bool operator >(const MuonicAtomData &other) const {
        return Z > other.Z || A > other.A;
    }

public:
    [[nodiscard]] auto getZ() const {
        return Z;
    }
    [[nodiscard]] auto getA() const {
        return A;
    }
    [[nodiscard]] const auto &getTransitions() const {
        return Transitions;
    }

    // Tries to add a transition to the list. If it already existed, returns false; true if it succeeds.
    bool addTransition(const std::string& name, const double &energy, const double &frequency) {
        for (const auto &transition : Transitions)
            if (transition.getName() == name)
                return false;
        Transitions.emplace_back(name, energy, frequency);
        return true;
    }

};

std::vector<MuonicAtomData> loadMuonicData(const std::string &filename);

#endif //MUDIRAC_MUONICATOMDATA_H
