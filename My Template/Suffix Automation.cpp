#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200000 + 5;
const int SIG = 26;

// Suffix Automaton của một chuỗi độ dài n có tối đa 2n - 1 trạng thái
// Vì vậy MAXNODE nên >= 2 * MAXN
const int MAXNODE = MAXN * 2;

struct SAM {
    int tot;                    // số lượng state hiện tại
    int last;                   // state đại diện cho toàn bộ string hiện tại

    int nxt[MAXNODE][SIG];      // nxt[u][c] = state sau khi thêm ký tự c từ state u
    int link[MAXNODE];          // suffix link của state u
    int len[MAXNODE];           // len[u] = độ dài lớn nhất của substring thuộc state u

    long long occ[MAXNODE];     // occ[u] = số lần xuất hiện của các substring thuộc state u
    int firstpos[MAXNODE];      // một vị trí kết thúc bất kỳ của state u trong chuỗi gốc

    vector<int> tree[MAXNODE];  // cây suffix link, dùng để cộng occ từ con lên cha

    // Chuyển ký tự sang số
    int get(char c) {
        return c - 'a';
    }

    // Khởi tạo SAM rỗng
    void init() {
        tot = last = 1;

        link[1] = 0;
        len[1] = 0;
        occ[1] = 0;
        firstpos[1] = 0;

        for (int i = 0; i < MAXNODE; i++) {
            link[i] = len[i] = firstpos[i] = 0;
            occ[i] = 0;
            tree[i].clear();

            for (int c = 0; c < SIG; c++) {
                nxt[i][c] = 0;
            }
        }

        tot = last = 1;
    }

    // Tạo một state mới
    int newNode() {
        ++tot;

        link[tot] = 0;
        len[tot] = 0;
        occ[tot] = 0;
        firstpos[tot] = 0;

        for (int c = 0; c < SIG; c++) {
            nxt[tot][c] = 0;
        }

        return tot;
    }

    // Thêm một ký tự vào cuối chuỗi hiện tại
    void extend(char ch) {
        int c = get(ch);

        // cur là state mới đại diện cho toàn bộ chuỗi sau khi thêm ch
        int cur = newNode();
        len[cur] = len[last] + 1;

        // Vì cur tương ứng với prefix hiện tại của chuỗi gốc,
        // nên state này xuất hiện ít nhất 1 lần.
        occ[cur] = 1;

        // firstpos[cur] là vị trí kết thúc của prefix hiện tại
        firstpos[cur] = len[cur];

        int p = last;

        // Đi ngược theo suffix link,
        // thêm transition bằng ch cho các state chưa có cạnh ch.
        while (p && !nxt[p][c]) {
            nxt[p][c] = cur;
            p = link[p];
        }

        // Nếu không còn p nào, cur nối suffix link về root
        if (!p) {
            link[cur] = 1;
        } else {
            int q = nxt[p][c];

            // Nếu q đã vừa đúng là state tiếp theo của p khi thêm 1 ký tự,
            // thì cur có thể link trực tiếp về q.
            if (len[p] + 1 == len[q]) {
                link[cur] = q;
            } else {
                // Ngược lại phải clone q.
                // clone đại diện cho phần chung bị tách ra.
                int clone = newNode();

                len[clone] = len[p] + 1;
                link[clone] = link[q];

                // clone copy toàn bộ transition của q
                for (int x = 0; x < SIG; x++) {
                    nxt[clone][x] = nxt[q][x];
                }

                // clone không phải state sinh trực tiếp từ một prefix mới,
                // nên occ[clone] ban đầu = 0.
                occ[clone] = 0;

                // firstpos của clone giống q vì clone đại diện cùng một tập endpos tạm thời
                firstpos[clone] = firstpos[q];

                // Các state đang trỏ bằng ch tới q nhưng thực ra nên trỏ tới clone
                while (p && nxt[p][c] == q) {
                    nxt[p][c] = clone;
                    p = link[p];
                }

                // Cả q và cur đều có suffix link về clone
                link[q] = link[cur] = clone;
            }
        }

        // Cập nhật last thành state đại diện cho toàn bộ chuỗi hiện tại
        last = cur;
    }

    // Build SAM từ một string
    void build(const string &s) {
        init();

        for (char c : s) {
            extend(c);
        }
    }

    // Build cây suffix link
    void buildTree() {
        for (int i = 1; i <= tot; i++) {
            tree[i].clear();
        }

        for (int u = 2; u <= tot; u++) {
            tree[link[u]].push_back(u);
        }
    }

    // DFS trên suffix link tree để cộng số lần xuất hiện
    //
    // occ[cur] ban đầu chỉ được cộng ở các state tương ứng prefix thật.
    // Khi cộng từ con lên cha:
    // occ[u] = số lần xuất hiện của mọi substring thuộc state u.
    void dfsOcc(int u) {
        for (int v : tree[u]) {
            dfsOcc(v);
            occ[u] += occ[v];
        }
    }

    // Chuẩn bị occ[u]
    void buildOcc() {
        buildTree();
        dfsOcc(1);
    }

    // Kiểm tra pattern có phải substring của chuỗi gốc không
    bool contain(const string &p) {
        int u = 1;

        for (char ch : p) {
            int c = get(ch);

            if (!nxt[u][c]) {
                return false;
            }

            u = nxt[u][c];
        }

        return true;
    }

    // Đếm số lần xuất hiện của pattern trong chuỗi gốc
    //
    // Cần gọi buildOcc() trước.
    long long countOcc(const string &p) {
        int u = 1;

        for (char ch : p) {
            int c = get(ch);

            if (!nxt[u][c]) {
                return 0;
            }

            u = nxt[u][c];
        }

        return occ[u];
    }

    // Đếm số substring phân biệt
    //
    // Mỗi state u đại diện cho các substring có độ dài:
    // len[link[u]] + 1 ... len[u]
    //
    // Số substring mới do state u đóng góp:
    // len[u] - len[link[u]]
    long long countDistinctSubstring() {
        long long ans = 0;

        for (int u = 2; u <= tot; u++) {
            ans += len[u] - len[link[u]];
        }

        return ans;
    }

    // Tính tổng độ dài các substring phân biệt
    //
    // State u đóng góp các độ dài:
    // L = len[link[u]] + 1
    // R = len[u]
    //
    // Tổng = L + (L + 1) + ... + R
    long long sumLengthDistinctSubstring() {
        long long ans = 0;

        for (int u = 2; u <= tot; u++) {
            long long L = len[link[u]] + 1;
            long long R = len[u];

            ans += (L + R) * (R - L + 1) / 2;
        }

        return ans;
    }

    // Tìm độ dài longest common substring giữa chuỗi gốc của SAM và string t
    int longestCommonSubstring(const string &t) {
        int u = 1;      // state hiện tại trong SAM
        int cur = 0;    // độ dài match hiện tại
        int ans = 0;    // độ dài LCS tốt nhất

        for (char ch : t) {
            int c = get(ch);

            // Nếu có cạnh c, kéo dài match hiện tại
            if (nxt[u][c]) {
                u = nxt[u][c];
                cur++;
            } else {
                // Nếu không có cạnh c, nhảy suffix link cho đến khi tìm được cạnh c
                while (u && !nxt[u][c]) {
                    u = link[u];
                }

                // Không tìm được, reset về root
                if (!u) {
                    u = 1;
                    cur = 0;
                } else {
                    // Tìm được state u có cạnh c
                    // Độ dài match hiện tại trước khi thêm c là len[u]
                    cur = len[u] + 1;
                    u = nxt[u][c];
                }
            }

            ans = max(ans, cur);
        }

        return ans;
    }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    SAM sam;
    sam.build(s);

    cout << sam.countDistinctSubstring() << '\n';
    cout << sam.sumLengthDistinctSubstring() << '\n';

    sam.buildOcc();

    string p;
    cin >> p;

    cout << sam.contain(p) << '\n';
    cout << sam.countOcc(p) << '\n';

    return 0;
}
