onHigh = function(this)
    --print(Core.System:getHardwareByName(hName))
end

onLow = function(this)
    --Core.System:test()
    --Core.S.data["abc"]="asd"
    --this:getData():setInt("Test",this:getData():getAsInt("Test")+1)
    --print(_VERSION)
    --print(Core.System)
end

onChange = function(this)
    --print("onChange")
end

function changeDev(hardware, devString)
    dev = hardware:getDeviceByNameInScript(devString)
    if dev ~= nil then
        state = dev:getState()

        if state == "On" then
            dev:setState("Off")
        elseif state == "Off" then
            dev:setState("On")
        elseif state == "High" then
            dev:setState("Low")
        elseif state == "Low" then
            dev:setState("High")
        end

    end
end
onRisingEdge = function(this)
    --print("onRisingEdge")
    --hName = this:getHardwareScriptName()
    --print("Amount "..Core.System:getHardwareAmount())
    --hardware = Core.System:getHardwareByName(hName)

    --if hardware~=nil then
        --changeDev(hardware,"MCP_20_7")
        --changeDev(hardware,"MCP_20_8")
    --end


end


onFallingEdge = function(this)
    --print("onFallingEdge")
end
