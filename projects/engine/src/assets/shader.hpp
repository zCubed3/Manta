#ifndef MANTA_SHADER_HPP
#define MANTA_SHADER_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace Manta {
    class Shader {
    public:
        std::string source, path;
        uint32_t handle = 0;

        static Shader* LoadCode(std::string code);
        static Shader* LoadFile(std::string path);

        void Reload();
        void ProcessSource(); // Analyzes the source, locates #version

        bool Compile();
        void Use();

        static void CreateEngineShaders();

        static Shader* error_shader;

    protected:
        int version = 330;
        bool analyzed = false;
    };
}

#endif
