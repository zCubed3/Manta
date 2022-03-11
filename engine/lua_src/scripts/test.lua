-- TODO: Phase me out and replace me with behaviors!
TestBehavior = {
    update = function()
        local act = world.get_actor("test")

        if act ~= nil then
            local fac = timing.delta_time() * 20
            act.euler = (act.euler + vector3.new(fac, fac, fac))
        end
    end
}