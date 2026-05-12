#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int N = 400000 + 5;
const ll INF = (ll)4e18;

/*
========================================================
                    LINK-CUT TREE TEMPLATE
========================================================

LCT dùng cho dynamic forest:

    link(u, v)      : nối 2 cây bằng cạnh u-v
    cut(u, v)       : cắt cạnh u-v
    connected(u,v)  : kiểm tra u và v có cùng cây không
    makeroot(u)     : đổi gốc cây represented tree về u
    split(u, v)     : expose đường đi u-v, sau đó v là root của splay chứa path
    get(u, v)       : query thông tin trên path u-v
    upd(u, val)     : update giá trị node u

Ý tưởng lõi:

    represented tree:
        cây thật của bài toán

    auxiliary tree / splay:
        cây phụ mà LCT dùng để biểu diễn các preferred path

Các hàm lõi:

    isroot(x)
    push(x)
    pull(x)
    rotate(x)
    splay(x)
    access(x)
    makeroot(x)
    findroot(x)
    split(u, v)
    link(u, v)
    cut(u, v)

========================================================
*/


/*
========================================================
1. LCT BASIC
========================================================

Hỗ trợ:

    - dynamic forest
    - link / cut
    - connected
    - point update
    - path query sum / min / max

Dùng khi giá trị nằm trên NODE.

Ví dụ:

    lct_basic::LCT lct;
    lct.init(n);

    for(int i = 1; i <= n; i++) {
        lct.val[i] = a[i];
    }

    lct.build(n);

    lct.link(u, v);
    lct.cut(u, v);

    auto ans = lct.get(u, v);

    ans.sum
    ans.mn
    ans.mx

========================================================
*/

namespace lct_basic {

struct Node {
    ll sum, mn, mx;

    Node(ll _sum = 0, ll _mn = INF, ll _mx = -INF) {
        sum = _sum;
        mn = _mn;
        mx = _mx;
    }
};

struct LCT {
    int ch[N][2], fa[N];
    bool rev[N];

    ll val[N];
    ll sum[N], mn[N], mx[N];

    int stk[N];

    Node merge(Node L, Node R) {
        Node res;

        res.sum = L.sum + R.sum;
        res.mn = min(L.mn, R.mn);
        res.mx = max(L.mx, R.mx);

        return res;
    }

    Node node(int x) {
        return Node(sum[x], mn[x], mx[x]);
    }

    /*
    isroot(x):
        kiểm tra x có phải root của splay hiện tại không.

    x là splay-root nếu:
        fa[x] không trỏ tới x bằng ch[fa[x]][0/1]
    */
    bool isroot(int x) {
        int f = fa[x];
        return !f || (ch[f][0] != x && ch[f][1] != x);
    }

    /*
    pull(x):
        cập nhật thông tin x từ 2 con splay.
    */
    void pull(int x) {
        sum[x] = val[x];
        mn[x] = val[x];
        mx[x] = val[x];

        if (ch[x][0]) {
            sum[x] += sum[ch[x][0]];
            mn[x] = min(mn[x], mn[ch[x][0]]);
            mx[x] = max(mx[x], mx[ch[x][0]]);
        }

        if (ch[x][1]) {
            sum[x] += sum[ch[x][1]];
            mn[x] = min(mn[x], mn[ch[x][1]]);
            mx[x] = max(mx[x], mx[ch[x][1]]);
        }
    }

    /*
    apply_rev(x):
        đảo path trong splay.
        Dùng khi makeroot.
    */
    void apply_rev(int x) {
        if (!x) return;

        swap(ch[x][0], ch[x][1]);
        rev[x] ^= 1;
    }

    /*
    push(x):
        đẩy tag đảo xuống con.
    */
    void push(int x) {
        if (!rev[x]) return;

        apply_rev(ch[x][0]);
        apply_rev(ch[x][1]);

        rev[x] = false;
    }

    /*
    rotate(x):
        xoay x lên một tầng trong splay.
    */
    void rotate(int x) {
        int y = fa[x];
        int z = fa[y];

        int k = (ch[y][1] == x);
        int b = ch[x][k ^ 1];

        if (!isroot(y)) {
            if (ch[z][0] == y) ch[z][0] = x;
            else if (ch[z][1] == y) ch[z][1] = x;
        }

        fa[x] = z;

        ch[x][k ^ 1] = y;
        fa[y] = x;

        ch[y][k] = b;
        if (b) fa[b] = y;

        pull(y);
        pull(x);
    }

    /*
    splay(x):
        đưa x lên root của splay hiện tại.
        Trước khi xoay phải push hết lazy từ trên xuống.
    */
    void splay(int x) {
        int y = x;
        int top = 0;

        stk[++top] = y;

        while (!isroot(y)) {
            y = fa[y];
            stk[++top] = y;
        }

        while (top) {
            push(stk[top--]);
        }

        while (!isroot(x)) {
            int y = fa[x];
            int z = fa[y];

            if (!isroot(y)) {
                bool zigzig = (ch[y][0] == x) == (ch[z][0] == y);

                if (zigzig) rotate(y);
                else rotate(x);
            }

            rotate(x);
        }

        pull(x);
    }

    /*
    access(x):
        expose đường từ represented-root tới x.
        Sau access(x), x nằm ở cuối preferred path.
    */
    void access(int x) {
        for (int y = 0; x; y = x, x = fa[x]) {
            splay(x);
            ch[x][1] = y;
            pull(x);
        }
    }

    /*
    makeroot(x):
        biến x thành root của represented tree.
    */
    void makeroot(int x) {
        access(x);
        splay(x);
        apply_rev(x);
    }

    /*
    findroot(x):
        tìm root thật của represented tree chứa x.
    */
    int findroot(int x) {
        access(x);
        splay(x);

        while (ch[x][0]) {
            push(x);
            x = ch[x][0];
        }

        splay(x);
        return x;
    }

    bool connected(int u, int v) {
        if (u == v) return true;
        return findroot(u) == findroot(v);
    }

    /*
    split(u, v):
        expose path u-v.
        Sau split:
            v là root của splay,
            splay của v chứa đúng path u-v.
    */
    void split(int u, int v) {
        makeroot(u);
        access(v);
        splay(v);
    }

    /*
    link(u, v):
        nối u-v nếu chưa cùng cây.
    */
    bool link(int u, int v) {
        makeroot(u);

        if (findroot(v) == u) {
            return false;
        }

        fa[u] = v;
        return true;
    }

    /*
    cut(u, v):
        cắt cạnh trực tiếp u-v nếu tồn tại.
    */
    bool cut(int u, int v) {
        makeroot(u);
        access(v);
        splay(v);

        /*
        Sau makeroot(u), nếu u-v là cạnh trực tiếp thì:
            ch[v][0] == u
            ch[u][1] == 0
        */
        if (ch[v][0] != u || ch[u][1]) {
            return false;
        }

        ch[v][0] = 0;
        fa[u] = 0;

        pull(v);
        return true;
    }

    /*
    update giá trị node x.
    */
    void upd(int x, ll v) {
        splay(x);
        val[x] = v;
        pull(x);
    }

    /*
    query path u-v.
    */
    Node get(int u, int v) {
        split(u, v);
        return Node(sum[v], mn[v], mx[v]);
    }

    void init(int n) {
        for (int i = 1; i <= n; i++) {
            ch[i][0] = ch[i][1] = fa[i] = 0;
            rev[i] = false;

            val[i] = 0;
            sum[i] = 0;
            mn[i] = INF;
            mx[i] = -INF;
        }
    }

    void build(int n) {
        for (int i = 1; i <= n; i++) {
            sum[i] = mn[i] = mx[i] = val[i];
        }
    }
};

}


/*
========================================================
2. LCT LAZY PATH ADD / ASSIGN
========================================================

Hỗ trợ:

    pathAdd(u, v, x):
        cộng x vào tất cả node trên path u-v

    pathSet(u, v, x):
        gán tất cả node trên path u-v thành x

    get(u, v):
        query sum / min / max trên path u-v

Lazy cần xử lý thứ tự:

    assign trước
    add sau
    reverse độc lập với giá trị

Nếu node đang có tag assign, rồi cộng add:
    tagSet += add

Nếu node chưa có assign:
    tagAdd += add

========================================================
*/

namespace lct_lazy {

struct Node {
    ll sum, mn, mx;

    Node(ll _sum = 0, ll _mn = INF, ll _mx = -INF) {
        sum = _sum;
        mn = _mn;
        mx = _mx;
    }
};

struct LCT {
    int ch[N][2], fa[N], sz[N];
    bool rev[N];

    ll val[N], sum[N], mn[N], mx[N];

    ll add[N];
    ll tag[N];
    bool has[N];

    int stk[N];

    bool isroot(int x) {
        int f = fa[x];
        return !f || (ch[f][0] != x && ch[f][1] != x);
    }

    void pull(int x) {
        sz[x] = 1;
        sum[x] = val[x];
        mn[x] = val[x];
        mx[x] = val[x];

        if (ch[x][0]) {
            int l = ch[x][0];

            sz[x] += sz[l];
            sum[x] += sum[l];
            mn[x] = min(mn[x], mn[l]);
            mx[x] = max(mx[x], mx[l]);
        }

        if (ch[x][1]) {
            int r = ch[x][1];

            sz[x] += sz[r];
            sum[x] += sum[r];
            mn[x] = min(mn[x], mn[r]);
            mx[x] = max(mx[x], mx[r]);
        }
    }

    void apply_rev(int x) {
        if (!x) return;

        swap(ch[x][0], ch[x][1]);
        rev[x] ^= 1;
    }

    void apply_add(int x, ll v) {
        if (!x) return;

        val[x] += v;
        sum[x] += v * sz[x];
        mn[x] += v;
        mx[x] += v;

        if (has[x]) {
            tag[x] += v;
        } else {
            add[x] += v;
        }
    }

    void apply_set(int x, ll v) {
        if (!x) return;

        val[x] = v;
        sum[x] = v * sz[x];
        mn[x] = v;
        mx[x] = v;

        tag[x] = v;
        has[x] = true;
        add[x] = 0;
    }

    void push(int x) {
        if (has[x]) {
            apply_set(ch[x][0], tag[x]);
            apply_set(ch[x][1], tag[x]);
            has[x] = false;
        }

        if (add[x] != 0) {
            apply_add(ch[x][0], add[x]);
            apply_add(ch[x][1], add[x]);
            add[x] = 0;
        }

        if (rev[x]) {
            apply_rev(ch[x][0]);
            apply_rev(ch[x][1]);
            rev[x] = false;
        }
    }

    void rotate(int x) {
        int y = fa[x];
        int z = fa[y];

        int k = (ch[y][1] == x);
        int b = ch[x][k ^ 1];

        if (!isroot(y)) {
            if (ch[z][0] == y) ch[z][0] = x;
            else if (ch[z][1] == y) ch[z][1] = x;
        }

        fa[x] = z;

        ch[x][k ^ 1] = y;
        fa[y] = x;

        ch[y][k] = b;
        if (b) fa[b] = y;

        pull(y);
        pull(x);
    }

    void splay(int x) {
        int y = x;
        int top = 0;

        stk[++top] = y;

        while (!isroot(y)) {
            y = fa[y];
            stk[++top] = y;
        }

        while (top) {
            push(stk[top--]);
        }

        while (!isroot(x)) {
            int y = fa[x];
            int z = fa[y];

            if (!isroot(y)) {
                bool zigzig = (ch[y][0] == x) == (ch[z][0] == y);

                if (zigzig) rotate(y);
                else rotate(x);
            }

            rotate(x);
        }

        pull(x);
    }

    void access(int x) {
        for (int y = 0; x; y = x, x = fa[x]) {
            splay(x);
            ch[x][1] = y;
            pull(x);
        }
    }

    void makeroot(int x) {
        access(x);
        splay(x);
        apply_rev(x);
    }

    int findroot(int x) {
        access(x);
        splay(x);

        while (ch[x][0]) {
            push(x);
            x = ch[x][0];
        }

        splay(x);
        return x;
    }

    bool connected(int u, int v) {
        if (u == v) return true;
        return findroot(u) == findroot(v);
    }

    void split(int u, int v) {
        makeroot(u);
        access(v);
        splay(v);
    }

    bool link(int u, int v) {
        makeroot(u);

        if (findroot(v) == u) {
            return false;
        }

        fa[u] = v;
        return true;
    }

    bool cut(int u, int v) {
        makeroot(u);
        access(v);
        splay(v);

        if (ch[v][0] != u || ch[u][1]) {
            return false;
        }

        ch[v][0] = 0;
        fa[u] = 0;

        pull(v);
        return true;
    }

    void upd(int x, ll v) {
        splay(x);
        val[x] = v;
        pull(x);
    }

    Node get(int u, int v) {
        split(u, v);
        return Node(sum[v], mn[v], mx[v]);
    }

    void pathAdd(int u, int v, ll x) {
        split(u, v);
        apply_add(v, x);
    }

    void pathSet(int u, int v, ll x) {
        split(u, v);
        apply_set(v, x);
    }

    void init(int n) {
        for (int i = 1; i <= n; i++) {
            ch[i][0] = ch[i][1] = fa[i] = 0;
            sz[i] = 1;

            rev[i] = false;
            has[i] = false;

            add[i] = 0;
            tag[i] = 0;

            val[i] = 0;
            sum[i] = 0;
            mn[i] = INF;
            mx[i] = -INF;
        }
    }

    void build(int n) {
        for (int i = 1; i <= n; i++) {
            sz[i] = 1;
            sum[i] = mn[i] = mx[i] = val[i];
        }
    }
};

}


/*
========================================================
3. LCT EDGE VALUE
========================================================

LCT tự nhiên query tốt trên NODE.

Nếu bài có trọng số trên CẠNH, ta biến mỗi cạnh thành một node riêng.

Với cạnh thứ i:

    u --w-- v

tạo node:

    e = n + i
    val[e] = w

nối:

    link(u, e)
    link(e, v)

Khi query path u-v:
    get(u, v) sẽ tính cả các node cạnh trên đường đi.

Nếu node gốc ban đầu không có giá trị, cho val[1..n] = 0.

Ứng dụng:

    - query max edge trên path
    - query sum edge trên path
    - dynamic MST
    - cut edge theo id

========================================================
*/

namespace lct_edge {

struct Node {
    ll sum, mn, mx;
    int idMax;

    Node(ll _sum = 0, ll _mn = INF, ll _mx = -INF, int _idMax = 0) {
        sum = _sum;
        mn = _mn;
        mx = _mx;
        idMax = _idMax;
    }
};

struct LCT {
    int ch[N][2], fa[N];
    bool rev[N];

    ll val[N];
    ll sum[N], mn[N], mx[N];
    int idMax[N];

    int stk[N];

    bool isroot(int x) {
        int f = fa[x];
        return !f || (ch[f][0] != x && ch[f][1] != x);
    }

    void pull(int x) {
        sum[x] = val[x];
        mn[x] = val[x];
        mx[x] = val[x];
        idMax[x] = x;

        if (ch[x][0]) {
            int l = ch[x][0];

            sum[x] += sum[l];
            mn[x] = min(mn[x], mn[l]);

            if (mx[l] > mx[x]) {
                mx[x] = mx[l];
                idMax[x] = idMax[l];
            }
        }

        if (ch[x][1]) {
            int r = ch[x][1];

            sum[x] += sum[r];
            mn[x] = min(mn[x], mn[r]);

            if (mx[r] > mx[x]) {
                mx[x] = mx[r];
                idMax[x] = idMax[r];
            }
        }
    }

    void apply_rev(int x) {
        if (!x) return;

        swap(ch[x][0], ch[x][1]);
        rev[x] ^= 1;
    }

    void push(int x) {
        if (!rev[x]) return;

        apply_rev(ch[x][0]);
        apply_rev(ch[x][1]);

        rev[x] = false;
    }

    void rotate(int x) {
        int y = fa[x];
        int z = fa[y];

        int k = (ch[y][1] == x);
        int b = ch[x][k ^ 1];

        if (!isroot(y)) {
            if (ch[z][0] == y) ch[z][0] = x;
            else if (ch[z][1] == y) ch[z][1] = x;
        }

        fa[x] = z;

        ch[x][k ^ 1] = y;
        fa[y] = x;

        ch[y][k] = b;
        if (b) fa[b] = y;

        pull(y);
        pull(x);
    }

    void splay(int x) {
        int y = x;
        int top = 0;

        stk[++top] = y;

        while (!isroot(y)) {
            y = fa[y];
            stk[++top] = y;
        }

        while (top) {
            push(stk[top--]);
        }

        while (!isroot(x)) {
            int y = fa[x];
            int z = fa[y];

            if (!isroot(y)) {
                bool zigzig = (ch[y][0] == x) == (ch[z][0] == y);

                if (zigzig) rotate(y);
                else rotate(x);
            }

            rotate(x);
        }

        pull(x);
    }

    void access(int x) {
        for (int y = 0; x; y = x, x = fa[x]) {
            splay(x);
            ch[x][1] = y;
            pull(x);
        }
    }

    void makeroot(int x) {
        access(x);
        splay(x);
        apply_rev(x);
    }

    int findroot(int x) {
        access(x);
        splay(x);

        while (ch[x][0]) {
            push(x);
            x = ch[x][0];
        }

        splay(x);
        return x;
    }

    bool connected(int u, int v) {
        if (u == v) return true;
        return findroot(u) == findroot(v);
    }

    void split(int u, int v) {
        makeroot(u);
        access(v);
        splay(v);
    }

    bool link(int u, int v) {
        makeroot(u);

        if (findroot(v) == u) {
            return false;
        }

        fa[u] = v;
        return true;
    }

    bool cut(int u, int v) {
        makeroot(u);
        access(v);
        splay(v);

        if (ch[v][0] != u || ch[u][1]) {
            return false;
        }

        ch[v][0] = 0;
        fa[u] = 0;

        pull(v);
        return true;
    }

    /*
    Thêm cạnh id có trọng số w giữa u-v.

    edge node = n + id.
    */
    void addEdge(int n, int id, int u, int v, ll w) {
        int e = n + id;

        val[e] = w;
        pull(e);

        link(u, e);
        link(e, v);
    }

    /*
    Xóa cạnh id giữa u-v.

    Phải biết hai đầu u, v của cạnh id.
    */
    void delEdge(int n, int id, int u, int v) {
        int e = n + id;

        cut(u, e);
        cut(e, v);
    }

    /*
    Query path u-v.
    */
    Node get(int u, int v) {
        split(u, v);
        return Node(sum[v], mn[v], mx[v], idMax[v]);
    }

    void upd(int x, ll v) {
        splay(x);
        val[x] = v;
        pull(x);
    }

    void init(int totalNode) {
        for (int i = 1; i <= totalNode; i++) {
            ch[i][0] = ch[i][1] = fa[i] = 0;
            rev[i] = false;

            val[i] = 0;
            sum[i] = 0;
            mn[i] = INF;
            mx[i] = -INF;
            idMax[i] = i;
        }
    }

    void build(int totalNode) {
        for (int i = 1; i <= totalNode; i++) {
            sum[i] = mn[i] = mx[i] = val[i];
            idMax[i] = i;
        }
    }
};

}


/*
========================================================
4. LCT FOR DYNAMIC MST / MAX EDGE ON PATH
========================================================

Mẫu tư duy cho bài dynamic MST hoặc incremental MST:

Khi thêm cạnh (u, v, w):

    Nếu u, v chưa connected:
        thêm cạnh vào forest.

    Nếu u, v đã connected:
        query max edge trên path u-v.
        Nếu maxEdgeWeight > w:
            xóa cạnh max đó.
            thêm cạnh mới.
        Ngược lại bỏ cạnh mới.

Cần edge-node:

    node 1..n          : node thật, val = -INF hoặc 0
    node n+edgeId      : node cạnh, val = weight

Mỗi node lưu:
    mx
    idMax

idMax cho biết node nào đang có trọng số lớn nhất trên path.

========================================================
*/

namespace lct_mst {

struct Edge {
    int u, v;
    ll w;
    bool in;
};

struct LCT {
    int ch[N][2], fa[N];
    bool rev[N];

    ll val[N], mx[N];
    int idMax[N];

    int stk[N];

    bool isroot(int x) {
        int f = fa[x];
        return !f || (ch[f][0] != x && ch[f][1] != x);
    }

    void pull(int x) {
        mx[x] = val[x];
        idMax[x] = x;

        if (ch[x][0]) {
            int l = ch[x][0];

            if (mx[l] > mx[x]) {
                mx[x] = mx[l];
                idMax[x] = idMax[l];
            }
        }

        if (ch[x][1]) {
            int r = ch[x][1];

            if (mx[r] > mx[x]) {
                mx[x] = mx[r];
                idMax[x] = idMax[r];
            }
        }
    }

    void apply_rev(int x) {
        if (!x) return;

        swap(ch[x][0], ch[x][1]);
        rev[x] ^= 1;
    }

    void push(int x) {
        if (!rev[x]) return;

        apply_rev(ch[x][0]);
        apply_rev(ch[x][1]);

        rev[x] = false;
    }

    void rotate(int x) {
        int y = fa[x];
        int z = fa[y];

        int k = (ch[y][1] == x);
        int b = ch[x][k ^ 1];

        if (!isroot(y)) {
            if (ch[z][0] == y) ch[z][0] = x;
            else if (ch[z][1] == y) ch[z][1] = x;
        }

        fa[x] = z;

        ch[x][k ^ 1] = y;
        fa[y] = x;

        ch[y][k] = b;
        if (b) fa[b] = y;

        pull(y);
        pull(x);
    }

    void splay(int x) {
        int y = x;
        int top = 0;

        stk[++top] = y;

        while (!isroot(y)) {
            y = fa[y];
            stk[++top] = y;
        }

        while (top) {
            push(stk[top--]);
        }

        while (!isroot(x)) {
            int y = fa[x];
            int z = fa[y];

            if (!isroot(y)) {
                bool zigzig = (ch[y][0] == x) == (ch[z][0] == y);

                if (zigzig) rotate(y);
                else rotate(x);
            }

            rotate(x);
        }

        pull(x);
    }

    void access(int x) {
        for (int y = 0; x; y = x, x = fa[x]) {
            splay(x);
            ch[x][1] = y;
            pull(x);
        }
    }

    void makeroot(int x) {
        access(x);
        splay(x);
        apply_rev(x);
    }

    int findroot(int x) {
        access(x);
        splay(x);

        while (ch[x][0]) {
            push(x);
            x = ch[x][0];
        }

        splay(x);
        return x;
    }

    bool connected(int u, int v) {
        if (u == v) return true;
        return findroot(u) == findroot(v);
    }

    void split(int u, int v) {
        makeroot(u);
        access(v);
        splay(v);
    }

    bool link(int u, int v) {
        makeroot(u);

        if (findroot(v) == u) {
            return false;
        }

        fa[u] = v;
        return true;
    }

    bool cut(int u, int v) {
        makeroot(u);
        access(v);
        splay(v);

        if (ch[v][0] != u || ch[u][1]) {
            return false;
        }

        ch[v][0] = 0;
        fa[u] = 0;

        pull(v);
        return true;
    }

    /*
    Query node có max weight trên path u-v.
    */
    int getMaxId(int u, int v) {
        split(u, v);
        return idMax[v];
    }

    ll getMaxVal(int u, int v) {
        split(u, v);
        return mx[v];
    }

    void init(int totalNode) {
        for (int i = 1; i <= totalNode; i++) {
            ch[i][0] = ch[i][1] = fa[i] = 0;
            rev[i] = false;

            val[i] = -INF;
            mx[i] = -INF;
            idMax[i] = i;
        }
    }

    void build(int totalNode) {
        for (int i = 1; i <= totalNode; i++) {
            mx[i] = val[i];
            idMax[i] = i;
        }
    }
};

}


/*
========================================================
5. QUICK USAGE
========================================================

--------------------------------------------------------
A. Dynamic forest, node value, path sum/min/max
--------------------------------------------------------

lct_basic::LCT lct;

lct.init(n);

for(int i = 1; i <= n; i++) {
    cin >> lct.val[i];
}

lct.build(n);

lct.link(u, v);
lct.cut(u, v);

if(lct.connected(u, v)) {
    auto ans = lct.get(u, v);

    cout << ans.sum << '\n';
    cout << ans.mn << '\n';
    cout << ans.mx << '\n';
}

lct.upd(u, newVal);


--------------------------------------------------------
B. Path add / path assign
--------------------------------------------------------

lct_lazy::LCT lct;

lct.init(n);

for(int i = 1; i <= n; i++) {
    cin >> lct.val[i];
}

lct.build(n);

lct.link(u, v);

lct.pathAdd(u, v, x);
lct.pathSet(u, v, x);

auto ans = lct.get(u, v);


--------------------------------------------------------
C. Edge weight
--------------------------------------------------------

Có n node thật, m cạnh động.

Cạnh id có hai đầu u, v, trọng số w.

Ta tạo node cạnh:

    e = n + id

Tổng node tối đa:

    n + m

lct_edge::LCT lct;

lct.init(n + m);

for(int i = 1; i <= n; i++) {
    lct.val[i] = 0; // node thật không đóng góp
}

lct.build(n + m);

lct.addEdge(n, id, u, v, w);

auto ans = lct.get(u, v);

cout << ans.sum << '\n';
cout << ans.mx << '\n';

lct.delEdge(n, id, u, v);


--------------------------------------------------------
D. Dynamic MST skeleton
--------------------------------------------------------

Ý tưởng thêm cạnh id = i, cạnh (u, v, w):

    edgeNode = n + i
    lct.val[edgeNode] = w
    lct.pull(edgeNode)

    if(!lct.connected(u, v)) {
        lct.link(u, edgeNode);
        lct.link(edgeNode, v);
        inMST[i] = true;
        total += w;
    }
    else {
        int mxNode = lct.getMaxId(u, v);
        ll mxVal = lct.getMaxVal(u, v);

        if(mxVal > w) {
            oldEdgeId = mxNode - n;

            cut old edge:
                lct.cut(edge[oldEdgeId].u, mxNode);
                lct.cut(mxNode, edge[oldEdgeId].v);

            add new edge:
                lct.link(u, edgeNode);
                lct.link(edgeNode, v);

            total += w - mxVal;
        }
    }

========================================================
*/


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /*
    Đây là file template tổng hợp Link-Cut Tree.

    Khi làm bài thật:
        - dynamic forest path query node value: dùng lct_basic
        - path add / assign: dùng lct_lazy
        - edge weight: dùng lct_edge
        - dynamic MST / max edge replacement: dùng lct_mst
    */

    return 0;
}
