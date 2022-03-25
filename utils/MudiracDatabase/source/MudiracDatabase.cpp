//
// Created by carlosvigo on 24/03/2022.
//

#include "MudiracDatabase.h"

// System
#include <fstream>
#include <ctime>
#include <iomanip>
#include <iostream>

void MudiracDatabase::loadFromFile(const std::string &filename) {
    // Open input file with database
    std::ifstream fIn;
    fIn.open(filename, std::ios::in);
    if (!fIn.is_open())
        throw std::runtime_error("Unable to open mudirac database file " + filename);
    // std::cout << "Loading mudirac database from " << filename << std::endl;


    // Library header
    std::string line, buffer;
    std::getline(fIn, line); // discard first line
    fIn.imbue(std::locale("C.UTF-8"));
    fIn >> buffer >> buffer >> std::get_time(&Time, "%c"); // Retrieve time
    // std::cout << "Database created on " << std::put_time(&Time, "%c") << std::endl;

    // Coming: database configuration

    // Transitions header
    for (int i = 0; i < 4; ++i)
        std::getline(fIn, line); // discard line

    // Parse transitions
    unsigned short Z, A;
    std::string name;
    double E, f;
    while (fIn >> Z >> A >> name >> E >> f) {
        const auto search = Database.find(Isotope(Z, A));
        if (search == Database.end()) {
            // New isotope
            auto data = MuonicAtomData(Z, A);
            data.addTransition(name, E, f);
            addIsotope(data);
        } else {
            if (Database.at(Isotope(Z, A)).addTransition(name, E, f))
                ++NumberTransitions;
        }
    }
    /*std::cout << "Database loaded with " << NumberIsotopes << " muonic isotopes and "
            << NumberTransitions << " transitions"<< std::endl;*/
    fIn.close();
}

void MudiracDatabase::writeToFile(const std::string &filename, const bool &updateTime) {
    // Open output file to save data
    std::ofstream fOut;
    fOut.open(filename, std::ios::out);
    if (!fOut.is_open())
        throw std::runtime_error("Unable to create mudirac database file " + filename);

    // Update time?
    if (updateTime) {
        auto t = std::time(nullptr);
        Time = *std::gmtime(&t);
    }

    // Library header
    fOut << "MUDIRAC DATABASE OF MUONIC ATOM TRANSITIONS" << std::endl;
    fOut.imbue(std::locale("C.UTF-8"));
    fOut << "Created on " << std::put_time(&Time, "%c") << std::endl;
    fOut << "Content: " << NumberIsotopes << " muonic isotopes and " << NumberTransitions << " transitions" << std::endl;

    // Transitions header
    fOut << std::endl;
    fOut << std::setw(5) << "Z"
         << std::setw(5) << "A"
         << std::setw(10) << "Line"
         << std::setw(25) << "Energy [eV]"
         << std::setw(25) << "Frequency [s^-1]"
         << std::endl;
    for (const auto & [isotope, data] : Database)
        for (const auto &line : data.Transitions)
            fOut << std::setw(5) << data.Z
                 << std::setw(5) << data.A
                 << std::setw(10) << line.Name
                 << std::setw(25) << line.Energy
                 << std::setw(25) << line.Frequency
                 << std::endl;

    // Exit
    fOut.close();
    std::cout << "mudirac database saved to " << filename
         << " with " << NumberIsotopes << " muonic isotopes"
         << " and " << NumberTransitions << " transitions"<< std::endl;
}
