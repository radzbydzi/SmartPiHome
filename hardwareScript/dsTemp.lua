pin = 17

function getTemperature(device)
    if Bus.OneWire.presence(pin)~=1 then
        matchROM(pin, device)
        Bus.OneWire.writeByte(pin,0x44)
        Bus.OneWire.presence(pin)
        matchROM(pin, device)
        Bus.OneWire.writeByte(pin,0xBE)

        data = {}

        for i=0,8 do
            data[i] = Bus.OneWire.readByte(pin)
        end
        t1 = data[0]
        t2 = data[1]
        v1 = {data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]}
        crc = Bus.OneWire.CRC8(v1)

        if crc == data[8] then
            tmp = bit32.bor(bit32.arshift(t2,-8),t1)/16
            temp1 = 0.0+tmp
            print("Crc "..crc)
            return temp1
        end
    end
    return nil
end

function sleep(s)
  local ntime = os.time() + s
  repeat until os.time() > ntime
end


function matchROM(pin, device)
    dev = string.sub(device, 4, string.len(device)-1-2)
    if string.len(dev)==16 then
        Bus.OneWire.writeByte(pin,0x55)
        for i=1,8 do
            byte = string.sub(device, string.len(device)-(2*i)-2, string.len(device)-(2*i)-1)
            Bus.OneWire.writeByte(pin, Bus.OneWire.hexStrToInt(byte))
        end
    end
end


create = function(this)
    dev = Core.Device()
    str = "DS_"..this:getAddress()
    dev:fillAsNewDevice(str, this:getId(),"??stC", "Temp", "data", "", 0)
    this:addDevice(dev)
end

init = function(this)

end


loop = function(this)
    device = this:getAddress()

    str = "DS_"..device
    --str = "DS_‭2401144A267FAA28‬"
    tempAvg = 0
    tempMeasured = false
    for i=0,5 do
        temp = getTemperature(device)
        if temp ~= nil then
            if tempAvg~=0 then
                tempAvg=(tempAvg+temp)/2
            else
                tempAvg=temp
            end
            tempMeasured = true
        end
    end

    if this:getDeviceByNameInScript(str)~= nil and tempMeasured==true then
        --print(tempAvg.."stC")
        this:getDeviceByNameInScript(str):setState(tempAvg.."stC")
        hName = "MCP_1"
        print(tempAvg)
        if tempAvg<=28 then
            hardware = Core.System:getHardwareByName(hName)

            if hardware~=nil then
                dev = hardware:getDeviceByNameInScript("MCP_20_8")
                if dev ~= nil then
                    dev:setState("Off")
                end
            end
        elseif tempAvg>=31 then
            hardware = Core.System:getHardwareByName(hName)

            if hardware~=nil then
                dev = hardware:getDeviceByNameInScript("MCP_20_8")
                if dev ~= nil then
                    dev:setState("On")
                end
            end
        end



    end
end
