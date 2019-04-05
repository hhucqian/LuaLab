#include "lzlgcanlib.h"

#include "ControlCAN.h"
#include <QDebug>

#define REC_BUFF_SIZE (50)

#define CAN_OK 1
#define CAN_ERR 0

VCI_CAN_OBJ recvBuf[REC_BUFF_SIZE];
VCI_CAN_OBJ sendBuf;

int BaudRateTimingTable[][3] = {
    {5, 0xBF, 0xFF},
    {10, 0x31, 0x1C},
    {20, 0x18, 0x1C},
    {40, 0x87, 0xFF},
    {50, 0x09, 0x1C},
    {80, 0x83, 0xFF},
    {100, 0x04, 0x1C},
    {125, 0x03, 0x1C},
    {200, 0x81, 0xFA},
    {250, 0x01, 0x1C},
    {400, 0x80, 0xFA},
    {500, 0x00, 0x1C},
    {666, 0x80, 0xB6},
    {800, 0x00, 0x16},
    {1000, 0x00, 0x14}};

int BoudForSetReferenceTable[][2] = {
    {1000, 0x060003},
    {800, 0x060004},
    {500, 0x060007},
    {250, 0x1C0008},
    {125, 0x1C0011},
    {100, 0x160023},
    {50, 0x1C002C},
    {20, 0x1600B3},
    {10, 0x1C00E0},
    {5, 0x1C01C1}};

int baudRateTiming(int baud, int j)
{
    int i, n, res;
    res = 0;
    n = sizeof(BaudRateTimingTable) / (sizeof(int) * 3);
    for (i = 0; i < n; i++)
    {
        if (BaudRateTimingTable[i][0] == baud)
        {
            res = BaudRateTimingTable[i][j];
            break;
        }
    }
    return res;
}

int baudRateTiming0(int baud)
{
    return baudRateTiming(baud, 1);
}

int baudRateTiming1(int baud)
{
    return baudRateTiming(baud, 2);
}

int needSetBaudBySetReference(int type)
{
    int res = FALSE;

    if (type == VCI_PCI5010U || type == VCI_PCI5020U || type == VCI_USBCAN_E_U || type == VCI_USBCAN_2E_U)
    {
        res = TRUE;
    }
    return res;
}

int getBoudForSetReference(int baud)
{
    int i, n, res;
    res = 0;
    n = sizeof(BoudForSetReferenceTable) / (sizeof(int) * 3);
    for (i = 0; i < n; i++)
    {
        if (BoudForSetReferenceTable[i][0] == baud)
        {
            res = BoudForSetReferenceTable[i][1];
            break;
        }
    }
    return res;
}

static int can_open_device(lua_State *L)
{
    int dev_type, dev_index;
    VCI_ERR_INFO errinfo;

    dev_type = luaL_checkinteger(L, 1);
    dev_index = luaL_checkinteger(L, 2);

    if (STATUS_ERR == VCI_OpenDevice(dev_type, dev_index, 0))
    {
        lua_pushinteger(L, CAN_ERR);
        VCI_ReadErrInfo(dev_type, dev_index, -1, &errinfo);
        lua_pushinteger(L, errinfo.ErrCode);
        return 2;
    }

    lua_pushinteger(L, CAN_OK);
    return 1;
}

static int can_close_device(lua_State *L)
{
    int dev_type, dev_index;
    VCI_ERR_INFO errinfo;

    dev_type = luaL_checkinteger(L, 1);
    dev_index = luaL_checkinteger(L, 2);

    if (STATUS_ERR == VCI_CloseDevice(dev_type, dev_index))
    {
        lua_pushinteger(L, CAN_ERR);
        VCI_ReadErrInfo(dev_type, dev_index, -1, &errinfo);
        lua_pushinteger(L, errinfo.ErrCode);
        return 2;
    }

    lua_pushinteger(L, CAN_OK);
    return 1;
}

static int can_init_can(lua_State *L)
{
    int dev_type, dev_index, can_index, baud;
    DWORD baud2;
    VCI_INIT_CONFIG initConfig;
    VCI_ERR_INFO errinfo;

    dev_type = luaL_checkinteger(L, 1);
    dev_index = luaL_checkinteger(L, 2);
    can_index = luaL_checkinteger(L, 3);
    baud = luaL_checkinteger(L, 4);

    initConfig.AccCode = 0x00000000;
    initConfig.AccMask = 0xFFFFFFFF;
    initConfig.Reserved = 0;
    initConfig.Filter = 1;
    initConfig.Timing0 = baudRateTiming0(baud);
    initConfig.Timing1 = baudRateTiming1(baud);
    initConfig.Mode = 0;

    if (needSetBaudBySetReference(dev_type))
    {
        baud2 = getBoudForSetReference(baud);
        if (STATUS_ERR == VCI_SetReference(dev_type, dev_index, can_index, 0, &baud2))
        {
            lua_pushinteger(L, CAN_ERR);
            VCI_ReadErrInfo(dev_type, dev_index, can_index, &errinfo);
            lua_pushinteger(L, errinfo.ErrCode);
            return 2;
        }
    }
    if (STATUS_ERR == VCI_InitCAN(dev_type, dev_index, can_index, &initConfig))
    {
        lua_pushinteger(L, CAN_ERR);
        VCI_ReadErrInfo(dev_type, dev_index, can_index, &errinfo);
        lua_pushinteger(L, errinfo.ErrCode);
        return 2;
    }

    lua_pushinteger(L, CAN_OK);
    return 1;
}

static int can_start_can(lua_State *L)
{

    int dev_type, dev_index, can_index;
    VCI_ERR_INFO errinfo;

    dev_type = luaL_checkinteger(L, 1);
    dev_index = luaL_checkinteger(L, 2);
    can_index = luaL_checkinteger(L, 3);

    if (STATUS_ERR == VCI_StartCAN(dev_type, dev_index, can_index))
    {
        lua_pushinteger(L, CAN_ERR);
        VCI_ReadErrInfo(dev_type, dev_index, can_index, &errinfo);
        lua_pushinteger(L, errinfo.ErrCode);
        return 2;
    }

    lua_pushinteger(L, CAN_OK);
    return 1;
}

static int can_open(lua_State *L)
{
    int dev_type, dev_index, can_index, baud;
    DWORD baud2;
    VCI_INIT_CONFIG initConfig;
    VCI_ERR_INFO errinfo;

    dev_type = luaL_checkinteger(L, 1);
    dev_index = luaL_checkinteger(L, 2);
    can_index = luaL_checkinteger(L, 3);
    baud = luaL_checkinteger(L, 4);

    initConfig.AccCode = 0x00000000;
    initConfig.AccMask = 0xFFFFFFFF;
    initConfig.Reserved = 0;
    initConfig.Filter = 1;
    initConfig.Timing0 = baudRateTiming0(baud);
    initConfig.Timing1 = baudRateTiming1(baud);
    initConfig.Mode = 0;

    if (STATUS_ERR == VCI_OpenDevice(dev_type, dev_index, 0))
    {
        lua_pushinteger(L, CAN_ERR);
        VCI_ReadErrInfo(dev_type, dev_index, -1, &errinfo);
        lua_pushinteger(L, errinfo.ErrCode);
        return 2;
    }

    if (needSetBaudBySetReference(dev_type))
    {
        baud2 = getBoudForSetReference(baud);
        if (STATUS_ERR == VCI_SetReference(dev_type, dev_index, can_index, 0, &baud2))
        {
            lua_pushinteger(L, CAN_ERR);
            VCI_ReadErrInfo(dev_type, dev_index, can_index, &errinfo);
            lua_pushinteger(L, errinfo.ErrCode);
            return 2;
        }
    }
    if (STATUS_ERR == VCI_InitCAN(dev_type, dev_index, can_index, &initConfig))
    {
        lua_pushinteger(L, CAN_ERR);
        VCI_ReadErrInfo(dev_type, dev_index, can_index, &errinfo);
        lua_pushinteger(L, errinfo.ErrCode);
        return 2;
    }
    if (STATUS_ERR == VCI_StartCAN(dev_type, dev_index, can_index))
    {
        lua_pushinteger(L, CAN_ERR);
        VCI_ReadErrInfo(dev_type, dev_index, can_index, &errinfo);
        lua_pushinteger(L, errinfo.ErrCode);
        return 2;
    }

    lua_pushinteger(L, CAN_OK);
    return 1;
}

static int can_read(lua_State *L)
{
    int dev_type, dev_index, can_index;
    int j = 0;
    ULONG i = 0;
    ULONG len = 0;
    VCI_ERR_INFO errinfo;

    dev_type = luaL_checkinteger(L, 1);
    dev_index = luaL_checkinteger(L, 2);
    can_index = luaL_checkinteger(L, 3);

    len = VCI_GetReceiveNum(dev_type, dev_index, can_index);
    if (len == 0)
    {
        lua_createtable(L, len, 0);
        return 1;
    }

    len = VCI_Receive(dev_type, dev_index, can_index, recvBuf, REC_BUFF_SIZE, 200);
    if (len == 0xFFFFFFFF)
    {
        VCI_ReadErrInfo(dev_type, dev_index, can_index, &errinfo);
        lua_createtable(L, 0, 0);
        lua_pushinteger(L, errinfo.ErrCode);
        return 2;
    }

    lua_createtable(L, len, 0);
    for (i = 0; i < len; i++)
    {
        lua_createtable(L, 0, 7);

        lua_pushinteger(L, recvBuf[i].ID);
        lua_setfield(L, -2, "id");

        lua_pushinteger(L, recvBuf[i].TimeStamp);
        lua_setfield(L, -2, "timestamp");

        lua_pushinteger(L, recvBuf[i].TimeFlag);
        lua_setfield(L, -2, "timeflag");

        lua_pushinteger(L, recvBuf[i].RemoteFlag);
        lua_setfield(L, -2, "remoteflag");

        lua_pushinteger(L, recvBuf[i].ExternFlag);
        lua_setfield(L, -2, "externflag");

        lua_pushinteger(L, recvBuf[i].DataLen);
        lua_setfield(L, -2, "datalen");

        lua_createtable(L, recvBuf[i].DataLen, 0);
        for (j = 0; j < recvBuf[i].DataLen; j++)
        {
            lua_pushinteger(L, recvBuf[i].Data[j]);
            lua_rawseti(L, -2, j + 1);
        }
        lua_setfield(L, -2, "data");

        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

static int can_write(lua_State *L)
{
    int dev_type, dev_index, can_index;
    int i;
    ULONG res;

    dev_type = luaL_checkinteger(L, 1);
    dev_index = luaL_checkinteger(L, 2);
    can_index = luaL_checkinteger(L, 3);

    lua_getfield(L, 4, "sendtype");
    sendBuf.SendType = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 4, "remoteflag");
    sendBuf.RemoteFlag = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 4, "id");
    sendBuf.ID = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 4, "externflag");
    sendBuf.ExternFlag = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 4, "datalen");
    sendBuf.DataLen = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 4, "data");
    for (i = 0; i < sendBuf.DataLen; i++)
    {
        lua_rawgeti(L, -1, i + 1);
        sendBuf.Data[i] = luaL_checkinteger(L, -1);
        lua_pop(L, 1);
    }

    res = VCI_Transmit(dev_type, dev_index, can_index, &sendBuf, 1);

    lua_pushinteger(L, CAN_OK);
    lua_pushinteger(L, res);
    return 2;
}

static int can_error(lua_State *L)
{
    int dev_type, dev_index, can_index;
    VCI_ERR_INFO errinfo;

    dev_type = luaL_checkinteger(L, 1);
    dev_index = luaL_checkinteger(L, 2);
    can_index = luaL_checkinteger(L, 3);

    VCI_ReadErrInfo(dev_type, dev_index, can_index, &errinfo);
    lua_pushinteger(L, errinfo.ErrCode);
    return 1;
}

static const luaL_Reg canlib[] = {
    {"open", can_open},
    {"openDevice", can_open_device},
    {"closeDevice", can_close_device},
    {"initCAN", can_init_can},
    {"startCAN", can_start_can},
    {"read", can_read},
    {"write", can_write},
    {"error", can_error},
    {NULL, NULL}};

/*
** Open can library
*/
int luaopen_zlgcan(lua_State *L)
{
    luaL_newlib(L, canlib);
    return 1;
}
