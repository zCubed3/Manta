#ifndef SILICA_SHADER_HPP
#define SILICA_SHADER_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace Silica {
    class ShaderProgram {
        uint32_t handle; // Does VK or DX use something else?
        std::string keywords;
    };

    class Shader {
    public:
        // Similar to Unity's shader approach, our shaders can have variants as well
        // The purpose of this is to provide a better way to optimize / trim down on the shader complexity
        // You won't find this used too often unless we call for it!
        // NOTE: Keywords that aren't in here are implicit ones like VERT / VERTEX, and FRAG / FRAGMENT
        // TODO: Shader analysis
        // TODO: Actually implement variants
        // TODO: #pragma or some other notation for variants!
        std::vector<std::string> shader_keywords; // Contains all found keywords
        std::string source, path;
        std::unordered_map<std::string, ShaderProgram*> compiled_variants;

        static Shader* LoadCode(std::string code);
        static Shader* LoadFile(std::string path);

        void Reload();
        void ProcessSource(); // Analyzes the source, locates #version, and finds variants

        // Rarely should you ever compile on demand, use GetVariant for safety!
        ShaderProgram* Compile(const std::vector<std::string>& keywords = {});
        ShaderProgram* GetVariant(const std::vector<std::string>& keywords);

    protected:
        int version = 330;
        bool analyzed = false;
    };
}

#endif
