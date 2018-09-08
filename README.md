# 常用数字转换

`gui.getS16(a, b)` 有符号16位整数

`gui.getU16(a, b)` 无符号16位整数

`gui.getS32(a, b, c, d)` 有符号32位整数

`gui.getU32(a, b, c, d)` 无符号32位整数

**注意**：参数顺序从低位到高位，例如：`gui.getU16(0, 1) == 256`

`gui.transS32` 有符号32位整数

`gui.transS16` 有符号16位整数

`gui.transU16` 无符号16位整数

`gui.transF32` 32位浮点数

**注意**：返回一个数组，顺序从低位到高位，例如：`gui.transS32(0x01020304)[4] == 1`

# 界面信息操作

`gui.settitle("标题")` 设置标题

`gui.setfntitle(n, "名字")` 设置Fn按钮文字

`gui.addmsgtype("类别名字")` 设置消息类别名，返回类别ID

`gui.push(type_id, "消息")` 显示一个消息，`type_id`为`gui.addmsgtype`返回的ID，不填默认为添加的第一个消息类别

`gui.setkv(key, value, color)` 设置键值对信息 `key` 键名， `vaule` 值，`color` 可选信息，表示背景色，格式为 `"r,g,b"` 或者 `"r,g,b,a"`，例如 `"255,0,0"` 表示红色，`"255,0,0,50"` 表示浅红色。

# 系统相关操作

`gui.getmstime` 获取一个系统时间戳，精确到毫秒

`gui.sleep` `gui.msleep` `gui.usleep` 硬延时，**注意：一般不使用，因为会挂起整个脚本的执行**

`gui.toclipboard(txt)` 将字符串`txt`复制到系统剪贴板

# task库使用

`tasklib:new()` 新建一个task

`tasklib:add_task(fun)` 添加一个task

`tasklib:delete_task(index)` 删除一个task

`tasklib:spawn(ms)` 释放ms毫秒控制权

`tasklib:sleep(ms)` 释放ms毫秒控制权

`tasklib:regist_fn_function(index, fn)` 注册功能按钮事件回调函数

`tasklib:regist_start_fn(fn)` 注册task初始化回调函数

`tasklib:regist_stop_fn(fn)` 注册task结束回调函数

`tasklib:pause(val)` 暂停或者继续执行task：`tasklib:pause(1)`暂停task，`tasklib:pause(0)`继续执行task

# zlgcanlib库使用

## dev结构体

```lua
local dev1 = {
    devType = canlib.devtype.VCI_USBCAN2,
    devIndex = 0,
    canIndex = 0,
    baud = 250
}
```
## pkg结构体
```lua
local pkg = {
    id = 0x01,
    datalen = 8,
    externflag = canlib.externflag.EXT,
    data = {0, 0, 0, 0, 0, 0, 0, 0},
}
```
**注意：data的索引从1开始**

`canlib.open(dev)` 打开一路CAN，内部调用了`openDevice` `initCAN` `startCAN`

`canlib.openDevice(dev)` 初始化设备

`canlib.closeDevice(dev)` 关闭设备

`canlib.initCAN(dev)` 初始化一路CAN

`canlib.startCAN(dev)` 打开一路CAN

`canlib.read(dev)` 收CAN

`canlib.write(dev, pkg)` 发CAN

`canlib.pkg_data_to_str(pkg)` 将一个CAN包转换成字符串

`canlib.error(dev)` 查询一路CAN的错误码

`canlib.errmsg(value)` 查询错误码对应的错误信息