#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// =========================================================
// Li Chao Tree:
//   - Thêm các đường thẳng y = a*x + b
//   - Query giá trị nhỏ nhất tại 1 điểm x
//
// Độ phức tạp:
//   - add_line : O(log X)
//   - query    : O(log X)
//
// Lưu ý:
//   - Phải biết trước miền x = [X_LEFT, X_RIGHT]
//   - Nếu bài cần max thay vì min, chỉ cần đổi các dấu so sánh
// =========================================================

struct LiChao {
    // -----------------------------------------------------
    // Chỉnh miền x tại đây tùy theo đề bài
    // Ví dụ:
    //   x là chỉ số dp / tọa độ / thời gian / ...
    //   Nếu x nằm trong [-1e6, 1e6] thì để như dưới
    // -----------------------------------------------------
    static const int X_LEFT  = -1000000;
    static const int X_RIGHT =  1000000;

    // Kích thước đủ lớn cho segment tree tĩnh trên miền x
    static const int LIM = X_RIGHT - X_LEFT + 5;

    // INF phải đủ lớn để đóng vai trò "đường rỗng"
    static const ll INF = (1LL << 62);

    #define lc (n << 1)
    #define rc ((n << 1) | 1)

    // -----------------------------------------------------
    // Mỗi node lưu đúng 1 đường thẳng:
    //   y = a*x + b
    // -----------------------------------------------------
    struct Line {
        ll a, b;

        // Mặc định là đường "rỗng" cho bài toán min:
        // y = +INF
        Line(ll _a = 0, ll _b = INF) : a(_a), b(_b) {}

        inline ll get(ll x) const {
            return a * x + b;
        }
    };

    // t[n] = đường thẳng đang được giữ tại node n
    Line t[4 * LIM];

    LiChao() {
        // Khởi tạo toàn bộ node là đường rỗng
        for (int i = 0; i < 4 * LIM; i++) t[i] = Line();
    }

    // -----------------------------------------------------
    // Thêm 1 đường thẳng v vào cây
    //
    // Ý tưởng:
    //   - Node hiện tại quản lý đoạn x = [b..e]
    //   - t[n] đang là đường cũ
    //   - v là đường mới
    //
    // Ta so sánh ở điểm đầu và điểm giữa:
    //   - đường nào tốt hơn ở mid thì giữ lại ở node
    //   - đường còn lại chỉ còn khả năng thắng ở 1 nửa đoạn,
    //     nên đẩy xuống con tương ứng
    // -----------------------------------------------------
    void upd(int n, int b, int e, Line v) {
        int mid = (b + e) >> 1;

        // lef = ở đầu trái b, đường mới có tốt hơn đường cũ không
        bool lef = v.get(b) < t[n].get(b);

        // midf = ở giữa mid, đường mới có tốt hơn đường cũ không
        bool midf = v.get(mid) < t[n].get(mid);

        // Nếu đường mới tốt hơn tại mid,
        // ta để nó ở lại node hiện tại
        if (midf) swap(t[n], v);

        // Nếu chỉ còn 1 điểm thì dừng
        if (b == e) return;

        // Nếu kết quả so sánh ở b và mid khác nhau,
        // nghĩa là đường "thua ở mid" vẫn có cơ hội thắng ở nửa trái
        if (lef != midf) upd(lc, b, mid, v);
        else upd(rc, mid + 1, e, v);
    }

    // -----------------------------------------------------
    // Query giá trị nhỏ nhất tại điểm x
    //
    // Ta đi từ gốc xuống lá chứa x:
    //   - lấy min giữa đường đang giữ ở node hiện tại
    //   - và kết quả ở con phù hợp
    // -----------------------------------------------------
    ll query(int n, int b, int e, int x) {
        ll res = t[n].get(x);

        if (b == e) return res;

        int mid = (b + e) >> 1;
        if (x <= mid) return min(res, query(lc, b, mid, x));
        else return min(res, query(rc, mid + 1, e, x));
    }

    // -----------------------------------------------------
    // Wrapper cho dễ dùng:
    //
    // add_line(a, b):
    //   thêm đường y = a*x + b
    //
    // get_min(x):
    //   trả về min(a_i*x + b_i) trên tất cả đường đã chèn
    // -----------------------------------------------------
    inline void add_line(ll a, ll b) {
        upd(1, X_LEFT, X_RIGHT, Line(a, b));
    }

    inline ll get_min(int x) {
        return query(1, X_LEFT, X_RIGHT, x);
    }

    // -----------------------------------------------------
    // ==================== CÁCH DÙNG =======================
    //
    // LiChao st;
    //
    // // Thêm các đường:
    // // y = 2x + 3
    // st.add_line(2, 3);
    //
    // // y = -x + 10
    // st.add_line(-1, 10);
    //
    // // y = 5
    // st.add_line(0, 5);
    //
    // // Query min tại x = 7
    // cout << st.get_min(7) << '\n';
    //
    //
    // -----------------------------------------------------
    // Ví dụ minh họa tay:
    //
    // Các đường:
    //   y1 = 2x + 3
    //   y2 = -x + 10
    //   y3 = 5
    //
    // Tại x = 7:
    //   y1 = 17
    //   y2 = 3
    //   y3 = 5
    //
    // => min = 3
    // -----------------------------------------------------
    //
    // Ví dụ dùng trong DP:
    //
    // Nếu có công thức:
    //   dp[i] = min(dp[j] + m[j] * x[i] + c[j])
    //
    // thì mỗi trạng thái j tạo ra 1 đường:
    //   y = m[j] * x + (dp[j] + c[j])
    //
    // Khi xử lý đến i:
    //   dp[i] = st.get_min(x[i]);
    //
    // Sau đó chèn đường sinh từ i:
    //   st.add_line(m[i], dp[i] + c[i]);
    //
    // -----------------------------------------------------
    // Lưu ý quan trọng:
    //
    // 1) Miền x phải nằm trong [X_LEFT, X_RIGHT]
    //    Nếu x vượt ra ngoài miền này thì template sai.
    //
    // 2) Nếu hệ số và x lớn, a*x+b có thể overflow long long.
    //    Khi đó cần:
    //      - dùng __int128 trong hàm get()
    //      - hoặc đảm bảo constraint an toàn
    //
    // 3) Template này là bản insert full line trên toàn miền.
    //    Nếu đề yêu cầu insert line trên đoạn [L..R],
    //    thì cần Li Chao segment tree nâng cao hơn.
    //
    // 4) Template này đang query MIN.
    //    Nếu muốn query MAX:
    //      - đổi INF thành -INF phù hợp
    //      - đổi các dấu < thành >
    //      - đổi min(...) thành max(...)
    // -----------------------------------------------------
};
