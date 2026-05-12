#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int N = 200000 + 5;
const ll INF = (ll)4e18;

/*
========================================================
                SEGMENT TREE TEMPLATE - CP STYLE
========================================================

Quy ước tên:

Node    : thông tin 1 node
Seg     : segment tree
merge   : gộp 2 node
pull    : cập nhật node cha từ 2 con
push    : đẩy lazy xuống con
apply   : áp tag lazy vào node
build   : xây cây
upd     : update
get     : query

Các namespace:

basic       : point update, query sum / min / max / gcd
addlazy     : range add, query sum / min / max
setlazy     : range assign, query sum / min / max
maxsub      : max subarray sum
tupletree   : node dùng tuple<sum, min, max>
matrixseg   : segment tree nhân ma trận
multisetseg : segment tree of multiset, update + lower_bound trong đoạn
mergesort   : merge sort tree, static count <= x
dynamicseg  : dynamic segment tree trên miền lớn
persistent  : persistent segment tree, kth trong đoạn

========================================================
*/


/*
========================================================
1. BASIC SEGMENT TREE
========================================================

Dạng bài:

    a[pos] = val
    query [l, r]

Lấy được:

    sum
    min
    max
    gcd

Cách dùng:

    basic::Seg seg;
    seg.n = n;
    for(int i = 1; i <= n; i++) cin >> seg.a[i];
    seg.build(n);

    seg.upd(pos, val);

    basic::Node ans = seg.get(l, r);
    cout << ans.sum << ' ' << ans.mn << ' ' << ans.mx << ' ' << ans.g << '\n';

*/

namespace basic {

struct Node {
    ll sum, mn, mx, g;

    Node(ll _sum = 0, ll _mn = INF, ll _mx = -INF, ll _g = 0) {
        sum = _sum;
        mn = _mn;
        mx = _mx;
        g = _g;
    }
};

struct Seg {
    int n;
    ll a[N];
    Node st[4 * N];

    Node merge(Node L, Node R) {
        Node res;

        res.sum = L.sum + R.sum;
        res.mn = min(L.mn, R.mn);
        res.mx = max(L.mx, R.mx);
        res.g = std::gcd(L.g, R.g);

        return res;
    }

    void pull(int id) {
        st[id] = merge(st[id << 1], st[id << 1 | 1]);
    }

    void build(int id, int l, int r) {
        if (l == r) {
            st[id] = Node(a[l], a[l], a[l], a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        pull(id);
    }

    void upd(int id, int l, int r, int pos, ll val) {
        if (l == r) {
            a[pos] = val;
            st[id] = Node(val, val, val, val);
            return;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid) {
            upd(id << 1, l, mid, pos, val);
        } else {
            upd(id << 1 | 1, mid + 1, r, pos, val);
        }

        pull(id);
    }

    Node get(int id, int l, int r, int u, int v) {
        if (v < l || r < u) {
            return Node(0, INF, -INF, 0);
        }

        if (u <= l && r <= v) {
            return st[id];
        }

        int mid = (l + r) >> 1;

        Node L = get(id << 1, l, mid, u, v);
        Node R = get(id << 1 | 1, mid + 1, r, u, v);

        return merge(L, R);
    }

    void build(int _n) {
        n = _n;
        build(1, 1, n);
    }

    void upd(int pos, ll val) {
        upd(1, 1, n, pos, val);
    }

    Node get(int l, int r) {
        return get(1, 1, n, l, r);
    }
};

}


/*
========================================================
2. LAZY SEGMENT TREE - RANGE ADD
========================================================

Dạng bài:

    a[l..r] += val
    query [l, r]

Lấy được:

    sum
    min
    max

Khi cộng val vào cả đoạn:

    sum += val * len
    mn += val
    mx += val

Cách dùng:

    addlazy::Seg seg;
    seg.n = n;
    for(int i = 1; i <= n; i++) cin >> seg.a[i];
    seg.build(n);

    seg.upd(l, r, val);

    auto ans = seg.get(l, r);
    cout << ans.sum << ' ' << ans.mn << ' ' << ans.mx << '\n';

*/

namespace addlazy {

struct Node {
    ll sum, mn, mx;

    Node(ll _sum = 0, ll _mn = INF, ll _mx = -INF) {
        sum = _sum;
        mn = _mn;
        mx = _mx;
    }
};

struct Seg {
    int n;
    ll a[N];
    Node st[4 * N];
    ll lazy[4 * N];

    Node merge(Node L, Node R) {
        Node res;

        res.sum = L.sum + R.sum;
        res.mn = min(L.mn, R.mn);
        res.mx = max(L.mx, R.mx);

        return res;
    }

    void pull(int id) {
        st[id] = merge(st[id << 1], st[id << 1 | 1]);
    }

    void apply(int id, int l, int r, ll val) {
        st[id].sum += val * (r - l + 1);
        st[id].mn += val;
        st[id].mx += val;
        lazy[id] += val;
    }

    void push(int id, int l, int r) {
        if (lazy[id] == 0) return;
        if (l == r) {
            lazy[id] = 0;
            return;
        }

        int mid = (l + r) >> 1;

        apply(id << 1, l, mid, lazy[id]);
        apply(id << 1 | 1, mid + 1, r, lazy[id]);

        lazy[id] = 0;
    }

    void build(int id, int l, int r) {
        lazy[id] = 0;

        if (l == r) {
            st[id] = Node(a[l], a[l], a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        pull(id);
    }

    void upd(int id, int l, int r, int u, int v, ll val) {
        if (v < l || r < u) return;

        if (u <= l && r <= v) {
            apply(id, l, r, val);
            return;
        }

        push(id, l, r);

        int mid = (l + r) >> 1;

        upd(id << 1, l, mid, u, v, val);
        upd(id << 1 | 1, mid + 1, r, u, v, val);

        pull(id);
    }

    Node get(int id, int l, int r, int u, int v) {
        if (v < l || r < u) {
            return Node(0, INF, -INF);
        }

        if (u <= l && r <= v) {
            return st[id];
        }

        push(id, l, r);

        int mid = (l + r) >> 1;

        Node L = get(id << 1, l, mid, u, v);
        Node R = get(id << 1 | 1, mid + 1, r, u, v);

        return merge(L, R);
    }

    void build(int _n) {
        n = _n;
        build(1, 1, n);
    }

    void upd(int l, int r, ll val) {
        upd(1, 1, n, l, r, val);
    }

    Node get(int l, int r) {
        return get(1, 1, n, l, r);
    }
};

}


/*
========================================================
3. LAZY SEGMENT TREE - RANGE ASSIGN
========================================================

Dạng bài:

    a[l..r] = val
    query [l, r]

Lấy được:

    sum
    min
    max

Phép assign là phép ghi đè, nên cần:

    tag[id]
    has[id]

Cách dùng:

    setlazy::Seg seg;
    seg.n = n;
    for(int i = 1; i <= n; i++) cin >> seg.a[i];
    seg.build(n);

    seg.upd(l, r, val);

    auto ans = seg.get(l, r);

*/

namespace setlazy {

struct Node {
    ll sum, mn, mx;

    Node(ll _sum = 0, ll _mn = INF, ll _mx = -INF) {
        sum = _sum;
        mn = _mn;
        mx = _mx;
    }
};

struct Seg {
    int n;
    ll a[N];
    Node st[4 * N];

    ll tag[4 * N];
    bool has[4 * N];

    Node merge(Node L, Node R) {
        Node res;

        res.sum = L.sum + R.sum;
        res.mn = min(L.mn, R.mn);
        res.mx = max(L.mx, R.mx);

        return res;
    }

    void pull(int id) {
        st[id] = merge(st[id << 1], st[id << 1 | 1]);
    }

    void apply(int id, int l, int r, ll val) {
        st[id].sum = val * (r - l + 1);
        st[id].mn = val;
        st[id].mx = val;

        tag[id] = val;
        has[id] = true;
    }

    void push(int id, int l, int r) {
        if (!has[id]) return;
        if (l == r) {
            has[id] = false;
            return;
        }

        int mid = (l + r) >> 1;

        apply(id << 1, l, mid, tag[id]);
        apply(id << 1 | 1, mid + 1, r, tag[id]);

        has[id] = false;
    }

    void build(int id, int l, int r) {
        tag[id] = 0;
        has[id] = false;

        if (l == r) {
            st[id] = Node(a[l], a[l], a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        pull(id);
    }

    void upd(int id, int l, int r, int u, int v, ll val) {
        if (v < l || r < u) return;

        if (u <= l && r <= v) {
            apply(id, l, r, val);
            return;
        }

        push(id, l, r);

        int mid = (l + r) >> 1;

        upd(id << 1, l, mid, u, v, val);
        upd(id << 1 | 1, mid + 1, r, u, v, val);

        pull(id);
    }

    Node get(int id, int l, int r, int u, int v) {
        if (v < l || r < u) {
            return Node(0, INF, -INF);
        }

        if (u <= l && r <= v) {
            return st[id];
        }

        push(id, l, r);

        int mid = (l + r) >> 1;

        Node L = get(id << 1, l, mid, u, v);
        Node R = get(id << 1 | 1, mid + 1, r, u, v);

        return merge(L, R);
    }

    void build(int _n) {
        n = _n;
        build(1, 1, n);
    }

    void upd(int l, int r, ll val) {
        upd(1, 1, n, l, r, val);
    }

    Node get(int l, int r) {
        return get(1, 1, n, l, r);
    }
};

}


/*
========================================================
4. MAX SUBARRAY SEGMENT TREE
========================================================

Dạng bài:

    update pos val
    query [l, r]: tổng đoạn con liên tiếp lớn nhất

Node lưu:

    sum  : tổng toàn đoạn
    pref : max prefix sum
    suff : max suffix sum
    best : max subarray sum

Merge:

    sum  = L.sum + R.sum
    pref = max(L.pref, L.sum + R.pref)
    suff = max(R.suff, R.sum + L.suff)
    best = max(L.best, R.best, L.suff + R.pref)

Cách dùng:

    maxsub::Seg seg;
    seg.n = n;
    for(int i = 1; i <= n; i++) cin >> seg.a[i];
    seg.build(n);

    cout << seg.get(l, r).best << '\n';

*/

namespace maxsub {

struct Node {
    ll sum, pref, suff, best;

    Node(ll val = 0) {
        sum = val;
        pref = val;
        suff = val;
        best = val;
    }
};

struct Seg {
    int n;
    ll a[N];
    Node st[4 * N];

    Node merge(Node L, Node R) {
        Node res;

        res.sum = L.sum + R.sum;
        res.pref = max(L.pref, L.sum + R.pref);
        res.suff = max(R.suff, R.sum + L.suff);
        res.best = max({L.best, R.best, L.suff + R.pref});

        return res;
    }

    void pull(int id) {
        st[id] = merge(st[id << 1], st[id << 1 | 1]);
    }

    void build(int id, int l, int r) {
        if (l == r) {
            st[id] = Node(a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        pull(id);
    }

    void upd(int id, int l, int r, int pos, ll val) {
        if (l == r) {
            a[pos] = val;
            st[id] = Node(val);
            return;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid) {
            upd(id << 1, l, mid, pos, val);
        } else {
            upd(id << 1 | 1, mid + 1, r, pos, val);
        }

        pull(id);
    }

    Node get(int id, int l, int r, int u, int v) {
        if (u <= l && r <= v) {
            return st[id];
        }

        int mid = (l + r) >> 1;

        if (v <= mid) {
            return get(id << 1, l, mid, u, v);
        }

        if (u > mid) {
            return get(id << 1 | 1, mid + 1, r, u, v);
        }

        Node L = get(id << 1, l, mid, u, v);
        Node R = get(id << 1 | 1, mid + 1, r, u, v);

        return merge(L, R);
    }

    void build(int _n) {
        n = _n;
        build(1, 1, n);
    }

    void upd(int pos, ll val) {
        upd(1, 1, n, pos, val);
    }

    Node get(int l, int r) {
        return get(1, 1, n, l, r);
    }
};

}


/*
========================================================
5. TUPLE SEGMENT TREE
========================================================

Dạng bài:

    node đơn giản gồm nhiều thuộc tính nhỏ

Ví dụ:

    tuple<sum, min, max>

Struct thường dễ đọc hơn tuple.
Nhưng tuple có thể dùng khi muốn code nhanh.

Cách dùng:

    tupletree::Seg seg;
    ...
    auto [sum, mn, mx] = seg.get(l, r);

*/

namespace tupletree {

struct Seg {
    int n;
    ll a[N];

    // Node = tuple<sum, min, max>
    tuple<ll, ll, ll> st[4 * N];

    tuple<ll, ll, ll> merge(tuple<ll, ll, ll> L, tuple<ll, ll, ll> R) {
        auto [sumL, mnL, mxL] = L;
        auto [sumR, mnR, mxR] = R;

        return {
            sumL + sumR,
            min(mnL, mnR),
            max(mxL, mxR)
        };
    }

    void pull(int id) {
        st[id] = merge(st[id << 1], st[id << 1 | 1]);
    }

    void build(int id, int l, int r) {
        if (l == r) {
            st[id] = {a[l], a[l], a[l]};
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        pull(id);
    }

    tuple<ll, ll, ll> get(int id, int l, int r, int u, int v) {
        if (v < l || r < u) {
            return {0, INF, -INF};
        }

        if (u <= l && r <= v) {
            return st[id];
        }

        int mid = (l + r) >> 1;

        return merge(
            get(id << 1, l, mid, u, v),
            get(id << 1 | 1, mid + 1, r, u, v)
        );
    }

    void build(int _n) {
        n = _n;
        build(1, 1, n);
    }

    tuple<ll, ll, ll> get(int l, int r) {
        return get(1, 1, n, l, r);
    }
};

}


/*
========================================================
6. MATRIX SEGMENT TREE
========================================================

Dạng bài:

    Mỗi vị trí i có một ma trận A[i].
    Query [l, r]:

        A[l] * A[l + 1] * ... * A[r]

Lưu ý:

    nhân ma trận không giao hoán
    merge(L, R) = L * R

Ứng dụng:

    DP chuyển trạng thái trên đoạn
    HLD + Segment Tree + Matrix
    automaton transition

Cách dùng:

    matrixseg::Seg seg;
    seg.a[i] = mat;
    seg.build(n);

    matrixseg::Mat res = seg.get(l, r);

*/

namespace matrixseg {

const ll MOD = 1000000007;
const int K = 2;

struct Mat {
    ll a[K][K];

    Mat(bool ident = false) {
        memset(a, 0, sizeof a);

        if (ident) {
            for (int i = 0; i < K; i++) {
                a[i][i] = 1;
            }
        }
    }
};

Mat operator * (const Mat &A, const Mat &B) {
    Mat C;

    for (int i = 0; i < K; i++) {
        for (int k = 0; k < K; k++) {
            if (!A.a[i][k]) continue;

            for (int j = 0; j < K; j++) {
                C.a[i][j] = (C.a[i][j] + A.a[i][k] * B.a[k][j]) % MOD;
            }
        }
    }

    return C;
}

struct Seg {
    int n;
    Mat a[N];
    Mat st[4 * N];

    Mat merge(Mat L, Mat R) {
        return L * R;
    }

    void pull(int id) {
        st[id] = merge(st[id << 1], st[id << 1 | 1]);
    }

    void build(int id, int l, int r) {
        if (l == r) {
            st[id] = a[l];
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        pull(id);
    }

    void upd(int id, int l, int r, int pos, Mat val) {
        if (l == r) {
            a[pos] = val;
            st[id] = val;
            return;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid) {
            upd(id << 1, l, mid, pos, val);
        } else {
            upd(id << 1 | 1, mid + 1, r, pos, val);
        }

        pull(id);
    }

    Mat get(int id, int l, int r, int u, int v) {
        if (v < l || r < u) {
            return Mat(true); // ma trận đơn vị
        }

        if (u <= l && r <= v) {
            return st[id];
        }

        int mid = (l + r) >> 1;

        Mat L = get(id << 1, l, mid, u, v);
        Mat R = get(id << 1 | 1, mid + 1, r, u, v);

        return merge(L, R);
    }

    void build(int _n) {
        n = _n;
        build(1, 1, n);
    }

    void upd(int pos, Mat val) {
        upd(1, 1, n, pos, val);
    }

    Mat get(int l, int r) {
        return get(1, 1, n, l, r);
    }
};

}


/*
========================================================
7. SEGMENT TREE OF MULTISET
========================================================

Dạng bài:

    a[pos] = val
    query [l, r], x:
        tìm giá trị nhỏ nhất >= x trong a[l..r]

Mỗi node lưu multiset các giá trị trong đoạn.

Complexity:

    build  O(n log n)
    update O(log^2 n)
    query  O(log^2 n)

Cách dùng:

    multisetseg::Seg seg;
    ...
    ll ans = seg.get(l, r, x);

Nếu ans == INF nghĩa là không tồn tại.

*/

namespace multisetseg {

struct Seg {
    int n;
    ll a[N];
    multiset<ll> st[4 * N];

    void build(int id, int l, int r) {
        st[id].clear();

        if (l == r) {
            st[id].insert(a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        for (ll x : st[id << 1]) {
            st[id].insert(x);
        }

        for (ll x : st[id << 1 | 1]) {
            st[id].insert(x);
        }
    }

    void upd(int id, int l, int r, int pos, ll oldVal, ll newVal) {
        auto it = st[id].find(oldVal);

        if (it != st[id].end()) {
            st[id].erase(it);
        }

        st[id].insert(newVal);

        if (l == r) return;

        int mid = (l + r) >> 1;

        if (pos <= mid) {
            upd(id << 1, l, mid, pos, oldVal, newVal);
        } else {
            upd(id << 1 | 1, mid + 1, r, pos, oldVal, newVal);
        }
    }

    // Trả về giá trị nhỏ nhất >= x trong đoạn [u, v]
    ll get(int id, int l, int r, int u, int v, ll x) {
        if (v < l || r < u) {
            return INF;
        }

        if (u <= l && r <= v) {
            auto it = st[id].lower_bound(x);

            if (it == st[id].end()) {
                return INF;
            }

            return *it;
        }

        int mid = (l + r) >> 1;

        return min(
            get(id << 1, l, mid, u, v, x),
            get(id << 1 | 1, mid + 1, r, u, v, x)
        );
    }

    void build(int _n) {
        n = _n;
        build(1, 1, n);
    }

    void upd(int pos, ll val) {
        ll oldVal = a[pos];
        a[pos] = val;

        upd(1, 1, n, pos, oldVal, val);
    }

    ll get(int l, int r, ll x) {
        return get(1, 1, n, l, r, x);
    }
};

}


/*
========================================================
8. MERGE SORT TREE
========================================================

Dạng bài static, không update:

    query [l, r], x:
        đếm số phần tử <= x trong a[l..r]

Có thể suy ra:

    count < x
    count > x
    count >= x

Complexity:

    build O(n log n)
    query O(log^2 n)

Cách dùng:

    mergesort::Seg seg;
    ...
    int cnt = seg.le(l, r, x);

*/

namespace mergesort {

struct Seg {
    int n;
    ll a[N];
    vector<ll> st[4 * N];

    void build(int id, int l, int r) {
        if (l == r) {
            st[id] = {a[l]};
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        st[id].resize(st[id << 1].size() + st[id << 1 | 1].size());

        merge(
            st[id << 1].begin(), st[id << 1].end(),
            st[id << 1 | 1].begin(), st[id << 1 | 1].end(),
            st[id].begin()
        );
    }

    // Đếm số phần tử <= x trong [u, v]
    int le(int id, int l, int r, int u, int v, ll x) {
        if (v < l || r < u) {
            return 0;
        }

        if (u <= l && r <= v) {
            return upper_bound(st[id].begin(), st[id].end(), x) - st[id].begin();
        }

        int mid = (l + r) >> 1;

        return le(id << 1, l, mid, u, v, x)
             + le(id << 1 | 1, mid + 1, r, u, v, x);
    }

    int lt(int l, int r, ll x) {
        return le(l, r, x - 1);
    }

    int gt(int l, int r, ll x) {
        return (r - l + 1) - le(l, r, x);
    }

    int ge(int l, int r, ll x) {
        return (r - l + 1) - lt(l, r, x);
    }

    void build(int _n) {
        n = _n;
        build(1, 1, n);
    }

    int le(int l, int r, ll x) {
        return le(1, 1, n, l, r, x);
    }
};

}


/*
========================================================
9. DYNAMIC SEGMENT TREE
========================================================

Dạng bài:

    miền chỉ số rất lớn, ví dụ [1, 1e9]
    số update ít

Không build toàn bộ cây.
Node nào cần mới tạo.

Ví dụ:

    dynamicseg::Seg seg;
    int root = 1;
    int L = 1, R = 1e9;

    seg.upd(root, L, R, pos, val);
    ll ans = seg.get(root, L, R, ql, qr);

*/

namespace dynamicseg {

struct Node {
    ll sum;
    int lc, rc;

    Node() {
        sum = 0;
        lc = rc = 0;
    }
};

struct Seg {
    vector<Node> st;

    Seg() {
        st.push_back(Node()); // node 0 rỗng
        st.push_back(Node()); // root = 1
    }

    int newnode() {
        st.push_back(Node());
        return (int)st.size() - 1;
    }

    void pull(int id) {
        ll L = st[id].lc ? st[st[id].lc].sum : 0;
        ll R = st[id].rc ? st[st[id].rc].sum : 0;

        st[id].sum = L + R;
    }

    // Cộng val vào vị trí pos trên miền [l, r]
    void upd(int id, int l, int r, int pos, ll val) {
        if (l == r) {
            st[id].sum += val;
            return;
        }

        int mid = l + ((r - l) >> 1);

        if (pos <= mid) {
            if (!st[id].lc) {
                st[id].lc = newnode();
            }

            upd(st[id].lc, l, mid, pos, val);
        } else {
            if (!st[id].rc) {
                st[id].rc = newnode();
            }

            upd(st[id].rc, mid + 1, r, pos, val);
        }

        pull(id);
    }

    ll get(int id, int l, int r, int u, int v) {
        if (!id || v < l || r < u) {
            return 0;
        }

        if (u <= l && r <= v) {
            return st[id].sum;
        }

        int mid = l + ((r - l) >> 1);

        return get(st[id].lc, l, mid, u, v)
             + get(st[id].rc, mid + 1, r, u, v);
    }
};

}


/*
========================================================
10. PERSISTENT SEGMENT TREE
========================================================

Dạng bài phổ biến:

    kth smallest trong a[l..r]

Cách làm:

    nén tọa độ a[i] thành id 1..M
    root[i] = version chứa a[1..i]

Query đoạn [l, r]:

    dùng root[r] - root[l - 1]

Complexity:

    build n version: O(n log n)
    kth: O(log n)

Cách dùng:

    persistent::Seg pst;

    pst.root[0] = 0;
    for(int i = 1; i <= n; i++) {
        pst.root[i] = pst.upd(pst.root[i - 1], 1, M, id[i]);
    }

    int idAns = pst.kth(pst.root[l - 1], pst.root[r], 1, M, k);
    answer = vals[idAns - 1];

*/

namespace persistent {

struct Node {
    int lc, rc, sum;

    Node() {
        lc = rc = sum = 0;
    }
};

struct Seg {
    vector<Node> st;
    int root[N];

    Seg() {
        st.push_back(Node()); // node 0 rỗng
    }

    int newnode() {
        st.push_back(Node());
        return (int)st.size() - 1;
    }

    // Tạo version mới từ pre, thêm 1 vào pos
    int upd(int pre, int l, int r, int pos) {
        int cur = newnode();

        st[cur] = st[pre];
        st[cur].sum++;

        if (l == r) {
            return cur;
        }

        int mid = (l + r) >> 1;

        if (pos <= mid) {
            st[cur].lc = upd(st[pre].lc, l, mid, pos);
        } else {
            st[cur].rc = upd(st[pre].rc, mid + 1, r, pos);
        }

        return cur;
    }

    // kth nhỏ nhất trong hiệu version R - L
    int kth(int Lroot, int Rroot, int l, int r, int k) {
        if (l == r) {
            return l;
        }

        int mid = (l + r) >> 1;

        int cntLeft = st[st[Rroot].lc].sum - st[st[Lroot].lc].sum;

        if (k <= cntLeft) {
            return kth(st[Lroot].lc, st[Rroot].lc, l, mid, k);
        } else {
            return kth(st[Lroot].rc, st[Rroot].rc, mid + 1, r, k - cntLeft);
        }
    }

    // Đếm số phần tử <= xId trong đoạn version R - L
    int le(int Lroot, int Rroot, int l, int r, int xId) {
        if (xId < l) return 0;

        if (r <= xId) {
            return st[Rroot].sum - st[Lroot].sum;
        }

        int mid = (l + r) >> 1;

        return le(st[Lroot].lc, st[Rroot].lc, l, mid, xId)
             + le(st[Lroot].rc, st[Rroot].rc, mid + 1, r, xId);
    }
};

}


/*
========================================================
11. QUICK USAGE SHEET
========================================================

1. Sum / min / max / gcd

    basic::Seg seg;
    seg.n = n;
    for(int i = 1; i <= n; i++) cin >> seg.a[i];
    seg.build(n);

    seg.upd(pos, val);

    basic::Node ans = seg.get(l, r);
    cout << ans.sum << ' ' << ans.mn << ' ' << ans.mx << ' ' << ans.g << '\n';

--------------------------------------------------------

2. Range add

    addlazy::Seg seg;
    seg.n = n;
    for(int i = 1; i <= n; i++) cin >> seg.a[i];
    seg.build(n);

    seg.upd(l, r, val);

    auto ans = seg.get(l, r);

--------------------------------------------------------

3. Range assign

    setlazy::Seg seg;
    seg.n = n;
    for(int i = 1; i <= n; i++) cin >> seg.a[i];
    seg.build(n);

    seg.upd(l, r, val);

    auto ans = seg.get(l, r);

--------------------------------------------------------

4. Max subarray

    maxsub::Seg seg;
    seg.n = n;
    for(int i = 1; i <= n; i++) cin >> seg.a[i];
    seg.build(n);

    cout << seg.get(l, r).best << '\n';

--------------------------------------------------------

5. Tuple node

    tupletree::Seg seg;
    ...
    auto [sum, mn, mx] = seg.get(l, r);

--------------------------------------------------------

6. Matrix segment tree

    matrixseg::Seg seg;
    ...
    matrixseg::Mat res = seg.get(l, r);

--------------------------------------------------------

7. Segment tree of multiset

    multisetseg::Seg seg;
    ...
    seg.upd(pos, val);
    ll ans = seg.get(l, r, x);

--------------------------------------------------------

8. Merge sort tree

    mergesort::Seg seg;
    ...
    int cnt = seg.le(l, r, x);

--------------------------------------------------------

9. Dynamic segment tree

    dynamicseg::Seg seg;
    int root = 1;
    int L = 1, R = 1000000000;

    seg.upd(root, L, R, pos, val);
    ll ans = seg.get(root, L, R, ql, qr);

--------------------------------------------------------

10. Persistent segment tree

    persistent::Seg pst;

    pst.root[0] = 0;

    for(int i = 1; i <= n; i++) {
        pst.root[i] = pst.upd(pst.root[i - 1], 1, M, id[i]);
    }

    int idAns = pst.kth(pst.root[l - 1], pst.root[r], 1, M, k);
    answer = vals[idAns - 1];

========================================================
*/


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /*
    Đây là file template tổng hợp.

    Khi làm bài thật:
    - giữ lại namespace cần dùng
    - xóa các namespace không dùng nếu muốn code ngắn hơn
    */

    return 0;
} 
