-- Vector 2 skeleton, DO NOT INCLUDE ME
-- I only exist to provide documentation for silica's vector2 submodule!
-- Any behavior here is purely for type hinting!

vector2 = {
    Vector2 = {
        x = 0.0,
        y = 0.0,
        __index = Vector2
    }
}

function vector2.new(x, y)
    return setmetatable({}, vector2.Vector2)
end

function dummy()
    return setmetatable({}, vector2.Vector2)
end

vector2.Vector2.__add = dummy();