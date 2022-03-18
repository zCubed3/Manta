#ifndef MANTA_SHADER_HPP
#define MANTA_SHADER_HPP

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

#include <glm/mat4x4.hpp>

namespace Manta {
    class EngineContext;

    class Shader {
    public:
        std::string source, path;
        std::optional<uint32_t> handle;

        static Shader* LoadCode(std::string code);
        static Shader* LoadFile(std::string path);

        void Reload();
        void ProcessSource(); // Analyzes the source, locates #version

        bool Compile();
        uint32_t Use(EngineContext* context);

        std::optional<uint32_t> GetUniform(const std::string& name);

        void SetFloat(const std::string &name, float value);
        void SetMat4x4(const std::string &name, const glm::mat4x4 &matrix);
        void SetVec3(const std::string &name, const glm::vec3 &vec);
        void SetVec4(const std::string &name, const glm::vec4 &vec);

        static void CreateEngineShaders(EngineContext* engine);

    protected:
        int version = 330;
        bool analyzed = false;

        std::unordered_map<std::string, std::optional<uint32_t>> uniform_cache;

    };
}

#endif
