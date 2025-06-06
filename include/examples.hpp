#ifndef EXAMPLES_HPP
#define EXAMPLES_HPP

#include "sim.hpp"


namespace Examples {
    void run_example(int example_number);
    void create_dna_helix(SFSim::sim& s);
    void create_particle_system(SFSim::sim& s);
    void create_mandala(SFSim::sim& s);
}

#endif