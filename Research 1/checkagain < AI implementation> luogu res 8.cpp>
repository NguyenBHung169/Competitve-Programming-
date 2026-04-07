#include <bits/stdc++.h>
using namespace std;

#define L(i,a,b) for (int i = (a); i <= (b); ++i)
using ll = long long;

const int MOD = 1000000007;
const int MAXM = 12;
const int MAXC = 6;
const int MAXS = 1 << 12;

/*
    Bài toán:
    - Có bàn cờ n * m
    - Mỗi ô có 3 trạng thái: X, W, B
    - Cho q mẫu, mỗi mẫu có kích thước 2 * c
    - Cần đếm số bàn cờ sao cho tồn tại ít nhất 1 cửa sổ 2*c khớp đúng mẫu

    Ý tưởng:
    - Đếm phần bù:
        total = 3^(n*m)
        bad   = số bàn cờ KHÔNG chứa mẫu ở đâu cả
        ans   = total - bad

    - bad được tính bằng:
        KMP + profile DP
*/

/*
    p[0][i] = ký tự thứ i của hàng 1 mẫu
    p[1][i] = ký tự thứ i của hàng 2 mẫu

    Mã hóa:
    X -> 0
    W -> 1
    B -> 2
*/
int n, m, c, q;
int p[2][MAXC + 1];

/*
    fail_[row][i] = failure function của KMP cho hàng row
    go_[row][st][ch] = nếu hiện tại đã match được st ký tự của hàng row,
                       rồi đọc thêm ký tự ch, thì trạng thái KMP mới là gì
*/
int fail_[2][MAXC + 1];
int go_[2][MAXC + 1][3];

/*
    dp[cur][mask][a][b]

    mask:
    - có độ dài W = m-c+1 bit
    - bit pos = 1 nghĩa là:
        ở HÀNG TRÊN, đoạn dài c bắt đầu tại cột pos đã khớp TRỌN hàng 1 của mẫu

    a:
    - trạng thái KMP hiện tại của hàng 1 mẫu trên HÀNG ĐANG QUÉT

    b:
    - trạng thái KMP hiện tại của hàng 2 mẫu trên HÀNG ĐANG QUÉT

    Giá trị:
    - số cách điền các ô đã quét để đi tới state này
*/
int dp[2][MAXS][MAXC + 1][MAXC + 1];

/*
    lũy thừa nhanh: tính a^e mod MOD
*/
int pw(int a, int e) {
    int r = 1;
    while (e) {
        if (e & 1) r = 1LL * r * a % MOD;
        a = 1LL * a * a % MOD;
        e >>= 1;
    }
    return r;
}

/*
    đổi ký tự mẫu sang số
*/
int cv(char ch) {
    if (ch == 'X') return 0;
    if (ch == 'W') return 1;
    return 2; // 'B'
}

/*
    build KMP cho 1 hàng mẫu

    id = 0 hoặc 1:
    - 0 là hàng trên của mẫu
    - 1 là hàng dưới của mẫu
*/
void build_kmp(int id) {
    fail_[id][0] = 0;

    // build failure function chuẩn
    for (int i = 2, j = 0; i <= c; i++) {
        while (j && p[id][j + 1] != p[id][i]) j = fail_[id][j];
        if (p[id][j + 1] == p[id][i]) j++;
        fail_[id][i] = j;
    }

    /*
        go_[id][st][ch]:
        - nếu đang ở trạng thái KMP st
        - đọc thêm ký tự ch
        - thì sang trạng thái nào

        Ta precompute sẵn để khi DP chỉ tra bảng O(1).
    */
    for (int st = 0; st <= c; st++) {
        for (int ch = 0; ch < 3; ch++) {
            if (st == c) {
                // nếu đang ở c thì theo đúng KMP phải lùi về fail[c] rồi chạy tiếp
                int j = fail_[id][c];
                while (j && p[id][j + 1] != ch) j = fail_[id][j];
                if (p[id][j + 1] == ch) j++;
                go_[id][st][ch] = j;
            } else {
                int j = st;
                while (j && p[id][j + 1] != ch) j = fail_[id][j];
                if (p[id][j + 1] == ch) j++;
                go_[id][st][ch] = j;
            }
        }
    }
}

int solve_one() {
    /*
        W = số vị trí bắt đầu có thể của một cửa sổ dài c trên 1 hàng
          = m-c+1
    */
    int W = m - c + 1;
    int LIM = 1 << W;

    memset(dp, 0, sizeof(dp));
    int cur = 0, nxt = 1;

    /*
        Ban đầu chưa điền gì:
        - mask = 0 (hàng trên chưa có thông tin match nào)
        - a = 0, b = 0 (KMP chưa match gì)
        - có 1 cách
    */
    dp[cur][0][0][0] = 1;

    /*
        Quét từng hàng
    */
    for (int i = 0; i < n; i++) {

        /*
            BƯỚC 1: sang hàng mới

            Khi vừa sang 1 hàng mới:
            - trạng thái KMP a,b của HÀNG HIỆN TẠI phải reset về 0
            - nhưng mask phải giữ nguyên

            Vì mask đang lưu thông tin:
            "ở hàng trên, vị trí nào đã match trọn hàng 1"
            thông tin này sẽ được dùng để ghép với hàng hiện tại.
        */
        memset(dp[nxt], 0, sizeof(dp[nxt]));
        for (int mask = 0; mask < LIM; mask++) {
            for (int a = 0; a <= c; a++) {
                for (int b = 0; b <= c; b++) {
                    int val = dp[cur][mask][a][b];
                    if (!val) continue;

                    dp[nxt][mask][0][0] += val;
                    if (dp[nxt][mask][0][0] >= MOD) dp[nxt][mask][0][0] -= MOD;
                }
            }
        }
        swap(cur, nxt);

        /*
            BƯỚC 2: quét từng cột trong hàng i
        */
        for (int j = 0; j < m; j++) {
            memset(dp[nxt], 0, sizeof(dp[nxt]));

            for (int mask = 0; mask < LIM; mask++) {
                for (int a = 0; a < c; a++) {
                    for (int b = 0; b < c; b++) {
                        int val = dp[cur][mask][a][b];
                        if (!val) continue;

                        /*
                            Thử điền ô (i,j) bằng 1 trong 3 ký tự:
                            X / W / B
                        */
                        for (int ch = 0; ch < 3; ch++) {

                            // cập nhật KMP cho hàng 1 và hàng 2 mẫu
                            int ta = go_[0][a][ch];
                            int tb = go_[1][b][ch];

                            /*
                                hitA = hàng hiện tại vừa match TRỌN hàng 1 mẫu
                                hitB = hàng hiện tại vừa match TRỌN hàng 2 mẫu
                            */
                            bool hitA = (ta == c);
                            bool hitB = (tb == c);

                            /*
                                Sau khi match trọn, để tiếp tục KMP cho các ký tự sau,
                                ta phải lùi về fail[c]
                            */
                            int na = hitA ? fail_[0][c] : ta;
                            int nb = hitB ? fail_[1][c] : tb;

                            int nmask = mask;

                            /*
                                Chỉ khi j >= c-1 thì cửa sổ dài c mới "kết thúc" tại cột j,
                                tức là ta mới biết được đoạn bắt đầu ở đâu.

                                pos = vị trí bắt đầu của đoạn dài c vừa kết thúc tại j
                            */
                            if (j >= c - 1) {
                                int pos = j - (c - 1);

                                /*
                                    old = ở HÀNG TRÊN,
                                          đoạn bắt đầu tại pos có match trọn hàng 1 hay không
                                */
                                int old = (mask >> pos) & 1;

                                /*
                                    Nếu:
                                    - hàng hiện tại match trọn hàng 2 tại pos
                                    - hàng trên đã match trọn hàng 1 tại pos

                                    => ta vừa tạo ra đúng 1 mẫu 2*c hoàn chỉnh
                                    => trạng thái này là "xấu" đối với phần bù
                                    => loại luôn
                                */
                                if (hitB && old) continue;

                                /*
                                    Bây giờ ta cần cập nhật bit pos cho HÀNG HIỆN TẠI,
                                    để sau khi sang hàng sau nó trở thành "thông tin hàng trên".

                                    Nếu hàng hiện tại match trọn hàng 1 tại pos:
                                        set bit pos = 1
                                    Ngược lại:
                                        set bit pos = 0
                                */
                                if (hitA) nmask |= (1 << pos);
                                else nmask &= ~(1 << pos);
                            }

                            /*
                                cộng dồn số cách
                            */
                            int &x = dp[nxt][nmask][na][nb];
                            x += val;
                            if (x >= MOD) x -= MOD;
                        }
                    }
                }
            }

            swap(cur, nxt);
        }
    }

    /*
        Sau khi quét xong toàn bộ bàn cờ:
        dp[cur] chứa số bàn cờ KHÔNG sinh ra mẫu nào cả
    */
    int bad = 0;
    for (int mask = 0; mask < LIM; mask++) {
        for (int a = 0; a < c; a++) {
            for (int b = 0; b < c; b++) {
                bad += dp[cur][mask][a][b];
                if (bad >= MOD) bad -= MOD;
            }
        }
    }

    int all = pw(3, n * m);
    int ans = all - bad;
    if (ans < 0) ans += MOD;
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> c >> q;

    while (q--) {
        string s, t;
        cin >> s >> t;

        /*
            đọc 2 hàng của mẫu
            p[0] = hàng 1
            p[1] = hàng 2
        */
        L(i,1,c) p[0][i] = cv(s[i - 1]);
        L(i,1,c) p[1][i] = cv(t[i - 1]);

        build_kmp(0);
        build_kmp(1);

        cout << solve_one() << '\n';
    }

    return 0;
}
