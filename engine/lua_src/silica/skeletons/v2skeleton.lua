-- Vector 2 skeleton, DO NOT INCLUDE ME
-- I only exist to provide documentation for silica's vector2 submodule!
-- Any behavior here is purely for type hinting!

vector2 = {
    new = function(x, y) return {} end,

    Vector2 = {
        x = 0.0,
        y = 0.0,

        __index = Vector2
    }
}