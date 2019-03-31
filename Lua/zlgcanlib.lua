local can = require("zlgcan")
local canlib = {}

canlib.OK  = 1
canlib.ERR = 0

canlib.devtype = {
    VCI_PCI5121      = 1,
    VCI_PCI9810      = 2,
    VCI_USBCAN1      = 3,
    VCI_USBCAN2      = 4,
    VCI_USBCAN2A     = 4,
    VCI_PCI9820      = 5,
    VCI_CAN232       = 6,
    VCI_PCI5110      = 7,
    VCI_CANLITE      = 8,
    VCI_ISA9620      = 9,
    VCI_ISA5420      = 10,
    VCI_PC104CAN     = 11,
    VCI_CANETUDP     = 12,
    VCI_CANETE       = 12,
    VCI_DNP9810      = 13,
    VCI_PCI9840      = 14,
    VCI_PC104CAN2    = 15,
    VCI_PCI9820I     = 16,
    VCI_CANETTCP     = 17,
    VCI_PEC9920      = 18,
    VCI_PCIE_9220    = 18,
    VCI_PCI5010U     = 19,
    VCI_USBCAN_E_U   = 20,
    VCI_USBCAN_2E_U  = 21,
    VCI_PCI5020U     = 22,
    VCI_EG20T_CAN    = 23,
    VCI_PCIE9221     = 24
}

canlib.sendtype = {
    NORMAL           = 0,
    SINGLE_SEND      = 1,
    SEND_RECV        = 2,
    SINGLE_SEND_RECV = 3
}

canlib.externflag = {
    NORMAL    = 0,
    EXT       = 1
}

canlib.errinfo = {
    [0x0100] = {"0x0100", "设备已经打开"},
    [0x0200] = {"0x0200", "打开设备错误"},
    [0x0400] = {"0x0400", "设备没有打开"},
    [0x0800] = {"0x0800", "缓冲区溢出"},
    [0x1000] = {"0x1000", "此设备不存在"},
    [0x2000] = {"0x2000", "装载动态库失败"},
    [0x4000] = {"0x4000", "执行命令失败"},
    [0x8000] = {"0x8000", "内存不足"},
    [0x0001] = {"0x0001", "CAN控制器内部FIFO溢出"},
    [0x0002] = {"0x0002", "CAN控制器错误报警"},
    [0x0004] = {"0x0004", "CAN控制器消极错误"},
    [0x0008] = {"0x0008", "CAN控制器仲裁丢失"},
    [0x0010] = {"0x0010", "CAN控制器总线错误"},
}

function canlib.open(dev)
    local devtype = dev.devType or canlib.devtype.VCI_USBCAN_2E_U
    local devindex = dev.devIndex or 0
    local canindex = dev.canIndex or 0
    local baud = dev.baud or 250
    return can.open(devtype, devindex, canindex, baud)
end

function canlib.openDevice(dev)
    local devtype = dev.devType or canlib.devtype.VCI_USBCAN_2E_U
    local devindex = dev.devIndex or 0
    return can.openDevice(devtype, devindex)
end

function canlib.closeDevice(dev)
    local devtype = dev.devType or canlib.devtype.VCI_USBCAN_2E_U
    local devindex = dev.devIndex or 0
    return can.closeDevice(devtype, devindex)
end

function canlib.initCAN(dev)
    local devtype = dev.devType or canlib.devtype.VCI_USBCAN_2E_U
    local devindex = dev.devIndex or 0
    local canindex = dev.canIndex or 0
    local baud = dev.baud or 250
    return can.initCAN(devtype, devindex, canindex, baud)
end

function canlib.startCAN(dev)
    local devtype = dev.devType or canlib.devtype.VCI_USBCAN_2E_U
    local devindex = dev.devIndex or 0
    local canindex = dev.canIndex or 0
    local baud = dev.baud or 250
    return can.startCAN(devtype, devindex, canindex)
end

function canlib.read(dev)
    local devtype = dev.devType or canlib.devtype.VCI_USBCAN_2E_U
    local devindex = dev.devIndex or 0
    local canindex = dev.canIndex or 0
    return can.read(devtype, devindex, canindex);
end

function canlib.write(dev, data)
    local devtype = dev.devType or canlib.devtype.VCI_USBCAN_2E_U
    local devindex = dev.devIndex or 0
    local canindex = dev.canIndex or 0
    local _data = {}
    _data.id = data.id or 0
    _data.sendtype = data.sendtype or canlib.sendtype.NORMAL
    _data.remoteflag = data.remoteflag or 0
    _data.externflag = data.externflag or canlib.externflag.NORMAL
    _data.datalen = data.datalen or 0
    if _data.datalen >= 8 then _data.datalen = 8 end
    _data.data = {}
    for i = 1, _data.datalen do
        _data.data[i] = data.data[i] or 0
    end
    return can.write(devtype, devindex, canindex, _data)
end

function canlib.pkg_to_str(pkg)
    local tohex = canlib.tohex
    local normal_or_ext = "标准帧"
    if pkg.externflag == canlib.externflag.EXT then normal_or_ext = "扩展帧" end
    pkg.timestamp = pkg.timestamp or 1
    return string.format("timestamp:%d\tid:0x%s\tlen:%d\tdata:%s %s %s %s %s %s %s %s\t%s",
        pkg.timestamp, tohex(pkg.id, 8), pkg.datalen,
        tohex(pkg.data[1]), tohex(pkg.data[2]), tohex(pkg.data[3]), tohex(pkg.data[4]),
        tohex(pkg.data[5]), tohex(pkg.data[6]), tohex(pkg.data[7]), tohex(pkg.data[8]),
        normal_or_ext)
end

function canlib.pkg_data_to_str(pkg)
    local tohex = canlib.tohex
    return string.format("%s %s %s %s %s %s %s %s",
        tohex(pkg.data[1]), tohex(pkg.data[2]), tohex(pkg.data[3]), tohex(pkg.data[4]),
        tohex(pkg.data[5]), tohex(pkg.data[6]), tohex(pkg.data[7]), tohex(pkg.data[8]))
end

function canlib.tohex(value, n)
    n = n or 2
    if value == nil then return string.rep('-', n) end
    return string.format("%0" .. n .."X", value)
end

function canlib.tobin(value)
    value = value or 0
    local res = ""
    while value ~= 0 do
        res = res .. tostring(value & 0x1)
        value = value >> 1
    end
    return res
end

function canlib.tobin2(value, n)
    value = value or 0
    local res = ""
    while n ~= 0 do
        res = res .. tostring(value & 0x1)
        value = value >> 1
        n = n - 1
    end
    return res
end

function canlib.error(dev)
    local devtype = dev.devType or canlib.devtype.VCI_USBCAN_2E_U
    local devindex = dev.devIndex or 0
    local canindex = dev.canIndex or 0
        return can.error(devtype, devindex, canindex)
end

function canlib.errmsg(value)
    local res = ""
    for k, v in pairs(canlib.errinfo) do
        if ((value & k) == k) then res = res .. v[2] .. "\t" end
    end
    return res
end

return canlib
