#ifndef SILICA_SHADER_HPP
#define SILICA_SHADER_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace Silica {
    class Shader {
    public:
        // Similar to Unity's shader approach, our shaders can have variants as well
        // The purpose of this is to provide a better way to optimize / trim down on the shader complexity
        // You won't find this used too often unless we call for it!
        // NOTE: Keywords that aren't in here are implicit ones like VERT / VERTEX, and FRAG / FRAGMENT
        // TODO: Proper shader analysis
        std::string source, path;
        uint32_t handle = 0;

        static Shader* LoadCode(std::string code);
        static Shader* LoadFile(std::string path);

        void Reload();
        void ProcessSource(); // Analyzes the source, locates #version, and finds variants

        bool Compile();
        void Use();

        static void CreateEngineShaders();

        static Shader* error_shader;

    protected:
        int version = 330;
        int sorting_order = 2000; // Does nothing right now, maybe in the future!
        bool analyzed = false;
    };
}

#endif
