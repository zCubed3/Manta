-- Basic hello function
local state, err = pcall(function()
    print("LUA: Hello from Lua!")
    print(string.format("LUA: Version is %s", _VERSION))

    if SILICA_LUA_DEBUG ~= nil then
        print("LUA: Debug isn't nil, prepare for verbosity!")

        print("LUA DEBUG: Listing locations we are going to search for lua scripts in...")
        for str in string.gmatch(package.path, '([^;]+)') do
            print("  PATH: " .. str)
        end

        print("LUA DEBUG: Loaded modules...")
        for key, value in pairs(package.loaded) do
            print("  MOD: " .. key)
        end

        print("LUA DEBUG: Checking that silica types have loaded correctly...")

        print("LUA DEBUG: Checking vector2")
        local v2 = vector2.new(1.0, 0.0)

        print("LUA DEBUG: Testing __index!")

        print("X = " .. v2.x) -- Should print 1
        print("Y = " .. v2.y) -- Should print 0

        print("LUA DEBUG: Testing __newindex!")

        v2.x = 0.0
        print("X = " .. v2.x) -- Should print 0

        v2.y = 1.0
        print("Y = " .. v2.y) -- Should print 1

        print("LUA DEBUG: Testing __tostring!")
        print(v2) -- Should print { 0, 1 }

        -- After all this we should return to { 0, 1 }
        print("LUA DEBUG: Testing artihmetic, we should return to { 0, 1 } at the end!")

        v2 = v2 + 1.0 -- Should become { 1, 2 }
        print(v2)

        v2 = v2 - 1.0 -- Should become { 0, 1 }
        print(v2)

        v2 = v2 * 2.0 -- Should become { 0, 2 }
        print(v2)

        v2 = v2 / 2.0 -- Should become { 0, 1 }
        print(v2)

        print("LUA DEBUG: Checking vector3")
        local v3 = vector3.new(2.0, 1.0, 0.0)

        print("LUA DEBUG: Testing __index!")

        print("X = " .. v3.x) -- Should print 2
        print("Y = " .. v3.y) -- Should print 1
        print("Z = " .. v3.z) -- Should print 0

        print("LUA DEBUG: Testing __newindex!")

        v3.x = 0.0
        print("X = " .. v3.x) -- Should print 0

        v3.y = 1.0
        print("Y = " .. v3.y) -- Should print 1

        v3.z = 2.0
        print("Z = " .. v3.z) -- Should print 2

        print("LUA DEBUG: Testing __tostring!")
        print(v3) -- Should print { 0, 1 }

        -- After all this we should return to { 0, 1, 2 }
        print("LUA DEBUG: Testing artihmetic, we should return to { 0, 1, 2 } at the end!")

        v3 = v3 + 1.0 -- Should become { 1, 2, 3 }
        print(v3)

        v3 = v3 - 1.0 -- Should become { 0, 1, 2 }
        print(v3)

        v3 = v3 * 2.0 -- Should become { 0, 2, 4 }
        print(v3)

        v3 = v3 / 2.0 -- Should become { 0, 1, 2 }
        print(v3)

        print("LUA DEBUG: Testing dot product!")
        -- We set it to 0, 1, 0 for testing
        v3 = vector3.new(0.0, 1.0, 0.0)

        local v3r = vector3.new(0.0, 1.0, 0.0);
        print("DOT = " .. v3:dot(v3r)); -- Should be 1.0

        v3r = vector3.new(0.0, 0.0, 0.0);
        print("DOT = " .. v3:dot(v3r)); -- Should be 0.0

        v3r = vector3.new(0.0, -1.0, 0.0);
        print("DOT = " .. v3:dot(v3r)); -- Should be -1.0

        print("LUA DEBUG: Testing cross product!")
        v3r = vector3.new(1.0, 0.0, 0.0)
        print(tostring(v3) .. " X " .. tostring(v3r) .. " = ".. tostring(v3:cross(v3r)))

        print("LUA DEBUG: Testing length and normalization!")
        
        v3 = vector3.new(0.0, 2.0, 0.0)
        print("LEN = " .. v3:length())

        print("PRE NORM = " .. tostring(v3))
        v3 = v3:normalize()
        print("POST NORM = " .. tostring(v3))
    end
end)

if state == false then
    print("LUA: hello_lua() failed!")
    print("LUA ERROR:\n" .. err)
end
