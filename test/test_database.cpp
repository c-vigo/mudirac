//
// Created by carlosvigo on 25/03/2022.
//

#include <vector>
#include <fstream>
#include <iostream>
#include "../utils/MudiracDatabase/include/MudiracDatabase.h"
#include "../vendor/catch/catch.hpp"
#include "datapath.h"

bool compareFiles(const std::string& p1, const std::string& p2) {
    std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false; //file problem
    }

    if (f1.tellg() != f2.tellg()) {
        return false; //size mismatch
    }

    //seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
}

TEST_CASE("Atomic Transitions", "[AtomicTransition]")
{
    auto t1 = AtomicTransition("K1:L1", 8000., 1.47E12);
    REQUIRE(t1.getName() == "K1:L1");
    REQUIRE(t1.getEnergy() == 8000.);
    REQUIRE(t1.getFrequency() == 1.47E12);

    // Failure cases
    REQUIRE_THROWS(AtomicTransition("line", -1., 2));
    REQUIRE_THROWS(AtomicTransition("line", 1., -2));
}

TEST_CASE("Muonic Atom Data", "[MuonicAtomData]")
{
    // Muonic Atom creation
    auto m = MuonicAtomData(3, 5);
    REQUIRE(m.getZ() == 3);
    REQUIRE(m.getA() == 5);
    REQUIRE_THROWS(MuonicAtomData(-1, 2));
    REQUIRE_THROWS(MuonicAtomData(0, 2));
    REQUIRE_THROWS(MuonicAtomData(120, 2));
    REQUIRE_THROWS(MuonicAtomData(1, -2));
    REQUIRE_THROWS(MuonicAtomData(1, 0));

    // Add transitions
    for (unsigned short i = 1; i <= 100; ++i) // Add 100 transitions
        REQUIRE(m.addTransition("line_" + std::to_string(i), 0.001 * i, 0.003 * i) == true);
    for (unsigned short i = 1; i <= 100; ++i) // Try to add the same transitions
        REQUIRE(m.addTransition("line_" + std::to_string(i), 0.001 * i, 0.003 * i) == false);
    REQUIRE(m.getTransitions().size() == 100);
    for (unsigned short i = 1; i <= 100; ++i) {
        REQUIRE(m.getTransitions().at(i - 1).getName() == "line_" + std::to_string(i));
        REQUIRE(m.getTransitions().at(i - 1).getEnergy() == 0.001 * i);
        REQUIRE(m.getTransitions().at(i - 1).getFrequency() == 0.003 * i);
    }

    // Comparator operator
    auto m2 = MuonicAtomData(3, 7);
    auto m3 = MuonicAtomData(4, 1);
    auto m4 = MuonicAtomData(4, 6);
    REQUIRE(m2 > m);
    REQUIRE(m3 > m2);
    REQUIRE(m4 > m3);
    auto m_copy = MuonicAtomData(3, 5);
    REQUIRE(!(m > m_copy));
    REQUIRE(!(m_copy > m));
    auto m_2 = MuonicAtomData(3, 4);
    auto m_3 = MuonicAtomData(2, 6);
    auto m_4 = MuonicAtomData(2, 1);
    REQUIRE(m > m_2);
    REQUIRE(m_2 > m_3);
    REQUIRE(m_3 > m_4);


    // Invalid transitions
    REQUIRE_THROWS(m.addTransition("fail", 0, 7));
    REQUIRE_THROWS(m.addTransition("fail", -1, 7));
    REQUIRE_THROWS(m.addTransition("fail", 2, 0));
    REQUIRE_THROWS(m.addTransition("fail", 2, -5));
}

TEST_CASE("Mudirac Database", "[MudiracDatabase]")
{
    // Invalid file
    REQUIRE_THROWS(MudiracDatabase("/path/to/nowhere/file.txt"));

    // Load file with 26 isotopes, 4449 transitions
    const auto dbFile = std::string(CURRENT_DATAPATH) + "/data/mudiracDatabase.dat";
    REQUIRE_NOTHROW(MudiracDatabase(dbFile));
    auto db = MudiracDatabase(dbFile);

    // Check database settings
    REQUIRE(db.getTimeStr() == "Fri Mar 25 14:30:34 2022");         // Date

    // Check content
    REQUIRE(db.getNumberTransitions() == 4449);
    REQUIRE(db.getNumberIsotopes() == 26);

    // Retrieve a non-existing isotope: Ti-45
    REQUIRE_THROWS(db.getIsotope(22, 45));

    // Retrieve an existing isotope: Ti-46 with 165 transitions
    auto ti46 = db.getIsotope(22, 46);
    REQUIRE(ti46.getZ() == 22);
    REQUIRE(ti46.getA() == 46);
    const auto& ti46_trans = ti46.getTransitions();
    REQUIRE(ti46_trans.size() == 165);

    // Check some lines
    // So far, just a vector of lines, no meaningful way to search for one
    auto line_O3_P5 = ti46_trans.at(138);
    REQUIRE(line_O3_P5.getName() == "O3-P5");
    REQUIRE(line_O3_P5.getEnergy() == 16681.6);
    REQUIRE(line_O3_P5.getFrequency() == 2.15821e+13);

    // Write database to a temporary file
    const auto outputFile = "/tmp/test_mudirac_database.dat";
    REQUIRE_NOTHROW(db.writeToFile(outputFile, false));

    // Compare content of both files
    REQUIRE(compareFiles(dbFile, dbFile));
}