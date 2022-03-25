//
// Created by carlosvigo on 24/03/2022.
//

#ifndef MUDIRAC_MUONICDATABASE_H
#define MUDIRAC_MUONICDATABASE_H

#include "MuonicAtomData.h"
#include "AtomicTransition.h"

#include <string>
#include <map>
#include <ctime>


typedef struct MudiracConfig {
    unsigned int MaxShell = 6;
    unsigned int MaxCalcShell = 6;
    bool UehlingCorrection = true;

    MudiracConfig() = default;
} MudiracConfig;

class MudiracDatabase {
private:
    typedef std::pair<unsigned short,unsigned short> Isotope;   // Pair <Z, A>
    typedef std::pair<Isotope, MuonicAtomData> DatabaseEntry;   // Pair <Isotope, Data>
    std::map<Isotope, MuonicAtomData> Database;
    MudiracConfig Config{};
    struct tm Time{};
    unsigned short NumberIsotopes{0};
    unsigned short NumberTransitions{0};

public:
    MudiracDatabase() = default;
    explicit MudiracDatabase(const std::string &filename) {
        loadFromFile(filename);
    }
    explicit MudiracDatabase(
            const std::vector<MuonicAtomData> &listIsotopes,
            const MudiracConfig config = MudiracConfig()) : Config(config) {
        // Add to library
        for (const auto &isotope : listIsotopes)
            if (! addIsotope(isotope))
                throw std::runtime_error("List of isotopes contains duplicates");
    }

public:
    [[nodiscard]] const auto &getDatabase() const {
        return Database;
    }
    [[nodiscard]] const auto &getConfig() const {
        return Config;
    }
    [[nodiscard]] auto getTime() const {
        return Time;
    }
    [[nodiscard]] auto getTimeStr() const {
        auto ascTime = std::string(std::asctime(&Time));
        ascTime.pop_back();
        return ascTime;
    }
    [[nodiscard]] auto getNumberIsotopes() const {
        return NumberIsotopes;
    }
    [[nodiscard]] auto getNumberTransitions() const {
        return NumberTransitions;
    }
    [[nodiscard]] auto getIsotope(const unsigned short &Z, const unsigned short &A) const {
        return Database.at(Isotope(Z, A));
    }

    // Returns true if a new isotope was inserted or false if it already existed.
    bool addIsotope(const MuonicAtomData &data) {
        if (Database.insert(DatabaseEntry(Isotope(data.Z, data.A), data )).second) {
            ++NumberIsotopes;
            NumberTransitions += data.getTransitions().size();
            return true;
        }
        return false;
    }

    // Loads the mudirac database from a file
    void loadFromFile(const std::string &filename);

    // Saves the mudirac database to a file
    void writeToFile(const std::string &filename, const bool &updateTime = true);
};


#endif //MUDIRAC_MUONICDATABASE_H
