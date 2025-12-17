-- 靶体接口容错测试
function Target()

  -- Test 0: 正向测试 (Happy Path)
    print("测试项0: 发送正确靶体数据 (Zone 1)")
    -- 发送: Zone 1 (内牛眼)
    write_msg(channels.target, protocols.target, {zone=1})
    etimer.delay(1000)
    local res = ask("yesno", {title='提示', msg='请确认界面是否显示击中"内牛眼"或分数增加？\n(预期：是)', default=true})
    check(res, "发送正确数据包成功", "发送正确数据包失败")

  -- Test 1: 校验和错误
    print("测试项1: 发送错误校验和 (0)")
    write_msg(channels.target, protocols.target, {check=0})
    etimer.delay(1000)
    local res = ask("yesno",{title='提示', msg='请确认是否收到了靶体信息\n(预期：否 - 应丢包)', default=false})
    check(not res, "校验和错误有丢包处理，正确", "校验和错误没有丢包处理，错误")

  -- Test 2: 包头错误
    print("测试项2: 发送错误包头 (0x5511)")
    write_msg(channels.target, protocols.target, {header=0x5511})
    etimer.delay(1000)
    local res = ask("yesno",{title='提示', msg='请确认是否收到了靶体信息\n(预期：否 - 应丢包)', default=false})
    check(not res, "包头错误有丢包处理，正确", "包头错误没有丢包处理，错误")

  -- Test 3: 数据标志1错误
    print("测试项3: 发送错误数据标志1 (0x55)")
    write_msg(channels.target, protocols.target, {data1=0x55})
    etimer.delay(1000)
    local res = ask("yesno",{title='提示', msg='请确认是否收到了靶体信息\n(预期：否 - 应丢包)', default=false})
    check(not res, "数据标志1错误有丢包处理，正确", "数据标志1错误没有丢包处理，错误")

  -- Test 4: 无效靶区号 (逻辑错误)
    print("测试项4: 发送无效靶区号 (9)")
    write_msg(channels.target, protocols.target, {zone=0x9})
    etimer.delay(1000)
    local res = ask("yesno",{title='提示', msg='请确认是否收到了靶体信息\n(预期：否 - 应丢包)', default=false})
    check(not res, "无效靶区号有丢包处理，正确", "无效靶区号没有丢包处理，错误")

  -- Test 5: 数据标志2错误
    print("测试项5: 发送错误数据标志2 (0x55)")
    write_msg(channels.target, protocols.target, {data2=0x55})
    etimer.delay(1000)
    local res = ask("yesno",{title='提示', msg='请确认是否收到了靶体信息\n(预期：否 - 应丢包)', default=false})
    check(not res, "数据标志2错误有丢包处理，正确", "数据标志2错误没有丢包处理，错误")

  -- Test 6: 包尾错误
    print("测试项6: 发送错误包尾 (0x5511)")
    write_msg(channels.target, protocols.target, {tail=0x5511})
    etimer.delay(1000)
    local res = ask("yesno",{title='提示', msg='请确认是否收到了靶体信息\n(预期：否 - 应丢包)', default=false})
    check(not res, "包尾错误有丢包处理，正确", "包尾错误没有丢包处理，错误")

end

function entry()
    Target()
    exit()
end