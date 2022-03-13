#ifndef MANTA_BEHAVIOR_HPP
#define MANTA_BEHAVIOR_HPP

#include <string>

namespace Manta {
    class Actor;

    class Behavior {
    public:
        bool enabled = true;

        virtual void Start(Actor* owner);
        virtual void Update(Actor* owner);

        virtual std::string get_TypeId() = 0;

        bool get_IsNew();

    protected:
        bool is_new = true;
        bool last_enabled = true;
    };
}

#endif
