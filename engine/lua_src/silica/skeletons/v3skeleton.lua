-- Vector 3 skeleton, DO NOT INCLUDE ME
-- I only exist to provide documentation for silica's vector3 submodule!
-- Any behavior here is purely for type hinting!

vector3 = {
    Vector3 = {
        x = 0.0,
        y = 0.0,
        z = 0.0,
        __index = Vector3
    }
}

function vector3.new(x, y, z)
    return setmetatable({}, vector2.Vector2)
end

function dummy()
    return setmetatable({}, vector2.Vector2)
end

vector3.Vector3.__add = dummy();