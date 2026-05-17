# 实验一（银行系统，C/S）UML模型（Mermaid）

本文件使用 Mermaid 进行 UML 文本化建模：可直接编辑，并在支持 Mermaid 的 Markdown 预览中渲染。

## 1. 用例图
```mermaid
flowchart LR
Customer[ATM客户]
Operator[ATM操作员]
UC_ValidatePIN([验证PIN码])
UC_Withdraw([取款])
UC_Query([查询账户])
UC_Transfer([转账])
UC_AddCash([添加现金])
UC_Startup([启动ATM])
UC_Shutdown([关闭ATM])

Customer --> UC_Withdraw
Customer --> UC_Query
Customer --> UC_Transfer

UC_Withdraw -. "include" .-> UC_ValidatePIN
UC_Query -. "include" .-> UC_ValidatePIN
UC_Transfer -. "include" .-> UC_ValidatePIN

Operator --> UC_AddCash
Operator --> UC_Startup
Operator --> UC_Shutdown
```

## 2. 领域/设计类图（核心类）

```mermaid
classDiagram
direction LR

class BankSystem {
  +processTransaction(tx)
}

class ATM {
  +atmId
  +location
}

class BankServer {
  +validateCard(cardNo, expDate)
  +validatePin(cardNo, pin)
  +authorizeWithdraw(accountNo, amount)
  +authorizeTransfer(fromAccountNo, toAccountNo, amount)
  +queryBalance(accountNo)
  +postTransaction(tx)
}

class CardReader {
  +readCard() DebitCard
  +ejectCard()
  +retainCard()
}

class KeypadDisplay {
  +prompt(msg)
  +readInput() string
}

class CashDispenser {
  +hasCash(amount) bool
  +dispense(amount)
}

class ReceiptPrinter {
  +print(receipt)
}

class CustomerInteraction {
  +promptPin()
  +selectTransaction()
  +inputAmount()
  +selectAccount()
}

class OperatorInteraction {
  +startup()
  +shutdown()
  +addCash(amount)
}

class ATMController {
  +startSession()
  +validatePin()
  +handleWithdraw()
  +handleQuery()
  +handleTransfer()
  +endSession()
}

class Customer {
  +customerId
  +name
}

class DebitCard {
  +cardNo
  +issueDate
  +expireDate
  +status
}

class Account {
  <<abstract>>
  +accountNo
  +balance
  +credit(amount)
  +debit(amount)
}

class CheckingAccount
class SavingsAccount

class ATMTransaction {
  <<abstract>>
  +txId
  +txTime
  +cardNo
  +type
}

class PinValidationTransaction {
  +pin
}

class WithdrawTransaction {
  +accountNo
  +amount
}

class QueryTransaction {
  +accountNo
}

class TransferTransaction {
  +fromAccountNo
  +toAccountNo
  +amount
}

BankSystem o-- ATM
BankSystem o-- BankServer

ATM *-- CardReader
ATM *-- KeypadDisplay
ATM *-- CashDispenser
ATM *-- ReceiptPrinter
ATM *-- CustomerInteraction
ATM *-- OperatorInteraction
ATM *-- ATMController

ATMController --> CardReader
ATMController --> CustomerInteraction
ATMController --> OperatorInteraction
ATMController --> BankServer : request/response
ATMController --> CashDispenser
ATMController --> ReceiptPrinter

Customer "1" --> "0..*" Account
Customer "1" --> "0..*" DebitCard
DebitCard "0..*" --> "0..*" Account : 可访问

Account <|-- CheckingAccount
Account <|-- SavingsAccount

ATMTransaction <|-- PinValidationTransaction
ATMTransaction <|-- WithdrawTransaction
ATMTransaction <|-- QueryTransaction
ATMTransaction <|-- TransferTransaction
```

## 3. 时序图：验证 PIN
```mermaid
sequenceDiagram
autonumber
actor C as ATM客户
participant UI as CustomerInteraction
participant CR as CardReader
participant AC as ATMController
participant BS as BankServer

C->>CR: 插卡
CR-->>AC: cardInfo(cardNo, expDate)
AC->>BS: validateCard(cardNo, expDate)
BS-->>AC: cardOk/cardInvalid

AC->>UI: promptPin()
C->>UI: 输入PIN
UI-->>AC: pin
AC->>BS: validatePin(cardNo, pin)
alt PIN正确
  BS-->>AC: ok + accessibleAccounts
  AC->>UI: 显示账户列表/交易选项
else PIN错误(次数<3)
  BS-->>AC: invalidPin
  AC->>UI: 提示重输PIN
else PIN错误(第3次)
  BS-->>AC: invalidPin
  AC->>CR: retainCard()
end
```

## 4. 时序图：取款
```mermaid
sequenceDiagram
autonumber
actor C as ATM客户
participant UI as CustomerInteraction
participant AC as ATMController
participant BS as BankServer
participant CD as CashDispenser
participant RP as ReceiptPrinter
participant CR as CardReader

AC->>UI: 选择交易(取款)
C->>UI: 输入金额 + 选择账户
UI-->>AC: (accountNo, amount)
AC->>BS: authorizeWithdraw(accountNo, amount)
alt 授权通过
  BS-->>AC: approved + newBalance
  AC->>CD: hasCash(amount)
  alt ATM现金足够
    CD-->>AC: true
    AC->>CD: dispense(amount)
    AC->>RP: print(凭条: txId,type,amount,newBalance)
    AC->>CR: ejectCard()
  else ATM现金不足
    CD-->>AC: false
    AC->>UI: 道歉信息 + 关闭ATM
    AC->>CR: ejectCard()
  end
else 余额不足/超限/账户无效
  BS-->>AC: rejected(reason)
  AC->>UI: 显示错误/道歉信息
  AC->>CR: ejectCard()
end
```

## 5. 状态图：ATM 控制器（简化）
```mermaid
stateDiagram
[*] --> Idle
Idle --> CardInserted: 插卡
CardInserted --> PinEntry: 识别卡
PinEntry --> SelectingTx: PIN验证成功
PinEntry --> EjectingCard: PIN失败/取消/卡无效

SelectingTx --> Processing: 取款/查询/转账
Processing --> DispensingCash: 取款且授权
Processing --> PrintingReceipt: 查询/转账授权
DispensingCash --> PrintingReceipt
PrintingReceipt --> EjectingCard
EjectingCard --> Idle: 退卡

Idle --> Maintenance: 操作员关闭
Maintenance --> Idle: 启动/维护完成
Idle --> OutOfService: 现金不足/故障
OutOfService --> Maintenance
```

## 6. 部署/通信视图（用流程图表示）

```mermaid
flowchart LR
AC["ATM终端（客户端子系统）\nATM控制器"]
UI["客户交互\n(键盘/显示器)"]
CR["读卡器"]
CD["吐钞器"]
RP["凭条打印机"]

BS["中心服务器（银行服务子系统）\nBankServer"]
DB[("银行数据库")]

UI --> AC
CR --> AC
CD --> AC
RP --> AC

AC -. "广域网 / 请求-响应" .- BS
BS <--> DB
```
