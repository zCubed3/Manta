#include "bifurcated_ostream.hpp"

namespace Manta {
    BifurcatedStream::BifurcatedStream(std::ostream &lhs, std::ostream &rhs) {
        this->lhs = &lhs;
        this->rhs = &rhs;
    }
}