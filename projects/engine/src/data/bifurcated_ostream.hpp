#ifndef MANTA_BIFURSTREAM_HPP
#define MANTA_BIFURSTREAM_HPP

#include <iostream>

namespace Manta {
    class BifurcatedStream {
    public:
        BifurcatedStream(std::ostream& lhs, std::ostream& rhs);

        template<typename input_t>
        std::ostream& operator<< (input_t in) {
            *lhs << in;
            *rhs << in;

            return *lhs;
        }

    protected:
        std::ostream *lhs, *rhs;
    };
}

#endif
