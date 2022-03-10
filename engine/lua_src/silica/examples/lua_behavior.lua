-- Example Lua Behavior
-- We provide a basic lua behavior metatable for you to implement some basic things within!
-- While in reality most of the stuff within your behavior is backed by C types, variables and such will be Lua only!

-- Remember, Lua copies data, meaning your lua behavior is a copy of the base with modifications!
-- First step is creating a base copy
ExampleLuaBehavior = behavior.new()

-- Then modify it accordingly
ExampleLuaBehavior.type_id = "example_behavior"

ExampleLuaBehavior.Start = function()
    -- Do something in start!
    print("Example start")
end

ExampleLuaBehavior.Update = function()
    -- Do something in update!
    print("Example update")
end

-- Then to finally register this into the engine's type registrar...
local result, err = behavior.register(ExampleLuaBehavior)

if result then
    print("Registered successfully")
else
    print("Failed to register\n" .. err) -- Print that we failed and why we failed!
end

-- From here on out this behavior will now be available to be attached to objects!