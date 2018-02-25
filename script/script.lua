local tasklib = require("tasklib")

gui.settitle("测试脚本")
gui.addmsgtype("类型1")
gui.addmsgtype("类型2")
gui.addmsgtype("类型3")
gui.addmsgtype("类型4")
gui.addmsgtype("类型5")

task = tasklib:new()

function Fn1()
    for k, v in pairs(task.kv_table) do
        gui.setkv(k, v)
    end
end
gui.setfntitle(1,"查看KV表")
task:regist_fn_function(1, Fn1)

function taskA()
    local tag = 0
    while true do
        tag = tag + 1
        gui.push("type1 -- " .. gui.getmstime())
        gui.setkv("count", tag)
        task:spawn()
    end
end

task:add_task(taskA)
task:run_loop()
