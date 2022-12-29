1. Producer 與 Consumer 皆以 process 來開發。
2. Producer 負責產生 M 個資料(例如：M=1000，資料內容”This is message
0”)，資料間隔 R millisecond (例如：R=300 ms，你會需要用到 usleep() 函
式)，然後設法將資料交給所有 Consumer (例如：N=200)。
3. R 越小代表資料產生越快，每份資料都是文字資料，內含序號，如上例。
4. 你必須要設計適當的資料傳遞之方式，否則當 N 很大時，consumer 會來
不及收到資料。
5. 系統只能有 B 個 buffers 來暫存 B 筆資料(例如：B=3)，第 B 筆資料，會
蓋到第 0 筆資料。所以資料必須被及時收到，否則會被蓋掉，如下圖依此類推

限制：
1. 每個 Data 內有 80 個 char(如: char message[80];)，且每個 message 都
要內含序號(如：0、1、2、3…)，讓接收訊息的 consumer 知道此為第
幾份資料。
2. 本作業優先使用Shared Memory + signal 的解決方法(若有更好解法不
受此限)，consumer 才能及時讀取資料。Consumer 被動被通知去讀取
資料，signal通知時訊息中必須帶資料序號(上課有教如何用 sigaction()
加參數到 signal 中)。
3. 輸出樣式為全部 consumers 總共接收到資料量，並且計算出 loss rate



$ ./program [資料數量 M] [傳送速率 R] [consumer數量 N] [buffer size B]


![image](https://user-images.githubusercontent.com/72913466/209909231-bddef935-454b-40fa-8635-40e36d3fb9e4.png)

1. 請畫出當資料數量固定為 1000，B=3，consumers 數量為 10、100、1000之
折線圖( 縱軸：loss rate；橫軸：傳送速率(ms) )如下範例。
2. 請畫出當資料數量固定為 1000，R=500，consumers 數量為 10、100、
1000 之折線圖( 縱軸：loss rate；橫軸：buffer size 1-10 )如下範例。

![image](https://user-images.githubusercontent.com/72913466/209909258-bb2f5f98-6071-4805-ac14-9fc6bceff76e.png)

3. 請描述您使用主機的作業系統、Memory、CPU
4. 您覺得造成資料 loss 影響最大的因素為 Memory or CPU or buffer size? 為什麼?
5. 另設計一個程式 (given fixed (M, B, R, N), 如：(1000, 3, 500, 150))，如何有
效降低 Loss rate?

![image](https://user-images.githubusercontent.com/72913466/209909281-9177cb06-fef0-40ed-b75d-3abc1434b1ff.png)
