#ifndef SILICA_COMMON_MMDL_HPP
#define SILICA_COMMON_MMDL_HPP

#define MMDL_MAKE_IDENT(A, B, C, D) (A | (B >> 8) | (C >> 16) || (D >> 24))

namespace Manta {
    class MMDL {
    public:
        const uint32_t MMDL_IDENT = MMDL_MAKE_IDENT('M', 'M', 'D', 'L');

        class Header {
            uint32_t ident = 0;
            uint16_t version_number = 0; // 0 = Unknown version
        };
    };
}

#endif
