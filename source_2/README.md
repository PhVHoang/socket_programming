Các bạn chạy make để build project: make project




+ Server sẽ lưu chứa thông tin về những client:
  - Tên client.
  - ID của client.
  - Địa chỉ server quả lí nó.
  - Cổng giao tiếp với server:
				* Cổng up(là cổng chẵn, bắt đầu từ 10000 trở lên.): một client có duy nhất cổng up-không đụng hàng- trong hệ thống. Cổng này client sẽ gửi thông tin lịch sử mua bán hàng, đồng thời là kênh gửi dữ liệu lên server trong trường hợp server ra lệnh gửi thông tin của client( các loại mặt hàng có trong ngăn của client, giá của chúng, số lượgn...) lên server.
				* Cổng down luôn là cổng lẻ và bằng cổng chẵn - 1.  Cổng này chỉ nhận lệnh từ server. Cổng up chỉ gửi dữ liệu, cổng down chỉ nhận dữ liệu mà thôi.

+ Muốn tạo một client mới ta chạy chương trình server_mamager.exec:
  Server manager có chức năng thêm một client vào dữ liệu của server, sửa thông tin của client, xuất file cấu hình cho client (file config.cfg).
  Khi xuất file cấu hình client, nó tạo ra một thư mục dạng "client<id của client>". Trong đó có file cấu hình và file client.exec

+ Một thư mục dạng "client<id của client>" đại diện cho một cái máy bán hàng. File client.exec được chạy bằng cách ./client.exec <mode>
  Có 2 mode là sale-(Bán hàng), và config-(thêm các mặt hàng, chỉnh giá các mặt hàng, bổ xung số lượng mặt hàng đang có trong máy.)

  Ví dụ mình mmuốn chạy mode bán hàng: ./client.exec sale

  Mode config cho phép ta quản lí mặt hàng trong máy:
							* Thêm mặt hàng:
									- Tên mặt hàng.
									- Gía mặt hàng.
									- Số lượg chứa tối đa.
									- Số lượng sẽ cảnh báo để ta bổ xung thêm hàng. Ví dụ khi hàng còn trong máy khỏang 3 lon coca cola thì sẽ báo server để nhân viên bổ xung thêm hàng.
							* Xóa một mặt hàng.
							* Chỉnh số lượng của mặt hàng trong máy, đổi tên, đổi giá.....

+ Mới chạy thì ta cần đăng kí client -> Xuất file cấu hình client-> chạy client.exe chế độ config để thêm mặt hàng -> Chạy server.exec -> Chạy chế độ bán hàng để bán hàng.
+ File history.log sẽ lưu trữ họat động của mỗi client.
