#pragma once

#include <list>

#include "barretenberg/vm2/simulation/events/events_container.hpp"
#include "barretenberg/vm2/simulation/lib/avm_inputs.hpp"

namespace bb::avm2 {

class AvmSimulationHelper {
  public:
    AvmSimulationHelper(simulation::AvmProvingInputs inputs)
        : inputs(std::move(inputs))
    {}

    // Full simulation with event collection.
    simulation::EventsContainer simulate();

    // Fast simulation without event collection.
    void simulate_fast();

  private:
    template <typename S> simulation::EventsContainer simulate_with_settings();

    simulation::AvmProvingInputs inputs;
};

} // namespace bb::avm2