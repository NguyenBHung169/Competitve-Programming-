#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200000 + 5;
const int LOG = 20;

struct SA {
    int n;                  // độ dài chuỗi
    string s;               // chuỗi sau khi thêm ' ' ở đầu để dùng 1-index

    int sa[MAXN];           // sa[i] = vị trí bắt đầu của suffix đứng thứ i trong thứ tự từ điển
    int rk[MAXN];           // rk[i] = thứ hạng của suffix bắt đầu tại vị trí i
    int oldrk[MAXN];        // rank cũ ở vòng doubling trước
    int id[MAXN];           // mảng phụ chứa thứ tự suffix trước khi counting sort
    int cnt[MAXN];          // mảng đếm dùng cho counting sort
    int h[MAXN];            // h[i] = LCP(sa[i], sa[i - 1])

    int st[LOG][MAXN];      // sparse table để query min trên mảng h[]
    int lg[MAXN];           // lg[i] = floor(log2(i))

    // Build suffix array cho chuỗi _s
    void build(const string &_s) {
        s = " " + _s;       // chuyển sang 1-index, s[1..n]
        n = (int)_s.size();

        int m = 256;        // miền giá trị rank ban đầu là mã ASCII ký tự

        // ==============================
        // Bước 1: Khởi tạo rank theo ký tự đầu tiên
        // ==============================

        for (int i = 1; i <= n; i++) {
            rk[i] = (unsigned char)s[i]; // rank ban đầu = mã ASCII của ký tự s[i]
            cnt[rk[i]]++;                // đếm số suffix có ký tự đầu là rk[i]
        }

        // Prefix sum để cnt[x] = số phần tử có rank <= x
        for (int i = 1; i <= m; i++) {
            cnt[i] += cnt[i - 1];
        }

        // Counting sort lần đầu theo ký tự đầu tiên
        // Duyệt ngược để giữ tính ổn định
        for (int i = n; i >= 1; i--) {
            sa[cnt[rk[i]]--] = i;
        }

        // ==============================
        // Bước 2: Doubling
        // Mỗi vòng xét 2 đoạn:
        // suffix i được đại diện bởi pair:
        // (rank[i], rank[i + w])
        // w = 1, 2, 4, 8, ...
        // ==============================

        for (int w = 1, p = 0; w < n; w <<= 1, m = p) {
            p = 0;

            // ------------------------------
            // Tạo mảng id[] theo khóa thứ hai đã được sắp sẵn
            // ------------------------------

            // Những suffix có đoạn thứ hai rỗng sẽ đứng đầu
            // Vì i + w > n, tức suffix bắt đầu ở gần cuối chuỗi
            for (int i = n - w + 1; i <= n; i++) {
                id[++p] = i;
            }

            // Nếu suffix sa[i] đang đứng trong thứ tự đã sort,
            // thì suffix bắt đầu tại sa[i] - w có khóa thứ hai là sa[i].
            // Do đó id[] được sắp theo khóa thứ hai.
            for (int i = 1; i <= n; i++) {
                if (sa[i] > w) {
                    id[++p] = sa[i] - w;
                }
            }

            // ------------------------------
            // Counting sort theo khóa thứ nhất rk[id[i]]
            // Vì id[] đã sort theo khóa thứ hai,
            // sort ổn định theo khóa thứ nhất sẽ cho thứ tự pair hoàn chỉnh.
            // ------------------------------

            for (int i = 0; i <= m; i++) {
                cnt[i] = 0;
            }

            for (int i = 1; i <= n; i++) {
                cnt[rk[i]]++;
            }

            for (int i = 1; i <= m; i++) {
                cnt[i] += cnt[i - 1];
            }

            // Duyệt ngược để counting sort ổn định
            for (int i = n; i >= 1; i--) {
                sa[cnt[rk[id[i]]]--] = id[i];
            }

            // ------------------------------
            // Cập nhật rank mới
            // ------------------------------

            for (int i = 1; i <= n; i++) {
                oldrk[i] = rk[i];
            }

            p = 0; // số lượng rank khác nhau hiện tại

            for (int i = 1; i <= n; i++) {
                if (i == 1) {
                    // suffix nhỏ nhất nhận rank 1
                    rk[sa[i]] = ++p;
                } else {
                    int x = sa[i];       // suffix hiện tại
                    int y = sa[i - 1];   // suffix đứng ngay trước nó

                    // Hai suffix có cùng rank mới nếu hai pair giống nhau:
                    // (oldrk[x], oldrk[x + w]) == (oldrk[y], oldrk[y + w])
                    if (oldrk[x] == oldrk[y] &&
                        oldrk[x + w] == oldrk[y + w]) {
                        rk[x] = p;
                    } else {
                        rk[x] = ++p;
                    }
                }
            }

            // Nếu mỗi suffix đã có rank riêng, suffix array đã hoàn chỉnh
            if (p == n) break;
        }

        // Build LCP array
        buildHeight();

        // Build sparse table để query LCP hai suffix bất kỳ
        buildRMQ();
    }

    // ==============================
    // Build mảng h[] bằng thuật toán Kasai
    //
    // h[i] = LCP(sa[i], sa[i - 1])
    //
    // Nếu suffix tại i và suffix trước nó có LCP = k,
    // thì khi chuyển sang i + 1, LCP giảm nhiều nhất 1.
    // Vì vậy tổng thời gian là O(n).
    // ==============================

    void buildHeight() {
        int k = 0; // độ dài LCP hiện tại

        for (int i = 1; i <= n; i++) {
            // suffix có rank 1 không có suffix đứng trước
            if (rk[i] == 1) {
                h[1] = 0;
                continue;
            }

            // Khi dịch từ suffix i-1 sang suffix i,
            // LCP có thể giảm ít nhất 1 ký tự
            if (k) k--;

            // j là vị trí bắt đầu của suffix đứng ngay trước suffix i trong SA
            int j = sa[rk[i] - 1];

            // Tăng k khi hai suffix còn giống nhau
            while (i + k <= n && j + k <= n && s[i + k] == s[j + k]) {
                k++;
            }

            // h[rank của suffix i] = LCP với suffix đứng trước nó
            h[rk[i]] = k;
        }
    }

    // ==============================
    // Build Sparse Table trên h[]
    //
    // LCP của suffix x và suffix y:
    // giả sử rk[x] < rk[y]
    // thì:
    // LCP(x, y) = min(h[rk[x] + 1 ... rk[y]])
    // ==============================

    void buildRMQ() {
        lg[1] = 0;

        for (int i = 2; i <= n; i++) {
            lg[i] = lg[i >> 1] + 1;
        }

        // st[0][i] là đoạn dài 1, chính là h[i]
        for (int i = 1; i <= n; i++) {
            st[0][i] = h[i];
        }

        // st[k][i] = min trên đoạn h[i ... i + 2^k - 1]
        for (int k = 1; k < LOG; k++) {
            for (int i = 1; i + (1 << k) - 1 <= n; i++) {
                st[k][i] = min(st[k - 1][i],
                               st[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    // ==============================
    // Query LCP của hai suffix bắt đầu tại x và y
    // ==============================

    int lcp(int x, int y) {
        // Hai suffix giống nhau hoàn toàn
        if (x == y) return n - x + 1;

        int l = rk[x];
        int r = rk[y];

        if (l > r) swap(l, r);

        // Cần lấy min trên h[l + 1 ... r]
        l++;

        int k = lg[r - l + 1];

        return min(st[k][l],
                   st[k][r - (1 << k) + 1]);
    }

    // ==============================
    // So sánh hai substring:
    //
    // A = s[l1 ... r1]
    // B = s[l2 ... r2]
    //
    // return -1 nếu A < B
    // return  0 nếu A = B
    // return  1 nếu A > B
    // ==============================

    int cmpSubstr(int l1, int r1, int l2, int r2) {
        int len1 = r1 - l1 + 1;
        int len2 = r2 - l2 + 1;

        // Độ dài phần đầu giống nhau của hai substring
        int common = lcp(l1, l2);
        int mn = min(len1, len2);

        // Nếu giống nhau ít nhất bằng độ dài substring ngắn hơn
        // thì substring ngắn hơn nhỏ hơn theo thứ tự từ điển
        if (common >= mn) {
            if (len1 == len2) return 0;
            return len1 < len2 ? -1 : 1;
        }

        // Nếu khác nhau tại vị trí common + 1,
        // so sánh ký tự đầu tiên khác nhau đó
        char c1 = s[l1 + common];
        char c2 = s[l2 + common];

        return c1 < c2 ? -1 : 1;
    }

    // ==============================
    // Đếm số substring phân biệt
    //
    // Mỗi suffix sa[i] có n - sa[i] + 1 prefix.
    // Trong đó h[i] prefix đầu đã xuất hiện ở suffix trước.
    // Số substring mới do suffix sa[i] tạo ra:
    //
    // n - sa[i] + 1 - h[i]
    // ==============================

    long long countDistinctSubstring() {
        long long ans = 0;

        for (int i = 1; i <= n; i++) {
            int lenSuffix = n - sa[i] + 1;
            ans += lenSuffix - h[i];
        }

        return ans;
    }
};
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    SA sa;
    sa.build(s);

    // In suffix array
    for (int i = 1; i <= sa.n; i++) {
        cout << sa.sa[i] << ' ';
    }
    cout << '\n';

    // In mảng LCP
    // h[i] = LCP(sa[i], sa[i - 1])
    for (int i = 1; i <= sa.n; i++) {
        cout << sa.h[i] << ' ';
    }
    cout << '\n';

    // Đếm số substring phân biệt
    cout << sa.countDistinctSubstring() << '\n';

    // Ví dụ query LCP của suffix bắt đầu tại 2 và 4
    // cout << sa.lcp(2, 4) << '\n';

    return 0;
}
