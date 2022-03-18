#include "mmdl.hpp"

#include <fstream>
#include <cstring>

namespace Manta::Data::Meshes {
    const uint32_t MantaMDL::MMDL_IDENT = MAKE_32_IDENT("MMDL");

    uint32_t get_type_size(MantaMDL::ChannelType type) {
        switch (type) {
            case MantaMDL::ChannelType::SCALAR:
                return sizeof(float);

            case MantaMDL::ChannelType::UINT:
                return sizeof(uint32_t);

            case MantaMDL::ChannelType::VEC2:
                return sizeof(float) * 2;

            case MantaMDL::ChannelType::VEC3:
                return sizeof(float) * 3;

            case MantaMDL::ChannelType::VEC4:
                return sizeof(float) * 4;

            default:
                return 0;
        }
    }

    MantaMDL *MantaMDL::LoadFromStream(std::istream &stream) {
        auto mdl = new MantaMDL();

        Header header {};
        stream.read(reinterpret_cast<char*>(&header), sizeof(Header));

        mdl->name.resize(header.name_len);
        stream.read(mdl->name.data(), header.name_len);

        uint32_t c = 0;
        for (auto & channel : mdl->channels) {
            channel.type = header.channel_types[c];
            channel.hint = header.channel_hints[c];

            for (int d = 0; d < header.channel_lens[c]; d++) {
                auto size = get_type_size(channel.type);
                void* data = malloc(size);
                stream.read(reinterpret_cast<char*>(data), size);

                channel.data.push_back(data);
            }

            c++;
        }

        return mdl;
    }

    void MantaMDL::WriteToFile(const std::string &path) {
        std::ofstream file(path, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);

        Header header {};

        header.ident = MMDL_IDENT;

        for (int c = 0; c < MMDL_CHANNEL_COUNT; c++) {
            header.channel_types[c] = channels[c].type;
            header.channel_hints[c] = channels[c].hint;
            header.channel_lens[c] = channels[c].data.size();
        }

        header.name_len = name.length();

        file.write(reinterpret_cast<char*>(&header), sizeof(Header));
        file.write(name.data(), header.name_len);

        for (auto & channel : channels) {
            for (auto raw_element : channel.data) {
                uint32_t len = get_type_size(channel.type);

                file.write(reinterpret_cast<char*>(raw_element), len);
            }
        }

        file.close();
    }

    void MantaMDL::SetChannelType(uint8_t idx, ChannelType type) {
        channels[idx].type = type;
    }

    void MantaMDL::SetChannelHint(uint8_t idx, ChannelHint hint) {
        channels[idx].hint = hint;
    }

    void MantaMDL::SetChannelProps(uint8_t idx, ChannelType type, ChannelHint hint) {
        channels[idx].type = type;
        channels[idx].hint = hint;
    }

    void MantaMDL::ClearChannel(uint8_t idx) {
        for (auto element : channels[idx].data) {
            free(element);
        }

        channels[idx].data.clear();
    }

    void* MantaMDL::CloneData(void* data, size_t size) {
        void* dupe = malloc(size);
        memcpy(dupe, data, size);
        return dupe;
    }

    void MantaMDL::PushRawData(uint8_t idx, void *data) {
        channels[idx].data.push_back(data);
    }
}