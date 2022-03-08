-- behavior skeleton, DO NOT INCLUDE ME
-- I only exist to provide documentation for the luabehavior module!

behavior = {
    Behavior = {
        type_id = "invalid",
        Start = function() end,
        Update = function() end
    }
}

function behavior.new()
    return setmetatable({}, behavior.Behavior)
end