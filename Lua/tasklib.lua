local _task = {}

_task.mt = {}

function _task:new()
    local res = {}
    res.tasks = {}
    res.is_pause = false
    res.is_stop = false
    res.fn_functions = {}
    res.fn_start = nil
    res.fn_stop = nil
    res.kv_table = {}
    setmetatable(res, {__index = self.mt})
    return res
end

function _task.mt:add_task(fun)
    table.insert(self.tasks, coroutine.create(fun))
end

function _task.mt:delete_task(index)
    table.remove(self.tasks, index)
end

function _task.mt:spawn(ms)
    ms = ms or 0
    if ms > 0 then self:sleep(ms) else coroutine.yield() end
end

function _task.mt:sleep(ms)
    local ts = gui.getmstime()
    while gui.getmstime() < ts + ms do
        self:spawn()
    end
end

function _task.mt:regist_fn_function(index, fn)
    self.fn_functions[index] = fn
end

function _task.mt:regist_start_fn(fn)
    self.fn_start = fn
end

function _task.mt:regist_stop_fn(fn)
    self.fn_stop = fn
end

function _task.mt:pause(val)
    self.is_pause = val == 1
end

function _task.mt:stop(val)
    self.is_stop = true
end

function _task.mt:process_event()
    local type, etype, extra = gui.getevent()
    if type ~= nil then
        if type == gui.EVENT_FUN then
            if self.fn_functions[extra] then self.fn_functions[extra]() end
        elseif type == gui.EVENT_EXIT then
            self:stop(extra)
        elseif type == gui.EVENT_PAUSE then
            self:pause(extra)
        elseif type == gui.EVENT_SET_TEXT then
            self.kv_table = extra
        end
    end
end

function _task.mt:run_loop(delay)
    delay = tonumber(delay) or 0
    if delay < 2 then delay = 2 end
    if self.fn_start ~= nil then self.fn_start() end
    while true do
        self:process_event()
        if self.is_stop then break end
        for i, v in ipairs(self.tasks) do
            if self.is_pause == false then
                if coroutine.status(v) == "dead" then
                    self:delete_task(i)
                    break
                end
                err, msg = coroutine.resume(v)
                if err == false then error(msg) end
            end
        end
        if #self.tasks == 0 then break end
        if delay > 0 then gui.msleep(delay) end
    end
    if self.fn_stop ~= nil then self.fn_stop() end
end

return _task
