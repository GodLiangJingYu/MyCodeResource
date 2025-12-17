-- 数据传送机接口容错测试
function Shoot()

  -- Test 0: 正向测试 (Happy Path)
  -- 目的：证明链路是通的，且参数正确时软件能正常响应
    print("测试项0: 发送正确数据包 (验证链路正常)")
    -- 发送: 轻镖(1), 10枚
    write_msg(channels.shoot, protocols.shoot, {type=1, number=10})
    etimer.delay(1000)
    local res = ask("yesno", {title='提示', msg='请确认是否正常收到了飞镖信息（类型/数量更新）？\n(预期：是)', default=true})
    check(res, "发送正确数据包成功", "发送正确数据包失败")

  -- Test 1: 包头错误
    print("测试项1: 发送错误包头 (0x5511)")
    write_msg(channels.shoot, protocols.shoot, {header=0x5511})
    etimer.delay(1000)
    local res = ask("yesno",{title='提示', msg='请确认是否收到了飞镖信息输入\n(预期：否 - 应丢包)', default=false})
    check(not res, "包头错误有丢包处理，正确", "包头错误没有丢包处理，错误")

  -- Test 2: 数据标志错误
    print("测试项2: 发送错误数据标志 (0xAA)")
    write_msg(channels.shoot, protocols.shoot, {data=0xAA})
    etimer.delay(1000)
    local res = ask("yesno",{title='提示', msg='请确认是否收到了飞镖信息输入\n(预期：否 - 应丢包)', default=false})
    check(not res, "数据标志错误有丢包处理，正确", "数据标志错误没有丢包处理，错误")
    
  -- Test 3: 包尾错误
    print("测试项3: 发送错误包尾 (0x5511)")
    write_msg(channels.shoot, protocols.shoot, {tail=0x5511})
    etimer.delay(1000)
    local res = ask("yesno",{title='提示', msg='请确认是否收到了飞镖信息输入\n(预期：否 - 应丢包)', default=false})
    check(not res, "包尾错误有丢包处理，正确", "包尾错误没有丢包处理，错误")

  -- Test 4: 校验和错误
    print("测试项4: 发送错误校验和 (0)")
    write_msg(channels.shoot, protocols.shoot, {check=0})
    etimer.delay(1000)
    local res = ask("yesno",{title='提示', msg='请确认是否收到了飞镖信息输入\n(预期：否 - 应丢包)', default=false})
    check(not res, "校验和错误有丢包处理，正确", "校验和错误没有丢包处理，错误")

end

function entry()
    Shoot()
    exit()
end