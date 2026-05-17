# 实验四（自动引导车辆系统，实时/并发）UML模型（Mermaid）

## 1. 用例图（flowchart 表示）
```mermaid
flowchart LR
Supervisory[监管系统]
Display[显示系统]
ArrivalSensor[到达传感器]
Clock[时钟]
Motor[发动机]
Arm[机械臂]

U1[移动到站点]
U2[发送车辆状态]

Supervisory --> U1
ArrivalSensor -. 次要参与者 .-> U1
Motor -. 设备反馈 .-> U1
Arm -. 设备反馈 .-> U1

Clock --> U2
Display -. 接收 .-> U2
```

## 2. 静态模型（关键类与接口）
```mermaid
classDiagram
direction LR

class VehicleControl {
  +handleEvent(e)
  +startMove(targetStation, mode)
  +onArrival(stationNo)
}

class VehicleStatus {
  +targetStation
  +mode
  +currentStation
  +busy
}

class VehicleTimer {
  +tick30s()
}

class ArrivalSensorInterface {
  +onArrival(stationNo)
}

class MotorInterface {
  +start()
  +stop()
  +onStarted()
  +onStopped()
}

class ArmInterface {
  +load()
  +unload()
  +onLoaded()
  +onUnloaded()
}

class DisplaySystemAdapter {
  +sendStatus(status)
}

class SupervisorySystemAdapter {
  +receiveMoveCommand(targetStation, mode)
  +sendAck(type, stationNo)
}

VehicleControl --> VehicleStatus
VehicleControl --> MotorInterface
VehicleControl --> ArmInterface
VehicleControl --> ArrivalSensorInterface
VehicleControl --> VehicleTimer
VehicleControl --> DisplaySystemAdapter
VehicleControl --> SupervisorySystemAdapter
```

## 3. 状态图：VehicleControl（简化）
```mermaid
stateDiagram
[*] --> Idle

Idle --> Starting: MoveToStation命令
Starting --> Moving: MotorStarted

Moving --> CheckingDestination: Arrival(stationNo)
CheckingDestination --> Moving: notTarget / Ack(Passed)
CheckingDestination --> StoppingToLoad: isTarget & mode=Load
CheckingDestination --> StoppingToUnload: isTarget & mode=Unload

StoppingToLoad --> Loading: MotorStopped
Loading --> Idle: ArmLoaded / Ack(Arrived)

StoppingToUnload --> Unloading: MotorStopped
Unloading --> Idle: ArmUnloaded / Ack(Arrived)
```

## 4. 时序图：移动到站点（典型场景）
```mermaid
sequenceDiagram
autonumber
participant SS as 监管系统适配器
participant VC as VehicleControl
participant MI as MotorInterface
participant AS as ArrivalSensorInterface
participant AI as ArmInterface

SS->>VC: MoveToStation(targetStation, mode)
VC->>MI: start()
MI-->>VC: onStarted()
VC-->>SS: Ack(Started)

AS-->>VC: onArrival(stationNo)
VC->>VC: checkDestination(stationNo)
alt 不是目标站点
  VC-->>SS: Ack(Passed, stationNo)
else 是目标站点
  VC->>MI: stop()
  MI-->>VC: onStopped()
  alt mode=Load
    VC->>AI: load()
    AI-->>VC: onLoaded()
    VC-->>SS: Ack(Arrived, stationNo)
  else mode=Unload
    VC->>AI: unload()
    AI-->>VC: onUnloaded()
    VC-->>SS: Ack(Arrived, stationNo)
  end
end
```

## 5. 并发任务与通信（任务接口/消息通道）
```mermaid
flowchart TB
CmdQ[命令队列\n来自监管系统]
EvtQ[事件队列\n到达/电机/机械臂/定时器]

TaskVC[任务: VehicleControl\n(状态机/E-C-A)]
TaskIO[任务: I/O适配器\n(电机/机械臂/到达传感器)]
TaskTimer[任务: VehicleTimer\n(30s tick)]
TaskSS[任务: SupervisoryAdapter]
TaskDS[任务: DisplayAdapter]

TaskSS --> CmdQ --> TaskVC
TaskIO --> EvtQ --> TaskVC
TaskTimer --> EvtQ

TaskVC --> TaskIO
TaskVC --> TaskSS
TaskVC --> TaskDS
```

## 6. 部署/通信视图（最简兼容画法）
```mermaid
flowchart LR
AGV[AGV车载控制器\n实时任务/状态机]
Sensors[到达传感器]
Actuators[发动机/机械臂]
WAN[工厂网络]
Supervisory[监管系统]
Display[显示系统]

Sensors --> AGV
Actuators <--> AGV
AGV <--> WAN
WAN <--> Supervisory
WAN <--> Display
```

