#ifndef MANTA_COMMON_MMDL_HPP
#define MANTA_COMMON_MMDL_HPP

#include <cstdint>
#include <vector>

#define MMDL_MAKE_IDENT(A, B, C, D) (A | (B >> 8) | (C >> 16) || (D >> 24))

namespace Manta {
    class MMDL {
    public:
        const uint32_t MMDL_IDENT = MMDL_MAKE_IDENT('M', 'M', 'D', 'L');

        struct Header {
            uint32_t ident = 0;
            uint16_t version_number = 0; // 0 = Unknown version
            // If no version is recognized then we quit, otherwise we read the next bytes which should be a descriptor for that version!
        };

        struct BaseDescriptor {

        };

        struct DescriptorV1 : public BaseDescriptor {
            uint8_t channel_mask; // By default you'd set the first 3 channels on and use FLOAT3, FLOAT3, and FLOAT2!
            uint8_t channel_types[8]; // Describes the datatype of a channel
            uint8_t channel_semantics[8]; // Describes the usage of a channel

            enum class ChannelType : uint8_t {
                EMPTY,
                SCALAR,
                FLOAT2,
                FLOAT3,
                FLOAT4
            };

            // The author can provide semantic hints as well for channels
            enum class ChannelSemantic : uint8_t {
                NONE,

                //
                // Common mesh data
                //
                VERTEX,
                NORMAL,
                TANGENT,
                UV0,
                UV1,
                UV2,

                //
                // Compressed directional data
                //
                SMALL_NORMAL,
                SMALL_TANGENT
            };

            DescriptorV1(const std::vector<ChannelType>& channels, const std::vector<ChannelSemantic>& hints = {});
        };
    };
}

#endif
