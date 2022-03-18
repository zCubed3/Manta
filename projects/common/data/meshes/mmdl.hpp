#ifndef MANTA_COMMON_MMDL_HPP
#define MANTA_COMMON_MMDL_HPP

#include "../ident.hpp"

#include <cstdint>
#include <vector>
#include <string>

namespace Manta::Data::Meshes {
    // V1 of MantaMDL format!
    // V2 and future versions might change a lot!
    class MantaMDL {
    public:
        #define MMDL_CHANNEL_COUNT 8
        static const uint32_t MMDL_IDENT;

        enum class ChannelType : uint8_t {
            NONE, // Special type, used by the writer mainly!
            SCALAR,
            VEC2,
            VEC3,
            VEC4,

            UINT32,

            BONE
        };

        enum class ChannelHint : uint8_t {
            NONE, // Nothing special in here!

            // Common data
            VERTEX,
            NORMAL,
            TANGENT,
            UV0,
            INDEXER, // AKA Indices

            // Extra texture channels
            UV1,
            UV2,

            // Compressed data
            SMALL_NORMAL,
            SMALL_TANGENT,

            // Bone data, redundant rn but V2 might add a purpose for it!
            SKELETON
        };

        struct MMDLHeader {
            uint32_t ident;
            ChannelType channel_types[MMDL_CHANNEL_COUNT]; // What type is contained in this channel?
            ChannelHint channel_hints[MMDL_CHANNEL_COUNT]; // What data is provided by this channel?
            uint32_t channel_lens[MMDL_CHANNEL_COUNT]; // How long are the blocks that represent this channel?
            uint16_t name_len;
        };

        struct MMDLChannel {
            ChannelType type;
            ChannelHint hint;
            std::vector<void*> data;
        };

        // Name is read, then channels are read
        // Channels are layered in series!

        struct Vec2 {
            float x, y;
        };

        struct Vec3 {
            float x, y, z;
        };

        struct Vec4 {
            float x, y, z, w;
        };

        struct Bone {
            Vec3 origin;
            Vec3 euler;
            Vec3 scale;
            uint32_t id;
            uint32_t connection; // If zero, this bone isn't parented!
        };

        std::string name;
        MMDLChannel channels[MMDL_CHANNEL_COUNT];

        static MantaMDL* LoadFromStream(std::istream& stream);

        void WriteToFile(const std::string& path);
    };
}

#endif
