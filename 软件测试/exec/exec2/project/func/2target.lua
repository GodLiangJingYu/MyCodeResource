-- 接收靶区信息功能测试
function Target()
  -- 遍历靶区 1 到 5 (需求文档定义: 1-内牛眼, 2-外牛眼, 3-三倍, 4-双倍, 5-单倍)
  for zone_id = 1, 5 do
    print(string.format("=== 开始测试靶区: %d ===", zone_id))
    
    -- 每个靶区连续发送 3 次，观察分数累加
    for i = 1, 3 do
      write_msg(channels.target, protocols.target, {zone = zone_id})
      -- 发送间隔 500ms，模拟真实投镖间隔
      etimer.delay(500)
    end
    
    -- 切换下一个区域前等待，方便观察界面变化
    etimer.delay(1000)
  end
  
  print("=== 靶区循环测试结束 ===")
end

function entry()
    Target()
    exit()
end