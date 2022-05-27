# BaiTapCuoiKhoaLTNC

BÁO CÁO DỰ ÁN CUỐI KHÓA LẬP TRÌNH NÂNG CAO
 

 



SKY FORCE




Họ và tên sinh viên: Tạ Xuân Tùng
Mã số sinh viên: 21020400
Lớp: K66-CD
Môn học: Lập trình nâng cao














I.	Mô tả chung về trò chơi, các ý tưởng chính
•	Tên trò chơi: SKY FORCE (bắn máy bay)
•	Luật chơi:
Điều khiển máy bay của bạn bằng cách sử dụng các nút mũi tên (sang phải), (sang trái), (lên trên), (xuống dưới) trên bàn phím.
Nhấn vào chuột trái hoặc chuột phải để bắn ra loạt đạn tiêu diệt máy bay địch.
Khi bắn trúng máy bay địch thì bạn được cộng 1 điểm.
Tiêu diệt càng nhiều địch càng tốt, đồng thời phải tiêu diệt boss cuối để giành lấy thắng lợi.
Trong trò chơi có tất cả 3 mạng. Nếu bị tiêu diệt hết 3 mạng thì trò chơi kết thúc.
•	Ý tưởng chính để phát triển game: cần xây dựng được
Chạy phông nền background cho game.
Chạy máy bay của mình.
Di chuyển máy bay của mình, xử lý đạn bắn của máy bay mình.
Tạo các đối tượng trở ngại ở các vị trí ngẫu nhiên, đồng thời xử lý loạt đạn bắn của máy bay địch.
Sử dụng kỹ thuật di chuyển màn hình, đến khi chạy hết màn hình thì gặp boss cuối.
Xử lý va chạm giữa máy bay với máy bay, đạn với máy bay.
Hiển thị số lượt chơi cho nhân vật.
Hiển thị thời gian.
Hiển thị các ký tự lên màn hình(score, time)
Tạo menu cho game.
II.	Các chức năng đã cài đặt:
•	BaseObject: chạy các background.
•	BulletObject: lớp đạn bắn.
•	Common_Function: xử lý bàn phím để di chuyển nhân vật, xử lý đạn bắn từ chuột trái hoặc chuột phải, xây dựng menu cho game.
•	ExplosionObject: khi va chạm giữa máy bay với máy bay hoặc va chạm giữa đạn với máy bay thì gây ra vụ nổ hoặc của máy bay mình, hoặc của máy bay địch.
•	MainObject: xây dựng máy bay mà mình điều khiển, bao gồm điều khiển, xử lsy đạn bắn, va chạm.
•	PlayerPower: Mạng của nhân vật khi chơi game
•	TextObject: Các ký tự được thể hiện trên màn hình
•	ThreatsObject: Xây dựng máy bay của địch.
Chi tiết: Được viết rõ trong game.

III.	Các kỹ thuật lập trình được sử dụng
Sử dụng kỹ thuật lập trình hướng đối tượng, xây dựng nhiều mảng, hàm, vector, cấu trúc.

IV.	Chi tiết về các hướng làm (được viết rõ trong code game)

V.	Hướng phát triển trong tương lai
Trong tương lai, để game được phát triển hơn thì
•	Xây dựng được nhiều màn chơi hơn thay vì chỉ có 1 màn.
•	Có nhiều máy bay địch hơn, đa dạng các loại đạn bắn và hướng di chuyển của đạn.
•	Có thể chơi được nhiều người chơi hơn.
