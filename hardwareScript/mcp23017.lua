IODIRA  = 0x00
IODIRB  = 0x01
IPOLA  = 0x02
IPOLB  = 0x03
GPINTENA  = 0x04
GPINTENB =  0x05
DEFVALA  = 0x06
DEFVALB  = 0x07
INTCONA  = 0x08
INTCONB  = 0x09
IOCONA  = 0x0A
IOCONB  = 0x0B
GPPUA  = 0x0C
GPPUB  = 0x0D
INTFA  = 0x0E
INTFB  = 0x0F
INTCAPA  = 0x10
INTCAPB  = 0x11
GPIOA  = 0x12
GPIOB  = 0x13
OLATA  = 0x14
OLATB  = 0x15

create = function(this)
    --create all Ports of mcp
    for i=1,16 do
       dev = Core.Device()
       str = "MCP_"..this:getAddress().."_"..i
       dev:fillAsNewDevice(str, this:getId(),"Off", "Port no ", "sw", "", 0)
       this:addDevice(dev)
    end
end

init = function(this)
    addr = Bus.I2C.hexStrToInt(this:getAddress())

    --set IOs
    print("IODIRA "..this:getData():getHexAsUChar("IODIRA"))
    Bus.I2C.writeByteToRegister(addr, IODIRA, this:getData():getHexAsUChar("IODIRA"))
    Bus.I2C.writeByteToRegister(addr, IODIRB, this:getData():getHexAsUChar("IODIRB"))

    --set polarity
    Bus.I2C.writeByteToRegister(addr, IPOLA, this:getData():getHexAsUChar("IPOLA"))
    Bus.I2C.writeByteToRegister(addr, IPOLB, this:getData():getHexAsUChar("IPOLB"))

    --set pullups
    Bus.I2C.writeByteToRegister(addr, GPPUA, this:getData():getHexAsUChar("GPPUA"))
    Bus.I2C.writeByteToRegister(addr, GPPUB, this:getData():getHexAsUChar("GPPUB"))

    --default gpio value
    Bus.I2C.writeByteToRegister(addr, GPIOA, this:getData():getHexAsUChar("GPIOA"))
    Bus.I2C.writeByteToRegister(addr, GPIOB, this:getData():getHexAsUChar("GPIOB"))

    this:getData():setUInt("portA_prev", 0x00)
    this:getData():setUInt("portA_curr", 0x00)
    this:getData():setUInt("portB_prev", 0x00)
    this:getData():setUInt("portB_curr", 0x00)

    --check if IODIR coresponds correctly to Devices states, IODIR state is the correct one, default Output is conntact, default Input is switch
    --operation is crucial cause when we change .in file, we will have unapriopriate type of device

    for pin=1,16 do
        str = "MCP_"..this:getAddress().."_"..pin
        dev = this:getDeviceByNameInScript(str)
        if dev ~= nil then
            if pin>0 and pin<=8 then
                gpio = this:getData():getHexAsUChar("IODIRA")
            elseif pin>8 and pin<=16 then
                gpio = this:getData():getHexAsUChar("IODIRB")
                pin=pin-8 -- cause we have 2x8 and for is from 1 to 16
            end

            mask = bit32.arshift(0x01,-(pin-1))
            gpio = bit32.band(gpio, mask)
            devType = dev:getType()
            if gpio==0 and (devType=="sw" or devType=="btn") then
                dev:setType("con")
            elseif gpio~=0 and devType=="con" then
                dev:setType("sw")
            end
            devType = dev:getType()
        end
    end


end

function checkDeviceChange(devState, maskedPort)
    if maskedPort~= 0 then
        if devState=="Off" or devState=="Low" then
            return true --change occured
        end
    else
        if devState=="On" or devState=="High" then
            return true --change occured
        end
    end

    return false --change didnt occur
end

function changeDevicesStates(dev, current, previous, port)
    pC = dev:getData():getAsUInt(current)
    pP = dev:getData():getAsUInt(previous)
    a = 1
    b = 8
    if port==GPIOB then
        a=9
        b=16
    end
    mask = 0x01
    --name
    name_part = "MCP_"..dev:getAddress().."_"
    for i=a,b do
        d = dev:getDeviceByNameInScript(name_part..i)
        if d~=nil then
            st = d:getState()
            if bit32.band(pC,mask)~= bit32.band(pP,mask) or checkDeviceChange(st,bit32.band(pC,mask)) then --each bit in number
                 devtype = d:getType()
                 if devtype=="sw" or devtype=="btn" then -- if difference is caused by external physical devices; change device object parameters
                    if bit32.band(pC,mask) == 0 then
                        d:setState("Off")
                    else
                        d:setState("On")
                    end
                 elseif devtype=="con" then -- if caused by device object change; save change to pC, after it wrtie to mcp
                    if st=="On" or st=="High" then
                        pC = bit32.bor(pC,mask)
                    elseif st=="Off" or st=="Low" then
                        pC = bit32.band(pC, bit32.bnot(mask))
                    end

                end

            end
        end

        mask = bit32.arshift(mask,-1)
    end
    if pC~=dev:getData():getAsUInt(current) then
        addr = Bus.I2C.hexStrToInt(dev:getAddress())
        Bus.I2C.writeByteToRegister(addr, port, pC)
    end
end

loop = function(this)
    --set previous current as previous
    this:getData():setUInt("portA_prev", this:getData():getAsUInt("portA_curr"))
    this:getData():setUInt("portB_prev", this:getData():getAsUInt("portB_curr"))



    addr = Bus.I2C.hexStrToInt(this:getAddress())
    --get state of ports from mcp
    this:getData():setUInt("portA_curr", Bus.I2C.readByteFromRegister(addr, GPIOA))

    this:getData():setUInt("portB_curr", Bus.I2C.readByteFromRegister(addr, GPIOB))

    --get devices of port A
    changeDevicesStates(this, "portA_curr", "portA_prev", GPIOA)

    changeDevicesStates(this, "portB_curr", "portB_prev", GPIOB)


    --print("asd")
    --print(this:getData():getAsString("mes"))

end
