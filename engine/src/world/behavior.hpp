#ifndef SILICA_BEHAVIOR_HPP
#define SILICA_BEHAVIOR_HPP

namespace Silica {
    class Actor;

    class Behavior {
    public:
        bool enabled = true;

        virtual void Start(Actor* owner);
        virtual void Update(Actor* owner);

        bool IsNew();

    protected:
        bool is_new = true;
        bool last_enabled = true;
    };
}

#endif
