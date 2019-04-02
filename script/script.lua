local tasklib = require("tasklib")

gui.settitle("测试脚本")
local mt1 = gui.addmsgtype("类型1")
local mt2 = gui.addmsgtype("类型2")
local mt3 = gui.addmsgtype("类型3")
local mt4 = gui.addmsgtype("类型4")
local mt5 = gui.addmsgtype("类型5")

task = tasklib:new()

function Fn1()
    for k, v in pairs(task.kv_table) do
        gui.setkv(k, v)
    end
end
gui.setfntitle(1,"查看KV表")
task:regist_fn_function(1, Fn1)

function Fn2()
    gui.toclipboard("hehe")
end
gui.setfntitle(2,"剪贴板")
task:regist_fn_function(2, Fn2)

function Fn3()
    gui.setkv("颜色1", "255,0,0", "255,0,0")
    gui.setkv("颜色2", "228,88,148", "228,88,148")
    gui.setkv("颜色3", "0,255,0", "0,255,0")
    gui.setkv("颜色4", ".....")
end
gui.setfntitle(3,"颜色")
task:regist_fn_function(3, Fn3)

function Fn4()
    gui.playmedia("E:\\tools\\LuaLabBinV23\\music\\冯提莫 - 佛系少女.mp3")
end
gui.setfntitle(4,"mp3")
task:regist_fn_function(4, Fn4)

function Fn5()
    gui.stopmedia()
end
gui.setfntitle(5,"stop mp3")
task:regist_fn_function(5, Fn5)

function taskA()
    while true do
        gui.push(mt1, "type1 -- " .. gui.getmstime())
        gui.push(mt2, "type2 -- " .. gui.getmstime())
        gui.push(mt3, "type3 -- " .. gui.getmstime())
        gui.push(mt4, "type4 -- " .. gui.getmstime())
        gui.push(mt5, "type5 -- " .. gui.getmstime())
        task:sleep(5)
    end
end

function taskB()
    while true do
        gui.push("timestamp:" .. gui.getmstime())
        task:sleep(1)
    end
end

task:add_task(taskA)
--task:add_task(taskB)
task:run_loop()
