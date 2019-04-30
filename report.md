# 2019 OS Project 1 - Scheduling

## Design

### Architecture

在這份Project中 我們透過功能取向 大致分為三個部分:

Main, Process, Scheduler等部分

透過Main呼叫Scheduler來使用對應的演算法排定Process執行順序

再實際呼叫Process模擬程序運作

<img src="res/architecture.png" height='300px'/>

### Scheduler

Scheduler要做的事情本身還算單純

透過不同的scheduling algorithm 我們可以判斷該 preempt 與否

而不同的scheduling algorithm 又有自己的比較方法

我們透過 `next_process` 這支函式來分辨不同的Scheduling Algorithm接著會呼叫哪支Process

而Scheduler每個cycle做的事情便是

1. 確定目前狀態 (是否running process仍要執行, 是否仍有process需要被執行)
2. 根據Scheduling Algorithm選取下一個要執行的Process
3. 執行一個 `UNIT_T()`
4. 重新對Process做排序

Scheduler將會重複執行以上的cycle直到沒有其他Process需要被執行

### Process

#### CPU affinity

在整支程式中 我們主要用兩個CPU來完成: Parent, Child

- Parent:
  負責生成, 呼叫, 中斷 Child Process 以及 Log 開始與結束時間進

- Child:
  負責執行程式本身 亦即用for loop執行 `UNIT_T()`

#### Time Log

為了讓dmesg能夠吃到我們餵進去的數字 我們大概想了以下幾種作法

1. 引入 `linux/kernel.h` 的資源 使用別人寫好的function
2. 自己實作 printk 的 system call 加入kernel中 (如同作業一)
3. 利用 `/dev/kmsg` 這個檔案介面 透過直接寫入的方式存取log

以上順序為我們想到以及嘗試的順序 一開始我們打算只接引用別人寫好的function

後來發現並非所有system都預設有這些標頭檔可以引用

因此 打算採取如同作業一的方法 自己實作 printk 的方式來處理

但一來覺得能不動kernel盡量不要動 另一方面則是覺得 可能還有更直覺的做法

因此 透過研究我們發現 `linux/kernel.h` 中的printk正是對 `/dev/kmsg` 進行寫入的動作

所以其實 我們寫入 `/dev/kmsg` 跟實作printk是有異曲同工之妙 

是故我們最後採取此簡單明瞭的做法



## Result



## Comparison



## Contribution

- B04505021 黃廉弼: 組織架構 檢查程式執行 撰寫報告
- B04505009 劉錫臻: 撰寫Scheduler部分程式碼 測試程式
- B04505019 唐國霖: 撰寫Process部分程式碼
- B04505024 李昊聰: 協調檔案介面 整理註解
- B04505041 黃博鈞: 撰寫Main Function程式碼

