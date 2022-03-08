-- Vector 3 skeleton, DO NOT INCLUDE ME
-- I only exist to provide documentation for silica's vector3 submodule!
-- Any behavior here is purely for type hinting!

vector3 = {
    new = function(x, y, z) return setmetatable({}, vector3.Vector3) end,

    Vector3 = {
        x = 0.0,
        y = 0.0,
        z = 0.0,

        __index = Vector3,
    
        length = function(rhs) return 0.0 end,
        dot = function(rhs) return 0.0 end,
        normalize = function() return {} end,
        cross = function(rhs) return {} end,
    }
}