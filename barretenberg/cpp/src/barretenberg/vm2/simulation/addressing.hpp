#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include "barretenberg/vm2/simulation/events/addressing_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class AddressingBase {
  public:
    virtual ~AddressingBase() = default;
    // We need this method to be non-templated so that we can mock it.
    virtual std::vector<uint32_t> resolve_(uint16_t indirect,
                                           const std::vector<uint32_t>& offsets,
                                           MemoryInterface& memory) const = 0;

    // Convenience function that returns an array so that it can be destructured.
    template <size_t N>
    std::array<uint32_t, N> resolve(uint16_t indirect,
                                    const std::array<uint32_t, N>& offsets,
                                    MemoryInterface& memory) const
    {
        assert(offsets.size() == N);
        auto resolved = resolve_(indirect, std::vector(offsets.begin(), offsets.end()), memory);
        std::array<uint32_t, N> result;
        std::copy(resolved.begin(), resolved.end(), result.begin());
        return result;
    }
};

class Addressing final : public AddressingBase {
  public:
    Addressing(EventEmitterInterface<AddressingEvent>& event_emitter)
        : events(event_emitter)
    {}

    std::vector<uint32_t> resolve_(uint16_t indirect,
                                   const std::vector<uint32_t>& offsets,
                                   [[maybe_unused]] MemoryInterface& memory) const override
    {
        // TODO: Doesn't do anything right now, but it could access memory.
        events.emit({ .indirect = indirect, .operands = offsets, .resolved_operands = offsets });
        return offsets;
    }

  private:
    EventEmitterInterface<AddressingEvent>& events;
};

} // namespace bb::avm::simulation