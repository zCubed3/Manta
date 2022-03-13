#include "mmdl.hpp"

#include <stdexcept>

namespace Manta {
    MMDL::DescriptorV1::DescriptorV1(const std::vector<ChannelType>& channel_types, const std::vector<ChannelSemantic>& hints) {
        if (channel_types.size() > 8)
            throw std::runtime_error("Only 8 channel types are allowed in a V1 descriptor!");

        if (hints.size() > 8)
            throw std::runtime_error("Only 8 channel hints are allowed in a V1 descriptor!");

        channel_mask = 0;
        for (int t = 0; t < 8; t++) {
            if (t >= channel_types.size()) {
                this->channel_types[t] = static_cast<uint8_t>(ChannelType::EMPTY);
                continue;
            }

            this->channel_types[t] = static_cast<uint8_t>(channel_types[t]);
            channel_mask |= (1 << t);
        }

        for (int s = 0; s < 8; s++) {
            if (s >= hints.size()) {
                this->channel_types[s] = static_cast<uint8_t>(ChannelSemantic::NONE);
                continue;
            }

            this->channel_semantics[s] = static_cast<uint8_t>(hints[s]);
        }
    }
}