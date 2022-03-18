#ifndef MANTA_CONSOLE_HPP
#define MANTA_CONSOLE_HPP

#include <vector>
#include <string>
#include <unordered_map>

namespace Manta {
    class EngineContext;
}

namespace Manta::Console {
    class CElem {
    public:
        virtual void Execute(EngineContext* context);
    };

    class Console {
    public:
        void RegisterCommand(const std::string& key, CElem* elem);
        void DoCommandLine(const std::vector<std::string>& argv);
        void DoString(const std::string& cmd);

    protected:
        std::unordered_map<std::string, CElem*> elements;
    };
}

#endif
