-- 飞镖信息输入功能测试
function Shoot()
  -- 检查是否存在外部导入的测试数据 (data.yml)
  if test_data and test_data.shoot then
    -- 模式1：数据驱动测试
    for _index, value in ipairs(test_data.shoot) do
      local type_val = value.type
      local num_val = value.number
      
      print(string.format(">>> 执行YAML用例 %d: 飞镖类型=%d, 数量=%d", _index, type_val, num_val))
      
      -- 发送协议数据
      write_msg(channels.shoot, protocols.shoot, {type = type_val, number = num_val})
      
      -- 延时 1000ms 以便观察软件反应 (如倒计时是否更新)
      etimer.delay(1000)
    end
  else
    -- 模式2：默认全覆盖测试 (当没有读取到数据时)
    print("警告：未读取到 data.yml 数据，执行内置默认逻辑")
    
    -- 场景1: 轻镖 (Type=1), 10枚 -> 预期时间 30秒
    print(">>> 默认场景1: 轻镖 (Type 1), 10枚 (预期: 30秒)")
    write_msg(channels.shoot, protocols.shoot, {type = 1, number = 10})
    etimer.delay(2000)

    -- 场景2: 重镖 (Type=2), 20枚 -> 预期时间 90秒
    print(">>> 默认场景2: 重镖 (Type 2), 20枚 (预期: 90秒)")
    write_msg(channels.shoot, protocols.shoot, {type = 2, number = 20})
    etimer.delay(2000)
    
    -- 场景3: 数量边界测试 (21枚) -> 预期截断为 20
    print(">>> 默认场景3: 边界测试 (21枚) (预期: 截断为20)")
    write_msg(channels.shoot, protocols.shoot, {type = 1, number = 21})
    etimer.delay(1000)
  end
end

function entry()
    Shoot()
    exit()
end