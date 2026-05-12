#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int N = 200000 + 5;
const ll INF = (ll)4e18;

/*
========================================================
        TEMPLATE TỔNG HỢP
        SEGMENT TREE + HLD + CENTROID DECOMPOSITION
========================================================

Quy ước tên:

Node    : thông tin 1 node segment tree
Seg     : struct segment tree / cấu trúc chính
merge   : gộp 2 node
pull    : cập nhật node cha từ 2 con
push    : đẩy lazy
apply   : áp lazy vào node
upd     : update
get     : query
dfs     : duyệt cây
build   : build cấu trúc
lca     : lowest common ancestor
dist    : khoảng cách trên cây

Các namespace:

1. segtree
   - Segment Tree cơ bản
   - point update
   - range query sum/min/max
   - lazy range add

2. hld
   - Heavy-Light Decomposition
   - query/update trên đường đi u-v
   - query/update trên subtree
   - dùng Segment Tree bên trong

3. centroid
   - Centroid Decomposition
   - mẫu bài dynamic red node:
        update u: tô đỏ node u
        query u : khoảng cách nhỏ nhất từ u tới node đỏ gần nhất
   - có LCA để tính distance nhanh

========================================================
*/


/*
========================================================
1. SEGMENT TREE
========================================================

Có 2 bản:

A. Seg
   - point update
   - range query
   - Node lưu sum/min/max

B. Lazy
   - range add
   - range query sum/min/max

========================================================
*/

namespace segtree {

/*
--------------------------------------------------------
A. Segment Tree cơ bản
--------------------------------------------------------

Dạng bài:

    a[pos] = val
    query [l, r]:
        sum / min / max

Cách dùng:

    segtree::Seg seg;
    seg.n = n;
    for(int i = 1; i <= n; i++) seg.a[i] = a[i];
    seg.build(n);

    seg.upd(pos, val);

    auto ans = seg.get(l, r);
    cout << ans.sum << ' ' << ans.mn << ' ' << ans.mx << '\n';

*/

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

    // Gộp 2 node con
    Node merge(Node L, Node R) {
        Node res;

        res.sum = L.sum + R.sum;
        res.mn = min(L.mn, R.mn);
        res.mx = max(L.mx, R.mx);

        return res;
    }

    // Cập nhật node cha từ 2 con
    void pull(int id) {
        st[id] = merge(st[id << 1], st[id << 1 | 1]);
    }

    void build(int id, int l, int r) {
        if (l == r) {
            st[id] = Node(a[l], a[l], a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        pull(id);
    }

    // Gán a[pos] = val
    void upd(int id, int l, int r, int pos, ll val) {
        if (l == r) {
            a[pos] = val;
            st[id] = Node(val, val, val);
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

    // Query đoạn [u, v]
    Node get(int id, int l, int r, int u, int v) {
        if (v < l || r < u) {
            return Node(0, INF, -INF);
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


/*
--------------------------------------------------------
B. Lazy Segment Tree: Range Add
--------------------------------------------------------

Dạng bài:

    a[l..r] += val
    query [l, r]:
        sum / min / max

Cách dùng:

    segtree::Lazy seg;
    seg.n = n;
    for(int i = 1; i <= n; i++) seg.a[i] = a[i];
    seg.build(n);

    seg.upd(l, r, val);

    auto ans = seg.get(l, r);

*/

struct Lazy {
    int n;
    ll a[N];
    Node st[4 * N];
    ll lz[4 * N];

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

    // Áp cộng val vào toàn bộ node id quản lý đoạn [l, r]
    void apply(int id, int l, int r, ll val) {
        st[id].sum += val * (r - l + 1);
        st[id].mn += val;
        st[id].mx += val;

        lz[id] += val;
    }

    // Đẩy tag xuống con
    void push(int id, int l, int r) {
        if (lz[id] == 0) return;
        if (l == r) {
            lz[id] = 0;
            return;
        }

        int mid = (l + r) >> 1;

        apply(id << 1, l, mid, lz[id]);
        apply(id << 1 | 1, mid + 1, r, lz[id]);

        lz[id] = 0;
    }

    void build(int id, int l, int r) {
        lz[id] = 0;

        if (l == r) {
            st[id] = Node(a[l], a[l], a[l]);
            return;
        }

        int mid = (l + r) >> 1;

        build(id << 1, l, mid);
        build(id << 1 | 1, mid + 1, r);

        pull(id);
    }

    // Cộng val vào [u, v]
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
2. HLD + SEGMENT TREE
========================================================

Heavy-Light Decomposition dùng để biến query trên đường đi u-v
thành O(log n) đoạn liên tiếp trên mảng dfn.

HLD thường dùng cho:

1. Query đường đi u-v:
    sum / min / max trên path

2. Update đường đi u-v:
    cộng val vào tất cả node trên path

3. Query subtree u:
    subtree u sau HLD là đoạn [dfn[u], dfn[u] + sz[u] - 1]

4. Update subtree u:
    cập nhật đoạn [dfn[u], dfn[u] + sz[u] - 1]

Template dưới đây dùng lazy segment tree bên trong,
hỗ trợ:

    pathAdd(u, v, val)
    pathGet(u, v)
    subAdd(u, val)
    subGet(u)
    lca(u, v)

Node query trả về:

    sum / min / max

Lưu ý về vertex/edge:

A. Nếu giá trị nằm trên node:
    base[dfn[u]] = val[u]

B. Nếu giá trị nằm trên edge parent[u] - u:
    base[dfn[u]] = weight(parent[u], u)
    root không có edge cha, base[dfn[root]] = 0
    Khi query path edge u-v, phải bỏ LCA:
        query đoạn dfn[lca] + 1 ... dfn[x]
    Template này mặc định là NODE value.
    Phần edge query có thêm hàm pathGetEdge/pathAddEdge.

========================================================
*/

namespace hld {

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
    ll lz[4 * N];

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

        lz[id] += val;
    }

    void push(int id, int l, int r) {
        if (lz[id] == 0) return;
        if (l == r) {
            lz[id] = 0;
            return;
        }

        int mid = (l + r) >> 1;

        apply(id << 1, l, mid, lz[id]);
        apply(id << 1 | 1, mid + 1, r, lz[id]);

        lz[id] = 0;
    }

    void build(int id, int l, int r) {
        lz[id] = 0;

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
        if (l > r) return;
        upd(1, 1, n, l, r, val);
    }

    Node get(int l, int r) {
        if (l > r) return Node(0, INF, -INF);
        return get(1, 1, n, l, r);
    }
};

struct HLD {
    int n, timer;
    vector<int> g[N];

    int par[N];      // cha trực tiếp
    int dep[N];      // độ sâu
    int sz[N];       // size subtree
    int heavy[N];    // heavy child
    int head[N];     // đầu chain
    int dfn[N];      // vị trí của node trên mảng HLD
    int rev[N];      // rev[dfn[u]] = u

    ll val[N];       // giá trị gốc trên node
    Seg seg;

    void init(int _n) {
        n = _n;
        timer = 0;

        for (int i = 1; i <= n; i++) {
            g[i].clear();
            par[i] = dep[i] = sz[i] = heavy[i] = 0;
            head[i] = dfn[i] = rev[i] = 0;
            val[i] = 0;
        }
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    /*
    dfs1:
        tính par, dep, sz
        chọn heavy child là con có subtree lớn nhất
    */
    void dfs1(int u, int p) {
        par[u] = p;
        dep[u] = dep[p] + 1;
        sz[u] = 1;
        heavy[u] = 0;

        int best = 0;

        for (int v : g[u]) {
            if (v == p) continue;

            dfs1(v, u);

            sz[u] += sz[v];

            if (sz[v] > best) {
                best = sz[v];
                heavy[u] = v;
            }
        }
    }

    /*
    dfs2:
        đánh số dfn
        các node trên cùng heavy path có dfn liên tiếp
    */
    void dfs2(int u, int h) {
        head[u] = h;
        dfn[u] = ++timer;
        rev[timer] = u;

        if (heavy[u]) {
            dfs2(heavy[u], h);
        }

        for (int v : g[u]) {
            if (v == par[u] || v == heavy[u]) continue;

            dfs2(v, v);
        }
    }

    /*
    Build HLD từ root.
    Sau khi có dfn, đưa val[u] vào seg.a[dfn[u]].
    */
    void build(int root = 1) {
        dfs1(root, 0);
        dfs2(root, root);

        for (int u = 1; u <= n; u++) {
            seg.a[dfn[u]] = val[u];
        }

        seg.build(n);
    }

    /*
    LCA bằng HLD.
    Mỗi lần đưa node có head sâu hơn lên cha của head.
    */
    int lca(int u, int v) {
        while (head[u] != head[v]) {
            if (dep[head[u]] < dep[head[v]]) {
                swap(u, v);
            }

            u = par[head[u]];
        }

        return dep[u] < dep[v] ? u : v;
    }

    int distEdge(int u, int v) {
        int w = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[w];
    }

    int distNode(int u, int v) {
        return distEdge(u, v) + 1;
    }

    /*
    Query đường đi theo NODE value.
    Tức là cả u, v và LCA đều được tính.
    */
    Node pathGet(int u, int v) {
        Node res;

        while (head[u] != head[v]) {
            if (dep[head[u]] < dep[head[v]]) {
                swap(u, v);
            }

            Node cur = seg.get(dfn[head[u]], dfn[u]);
            res = seg.merge(res, cur);

            u = par[head[u]];
        }

        if (dep[u] > dep[v]) {
            swap(u, v);
        }

        Node cur = seg.get(dfn[u], dfn[v]);
        res = seg.merge(res, cur);

        return res;
    }

    /*
    Cộng val vào tất cả node trên đường đi u-v.
    */
    void pathAdd(int u, int v, ll x) {
        while (head[u] != head[v]) {
            if (dep[head[u]] < dep[head[v]]) {
                swap(u, v);
            }

            seg.upd(dfn[head[u]], dfn[u], x);

            u = par[head[u]];
        }

        if (dep[u] > dep[v]) {
            swap(u, v);
        }

        seg.upd(dfn[u], dfn[v], x);
    }

    /*
    Query subtree u.
    Vì HLD đánh dfn theo DFS order, subtree u là đoạn liên tiếp:
        [dfn[u], dfn[u] + sz[u] - 1]
    */
    Node subGet(int u) {
        return seg.get(dfn[u], dfn[u] + sz[u] - 1);
    }

    /*
    Cộng x vào toàn bộ subtree u.
    */
    void subAdd(int u, ll x) {
        seg.upd(dfn[u], dfn[u] + sz[u] - 1, x);
    }

    /*
    Point update theo kiểu cộng thêm x vào node u.
    */
    void pointAdd(int u, ll x) {
        seg.upd(dfn[u], dfn[u], x);
    }

    /*
    ====================================================
    Edge-value version
    ====================================================

    Nếu giá trị nằm trên cạnh parent[u] - u,
    ta thường lưu giá trị cạnh đó tại node u:

        seg.a[dfn[u]] = weight(parent[u], u)

    Khi query path edge u-v, cần bỏ LCA.
    Nghĩa là query:
        dfn[lca] + 1 ... dfn[node dưới]

    Các hàm dưới đây dùng cho edge-value.
    */

    Node pathGetEdge(int u, int v) {
        Node res;

        while (head[u] != head[v]) {
            if (dep[head[u]] < dep[head[v]]) {
                swap(u, v);
            }

            Node cur = seg.get(dfn[head[u]], dfn[u]);
            res = seg.merge(res, cur);

            u = par[head[u]];
        }

        if (dep[u] > dep[v]) {
            swap(u, v);
        }

        // Lúc này u là LCA, bỏ dfn[u]
        Node cur = seg.get(dfn[u] + 1, dfn[v]);
        res = seg.merge(res, cur);

        return res;
    }

    void pathAddEdge(int u, int v, ll x) {
        while (head[u] != head[v]) {
            if (dep[head[u]] < dep[head[v]]) {
                swap(u, v);
            }

            seg.upd(dfn[head[u]], dfn[u], x);

            u = par[head[u]];
        }

        if (dep[u] > dep[v]) {
            swap(u, v);
        }

        // Bỏ LCA
        seg.upd(dfn[u] + 1, dfn[v], x);
    }
};

}


/*
========================================================
3. CENTROID DECOMPOSITION
========================================================

Centroid Decomposition chia cây thành các centroid nhiều tầng.

Tính chất quan trọng:

    Mỗi node u có O(log n) tổ tiên centroid.

Vì vậy nếu ta cần xử lý query khoảng cách trên cây,
có thể update/query bằng cách đi qua các centroid ancestor.

Template mẫu bài phổ biến:

    Ban đầu chưa có node đỏ.
    update(u): tô đỏ node u.
    get(u): khoảng cách nhỏ nhất từ u đến một node đỏ.

Công thức:

    Khi tô đỏ u:
        với mỗi centroid c trên đường centroid parent của u:
            best[c] = min(best[c], dist(u, c))

    Khi hỏi u:
        ans = min(best[c] + dist(u, c))
        với mọi centroid c là tổ tiên centroid của u

Cần LCA để tính dist(u, v) nhanh.

Template này gồm:

    - LCA binary lifting
    - build centroid decomposition
    - upd(u)
    - get(u)

========================================================
*/

namespace centroid {

const int LOG = 20;

struct CD {
    int n;
    vector<int> g[N];

    /*
    Phần LCA
    */
    int dep[N];
    int up[LOG][N];

    /*
    Phần centroid decomposition
    */
    int sz[N];
    int par[N];       // centroid parent
    bool dead[N];     // node đã được chọn làm centroid chưa

    /*
    best[c] = khoảng cách nhỏ nhất từ centroid c tới một node đỏ đã update
    */
    int best[N];

    void init(int _n) {
        n = _n;

        for (int i = 1; i <= n; i++) {
            g[i].clear();

            dep[i] = 0;
            sz[i] = 0;
            par[i] = 0;
            dead[i] = false;
            best[i] = (int)1e9;

            for (int j = 0; j < LOG; j++) {
                up[j][i] = 0;
            }
        }
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    /*
    DFS LCA.
    Gọi trước khi build centroid.
    */
    void dfsLCA(int u, int p) {
        up[0][u] = p;
        dep[u] = dep[p] + 1;

        for (int i = 1; i < LOG; i++) {
            up[i][u] = up[i - 1][up[i - 1][u]];
        }

        for (int v : g[u]) {
            if (v == p) continue;

            dfsLCA(v, u);
        }
    }

    int lca(int u, int v) {
        if (dep[u] < dep[v]) {
            swap(u, v);
        }

        int diff = dep[u] - dep[v];

        for (int i = LOG - 1; i >= 0; i--) {
            if (diff >> i & 1) {
                u = up[i][u];
            }
        }

        if (u == v) return u;

        for (int i = LOG - 1; i >= 0; i--) {
            if (up[i][u] != up[i][v]) {
                u = up[i][u];
                v = up[i][v];
            }
        }

        return up[0][u];
    }

    int dist(int u, int v) {
        int w = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[w];
    }

    /*
    Tính size subtree trong phần cây chưa bị dead.
    */
    void dfsSz(int u, int p) {
        sz[u] = 1;

        for (int v : g[u]) {
            if (v == p || dead[v]) continue;

            dfsSz(v, u);
            sz[u] += sz[v];
        }
    }

    /*
    Tìm centroid của component có tổng size = total.

    Centroid là node sao cho mọi phần con sau khi cắt nó
    đều có size <= total / 2.
    */
    int getCentroid(int u, int p, int total) {
        for (int v : g[u]) {
            if (v == p || dead[v]) continue;

            if (sz[v] > total / 2) {
                return getCentroid(v, u, total);
            }
        }

        return u;
    }

    /*
    Build centroid decomposition.

    p = centroid parent của component hiện tại.
    */
    void buildCD(int u, int p) {
        dfsSz(u, 0);

        int c = getCentroid(u, 0, sz[u]);

        par[c] = p;
        dead[c] = true;

        for (int v : g[c]) {
            if (dead[v]) continue;

            buildCD(v, c);
        }
    }

    /*
    Build toàn bộ cấu trúc.

    root thường là 1.
    */
    void build(int root = 1) {
        dfsLCA(root, 0);
        buildCD(root, 0);
    }

    /*
    Update:
        tô đỏ node u.

    Đi qua tất cả centroid ancestor của u:
        u -> par[u] -> par[par[u]] -> ...
    nhưng ở đây par[] là centroid parent.

    Mỗi centroid c lưu best[c] tốt nhất.
    */
    void upd(int u) {
        int x = u;

        while (x) {
            best[x] = min(best[x], dist(u, x));
            x = par[x];
        }
    }

    /*
    Query:
        tìm khoảng cách từ u tới node đỏ gần nhất.

    Với mỗi centroid ancestor c:
        candidate = best[c] + dist(u, c)

    Vì mọi đường đi trong cây khi xét qua centroid decomposition
    sẽ được cover bởi một centroid chung nào đó.
    */
    int get(int u) {
        int ans = (int)1e9;
        int x = u;

        while (x) {
            ans = min(ans, best[x] + dist(u, x));
            x = par[x];
        }

        return ans;
    }
};

}


/*
========================================================
4. QUICK USAGE
========================================================

--------------------------------------------------------
A. Segment Tree cơ bản
--------------------------------------------------------

segtree::Seg seg;

seg.n = n;

for(int i = 1; i <= n; i++) {
    cin >> seg.a[i];
}

seg.build(n);

seg.upd(pos, val);

auto ans = seg.get(l, r);

cout << ans.sum << '\n';
cout << ans.mn << '\n';
cout << ans.mx << '\n';


--------------------------------------------------------
B. Lazy Segment Tree range add
--------------------------------------------------------

segtree::Lazy seg;

seg.n = n;

for(int i = 1; i <= n; i++) {
    cin >> seg.a[i];
}

seg.build(n);

seg.upd(l, r, val);

auto ans = seg.get(l, r);


--------------------------------------------------------
C. HLD node-value
--------------------------------------------------------

hld::HLD h;

h.init(n);

for(int i = 1; i <= n; i++) {
    cin >> h.val[i];
}

for(int i = 1; i < n; i++) {
    int u, v;
    cin >> u >> v;
    h.addEdge(u, v);
}

h.build(1);

// cộng x vào toàn bộ path u-v
h.pathAdd(u, v, x);

// query path u-v
auto ans = h.pathGet(u, v);

cout << ans.sum << '\n';
cout << ans.mn << '\n';
cout << ans.mx << '\n';

// cộng x vào subtree u
h.subAdd(u, x);

// query subtree u
auto sub = h.subGet(u);


--------------------------------------------------------
D. HLD edge-value
--------------------------------------------------------

Giả sử cạnh input là:

    u v w

Muốn lưu w ở node con.

Sau khi đọc tree, cần biết parent bằng dfs1.
Cách đơn giản:

    1. addEdge không trọng số.
    2. lưu danh sách cạnh.
    3. h.dfs1(1, 0) trước để biết parent.
    4. Với cạnh (u, v, w):
        nếu h.par[u] == v thì h.val[u] = w;
        else h.val[v] = w;
    5. h.dfs2(1, 1), build seg.

Trong template hiện tại h.build() gọi cả dfs1 và dfs2.
Nếu cần edge-value, bạn có thể:
    - đọc cạnh có trọng số vào vector
    - addEdge trước
    - gọi h.dfs1(1, 0)
    - set h.val[child] = w theo parent
    - gọi h.dfs2(1, 1)
    - build segment tree thủ công:

        for(int u = 1; u <= n; u++) {
            h.seg.a[h.dfn[u]] = h.val[u];
        }
        h.seg.build(n);

Query edge path:

    auto ans = h.pathGetEdge(u, v);

Update edge path:

    h.pathAddEdge(u, v, x);


--------------------------------------------------------
E. Centroid Decomposition: dynamic nearest red
--------------------------------------------------------

centroid::CD cd;

cd.init(n);

for(int i = 1; i < n; i++) {
    int u, v;
    cin >> u >> v;
    cd.addEdge(u, v);
}

cd.build(1);

// tô đỏ node 1 ban đầu, nếu đề yêu cầu
cd.upd(1);

// update node u thành đỏ
cd.upd(u);

// hỏi khoảng cách từ u tới node đỏ gần nhất
int ans = cd.get(u);

Nếu ans == 1e9 nghĩa là chưa có node đỏ nào.

========================================================
*/


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /*
    Đây là file template tổng hợp.

    Khi làm bài thật:
    - nếu cần HLD, dùng namespace hld
    - nếu cần Centroid Decomposition, dùng namespace centroid
    - nếu chỉ cần Segment Tree, dùng namespace segtree

    Xóa các namespace không dùng nếu muốn code ngắn hơn.
    */

    return 0;
}
