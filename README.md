# Assignment 2

![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_2.jpg)

Trong bài tập này, bạn sẽ học về một số chủ đề về hình học được đề cập trong bài giảng.
Bạn sẽ xây dựng các đường cong và bề mặt Bezier bằng cách sử dụng thuật toán của de Casteljau,
điều khiển các mesh nửa cạnh và cài đặt sub-division! Khi bạn hoàn tất,
bạn sẽ tạo một công cụ cho phép bạn tải và chỉnh sửa các tệp mesh COLLADA 
cơ bản, loại tệp này hiện được sử dụng bởi nhiều engine và công cụ đồ họa thời gian thực. Cuối cùng, bạn có thể tùy chọn (nếu có thêm tín dụng) thiết kế tệp COLLADA của riêng bạn bằng cách xây dựng mesh của riêng bạn bằng task mềm miễn phí như [Máy ​​xay sinh tố] (http://blender.org).


### Getting Started

Như các bài trước đây, làm theo hướng dẫn ở hw0 để cài đặt repo của các bạn.

## Project Structure

Dự án có 7 task, được chia thành 4 task. Một số chỉ yêu cầu một vài dòng code, 
trong khi những dòng khác thì phức tạp hơn.

week 4:
- task 1: Bezier curves with 1D de Casteljau subdivision (10 pts)
- task 2: Bezier surfaces with separable 1D de Casteljau (15 pts)

week 5:
- task 3: Area-weighted vertex normals (10 pts)

week 6:

- task 4: Edge flip (15 pts)
- task 5: Edge split (15 pts)

week 7:

- task 6: Loop subdivision for mesh upsampling (25 pts)
- task 7: Design and edit your own mesh!

## Sử dụng GUI

Bạn có thể chạy tệp thực thi trên bất kỳ tệp COLLADA nào (.dae, .bzc, .bez):

``
./meshedit <PATH_TO_COLLADA_FILE>
``

Ví dụ, trong Task 1, bạn sẽ có thể vẽ đường cong Bezier bằng cách chạy một lệnh như:

``
./meshedit ../bzc/curve1.bzc
``

Và trong task 2, bạn sẽ có thể vẽ các bề mặt Bezier bằng cách chạy một lệnh như:

``
./meshedit ../bez/teapot.bez
``

Lưu ý rằng GUI cho task 1 hơi khác và sẽ được mô tả kỹ hơn trong [Thông số kỹ thuật của task 1] (#task1gui). 
Khi chạy ứng dụng từ task 2 trở đi, bạn sẽ thấy hình ảnh một tấm mesh được tạo thành từ các hình tam giác.
![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_3.jpg)

Khi bạn di chuyển con trỏ xung quanh màn hình, 
bạn sẽ nhận thấy rằng các phần tử trong mesh (mặt, cạnh và đỉnh) dưới con trỏ được
đánh dấu. Nhấp vào một trong những phần tử này sẽ hiển thị một số thông tin về 
phần tử và dữ liệu liên quan của nó.

Dưới đây là thông số kỹ thuật đầy đủ về các điều khiển bàn phím cho GUI:

| Chìa khóa | Hành động |
|: ---------------: | ------ |
| <kbd> F </kbd> | Lật cạnh đã chọn |
| <kbd> S </kbd> | Tách cạnh đã chọn |
| <kbd> U </kbd> | Nâng mức lấy mẫu mesh hiện tại |
| <kbd> I </kbd> | Hiện ẩn thông tin |
| <kbd> N </kbd> | Chọn halfedge tiếp theo |
| <kbd> T </kbd> | Chọn twin halfedge |
| <kbd> W </kbd> | Chuyển sang bóng đổ GLSL |
| <kbd> 0-9 </kbd> | Chuyển đổi giữa các bộ đổ bóng GLSL |
| <kbd> Q </kbd> | Chuyển đổi sang dạng vector normal trung bình |
| <kbd> R </kbd> | Biên dịch lại shader |
| <kbd> Space </kbd> | Đặt lại camera về vị trí mặc định |

Ngoài ra còn có một số lệnh chuột:

* **Nhấp và kéo một đỉnh** để thay đổi vị trí của nó
* **Nhấp và kéo nền** hoặc **nhấp chuột phải** để xoay máy ảnh.
* **Cuộn** để điều chỉnh thu phóng máy ảnh.


Trong số các lệnh này, bạn sẽ cài đặt các lệnh sau, 
cho phép bạn sửa đổi mesh theo nhiều cách khác nhau.

1. Thao tác lật cục bộ (**F**) và tách (**S**), 
   điều này sửa đổi mesh trong một vùng lân cận nhỏ xung quanh phần tử mesh 
   hiện đang được chọn. Những điều này sẽ được thực hiện trong task 4 và task 5.
2. Subdivision (**U**), tinh chỉnh và làm mịn toàn bộ mesh. 
   Điều này sẽ được thực hiện trong task 6.

Lưu ý rằng mỗi tệp COLLADA có thể chứa nhiều đối tượng mesh. 
Nói một cách tổng quát hơn, tệp COLLADA mô tả một __scene_graph__ 
(giống như SVG) là một biểu diễn phân cấp của tất cả các đối tượng trong khung hình 
(mesh, camera, ánh sáng, v.v.), cũng như các phép biến đổi tọa độ của chúng.
Các phương pháp lấy mẫu lại toàn cục sẽ được chạy trên bất kỳ mắt mesh nào hiện được chọn.

## Getting Acquainted with the Starter Code
Trước khi bạn bắt đầu, đây là một số thông tin cơ bản về cấu trúc của chương trình. 
**Code của bạn cho tất cả các task sẽ được chứa bên trong **student_code.cpp***.

Đối với các đường cong và bề mặt Bezier (Phần I), 
bạn sẽ điền vào các hàm con của các lớp `BezierCurve` và` BezierPatch`, 
được khai báo trong bezierCurve.* và bezierPatch.*.

Đối với mesh nửa cạnh (Phần II), 
bạn sẽ điền vào các hàm con của lớp `HalfedgeMesh`, 
được khai báo trong halfEdgeMesh.*.

**chúng ta đã đặt các dummy definition cho tất cả các hàm bạn cần sửa đổi bên
trong *student_code.cpp*, nơi bạn sẽ viết code của mình.**

## Week 4: Bezier Curves and Surfaces

Trong Phần I, chúng ta xem xét kỹ hơn các đường cong và bề mặt Bezier. 
Trong đồ họa máy tính, các đường cong và bề mặt Bezier là các đường cong 
và bề mặt có tham số, thường được sử dụng để mô hình hóa các đường cong và 
bề mặt mịn và có thể mở rộng ra vô hạn.

Đường cong Bezier có độ $n$ được xác định bởi $(n + 1)$ điểm điều khiển 
và là một đường tham số dựa trên một tham số duy nhất $t$, 
nằm trong khoảng từ 0 đến 1.

Tương tự, bề mặt Bezier của độ $(n, m)$ 
được xác định bởi các điểm điều khiển $(n + 1)(m + 1)$ 
và là một bề mặt tham số dựa trên hai tham số $u$ và $v$, 
cả hai vẫn nằm trong khoảng từ 0 đến 1.

Sử dụng thuật toán của de Casteljau, 
chúng ta có thể tính ra các đường cong và 
bề mặt tham số này cho bất kỳ tập hợp tham số nhất định nào.

![](http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_20.jpg)

### task 1: Bezier curves with 1D de Casteljau subdivision (10 pts)
[**Bài giảng liên quan:7**](https://cs184.eecs.berkeley.edu/sp19/lecture/7/)

Trong task 1, chúng ta sẽ làm việc với các đường cong Bezier.

Để bắt đầu, hãy xem *bezierCurve.h* và 
kiểm tra các biến trong lớp. 
Hãy xem qua mục đích của những biến mà bạn sẽ dùng:

* `std :: vector<Vector2D> controlPoints`: 
  Một vector chứa các điểm điều khiển, dùng để xác định đường cong Bezier. 
  Vector này được khởi tạo với các điểm điều khiển của tệp đường cong Bezier 
  đầu vào.
* `std :: vector<std::vector<Vector2D>> evalLevels`: 
  Một vector 2D chứa các điểm được tính
  ở mỗi cấp độ khi áp dụng thuật toán de Casteljau. 
  Ban đầu, nó chứa một vector duy nhất đại diện cho 
  mức bắt đầu chỉ chứa các điểm điều khiển ban đầu. 
  Bạn nên thêm các cấp bổ sung với các "điểm điều khiển trung gian" 
  tương ứng của chúng mỗi khi hàm `evalStep` được gọi. 
  Ví dụ, một đường cong Bezier sẽ bắt đầu với các điểm điều khiển ban đầu. 
  Sau ba lần gọi `evalStep`, tổng cộng sẽ có bốn mức, 
  trong đó mức cuối cùng chỉ chứa một điểm điều khiển duy nhất đại diện cho $B(t)$ 
  (đường cong Bezier tại $t$).

cài đặt `evalStep`, sử dụng các điểm điều khiển từ cấp được đánh giá gần đây nhất 
và thực hiện thuật toán của de Casteljau để tính toán cấp tiếp theo. 
**Mỗi lệnh gọi hàm này thực hiện một bước của thuật toán.** 
Các điểm điều khiển trung gian này ở mỗi cấp phải được lưu trữ thích hợp 
vào biến `evalLevels`, được chương trình sử dụng để hiển thị đường cong Bezier.

#### Ghi chú cài đặt
* `evalLevels` được khởi tạo từ các điểm điều khiển ban đầu của đường cong Bezier, 
  vì vậy bạn nên có một biến "cấp độ gần đây nhất" để xem và 
  sử dụng để tính toán tập hợp các điểm điều khiển trung gian đầu tiên của mình.
* **KHÔNG** sử dụng hoặc sửa đổi biến `eval_level` được định nghĩa trong` bezierCurve.h`. Điều này được sử dụng trong nội bộ để hiển thị đường cong.
* `std::vector` tương tự như lớp` ArrayList` của Java; 
  bạn nên sử dụng phương thức `push_back` của` std::vector` 
  để thêm các phần tử, tương tự như phương thức `append` của` ArrayList` của Java.

Nhớ lại bài giảng rằng thuật toán của de Casteljau cung cấp 
cho chúng ta bước đệ quy mà chúng ta có thể áp dụng nhiều lần 
để đánh giá đường cong Bezier:

Cho $n$ điểm $p_0, ..., p_n$, tính tập hợp mới $n-1$ điểm $p_0 ', ..., p_ {n-1}'$ 
thông qua phép nội suy tuyến tính (lerp):

$$p_i'= lerp (p_i, p_ {i + 1}, t)$$

trong đó lerp ($p_i$, $p_{i + 1}$, $t$) = $(1 - t) p_i + tp_ {i + 1}$.

`evalStep` sẽ trả về ngay lập tức nếu đường cong Bezier đã
được đánh giá ở $t$ (tức là` evalStep` đã được gọi đủ số lần để giải hoàn toàn 
cho điểm tại $B(t)$).

Kiểm tra việc cài đặt của bạn bằng cách chạy chương trình với cú pháp sau: 
`./meshedit <đường dẫn đến tệp .bzc>`:


    ./meshedit ../bzc/curve1.bzc


*bzc/curve1.bzc* là tệp đường cong Bezier, 
trong khi *bzc/curve2.bzc* là đường cong Bezier bậc 4. 
Hãy thoải mái chơi với các đường cong Bezier bậc cao hơn bằng cách tạo các tệp
*bzc* của riêng bạn.


#### Sử dụng GUI: task 1

Đối với tác vụ này, GUI hơi khác một chút. Có hai lệnh bàn phím:

| Phím | Tác dụng |
|: ---------------: | ------ |
| <kbd>E</kbd> | Thực hiện một lệnh gọi tới `evalStep`; sẽ chuyển qua các cấp sau khi được đánh giá đầy đủ |
| <kbd>C</kbd> | Chuyển đổi xem đường cong Bezier được đánh giá hoàn toàn có được vẽ lên màn hình hay không |

Bước qua việc đánh giá $B(t)$ bằng cách nhấn liên tục **E**
để xác minh việc cài đặt của bạn là chính xác. 
Chuyển đổi đường cong Bezier bằng **C** 
để kiểm tra xem đường cong có được tạo chính xác dựa trên các
điểm điều khiển của nó hay không.

Bạn cũng có thể sử dụng chuột để:

* **Nhấp và kéo** các điểm điều khiển để di chuyển chúng và xem đường cong Bezier của bạn (và tất cả các điểm điều khiển trung gian) thay đổi như thế nào cho phù hợp
* **Cuộn** để di chuyển điểm được đánh giá dọc theo đường cong Bezier và 
  xem cách các điểm điều khiển trung gian di chuyển cùng với nó. 
  Điều này về cơ bản thay đổi $t$ giữa 0,0 và 1,0


#### For Your Writeup: task 1
* Giải thích ngắn gọn thuật toán của de Casteljau và
  cách bạn cài đặt nó để đánh giá các đường cong Bezier.
* Xem qua các tệp *bzc* được cung cấp và tạo đường cong Bezier của riêng bạn với
  **sáu (6)** điểm điều khiển do bạn chọn. 
  Sử dụng đường cong Bezier này cho ảnh chụp màn hình của bạn bên dưới.
* Hiển thị ảnh chụp màn hình của từng bước đánh giá từ điểm điều khiển ban đầu 
  đến điểm đánh giá cuối cùng. Sử dụng lệnh bàn phím ** E ** để thực hiện từng bước. 
  Chuyển đổi ** C ** để hiển thị đường cong đã hoàn thành.
* Di chuyển các điểm xung quanh và sửa đổi giá trị của $t$ bằng cách cuộn
  và cho chúng ta thấy một đường cong Bezier hơi khác.

### task 2: Bezier surfaces with separable 1D de Casteljau subdivision (15 pts)
[**Bài giảng liên quan: 7**] (https://cs184.eecs.berkeley.edu/sp19/lecture/7/)

Trong nhiệm vụ 2, chúng ta sẽ chỉ làm việc với các bề mặt Bezier hình khối.

Để bắt đầu, hãy xem * bezierPatch.h * và kiểm tra định nghĩa lớp. Trong nhiệm vụ này, bạn sẽ làm việc với:

* `std :: vector <std :: vector <Vector3D>> controlPoints`: Một vector 2D đại diện cho lưới 4x4 các điểm điều khiển xác định bề mặt Bezier khối. Biến này được khởi tạo với tất cả 16 điểm điều khiển.
* `Vector3D eval (double u, double v) const`: Bạn sẽ điền vào hàm này, hàm này đánh giá đường cong Bezier tại các tham số $ (u, v) $. Theo thuật ngữ toán học, nó tính $ B (u, v) $.
* `Vector3D eval1D (std :: vector <Vector3D> points, double t) const`: Một chức năng trợ giúp tùy chọn mà bạn có thể thấy hữu ích để cài đặt để giúp bạn cài đặt` eval`. Cho một mảng gồm 4 điểm nằm trên một đường cong, đánh giá đường cong tại tham số $ t $ bằng cách sử dụng chia nhỏ 1D de Casteljau.

Trong lớp, chúng ta đề cập đến ba phương pháp khác nhau để đánh giá bề mặt Bezier. (1) "1D de Casteljau có thể tách rời", trong đó chúng ta thực hiện 1D de Casteljau trong $ uv $ và sau đó trong $ v $, (2) "2D de Casteljau", trong đó chúng ta thực hiện nội suy song tuyến lặp lại trong cả $ u $ và $ v $, và (3) đánh giá đại số bằng cách sử dụng công thức nhị thức và đa thức Bernstein. Trong dự án này, chúng ta sẽ sử dụng phương pháp đầu tiên, "tách 1D de Casteljau".

Thực hiện `eval`, đánh giá hoàn toàn bề mặt Bezier ở các tham số $ u $ và $ v $. Không giống như nhiệm vụ 1, bạn sẽ không thực hiện chỉ một bước duy nhất tại một thời điểm - thay vào đó bạn sẽ đánh giá hoàn toàn bề mặt Bezier bằng cách sử dụng các thông số được chỉ định. Hàm này sẽ trả về điểm được đánh giá cuối cùng đó.

Sử dụng thuật toán sau để áp dụng nhiều lần thuật toán 1D de Casteljau có thể phân tách theo cả hai chiều để đánh giá bề mặt Bezier:

    Đối với mỗi hàng tôi:
      Đặt q (i, u): = áp dụng thuật toán de Casteljau với tham số $ u $ cho hàng điểm điều khiển thứ i

    Cho p (u, v): = áp dụng thuật toán de Casteljau với tham số $ v $ cho mọi q (i, u)
    Trả về p (u, v)

Nếu cách cài đặt của bạn đúng, bạn sẽ thấy một ấm trà bằng cách chạy lệnh sau:

    ./meshedit ../bez/teapot.bez


#### For Your Writeup: task 2
* Giải thích ngắn gọn cách thuật toán của de Casteljau mở rộng đến bề mặt Bezier và cách bạn cài đặt nó để đánh giá bề mặt Bezier.
* Hiển thị ảnh chụp màn hình kết xuất * bez / teapot.bez *. (Lưu ý phần mở rộng tệp!)


## Phần II: Chia nhỏ vòng lặp của lưới tam giác chung

Trong Phần I, chúng ta đã đề cập đến các đường cong và bề mặt Bezier, các hàm tham số được xác định bởi một tập hợp các điểm điều khiển. Thông qua thuật toán của de Casteljau, chúng ta đã thực hiện chia nhỏ cho phép chúng ta đánh giá các chức năng đó.

Với các đường cong Bezier, chúng ta thực hiện chia nhỏ 1D và các bước đánh giá tương đối đơn giản vì các điểm điều khiển liền kề được truy xuất một cách nhỏ giọt từ một vector 1D. Với các bề mặt Bezier, chúng ta đã áp dụng các khái niệm chia nhỏ 1D giống nhau ở cả hai chiều và các bước đánh giá phức tạp hơn một chút, nhưng các điểm điều khiển liền kề vẫn có thể dễ dàng truy xuất từ ​​lưới điểm điều khiển 2D.

Còn về lưới tam giác chung chung thì sao? Ở đây, các quy tắc duy nhất là lưới của chúng ta được tạo thành từ các hình tam giác và các hình tam giác kết nối với nhau thông qua các cạnh và đỉnh của chúng. Không có lưới 2D ràng buộc như trước đây với các bề mặt Bezier. Kết quả là, sự cận kề đáng tiếc là không còn tầm thường nữa. Nhập cấu trúc dữ liệu nửa cạnh, một cấu trúc dữ liệu mạnh mẽ và phổ biến thường được sử dụng để lưu trữ các thực thể lưới và thông tin kết nối của chúng. chúng ta sẽ làm việc với cấu trúc dữ liệu này cho phần còn lại của dự án.


### nhiệm vụ 3: Định mức trung bình cho các mắt lưới nửa cạnh (10 điểm)

[** Bài giảng liên quan: 8 **] (https://cs184.eecs.berkeley.edu/sp19/lecture/8/)

** Lưu ý quan trọng: ** Trước khi đi sâu vào Phần II, trước tiên hãy nhớ tham khảo bài giảng 8 để xem lại bài giảng 8 về nửa cạnh và đọc [bài viết này] (https://cs184.eecs.berkeley.edu/sp19/article / 15 / the-half-edge-data-structure) để giúp bạn điều hướng lớp `HalfedgeMesh` mà bạn sẽ sử dụng rộng rãi trong phần tiếp theo của dự án. Sau khi đọc nó, bạn sẽ hiểu rõ hơn về các chi tiết về cấu trúc dữ liệu nửa cạnh và cách cài đặt của nó trong mã khởi động.

Đối với nhiệm vụ 3, hãy đảm bảo rằng bạn cũng hiểu mã được cung cấp cho hàm `printNeighborPositions`.

Trong tác vụ này, bạn sẽ thực hiện hàm `Vertex :: normal` bên trong * student_code.cpp *. Hàm này trả về vector pháp tuyến trung bình có trọng số diện tích tại một đỉnh, sau đó có thể được sử dụng để tạo bóng cục bộ thực tế hơn so với kỹ thuật tô bóng phẳng mặc định.

Để tính toán giá trị này, bạn sẽ muốn sử dụng `HalfedgeIter` để trỏ đến` Halfedge` mà bạn hiện đang theo dõi. Một `HalfedgeIter` (tương tự là` VertexIter`, `EdgeIter` và` FaceIter`) về cơ bản là một con trỏ đến một `Halfedge` (tương ứng là` Vertex`, `Edge` và` Face`), theo nghĩa mà bạn sẽ sử dụng `->` để giới thiệu thành viên của nó

** Cảnh báo cài đặt kỹ thuật **: Chỉ đối với tác vụ này, bạn đang cài đặt một hàm thành viên `const`, có nghĩa là bạn cần sử dụng` HalfedgeCIter`s thay vì `HalfedgeIter`s. Những điều này chỉ hứa hẹn sẽ không thay đổi giá trị của những thứ mà chúng trỏ đến.

Các hàm thành viên có liên quan cho tác vụ này là `Vertex :: halfedge ()`, `Halfedge :: next ()` và `Halfedge :: twin ()`. Bạn cũng sẽ cần biến thành viên công khai `Vector3D Vertex :: position`.

Cách bạn có thể sử dụng những thứ này để bắt đầu cài đặt chức năng này:

    Vector3D n (0,0,0); // khởi tạo một vector để lưu trữ tổng bình thường của bạn
    HalfedgeCIter h = halfedge (); // Vì chúng ta đang ở trong một Vertex, điều này trả về một halfedge
                                 // trỏ _away_ từ đỉnh đó
    h = h-> twin (); // Lùi lại tới nửaedge trỏ _toward_ đỉnh.
                   // Bây giờ h-> next () sẽ là một cạnh khác trên cùng một mặt,
                   // chia sẻ đỉnh trung tâm.

Tại thời điểm này, bạn nên

1. Lưu bản sao giá trị của `h` trong một` HalfedgeCIter h_orig` khác.
2. Bắt đầu vòng lặp `while` kết thúc khi` h == h_orig`.
3. Bên trong mỗi lần lặp vòng lặp:
    * Tích lũy pháp tuyến trọng số diện tích của mặt hiện tại trong biến `n`. Bạn có thể làm điều này bằng cách sử dụng tích chéo của các cạnh tam giác. chúng ta đã xác định sản phẩm chéo cho bạn, vì vậy đừng tự cài đặt lại! Vì tích chéo của hai vector có chuẩn bằng hai lần diện tích tam giác mà chúng xác định, các vector này là * đã có trọng số diện tích *!
    * Khi bạn đã thêm vào bình thường có trọng số diện tích, bạn nên tiến `h` đến nửa cạnh cho mặt tiếp theo bằng cách sử dụng các hàm` next () 'và `twin ()`.
4. Sau khi kết thúc vòng lặp, trả về vector đơn vị được chuẩn hóa lại `n.unit ()`.


Sau khi hoàn thành tác vụ này, hãy tải lên một * dae * chẳng hạn như * dae / teapot.dae * và nhấn ** W ** để chuyển sang bộ đổ bóng GLSL rồi nhấn ** Q ** để chuyển đổi các vector bình thường được tính trung bình theo diện tích (sẽ gọi hàm `Vertex :: normal` của bạn). Dưới đây là một ví dụ về * dae / teapot.dae * sẽ trông như thế nào với các tiêu chuẩn trung bình theo khu vực được cài đặt chính xác.

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_18.jpg)

Nó KHÔNG nên trông như thế này!

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_19.jpg)

#### For Your Writeup: task 3
* Giải thích ngắn gọn những gì bạn đã làm.
* Hiển thị kết xuất lưới của * dae / teapot.dae *, so sánh đổ bóng OpenGL mặc định có và không có chuẩn làm mịn (sử dụng ** Q ** để chuyển đổi giữa chuẩn mặt và chuẩn đỉnh trung bình). (Lưu ý phần mở rộng tệp!)


### nhiệm vụ 4: Lật nửa cạnh (15 điểm)

** Các bài giảng có liên quan: [8] (https://cs184.eecs.berkeley.edu/sp19/lecture/8/) **

Bây giờ bạn sẽ thấy thoải mái hơn một chút khi lướt qua các con trỏ nửa cạnh. Trong tác vụ này, bạn sẽ cài đặt một phương pháp quan trọng hơn: hoạt động remeshing cục bộ "lật" một cạnh, được cài đặt bên trong phương thức `HalfedgeMesh :: flipEdge` trong tệp * student_code.cpp *.

Chính xác hơn, giả sử chúng ta có một cặp tam giác $ (a, b, c) $ và $ (c, b, d) $. Sau khi lật cạnh $ (b, c) $, bây giờ chúng ta sẽ có các tam giác $ (a, d, c) $ và $ (a, b, d) $:

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_9.jpg)

Giải pháp của bạn nên:

* Không bao giờ lật các cạnh biên (chỉ quay lại ngay lập tức nếu một trong hai mặt lân cận là một vòng biên). Mọi đối tượng đều có một hàm `ranh giới ()` hữu ích có thể cho bạn biết liệu nó có nằm trên ranh giới hay không.
* Chỉ thực hiện một lượng công việc không đổi - chi phí lật một cạnh đơn không ** không ** tỷ lệ thuận với kích thước của lưới!
* Không thêm hoặc xóa bất kỳ phần tử nào. Vì có cùng số lượng phần tử lưới trước và sau khi lật, bạn chỉ cần gán lại con trỏ.

Thách thức lớn nhất trong việc thực hiện đúng thao tác này (cũng như tách) là đảm bảo rằng tất cả các con trỏ vẫn trỏ đến đúng vị trí trong lưới đã sửa đổi. Một công thức dễ dàng để đảm bảo rằng tất cả các con trỏ vẫn hợp lệ sau bất kỳ thao tác remeshing chung nào là:

1. Vẽ một hình ảnh và / hoặc viết ra danh sách tất cả các phần tử (đỉnh, cạnh, mặt, nửa cạnh) sẽ cần từ lưới ban đầu.
2. Vẽ một bức tranh và / hoặc viết ra danh sách tất cả các phần tử sẽ xuất hiện trong lưới đã sửa đổi.
3. Phân bổ bất kỳ phần tử mới nào cần thiết trong lưới đã sửa đổi, nhưng không xuất hiện trong lưới ban đầu (chỉ có liên quan cho nhiệm vụ tiếp theo).
4. Đối với mọi phần tử trong hình "đã sửa đổi", hãy đặt ** tất cả ** các con trỏ của nó - ngay cả khi chúng không thay đổi. Ví dụ: đối với mỗi halfedge, hãy đảm bảo đặt `tiếp theo`,` đôi`, `đỉnh`,` cạnh` và `mặt` thành các giá trị chính xác trong hình mới (đã sửa đổi). Đối với mỗi đỉnh, cạnh và mặt, hãy đảm bảo đặt con trỏ `halfedge` của nó. Một phương thức tiện lợi `Halfedge :: setNeighbors ()` đã được tạo với mục đích thiết lập tất cả các con trỏ bên trong halfedge cùng một lúc.

Lý do để thiết lập tất cả các con trỏ (và không chỉ những con trỏ đã thay đổi) là rất dễ bỏ sót một con trỏ, khiến mã của bạn bị lỗi. Sau khi mã ** hoạt động **, bạn có thể xóa những thứ không cần thiết này

** Cảnh báo cài đặt kỹ thuật **: Chỉ đối với tác vụ này, bạn đang cài đặt một hàm thành viên `const`, có nghĩa là bạn cần sử dụng` HalfedgeCIter`s thay vì `HalfedgeIter`s. Những điều này chỉ hứa hẹn sẽ không thay đổi giá trị của những thứ mà chúng trỏ đến.

Các hàm thành viên có liên quan cho tác vụ này là `Vertex :: halfedge ()`, `Halfedge :: next ()` và `Halfedge :: twin ()`. Bạn cũng sẽ cần biến thành viên công khai `Vector3D Vertex :: position`.

Cách bạn có thể sử dụng những thứ này để bắt đầu cài đặt chức năng này:

    Vector3D n (0,0,0); // khởi tạo một vector để lưu trữ tổng bình thường của bạn
    HalfedgeCIter h = halfedge (); // Vì chúng ta đang ở trong một Vertex, điều này trả về một halfedge
                                 // trỏ _away_ từ đỉnh đó
    h = h-> twin (); // Lùi lại tới nửaedge trỏ _toward_ đỉnh.
                   // Bây giờ h-> next () sẽ là một cạnh khác trên cùng một mặt,
                   // chia sẻ đỉnh trung tâm.

Tại thời điểm này, bạn nên

1. Lưu bản sao giá trị của `h` trong một` HalfedgeCIter h_orig` khác.
2. Bắt đầu vòng lặp `while` kết thúc khi` h == h_orig`.
3. Bên trong mỗi lần lặp vòng lặp:
    * Tích lũy pháp tuyến trọng số diện tích của mặt hiện tại trong biến `n`. Bạn có thể làm điều này bằng cách sử dụng tích chéo của các cạnh tam giác. chúng ta đã xác định sản phẩm chéo cho bạn, vì vậy đừng tự cài đặt lại! Vì tích chéo của hai vector có chuẩn bằng hai lần diện tích tam giác mà chúng xác định, các vector này là * đã có trọng số diện tích *!
    * Khi bạn đã thêm vào bình thường có trọng số diện tích, bạn nên tiến `h` đến nửa cạnh cho mặt tiếp theo bằng cách sử dụng các hàm` next () 'và `twin ()`.
4. Sau khi kết thúc vòng lặp, trả về vector đơn vị được chuẩn hóa lại `n.unit ()`.


Sau khi hoàn thành tác vụ này, hãy tải lên một * dae * chẳng hạn như * dae / teapot.dae * và nhấn ** W ** để chuyển sang bộ đổ bóng GLSL rồi nhấn ** Q ** để chuyển đổi các vector bình thường được tính trung bình theo diện tích (sẽ gọi hàm `Vertex :: normal` của bạn). Dưới đây là một ví dụ về * dae / teapot.dae * sẽ trông như thế nào với các tiêu chuẩn trung bình theo khu vực được cài đặt chính xác.

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_18.jpg)

Nó KHÔNG nên trông như thế này!

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_19.jpg)

#### For Your Writeup: task 3
* Giải thích ngắn gọn những gì bạn đã làm.
* Hiển thị kết xuất lưới của * dae / teapot.dae *, so sánh đổ bóng OpenGL mặc định có và không có chuẩn làm mịn (sử dụng ** Q ** để chuyển đổi giữa chuẩn mặt và chuẩn đỉnh trung bình). (Lưu ý phần mở rộng tệp!)


### nhiệm vụ 4: Lật nửa cạnh (15 điểm)

** Các bài giảng có liên quan: [8] (https://cs184.eecs.berkeley.edu/sp19/lecture/8/) **

Bây giờ bạn sẽ thấy thoải mái hơn một chút khi lướt qua các con trỏ nửa cạnh. Trong tác vụ này, bạn sẽ cài đặt một phương pháp quan trọng hơn: hoạt động remeshing cục bộ "lật" một cạnh, được cài đặt bên trong phương thức `HalfedgeMesh :: flipEdge` trong tệp * student_code.cpp *.

Chính xác hơn, giả sử chúng ta có một cặp tam giác $ (a, b, c) $ và $ (c, b, d) $. Sau khi lật cạnh $ (b, c) $, bây giờ chúng ta sẽ có các tam giác $ (a, d, c) $ và $ (a, b, d) $:

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_9.jpg)

Giải pháp của bạn nên:

* Không bao giờ lật các cạnh biên (chỉ quay lại ngay lập tức nếu một trong hai mặt lân cận là một vòng biên). Mọi đối tượng đều có một hàm `ranh giới ()` hữu ích có thể cho bạn biết liệu nó có nằm trên ranh giới hay không.
* Chỉ thực hiện một lượng công việc không đổi - chi phí lật một cạnh đơn không ** không ** tỷ lệ thuận với kích thước của lưới!
* Không thêm hoặc xóa bất kỳ phần tử nào. Vì có cùng số lượng phần tử lưới trước và sau khi lật, bạn chỉ cần gán lại con trỏ.

Thách thức lớn nhất trong việc thực hiện đúng thao tác này (cũng như tách) là đảm bảo rằng tất cả các con trỏ vẫn trỏ đến đúng vị trí trong lưới đã sửa đổi. Một công thức dễ dàng để đảm bảo rằng tất cả các con trỏ vẫn hợp lệ sau bất kỳ thao tác remeshing chung nào là:

1. Vẽ một hình ảnh và / hoặc viết ra danh sách tất cả các phần tử (đỉnh, cạnh, mặt, nửa cạnh) sẽ cần từ lưới ban đầu.
2. Vẽ một bức tranh và / hoặc viết ra danh sách tất cả các phần tử sẽ xuất hiện trong lưới đã sửa đổi.
3. Phân bổ bất kỳ phần tử mới nào cần thiết trong lưới đã sửa đổi, nhưng không xuất hiện trong lưới ban đầu (chỉ có liên quan cho nhiệm vụ tiếp theo).
4. Đối với mọi phần tử trong hình "đã sửa đổi", hãy đặt ** tất cả ** các con trỏ của nó - ngay cả khi chúng không thay đổi. Ví dụ: đối với mỗi halfedge, hãy đảm bảo đặt `tiếp theo`,` đôi`, `đỉnh`,` cạnh` và `mặt` thành các giá trị chính xác trong hình mới (đã sửa đổi). Đối với mỗi đỉnh, cạnh và mặt, hãy đảm bảo đặt con trỏ `halfedge` của nó. Một phương thức tiện lợi `Halfedge :: setNeighbors ()` đã được tạo với mục đích thiết lập tất cả các con trỏ bên trong halfedge cùng một lúc.

Lý do để thiết lập tất cả các con trỏ (và không chỉ những con trỏ đã thay đổi) là rất dễ bỏ sót một con trỏ, khiến mã của bạn bị lỗi. Sau khi mã ** hoạt động **, bạn có thể xóa những thứ không cần thiết này
** Cảnh báo cài đặt kỹ thuật **: Chỉ đối với tác vụ này, bạn đang cài đặt một hàm thành viên `const`, có nghĩa là bạn cần sử dụng` HalfedgeCIter`s thay vì `HalfedgeIter`s. Những điều này chỉ hứa hẹn sẽ không thay đổi giá trị của những thứ mà chúng trỏ đến.

Các hàm thành viên có liên quan cho tác vụ này là `Vertex :: halfedge ()`, `Halfedge :: next ()` và `Halfedge :: twin ()`. Bạn cũng sẽ cần biến thành viên công khai `Vector3D Vertex :: position`.

Cách bạn có thể sử dụng những thứ này để bắt đầu cài đặt chức năng này:

    Vector3D n (0,0,0); // khởi tạo một vector để lưu trữ tổng bình thường của bạn
    HalfedgeCIter h = halfedge (); // Vì chúng ta đang ở trong một Vertex, điều này trả về một halfedge
                                 // trỏ _away_ từ đỉnh đó
    h = h-> twin (); // Lùi lại tới nửaedge trỏ _toward_ đỉnh.
                   // Bây giờ h-> next () sẽ là một cạnh khác trên cùng một mặt,
                   // chia sẻ đỉnh trung tâm.

Tại thời điểm này, bạn nên

1. Lưu bản sao giá trị của `h` trong một` HalfedgeCIter h_orig` khác.
2. Bắt đầu vòng lặp `while` kết thúc khi` h == h_orig`.
3. Bên trong mỗi lần lặp vòng lặp:
    * Tích lũy pháp tuyến trọng số diện tích của mặt hiện tại trong biến `n`. Bạn có thể làm điều này bằng cách sử dụng tích chéo của các cạnh tam giác. chúng ta đã xác định sản phẩm chéo cho bạn, vì vậy đừng tự cài đặt lại! Vì tích chéo của hai vector có chuẩn bằng hai lần diện tích tam giác mà chúng xác định, các vector này là * đã có trọng số diện tích *!
    * Khi bạn đã thêm vào bình thường có trọng số diện tích, bạn nên tiến `h` đến nửa cạnh cho mặt tiếp theo bằng cách sử dụng các hàm` next () 'và `twin ()`.
4. Sau khi kết thúc vòng lặp, trả về vector đơn vị được chuẩn hóa lại `n.unit ()`.


Sau khi hoàn thành tác vụ này, hãy tải lên một * dae * chẳng hạn như * dae / teapot.dae * và nhấn ** W ** để chuyển sang bộ đổ bóng GLSL rồi nhấn ** Q ** để chuyển đổi các vector bình thường được tính trung bình theo diện tích (sẽ gọi hàm `Vertex :: normal` của bạn). Dưới đây là một ví dụ về * dae / teapot.dae * sẽ trông như thế nào với các tiêu chuẩn trung bình theo khu vực được cài đặt chính xác.

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_18.jpg)

Nó KHÔNG nên trông như thế này!

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_19.jpg)

#### For Your Writeup: task 3
* Giải thích ngắn gọn những gì bạn đã làm.
* Hiển thị kết xuất lưới của * dae / teapot.dae *, so sánh đổ bóng OpenGL mặc định có và không có chuẩn làm mịn (sử dụng ** Q ** để chuyển đổi giữa chuẩn mặt và chuẩn đỉnh trung bình). (Lưu ý phần mở rộng tệp!)


### nhiệm vụ 4: Lật nửa cạnh (15 điểm)

** Các bài giảng có liên quan: [8] (https://cs184.eecs.berkeley.edu/sp19/lecture/8/) **

Bây giờ bạn sẽ thấy thoải mái hơn một chút khi lướt qua các con trỏ nửa cạnh. Trong tác vụ này, bạn sẽ cài đặt một phương pháp quan trọng hơn: hoạt động remeshing cục bộ "lật" một cạnh, được cài đặt bên trong phương thức `HalfedgeMesh :: flipEdge` trong tệp * student_code.cpp *.

Chính xác hơn, giả sử chúng ta có một cặp tam giác $ (a, b, c) $ và $ (c, b, d) $. Sau khi lật cạnh $ (b, c) $, bây giờ chúng ta sẽ có các tam giác $ (a, d, c) $ và $ (a, b, d) $:

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_9.jpg)

Giải pháp của bạn nên:

* Không bao giờ lật các cạnh biên (chỉ quay lại ngay lập tức nếu một trong hai mặt lân cận là một vòng biên). Mọi đối tượng đều có một hàm `ranh giới ()` hữu ích có thể cho bạn biết liệu nó có nằm trên ranh giới hay không.
* Chỉ thực hiện một lượng công việc không đổi - chi phí lật một cạnh đơn không ** không ** tỷ lệ thuận với kích thước của lưới!
* Không thêm hoặc xóa bất kỳ phần tử nào. Vì có cùng số lượng phần tử lưới trước và sau khi lật, bạn chỉ cần gán lại con trỏ.

Thách thức lớn nhất trong việc thực hiện đúng thao tác này (cũng như tách) là đảm bảo rằng tất cả các con trỏ vẫn trỏ đến đúng vị trí trong lưới đã sửa đổi. Một công thức dễ dàng để đảm bảo rằng tất cả các con trỏ vẫn hợp lệ sau bất kỳ thao tác remeshing chung nào là:

1. Vẽ một hình ảnh và / hoặc viết ra danh sách tất cả các phần tử (đỉnh, cạnh, mặt, nửa cạnh) sẽ cần từ lưới ban đầu.
2. Vẽ một bức tranh và / hoặc viết ra danh sách tất cả các phần tử sẽ xuất hiện trong lưới đã sửa đổi.
3. Phân bổ bất kỳ phần tử mới nào cần thiết trong lưới đã sửa đổi, nhưng không xuất hiện trong lưới ban đầu (chỉ có liên quan cho nhiệm vụ tiếp theo).
4. Đối với mọi phần tử trong hình "đã sửa đổi", hãy đặt ** tất cả ** các con trỏ của nó - ngay cả khi chúng không thay đổi. Ví dụ: đối với mỗi halfedge, hãy đảm bảo đặt `tiếp theo`,` đôi`, `đỉnh`,` cạnh` và `mặt` thành các giá trị chính xác trong hình mới (đã sửa đổi). Đối với mỗi đỉnh, cạnh và mặt, hãy đảm bảo đặt con trỏ `halfedge` của nó. Một phương thức tiện lợi `Halfedge :: setNeighbors ()` đã được tạo với mục đích thiết lập tất cả các con trỏ bên trong halfedge cùng một lúc.

Lý do để thiết lập tất cả các con trỏ (và không chỉ những con trỏ đã thay đổi) là rất dễ bỏ sót một con trỏ, khiến mã của bạn bị lỗi. Sau khi mã ** hoạt động **, bạn có thể xóa những thứ không cần thiết này


Viết ra, vị trí mới của đỉnh cũ là

    (1 - n * u) * original_position + u * hàng xóm_position_sum

trong đó `n` là số đỉnh lân cận,` u` là một hằng số như được mô tả trong hình trên, `original_position` là vị trí ban đầu của đỉnh, và` Neighbor_position_sum` là tổng của tất cả các vị trí của các đỉnh lân cận.

Vị trí của đỉnh v mới tạo ra chia một cạnh AB nối các đỉnh A và B và được bao quanh bởi các đỉnh C và D đối diện qua hai mặt nối với AB trong lưới ban đầu sẽ là

    3/8 * (A + B) + 1/8 * (C + D)

Nếu chúng ta liên tục áp dụng hai bước này, chúng ta sẽ hội tụ thành một giá trị gần đúng được làm mịn của lưới ban đầu của chúng ta. Trong nhiệm vụ này, bạn sẽ thực hiện chia nhỏ Vòng lặp, tận dụng các hoạt động tách và lật để xử lý các thay đổi cấu trúc liên kết. Trong dạng phân chia nhiệm vụ, bạn có thể đạt được phân khu 4-1 bằng cách áp dụng chiến lược sau:

1. Tách mọi cạnh của lưới theo bất kỳ thứ tự nào.
2. Lật bất kỳ cạnh mới nào chạm vào đỉnh mới và đỉnh cũ. * Lưu ý *: Mọi cạnh ban đầu bây giờ sẽ được biểu diễn bằng 2 cạnh, bạn * không nên * lật các cạnh này, vì chúng luôn nằm dọc theo ranh giới của hình tam giác được chia 4 cách. Trong sơ đồ dưới đây, bạn chỉ nên lật các cạnh màu xanh lam nối giữa đỉnh cũ và mới, nhưng bạn không nên lật bất kỳ cạnh mới màu đen nào.

Những hình ảnh sau đây (lịch sự của Denis Zorin) minh họa cho ý tưởng này:

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_14.jpg)

#### Hướng dẫn cài đặt

Đối với chia nhỏ Loop, chúng ta cũng đã cung cấp thêm một số thành viên dữ liệu sẽ giúp bạn dễ dàng theo dõi dữ liệu cần thiết để cập nhật kết nối và vị trí đỉnh. Trong nhiệm vụ:

* `Vertex :: newPosition` có thể được sử dụng làm nơi lưu trữ tạm thời cho vị trí mới (được tính bằng giá trị trung bình có trọng số ở trên). Lưu ý rằng bạn nên _not_ thay đổi giá trị của `Vertex :: position` cho đến khi _ tất cả_ các vị trí đỉnh mới đã được tính toán - nếu không, bạn đang lấy giá trị trung bình của các giá trị đã được tính trung bình!
* Tương tự như vậy, `Edge :: newPosition` có thể được sử dụng để lưu trữ vị trí của các đỉnh cuối cùng sẽ được chèn vào các điểm giữa của cạnh. Một lần nữa, các giá trị này nên được tính từ các giá trị ban đầu (trước khi chia nhỏ) và chỉ áp dụng cho các đỉnh mới ở phần cuối. Giá trị `Edge :: newPosition` sẽ được sử dụng cho vị trí của đỉnh sẽ xuất hiện dọc theo cạnh cũ sau khi cạnh được tách. chúng ta tính toán trước vị trí của đỉnh mới trước khi tách các cạnh và phân bổ các đỉnh mới vì dễ dàng đi qua lưới ban đầu đơn giản hơn để tìm các vị trí cho trung bình có trọng số xác định vị trí của các đỉnh mới.
* `Vertex :: isNew` có thể được sử dụng để gắn cờ xem một đỉnh có phải là nhiệm vụ của lưới ban đầu hay là một đỉnh mới được chèn bằng cách chia nhỏ (tại một điểm giữa của cạnh).
* `Edge :: isNew` cũng gắn cờ tương tự cho dù một cạnh là một phần của một cạnh trong lưới ban đầu hay là một cạnh hoàn toàn mới được tạo ra trong bước chia nhỏ.

Với thiết lập này, chúng ta đặc biệt khuyên rằng sẽ dễ dàng nhất để cài đặt chia nhỏ theo "công thức" sau (mặc dù tất nhiên bạn có thể thử làm mọi thứ theo cách khác!). Chiến lược cơ bản là _first_ tính toán các vị trí đỉnh mới (lưu trữ kết quả trong các thành viên `newPosition` của cả đỉnh và cạnh), và chỉ _then_ cập nhật kết nối. Làm theo cách này sẽ dễ dàng hơn nhiều, vì việc truyền qua kết nối ban đầu (thô) đơn giản hơn nhiều so với chuyển qua kết nối mới (tốt). Chi tiết hơn:

1. Đánh dấu tất cả các đỉnh là thuộc lưới ban đầu bằng cách đặt `Vertex :: isNew` thành` false` cho tất cả các đỉnh trong lưới.
2. Tính toán các vị trí đã cập nhật cho tất cả các đỉnh trong lưới ban đầu bằng cách sử dụng quy tắc chia nhỏ đỉnh và lưu trữ chúng trong `Vertex :: newPosition`.
3. Tính toán các vị trí mới được liên kết với các đỉnh sẽ được chèn tại các điểm giữa của cạnh và lưu trữ chúng trong `Edge :: newPosition`.
4. Tách mọi cạnh trong lưới, cẩn thận về cách viết vòng lặp. Trong tasklass, bạn nên đảm bảo chỉ lặp qua các cạnh của lưới ban đầu. Nếu không, bạn sẽ tiếp tục tách các cạnh mà bạn vừa tạo!
5. Lật bất kỳ cạnh mới nào nối đỉnh cũ và mới.
6. Cuối cùng, sao chép các vị trí đỉnh mới (`Vertex :: newPosition`) vào các vị trí đỉnh thông thường (` Vertex :: position`).

Nếu bạn thực hiện sửa đổi được yêu cầu đối với giá trị trả về của `HalfedgeMesh :: splitEdge ()` (xem ở trên), thì một phép tách cạnh bây giờ sẽ trả về một trình lặp về đỉnh mới được chèn và nửaedge của đỉnh này sẽ hướng dọc theo cạnh của lưới ban đầu. Trình lặp này rất hữu ích vì nó có thể được sử dụng để (i) gắn cờ cho đỉnh ret


Viết ra, vị trí mới của đỉnh cũ là

    (1 - n * u) * original_position + u * hàng xóm_position_sum

trong đó `n` là số đỉnh lân cận,` u` là một hằng số như được mô tả trong hình trên, `original_position` là vị trí ban đầu của đỉnh, và` Neighbor_position_sum` là tổng của tất cả các vị trí của các đỉnh lân cận.

Vị trí của đỉnh v mới tạo ra chia một cạnh AB nối các đỉnh A và B và được bao quanh bởi các đỉnh C và D đối diện qua hai mặt nối với AB trong lưới ban đầu sẽ là

    3/8 * (A + B) + 1/8 * (C + D)

Nếu chúng ta liên tục áp dụng hai bước này, chúng ta sẽ hội tụ thành một giá trị gần đúng được làm mịn của lưới ban đầu của chúng ta. Trong nhiệm vụ này, bạn sẽ thực hiện chia nhỏ Vòng lặp, tận dụng các hoạt động tách và lật để xử lý các thay đổi cấu trúc liên kết. Trong dạng phân chia nhiệm vụ, bạn có thể đạt được phân khu 4-1 bằng cách áp dụng chiến lược sau:

1. Tách mọi cạnh của lưới theo bất kỳ thứ tự nào.
2. Lật bất kỳ cạnh mới nào chạm vào đỉnh mới và đỉnh cũ. * Lưu ý *: Mọi cạnh ban đầu bây giờ sẽ được biểu diễn bằng 2 cạnh, bạn * không nên * lật các cạnh này, vì chúng luôn nằm dọc theo ranh giới của hình tam giác được chia 4 cách. Trong sơ đồ dưới đây, bạn chỉ nên lật các cạnh màu xanh lam nối giữa đỉnh cũ và mới, nhưng bạn không nên lật bất kỳ cạnh mới màu đen nào.

Những hình ảnh sau đây (lịch sự của Denis Zorin) minh họa cho ý tưởng này:

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_14.jpg)

#### Hướng dẫn cài đặt

Đối với chia nhỏ Loop, chúng ta cũng đã cung cấp thêm một số thành viên dữ liệu sẽ giúp bạn dễ dàng theo dõi dữ liệu cần thiết để cập nhật kết nối và vị trí đỉnh. Trong nhiệm vụ:

* `Vertex :: newPosition` có thể được sử dụng làm nơi lưu trữ tạm thời cho vị trí mới (được tính bằng giá trị trung bình có trọng số ở trên). Lưu ý rằng bạn nên _not_ thay đổi giá trị của `Vertex :: position` cho đến khi _ tất cả_ các vị trí đỉnh mới đã được tính toán - nếu không, bạn đang lấy giá trị trung bình của các giá trị đã được tính trung bình!
* Tương tự như vậy, `Edge :: newPosition` có thể được sử dụng để lưu trữ vị trí của các đỉnh cuối cùng sẽ được chèn vào các điểm giữa của cạnh. Một lần nữa, các giá trị này nên được tính từ các giá trị ban đầu (trước khi chia nhỏ) và chỉ áp dụng cho các đỉnh mới ở phần cuối. Giá trị `Edge :: newPosition` sẽ được sử dụng cho vị trí của đỉnh sẽ xuất hiện dọc theo cạnh cũ sau khi cạnh được tách. chúng ta tính toán trước vị trí của đỉnh mới trước khi tách các cạnh và phân bổ các đỉnh mới vì dễ dàng đi qua lưới ban đầu đơn giản hơn để tìm các vị trí cho trung bình có trọng số xác định vị trí của các đỉnh mới.
* `Vertex :: isNew` có thể được sử dụng để gắn cờ xem một đỉnh có phải là nhiệm vụ của lưới ban đầu hay là một đỉnh mới được chèn bằng cách chia nhỏ (tại một điểm giữa của cạnh).
* `Edge :: isNew` cũng gắn cờ tương tự cho dù một cạnh là một phần của một cạnh trong lưới ban đầu hay là một cạnh hoàn toàn mới được tạo ra trong bước chia nhỏ.

Với thiết lập này, chúng ta đặc biệt khuyên rằng sẽ dễ dàng nhất để cài đặt chia nhỏ theo "công thức" sau (mặc dù tất nhiên bạn có thể thử làm mọi thứ theo cách khác!). Chiến lược cơ bản là _first_ tính toán các vị trí đỉnh mới (lưu trữ kết quả trong các thành viên `newPosition` của cả đỉnh và cạnh), và chỉ _then_ cập nhật kết nối. Làm theo cách này sẽ dễ dàng hơn nhiều, vì việc truyền qua kết nối ban đầu (thô) đơn giản hơn nhiều so với chuyển qua kết nối mới (tốt). Chi tiết hơn:

1. Đánh dấu tất cả các đỉnh là thuộc lưới ban đầu bằng cách đặt `Vertex :: isNew` thành` false` cho tất cả các đỉnh trong lưới.
2. Tính toán các vị trí đã cập nhật cho tất cả các đỉnh trong lưới ban đầu bằng cách sử dụng quy tắc chia nhỏ đỉnh và lưu trữ chúng trong `Vertex :: newPosition`.
3. Tính toán các vị trí mới được liên kết với các đỉnh sẽ được chèn tại các điểm giữa của cạnh và lưu trữ chúng trong `Edge :: newPosition`.
4. Tách mọi cạnh trong lưới, cẩn thận về cách viết vòng lặp. Trong tasklass, bạn nên đảm bảo chỉ lặp qua các cạnh của lưới ban đầu. Nếu không, bạn sẽ tiếp tục tách các cạnh mà bạn vừa tạo!
5. Lật bất kỳ cạnh mới nào nối đỉnh cũ và mới.
6. Cuối cùng, sao chép các vị trí đỉnh mới (`Vertex :: newPosition`) vào các vị trí đỉnh thông thường (` Vertex :: position`).

Nếu bạn thực hiện sửa đổi được yêu cầu đối với giá trị trả về của `HalfedgeMesh :: splitEdge ()` (xem ở trên), thì một phép tách cạnh bây giờ sẽ trả về một trình lặp về đỉnh mới được chèn và nửaedge của đỉnh này sẽ hướng dọc theo cạnh của lưới ban đầu. Trình lặp này rất hữu ích vì nó có thể được sử dụng để (i) gắn cờ cho đỉnh ret

được thúc đẩy bởi phép toán tách như một đỉnh mới, và (ii) gắn cờ cho mỗi cạnh đi là mới hoặc là nhiệm vụ của lưới ban đầu. (Nói cách khác, Bước 3 là thời điểm tuyệt vời để đặt các thành viên `isNew` cho các đỉnh và cạnh được tạo bởi phép tách. Đây cũng là thời điểm tốt để sao chép trường` newPosition` từ cạnh được tách thành `newPosition` trường của đỉnh mới được chèn.)

Bạn có thể thử cài đặt thuật toán này theo từng giai đoạn, ví dụ: _first_ xem liệu bạn có thể cập nhật kết nối một cách chính xác hay không, _then_ lo lắng về việc đặt đúng vị trí đỉnh. Một số ví dụ dưới đây minh họa hành vi chính xác của thuật toán.

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_12.jpg)


** Tín dụng bổ sung: ** * Hỗ trợ các bề mặt có đường biên. * Để làm điều này, trước tiên bạn cần đảm bảo thao tác tách cạnh của mình xử lý các cạnh biên một cách thích hợp (bạn không cần phải thay đổi chức năng lật cạnh của mình). Bạn cũng sẽ cần sử dụng một mức trung bình có trọng số khác cho các đỉnh biên; xem [Boier-Martin và cộng sự, "Khảo sát về các công cụ dựa trên phân khu để tạo mô hình bề mặt"] (http://mrl.nyu.edu/~dzorin/papers/boiermartin2005sbt.pdf) để biết thêm thông tin.

** Tín dụng bổ sung: ** * Thực hiện các kế hoạch chia nhỏ bổ sung. * Có nhiều lựa chọn thay thế cho chia nhỏ Vòng lặp. Các lược đồ chia nhỏ hình tam giác bao gồm Butterfly và [Butterfly đã sửa đổi] (http://mrl.nyu.edu/~dzorin/papers/zorin1996ism.pdf) (đang nội suy thay vì xấp xỉ) và [Sqrt (3)] (https: // www.graphics.rwth-aachen.de/media/papers/sqrt31.pdf) (tinh chỉnh lưới với tốc độ "chậm hơn"); sơ đồ chia nhỏ phổ biến nhất cho các mắt lưới tứ giác là Catmull-Clark. Ngoài ra còn có các lược đồ chia nhỏ đặc biệt để xử lý các mắt lưới có đỉnh cấp cao, ví dụ: các đỉnh bất thường, được gọi là [phân chia cực] (http://www.cise.ufl.edu/research/SurfLab/papers/09bi3c2polar.pdf).


#### For Your Writeup: task 6
* Giải thích ngắn gọn cách bạn cài đặt phân khu Vòng lặp và mô tả bất kỳ thủ thuật cài đặt / gỡ lỗi thú vị nào mà bạn đã sử dụng.
* Ghi lại một số ghi chú cũng như một số ảnh chụp màn hình để ghi lại những quan sát của bạn về cách các mắt lưới hoạt động sau khi chia nhỏ Vòng lặp. Điều gì xảy ra với các góc và cạnh sắc nhọn? Bạn có thể giảm bớt hiệu ứng này bằng cách tách trước một số cạnh không?
* Tải lên * dae / cube.dae *. Hãy thử một số lần lặp lại chia nhỏ Vòng lặp trên khối lập phương. Lưu ý rằng cube.dae trở nên hơi bất đối xứng sau các bước chia nhỏ lặp đi lặp lại. Chơi xung quanh điều này bằng cách sử dụng lật và tách. Bạn có thể xử lý trước khối lập phương bằng cách lật và chia để nó chia nhỏ đối xứng không? Ghi lại những ảnh hưởng này và giải thích lý do tại sao chúng xảy ra và cách xử lý trước của bạn giúp giảm bớt ảnh hưởng.
* Nếu bạn đã cài đặt bất kỳ tiện ích mở rộng tín dụng bổ sung nào, hãy giải thích những gì bạn đã làm và ghi lại cách chúng hoạt động với ảnh chụp màn hình.


## Phần III: (tùy chọn, có thể có thêm tín dụng)

### nhiệm vụ 7: Thiết kế lưới của riêng bạn!

Đối với tác vụ này, bạn sẽ thiết kế lưới định dạng tệp COLLADA * .dae * của riêng mình bằng cách sử dụng chương trình miễn phí [Blender] (http://blender.org). Đề xuất của chúng ta về điểm bắt đầu cơ bản là thiết kế một lưới hình người với một đầu, hai tay và hai chân. chúng ta đã tạo [Video hướng dẫn máy xay sinh tố] (http://cs184.eecs.berkeley.edu/cs184_sp16_content/article_images/blender.mp4) và [Bài viết hướng dẫn máy xay sinh tố] (https://cs184.eecs.berkeley.edu / sp19 / article / 16 / maker-tutorial) để hướng dẫn bạn các kiến ​​thức cơ bản về cách tạo lưới hình người đơn giản.

Sau khi tạo lưới, bạn nên tải nó vào * meshedit * và hiển thị những gì bạn đã cài đặt! Chia nhỏ nó để làm mịn nó.

Dưới đây là một số ví dụ về mesh-man trước và sau khi chia nhỏ và cũng có áp dụng bộ đổ bóng phản chiếu bản đồ môi trường:

! [] (http://cs184.eecs.berkeley.edu/cs184_sp17_content/article_images/9_15.jpg)

Dưới đây là một số ý tưởng về những điều bạn có thể làm trong nhiệm vụ này để thể hiện sự sáng tạo của bạn trên lưới của bạn:

* Thêm chi tiết bổ sung vào lưới của bạn - ngón tay, các đặc điểm trên khuôn mặt.
* Google "mô hình hóa hộp" (hình ảnh và video) để lấy cảm hứng.
* Điều tra chức năng bổ sung trong Máy xay sinh tố để thiết kế một hình dạng thay thế.
* Lập trình tạo lưới gần đúng với hình dạng 3D fractal hoặc hình dạng phức tạp khác.
* Viết một bộ đổ bóng tùy chỉnh cực hay để làm cho lưới của bạn trông tuyệt vời.
* Thực hiện các phép toán hình học bổ sung và thể hiện chúng trên lưới của bạn.

#### For Your Writeup: task 7
* Lưu tệp * dae * tốt nhất của bạn dưới dạng * cạnh tranh.dae * trong thư mục * docs / * của bạn và hiển thị cho chúng ta ảnh chụp màn hình của tệp đó trong bản ghi của bạn.
* Bao gồm một loạt ảnh chụp màn hình hiển thị lưới ban đầu của bạn, cũng như lưới của bạn sau một và hai vòng chia nhỏ. Nếu bạn đã sử dụng bất kỳ bộ đổ bóng tùy chỉnh nào, hãy bao gồm các ví dụ về lưới của bạn với các bộ đổ bóng đó được áp dụng.
* Mô tả những gì bạn đã làm để nâng cao mesh-man của mình ngoài mesh-man đơn giản được mô tả trong hướng dẫn.

## Lời khuyên thân thiện từ GSI của bạn

* Bắt đầu sớm. Một lần nữa, đây là một công việc nhiều nhiệm vụ và bạn nên quản lý thời gian của mình một cách hợp lý. Hãy nhớ rằng những con bọ hung dữ có thể làm mất thời gian của bạn như không có loài nào khác và hãy luôn ghi nhớ [Định luật Hofstadter] (https://en.wikipedia.org/wiki/Hofstadter's_law).
* nhiệm vụ 1 và 2 sẽ tương đối đơn giản để thực hiện một khi bạn hiểu các khái niệm về thuật toán de Casteljau cũng như các đường cong và bề mặt Bezier.
* các nhiệm vụ từ 3 đến 6 sẽ phức tạp hơn để viết chính xác, vì chúng liên quan đến các con trỏ (được hiển thị cho bạn dưới dạng trình vòng lặp). Đảm bảo rằng bạn kiểm tra các tác vụ này (cụ thể là các tác vụ 4 và 5) * cùng nhau * trên một vài mắt lưới - luân phiên lật và tách các cạnh ít nhất mười lần ở các vùng gần và xa của lưới như lần kiểm tra đầu tiên. Rõ ràng, hành vi đúng không ngụ ý mã chính xác, nhưng hành vi * trong * đúng ngụ ý mã không chính xác :)
* Cuộc thi lưới nhiệm vụ 7 tùy chọn để có thêm tín chỉ liên quan đến việc học cách sử dụng công cụ tạo mô hình 3D. Video hướng dẫn sẽ hữu ích, nhưng nó có thể sẽ là một trải nghiệm mới với một công cụ mới đối với hầu hết mọi người. chúng ta khuyên bạn nên bắt đầu sớm trong trường hợp bạn có thắc mắc. Một cột mốc quan trọng đầu tiên sẽ là tạo một lưới hộp đơn giản trong Blender và tải nó vào ứng dụng * meshedit * dưới dạng tệp * .dae *. Điều này không phụ thuộc vào bất kỳ nhiệm vụ nào khác của bài tập, vì vậy bạn có thể thực hiện nó bất cứ lúc nào (có lẽ là một quãng nghỉ tuyệt vời khi đuổi theo con trỏ ...).
* Hãy chắc chắn rằng bạn phân bổ đủ thời gian để hoàn thành tốt công việc viết bài!


## Nộp hồ sơ

Vui lòng tham khảo bài viết này về [cách tạo và gửi bài tập cho CS 184] (https://cs184.eecs.berkeley.edu/sp19/article/8/building-and-submitting-cs184-as).

Bạn sẽ gửi mã của mình cũng như một số sản phẩm phân phối (xem bên dưới) trong một bản ghi dự án trang web.

### Hướng dẫn và hướng dẫn viết dự án

chúng ta đã cung cấp một khung HTML đơn giản trong * index.html * được tìm thấy trong thư mục * docs * để giúp bạn bắt đầu và cấu trúc bài viết của mình.

Bạn cũng có thể tạo báo cáo trang web của riêng mình từ đầu bằng cách sử dụng các công cụ hoặc khuôn khổ ưa thích của riêng bạn. Tuy nhiên, ** vui lòng tuân theo cấu trúc tổng thể tương tự như được mô tả trong phần phân phối bên dưới **.

Mục tiêu của bài viết của bạn là để bạn (a) suy nghĩ và trình bày rõ ràng những gì bạn đã xây dựng và học được bằng từ ngữ của riêng bạn, (b) viết lại dự án để mang đi học. Bài viết của bạn nên bao gồm:

* Tổng quan về dự án, cách tiếp cận và thực hiện của bạn đối với từng nhiệm vụ cũng như những vấn đề bạn gặp phải và cách bạn giải quyết chúng. Phấn đấu cho sự rõ ràng và ngắn gọn.
* Trên mỗi nhiệm vụ, hãy đảm bảo bao gồm các kết quả được mô tả trong phần Sản phẩm được giao tương ứng cùng với phần giải thích của bạn. Nếu bạn không tạo được bất kỳ kết quả nào một cách chính xác, hãy cung cấp giải thích ngắn gọn về lý do.
* Nhiệm vụ cuối cùng (tùy chọn) cho cuộc thi lưới là nơi bạn có cơ hội để sáng tạo và cá nhân, vì vậy hãy đảm bảo cung cấp mô tả tốt về những gì bạn sẽ làm, những gì bạn đã làm và cách bạn đã làm điều đó.
* Chỉ ra rõ ràng bất kỳ mục tín dụng bổ sung nào bạn đã hoàn thành và cung cấp giải thích kỹ lưỡng và minh họa cho từng mục trong số đó.

Viết lên là một trong những phương pháp chính của chúng ta để đánh giá công việc của bạn, vì vậy điều quan trọng là bạn phải dành thời gian để thực hiện nó một cách chính xác và kỹ lưỡng. Lên kế hoạch trước để phân bổ thời gian cho việc viết đơn trước thời hạn.


### <a name="rubric"> </a> Phân phối và Phiếu đánh giá Viết lên Dự án

Phiếu tự đánh giá này liệt kê các yêu cầu cơ bản, tối thiểu cho bài viết của bạn. Nội dung và chất lượng của bài viết của bạn là cực kỳ quan trọng, và bạn nên đảm bảo ít nhất giải quyết tất cả các điểm được liệt kê dưới đây. Phần tín chỉ bổ sung dành cho những sinh viên muốn thử thách bản thân và khám phá các phương pháp vượt ra ngoài các nguyên tắc cơ bản, và không có giá trị nhiều điểm. Nói cách khác, không nhất thiết phải mong đợi sử dụng điểm tín dụng bổ sung cho các dự án này để bù đắp cho số điểm đã mất ở những nơi khác.

#### Tổng quat
Cung cấp một cái nhìn tổng quan cấp cao về những gì bạn đã cài đặt trong dự án này. Suy nghĩ về những gì bạn đã xây dựng một cách tổng thể. Chia sẻ suy nghĩ của bạn về những điều thú vị bạn đã học được từ việc hoàn thành dự án.

#### nhiệm vụ 1

* Giải thích ngắn gọn thuật toán của de Casteljau và cách bạn cài đặt nó để đánh giá các đường cong Bezier.
* Xem qua các tệp * bzc * được cung cấp và tạo đường cong Bezier của riêng bạn với ** sáu (6) ** điểm điều khiển do bạn chọn. Sử dụng đường cong Bezier này cho ảnh chụp màn hình của bạn bên dưới.
* Hiển thị ảnh chụp màn hình của từng bước đánh giá từ điểm điều khiển ban đầu đến điểm đánh giá cuối cùng. Sử dụng lệnh bàn phím ** E ** để thực hiện từng bước. Chuyển đổi ** C ** để hiển thị đường cong đã hoàn thành.
* Di chuyển các điểm xung quanh và sửa đổi giá trị của $ t $ bằng cách cuộn và cho chúng ta thấy một đường cong Bezier hơi khác.

#### nhiệm vụ 2

* Giải thích ngắn gọn cách thuật toán của de Casteljau mở rộng đến bề mặt Bezier và cách bạn cài đặt nó để đánh giá bề mặt Bezier.
* Hiển thị ảnh chụp màn hình kết xuất * bez / teapot.bez *.

#### nhiệm vụ 3

* Giải thích ngắn gọn những gì bạn đã làm.
* Hiển thị kết xuất lưới của * dae / teapot.dae *, so sánh đổ bóng OpenGL mặc định có và không có chuẩn làm mịn (sử dụng ** Q ** để chuyển đổi giữa chuẩn mặt và chuẩn đỉnh trung bình).

#### task 4

* Giải thích ngắn gọn cách bạn cài đặt thao tác lật nửa cạnh và mô tả bất kỳ thủ thuật cài đặt / gỡ lỗi thú vị nào mà bạn đã sử dụng.
* Hiển thị ảnh chụp màn hình lưới trước và sau khi lật một số cạnh.
* Viết về hành trình gỡ lỗi đầy sự kiện của bạn, nếu bạn đã trải nghiệm.

#### nhiệm vụ 5

* Giải thích ngắn gọn cách bạn cài đặt hoạt động chia nửa cạnh và mô tả bất kỳ thủ thuật cài đặt / gỡ lỗi thú vị nào mà bạn đã sử dụng.
* Hiển thị ảnh chụp màn hình của lưới trước và sau khi tách cạnh.
* Hiển thị ảnh chụp màn hình lưới trước và sau khi kết hợp cả tách cạnh và lật cạnh.
* Viết về nhiệm vụ gỡ lỗi hoành tráng của bạn, nếu bạn đã thực hiện một nhiệm vụ.
* Nếu bạn đã cài đặt hỗ trợ cho các cạnh biên, hãy đưa ra một số ví dụ ảnh chụp màn hình về việc cài đặt của bạn xử lý đúng cách các hoạt động phân chia trên các cạnh biên.

#### nhiệm vụ 6

* Giải thích ngắn gọn cách bạn cài đặt phân khu Vòng lặp và mô tả bất kỳ thủ thuật cài đặt / gỡ lỗi thú vị nào mà bạn đã sử dụng.
* Ghi lại một số ghi chú cũng như một số ảnh chụp màn hình để ghi lại những quan sát của bạn về cách các mắt lưới hoạt động sau khi chia nhỏ Vòng lặp. Điều gì xảy ra với các góc và cạnh sắc nhọn? Bạn có thể giảm bớt hiệu ứng này bằng cách tách trước một số cạnh không?
* Tải lên * dae / cube.dae *. Hãy thử một số lần lặp lại chia nhỏ Vòng lặp trên khối lập phương. Lưu ý rằng cube.dae trở nên hơi bất đối xứng sau các bước chia nhỏ lặp đi lặp lại. Chơi xung quanh điều này bằng cách sử dụng lật và tách. Bạn có thể xử lý trước khối lập phương bằng cách lật và chia để nó chia nhỏ đối xứng không? Ghi lại những ảnh hưởng này và giải thích lý do tại sao chúng xảy ra và cách xử lý trước của bạn giúp giảm bớt ảnh hưởng.
* Nếu bạn đã cài đặt bất kỳ tiện ích mở rộng tín dụng bổ sung nào, hãy giải thích những gì bạn đã làm và ghi lại cách chúng hoạt động với ảnh chụp màn hình.

#### nhiệm vụ 7

* Lưu tệp * dae * tốt nhất của bạn dưới dạng * cạnh tranh.dae * trong thư mục * docs / * của bạn và hiển thị cho chúng ta ảnh chụp màn hình của tệp đó trong bản ghi của bạn.
* Bao gồm một loạt ảnh chụp màn hình hiển thị lưới ban đầu của bạn, cũng như lưới của bạn sau một và hai vòng chia nhỏ. Nếu bạn đã sử dụng bất kỳ bộ đổ bóng tùy chỉnh nào, hãy bao gồm các ví dụ về lưới của bạn với các bộ đổ bóng đó được áp dụng.
* Mô tả những gì bạn đã làm để nâng cao mesh-man của mình ngoài mesh-man đơn giản được mô tả trong hướng dẫn.


### Mẹo và lời khuyên về trang web

* Trang báo cáo chính của bạn nên được gọi là * index.html *.
* Đảm bảo bao gồm và nộp tất cả các tệp khác (chẳng hạn như hình ảnh) được liên kết trong báo cáo của bạn!
* Chỉ sử dụng các đường dẫn * tương đối * đến các tệp, chẳng hạn như `" ./images/image.jpg "`
* KHÔNG * KHÔNG * sử dụng các đường dẫn tuyệt đối, chẳng hạn như `" /Users/student/Desktop/image.jpg "'
* Hãy chú ý đến phần mở rộng tên tệp của bạn. Hãy nhớ rằng trên hệ thống UNIX (chẳng hạn như máy hướng dẫn), viết hoa rất quan trọng. `.png! = .jpeg! = .jpg! = .JPG`
* Đảm bảo điều chỉnh các quyền trên tệp của bạn để chúng có thể đọc được trên toàn thế giới. Để biết thêm thông tin về điều này, vui lòng xem [hướng dẫn] này (http://www.grymoire.com/Unix/Permissions.html "> http://www.grymoire.com/Unix/Permissions.html).
* Bắt đầu lắp ráp trang web của bạn sớm để đảm bảo rằng bạn có khả năng chỉnh sửa mã HTML để chèn hình ảnh và các phần định dạng.

