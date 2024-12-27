# Smart Garden

## Lời cảm ơn
Trước hết, tôi xin chân thành cảm ơn thầy Nguyễn Anh Tuấn, giảng viên môn IoT, đã tận tình hướng dẫn và hỗ trợ tôi trong suốt thời gian 15 tuần của môn học IoT. Những lời góp ý và sự tâm huyết của thầy giúp ích cho tôi rất nhiều để hoàn thành sản phẩm.

## Giới thiệu
"Smart Garden" là một hệ thống tưới cây thông minh được CẢI TIẾN từ đề tài giữa kỳ với mục tiêu nâng cao hiệu quả và bảo vệ cây trồng. Hệ thống không chỉ tự động tưới cây dựa trên độ ẩm đất mà còn kết hợp điều kiện ánh sáng để tránh tưới vào thời điểm không phù hợp (ví dụ: trời nắng to). Ngoài ra, hệ thống còn bổ sung cảm biến DHT11 để đo nhiệt độ và độ ẩm không khí, đồng thời gửi dữ liệu thời gian thực qua MQTT lên Node-RED Dashboard.

## Tính năng bổ sung với đề tài giữa kỳ
- Tưới cây tự động khi:
  - Độ ẩm đất dưới 60%.
  - Cường độ ánh sáng thấp (mô phỏng thời điểm sáng sớm, tối muộn hoặc trời râm mát).
- Đọc và hiển thị thông số môi trường:
  - Độ ẩm đất.
  - Nhiệt độ và độ ẩm không khí.
  - Cường độ ánh sáng.
- Gửi dữ liệu thời gian thực lên Node-RED Dashboard thông qua giao thức MQTT.
- Điều khiển bật/tắt bơm nước từ xa qua MQTT.

## Phần cứng sử dụng
- **ESP32**: Bộ vi điều khiển chính.
- **Cảm biến độ ẩm đất**: Đo độ ẩm của đất.
- **Cảm biến DHT11**: Đo nhiệt độ và độ ẩm không khí.
- **Quang trở (LDR)**: Đo cường độ ánh sáng.
- **Relay**: Điều khiển bơm nước mini.
- **Bơm nước mini**: Tưới cây tự động.

## Phần mềm sử dụng
- **PlatformIO/VS Code**: Phát triển và lập trình ESP32.
- **Node-RED**: Hiển thị dữ liệu và điều khiển qua giao diện Dashboard.
- **MQTT**: Giao thức truyền dữ liệu giữa ESP32 và Node-RED.

## Quá trình hoạt động
1. **Đọc dữ liệu cảm biến:**
   - Cảm biến độ ẩm đất đo độ ẩm hiện tại của đất.
   - Cảm biến DHT11 đo nhiệt độ và độ ẩm không khí.
   - Quang trở đo cường độ ánh sáng.

2. **Kiểm tra điều kiện tưới cây:**
   - Độ ẩm đất < 60%.
   - Cường độ ánh sáng < 30%.

3. **Kích hoạt bơm nước:**
   - Nếu cả hai điều kiện trên thỏa mãn, bơm nước được bật.
   - Nếu không, bơm nước sẽ tắt.

4. **Gửi dữ liệu qua MQTT:**
   - Các thông số như độ ẩm đất, nhiệt độ, độ ẩm không khí, và cường độ ánh sáng được gửi lên Node-RED Dashboard.
   - Trạng thái bơm nước (Bật/Tắt) cũng được cập nhật.

## Cách triển khai
1. **Kết nối phần cứng:**
   - Lắp đặt các cảm biến và bơm nước theo sơ đồ kết nối.

2. **Lập trình ESP32:**
   - Nạp mã nguồn chính (main code) vào ESP32.

3. **Cấu hình Node-RED:**
   - Tạo giao diện Dashboard để hiển thị dữ liệu và điều khiển bơm nước.
![node_red dashboard_config](https://github.com/user-attachments/assets/2da8cff6-f716-4235-bd43-c426dc93cbd6)


4. **Kiểm tra và chạy hệ thống:**
   - Đảm bảo dữ liệu hiển thị đúng trên Dashboard và hệ thống hoạt động theo điều kiện đã thiết lập.

## Kết quả
![node_red_dashboard_4](https://github.com/user-attachments/assets/f37cdb60-61b5-4c2c-b5ea-aaccb49194f6)

## Hướng phát triển thêm
- Lưu trữ dữ liệu nhiệt độ, độ ẩm, và ánh sáng để phân tích xu hướng.
- Sử dụng AI để dự đoán các sự kiện như thời điểm cây cần tưới hoặc khi thiết bị cần bảo trì.
- Đồng bộ hệ thống với API thời tiết để tối ưu hóa việc tưới cây.
- Tích hợp van nước để tưới từng khu vực khác nhau dựa trên nhu cầu riêng biệt.

## Ghi chú
- Mã nguồn trên có thể cần tùy chỉnh theo thông tin mạng WiFi và MQTT cụ thể của bạn.


