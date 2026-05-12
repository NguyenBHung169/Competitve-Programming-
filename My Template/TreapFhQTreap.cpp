#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int N = 400000 + 5;
const ll INF = (ll)4e18;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

/*
========================================================
                    TREAP / FHQ TREAP TEMPLATE
========================================================

FHQ Treap có 2 hàm lõi:

    split()
    merge()

Có 2 nhóm bài lớn:

1. Ordered Treap:
    quản lý tập giá trị theo key
    giống set / multiset

2. Implicit Treap:
    quản lý dãy theo vị trí
    key ngầm là thứ tự trong cây

========================================================
*/


/*
========================================================
1. FHQ TREAP ORDERED MULTISET
========================================================

Dạng bài:

    insert x
    erase x
    rank x        : số phần tử < x + 1
    kth k         : phần tử nhỏ thứ k
    pre x         : phần tử lớn nhất < x
    suf x         : phần tử nhỏ nhất > x

Cho phép trùng giá trị nhờ cnt[u].

Ý tưởng:

Mỗi node lưu:
    val[u] : giá trị
    pri[u] : priority random
    sz[u]  : tổng số phần tử trong subtree, tính cả cnt
    cnt[u] : số lần val[u] xuất hiện
    ch[u][0], ch[u][1] : con trái/phải

Điều kiện BST:
    trái < val[u] < phải

Treap giữ cân bằng bằng priority random.

========================================================
*/

namespace fhq_set {

int tot, root;
int ch[N][2], pri[N], val[N], sz[N], cnt[N];

int node(int x) {
    ++tot;
    ch[tot][0] = ch[tot][1] = 0;
    pri[tot] = rng();
    val[tot] = x;
    sz[tot] = cnt[tot] = 1;
    return tot;
}

int size(int u) {
    return u ? sz[u] : 0;
}

void pull(int u) {
    if (!u) return;
    sz[u] = size(ch[u][0]) + size(ch[u][1]) + cnt[u];
}

/*
split(u, x, a, b)

Tách cây u thành 2 cây:
    a chứa các giá trị <= x
    b chứa các giá trị > x
*/
void split(int u, int x, int &a, int &b) {
    if (!u) {
        a = b = 0;
        return;
    }

    if (val[u] <= x) {
        a = u;
        split(ch[u][1], x, ch[u][1], b);
        pull(a);
    } else {
        b = u;
        split(ch[u][0], x, a, ch[u][0]);
        pull(b);
    }
}

/*
merge(a, b)

Gộp 2 cây a, b.
Điều kiện:
    mọi giá trị trong a <= mọi giá trị trong b
*/
int merge(int a, int b) {
    if (!a || !b) return a | b;

    if (pri[a] < pri[b]) {
        ch[a][1] = merge(ch[a][1], b);
        pull(a);
        return a;
    } else {
        ch[b][0] = merge(a, ch[b][0]);
        pull(b);
        return b;
    }
}

/*
Tìm node có giá trị x.
*/
int find(int u, int x) {
    while (u) {
        if (val[u] == x) return u;
        if (x < val[u]) u = ch[u][0];
        else u = ch[u][1];
    }

    return 0;
}

/*
insert x.

Cách 1:
    nếu x đã tồn tại thì cnt++
    nếu chưa tồn tại thì split rồi merge node mới.

Cách này giúp multiset gọn hơn.
*/
void ins(int x) {
    int p = find(root, x);

    if (p) {
        cnt[p]++;
        // Sau khi cnt thay đổi, cần cập nhật lại sz trên đường đi.
        // Vì find không lưu đường, cách đơn giản hơn là dùng split.
    }

    int a, b, c;
    split(root, x, a, c);        // a <= x, c > x
    split(a, x - 1, a, b);       // a < x, b == x

    if (b) {
        cnt[b]++;
        pull(b);
    } else {
        b = node(x);
    }

    root = merge(merge(a, b), c);
}

/*
erase x.
Xóa một lần xuất hiện của x.
*/
void del(int x) {
    int a, b, c;
    split(root, x, a, c);        // a <= x, c > x
    split(a, x - 1, a, b);       // a < x, b == x

    if (b) {
        if (cnt[b] > 1) {
            cnt[b]--;
            pull(b);
        } else {
            b = merge(ch[b][0], ch[b][1]);
        }
    }

    root = merge(merge(a, b), c);
}

/*
rank(x):
    trả về thứ hạng của x nếu chèn vào.
    = số phần tử < x + 1
*/
int rank_of(int x) {
    int a, b;
    split(root, x - 1, a, b);

    int ans = size(a) + 1;

    root = merge(a, b);

    return ans;
}

/*
kth(k):
    trả về phần tử nhỏ thứ k.
*/
int kth(int u, int k) {
    if (!u) return -1;

    if (k <= size(ch[u][0])) {
        return kth(ch[u][0], k);
    }

    k -= size(ch[u][0]);

    if (k <= cnt[u]) {
        return val[u];
    }

    k -= cnt[u];

    return kth(ch[u][1], k);
}

/*
pre(x):
    phần tử lớn nhất < x.
*/
int pre(int x) {
    int a, b;
    split(root, x - 1, a, b);

    if (!a) {
        root = merge(a, b);
        return INT_MIN;
    }

    int ans = kth(a, size(a));

    root = merge(a, b);

    return ans;
}

/*
suf(x):
    phần tử nhỏ nhất > x.
*/
int suf(int x) {
    int a, b;
    split(root, x, a, b);

    if (!b) {
        root = merge(a, b);
        return INT_MAX;
    }

    int ans = kth(b, 1);

    root = merge(a, b);

    return ans;
}

void init() {
    tot = root = 0;
}

}


/*
========================================================
2. FHQ TREAP IMPLICIT SEQUENCE
========================================================

Dạng bài:

    quản lý dãy a[1..n]

Hỗ trợ:

    insert(pos, x)       : chèn x sau pos phần tử đầu
    erase(l, r)          : xóa đoạn [l, r]
    reverse(l, r)        : đảo đoạn [l, r]
    add(l, r, val)       : cộng val vào đoạn [l, r]
    set(l, r, val)       : gán đoạn [l, r] = val
    get(l, r)            : query sum / min / max đoạn [l, r]
    kth(pos)             : lấy giá trị tại vị trí pos

Ở implicit treap:
    không có val làm key BST.
    key là size bên trái, tức thứ tự trong dãy.

split(u, k, a, b):
    a chứa k phần tử đầu
    b chứa phần còn lại

========================================================
*/

namespace fhq_seq {

int tot, root;
int ch[N][2], pri[N], sz[N];

ll val[N];        // giá trị tại node
ll sum[N];        // tổng đoạn
ll mn[N];         // min đoạn
ll mx[N];         // max đoạn

bool rev[N];      // lazy reverse
ll add[N];        // lazy add
ll tag[N];        // lazy assign value
bool has[N];      // có tag assign hay không

int size(int u) {
    return u ? sz[u] : 0;
}

ll getsum(int u) {
    return u ? sum[u] : 0;
}

ll getmn(int u) {
    return u ? mn[u] : INF;
}

ll getmx(int u) {
    return u ? mx[u] : -INF;
}

int node(ll x) {
    ++tot;

    ch[tot][0] = ch[tot][1] = 0;
    pri[tot] = rng();
    sz[tot] = 1;

    val[tot] = x;
    sum[tot] = x;
    mn[tot] = x;
    mx[tot] = x;

    rev[tot] = false;
    add[tot] = 0;
    tag[tot] = 0;
    has[tot] = false;

    return tot;
}

/*
pull(u):
    cập nhật thông tin node u từ 2 con.
*/
void pull(int u) {
    if (!u) return;

    sz[u] = size(ch[u][0]) + size(ch[u][1]) + 1;

    sum[u] = getsum(ch[u][0]) + getsum(ch[u][1]) + val[u];

    mn[u] = min({getmn(ch[u][0]), getmn(ch[u][1]), val[u]});
    mx[u] = max({getmx(ch[u][0]), getmx(ch[u][1]), val[u]});
}

/*
apply_rev(u):
    đảo cả subtree u.
    Chỉ cần swap con trái/phải và đánh dấu rev.
*/
void apply_rev(int u) {
    if (!u) return;

    swap(ch[u][0], ch[u][1]);
    rev[u] ^= 1;
}

/*
apply_add(u, x):
    cộng x vào toàn bộ subtree u.
*/
void apply_add(int u, ll x) {
    if (!u) return;

    val[u] += x;
    sum[u] += x * sz[u];
    mn[u] += x;
    mx[u] += x;

    if (has[u]) {
        tag[u] += x;
    } else {
        add[u] += x;
    }
}

/*
apply_set(u, x):
    gán toàn bộ subtree u thành x.
    Gán sẽ ghi đè add cũ.
*/
void apply_set(int u, ll x) {
    if (!u) return;

    val[u] = x;
    sum[u] = x * sz[u];
    mn[u] = x;
    mx[u] = x;

    tag[u] = x;
    has[u] = true;
    add[u] = 0;
}

/*
push(u):
    đẩy lazy xuống con.

Thứ tự quan trọng:
    1. assign
    2. add
    3. reverse

Vì assign ghi đè giá trị,
add tác động sau assign,
reverse chỉ đổi cấu trúc trái/phải.
*/
void push(int u) {
    if (!u) return;

    if (has[u]) {
        apply_set(ch[u][0], tag[u]);
        apply_set(ch[u][1], tag[u]);
        has[u] = false;
    }

    if (add[u] != 0) {
        apply_add(ch[u][0], add[u]);
        apply_add(ch[u][1], add[u]);
        add[u] = 0;
    }

    if (rev[u]) {
        apply_rev(ch[u][0]);
        apply_rev(ch[u][1]);
        rev[u] = false;
    }
}

/*
split(u, k, a, b)

Tách cây u thành:
    a = k phần tử đầu
    b = phần còn lại
*/
void split(int u, int k, int &a, int &b) {
    if (!u) {
        a = b = 0;
        return;
    }

    push(u);

    if (size(ch[u][0]) + 1 <= k) {
        a = u;
        split(ch[u][1], k - size(ch[u][0]) - 1, ch[u][1], b);
        pull(a);
    } else {
        b = u;
        split(ch[u][0], k, a, ch[u][0]);
        pull(b);
    }
}

/*
merge(a, b)

Gộp hai dãy:
    toàn bộ a đứng trước toàn bộ b.
*/
int merge(int a, int b) {
    if (!a || !b) return a | b;

    if (pri[a] < pri[b]) {
        push(a);
        ch[a][1] = merge(ch[a][1], b);
        pull(a);
        return a;
    } else {
        push(b);
        ch[b][0] = merge(a, ch[b][0]);
        pull(b);
        return b;
    }
}

/*
build từ vector 1-index hoặc 0-index đều được.
Ở đây dùng vector truyền vào theo thứ tự.
*/
void build(vector<ll> a) {
    root = 0;

    for (ll x : a) {
        root = merge(root, node(x));
    }
}

/*
insert(pos, x):
    chèn x sau pos phần tử đầu.

Ví dụ:
    pos = 0 -> chèn đầu
    pos = size(root) -> chèn cuối
*/
void ins(int pos, ll x) {
    int a, b;

    split(root, pos, a, b);

    root = merge(merge(a, node(x)), b);
}

/*
erase(l, r):
    xóa đoạn vị trí [l, r].
*/
void del(int l, int r) {
    int a, b, c;

    split(root, r, a, c);
    split(a, l - 1, a, b);

    // b là đoạn bị xóa.
    // Nếu cần thu hồi node thì tự viết thêm recycle.
    root = merge(a, c);
}

/*
reverse(l, r):
    đảo đoạn [l, r].
*/
void reverse(int l, int r) {
    int a, b, c;

    split(root, r, a, c);
    split(a, l - 1, a, b);

    apply_rev(b);

    root = merge(merge(a, b), c);
}

/*
addRange(l, r, x):
    cộng x vào đoạn [l, r].
*/
void addRange(int l, int r, ll x) {
    int a, b, c;

    split(root, r, a, c);
    split(a, l - 1, a, b);

    apply_add(b, x);

    root = merge(merge(a, b), c);
}

/*
setRange(l, r, x):
    gán đoạn [l, r] = x.
*/
void setRange(int l, int r, ll x) {
    int a, b, c;

    split(root, r, a, c);
    split(a, l - 1, a, b);

    apply_set(b, x);

    root = merge(merge(a, b), c);
}

/*
get(l, r):
    query sum/min/max đoạn [l, r].
*/
tuple<ll, ll, ll> get(int l, int r) {
    int a, b, c;

    split(root, r, a, c);
    split(a, l - 1, a, b);

    tuple<ll, ll, ll> ans = {getsum(b), getmn(b), getmx(b)};

    root = merge(merge(a, b), c);

    return ans;
}

/*
kth(pos):
    lấy giá trị tại vị trí pos.
*/
ll kth(int u, int k) {
    push(u);

    if (k <= size(ch[u][0])) {
        return kth(ch[u][0], k);
    }

    if (k == size(ch[u][0]) + 1) {
        return val[u];
    }

    return kth(ch[u][1], k - size(ch[u][0]) - 1);
}

ll kth(int pos) {
    return kth(root, pos);
}

/*
inorder:
    xuất dãy hiện tại.
*/
void dfs(int u, vector<ll> &res) {
    if (!u) return;

    push(u);

    dfs(ch[u][0], res);
    res.push_back(val[u]);
    dfs(ch[u][1], res);
}

vector<ll> getAll() {
    vector<ll> res;
    dfs(root, res);
    return res;
}

void init() {
    tot = root = 0;
}

}


/*
========================================================
3. FHQ TREAP ORDERED SET KHÔNG CNT
========================================================

Dạng này dùng khi giá trị là unique.

Ưu điểm:
    code split/merge đơn giản hơn.

Hỗ trợ:
    insert unique
    erase
    rank
    kth

========================================================
*/

namespace fhq_unique {

int tot, root;
int ch[N][2], pri[N], val[N], sz[N];

int size(int u) {
    return u ? sz[u] : 0;
}

int node(int x) {
    ++tot;

    ch[tot][0] = ch[tot][1] = 0;
    pri[tot] = rng();
    val[tot] = x;
    sz[tot] = 1;

    return tot;
}

void pull(int u) {
    if (!u) return;
    sz[u] = size(ch[u][0]) + size(ch[u][1]) + 1;
}

/*
split by value:
    a <= x
    b > x
*/
void split(int u, int x, int &a, int &b) {
    if (!u) {
        a = b = 0;
        return;
    }

    if (val[u] <= x) {
        a = u;
        split(ch[u][1], x, ch[u][1], b);
        pull(a);
    } else {
        b = u;
        split(ch[u][0], x, a, ch[u][0]);
        pull(b);
    }
}

int merge(int a, int b) {
    if (!a || !b) return a | b;

    if (pri[a] < pri[b]) {
        ch[a][1] = merge(ch[a][1], b);
        pull(a);
        return a;
    } else {
        ch[b][0] = merge(a, ch[b][0]);
        pull(b);
        return b;
    }
}

bool exist(int u, int x) {
    while (u) {
        if (val[u] == x) return true;
        if (x < val[u]) u = ch[u][0];
        else u = ch[u][1];
    }

    return false;
}

void ins(int x) {
    if (exist(root, x)) return;

    int a, b;
    split(root, x, a, b);

    root = merge(merge(a, node(x)), b);
}

void del(int x) {
    int a, b, c;

    split(root, x, a, c);
    split(a, x - 1, a, b);

    b = merge(ch[b][0], ch[b][1]);

    root = merge(merge(a, b), c);
}

int kth(int u, int k) {
    if (k <= size(ch[u][0])) {
        return kth(ch[u][0], k);
    }

    if (k == size(ch[u][0]) + 1) {
        return val[u];
    }

    return kth(ch[u][1], k - size(ch[u][0]) - 1);
}

int rank_of(int x) {
    int a, b;

    split(root, x - 1, a, b);

    int ans = size(a) + 1;

    root = merge(a, b);

    return ans;
}

void init() {
    tot = root = 0;
}

}


/*
========================================================
4. PERSISTENT FHQ TREAP ORDERED MULTISET
========================================================

Dạng bài:

    cần lưu nhiều version của multiset.
    mỗi version insert / erase tạo root mới.

Ý tưởng:
    Khi thay đổi node, clone node đó.
    Merge/split đều clone trên đường đi.

Lưu ý:
    Persistent FHQ Treap code dài và dễ sai.
    Nếu bài chỉ cần kth trong đoạn static, thường Persistent Segment Tree dễ hơn.

Template này hỗ trợ:
    insert version mới
    erase version mới
    rank theo version
    kth theo version

========================================================
*/

namespace persistent_fhq {

int tot;
int ch[N * 20][2], pri[N * 20], val[N * 20], sz[N * 20], cnt[N * 20];

int size(int u) {
    return u ? sz[u] : 0;
}

int clone(int u) {
    if (!u) return 0;

    ++tot;

    ch[tot][0] = ch[u][0];
    ch[tot][1] = ch[u][1];
    pri[tot] = pri[u];
    val[tot] = val[u];
    sz[tot] = sz[u];
    cnt[tot] = cnt[u];

    return tot;
}

int node(int x) {
    ++tot;

    ch[tot][0] = ch[tot][1] = 0;
    pri[tot] = rng();
    val[tot] = x;
    sz[tot] = cnt[tot] = 1;

    return tot;
}

void pull(int u) {
    if (!u) return;
    sz[u] = size(ch[u][0]) + size(ch[u][1]) + cnt[u];
}

/*
Persistent split:
    a <= x
    b > x

Có clone node trước khi sửa con.
*/
void split(int u, int x, int &a, int &b) {
    if (!u) {
        a = b = 0;
        return;
    }

    u = clone(u);

    if (val[u] <= x) {
        a = u;
        split(ch[u][1], x, ch[u][1], b);
        pull(a);
    } else {
        b = u;
        split(ch[u][0], x, a, ch[u][0]);
        pull(b);
    }
}

int merge(int a, int b) {
    if (!a || !b) return a | b;

    if (pri[a] < pri[b]) {
        a = clone(a);
        ch[a][1] = merge(ch[a][1], b);
        pull(a);
        return a;
    } else {
        b = clone(b);
        ch[b][0] = merge(a, ch[b][0]);
        pull(b);
        return b;
    }
}

int ins(int root, int x) {
    int a, b, c;

    split(root, x, a, c);
    split(a, x - 1, a, b);

    if (b) {
        b = clone(b);
        cnt[b]++;
        pull(b);
    } else {
        b = node(x);
    }

    return merge(merge(a, b), c);
}

int del(int root, int x) {
    int a, b, c;

    split(root, x, a, c);
    split(a, x - 1, a, b);

    if (b) {
        b = clone(b);

        if (cnt[b] > 1) {
            cnt[b]--;
            pull(b);
        } else {
            b = merge(ch[b][0], ch[b][1]);
        }
    }

    return merge(merge(a, b), c);
}

int kth(int u, int k) {
    if (!u) return -1;

    if (k <= size(ch[u][0])) {
        return kth(ch[u][0], k);
    }

    k -= size(ch[u][0]);

    if (k <= cnt[u]) {
        return val[u];
    }

    return kth(ch[u][1], k - cnt[u]);
}

/*
rank trong một version:
    số phần tử < x + 1

Không split để tránh sinh node mới.
*/
int rank_of(int u, int x) {
    int ans = 1;

    while (u) {
        if (x <= val[u]) {
            u = ch[u][0];
        } else {
            ans += size(ch[u][0]) + cnt[u];
            u = ch[u][1];
        }
    }

    return ans;
}

void init() {
    tot = 0;
}

}


/*
========================================================
5. POINTER TREAP ORDERED SET
========================================================

Dạng pointer, gần với Treap cổ điển.

Ưu điểm:
    linh hoạt, dễ viết theo node*
Nhược điểm:
    trong CP đôi khi chậm hơn mảng tĩnh, khó reset hơn.

Hỗ trợ:
    insert
    erase
    rank
    kth

========================================================
*/

namespace ptr_treap {

struct Node {
    Node *l, *r;
    int val, pri, sz, cnt;

    Node(int _val) {
        l = r = nullptr;
        val = _val;
        pri = rng();
        sz = cnt = 1;
    }
};

using pNode = Node*;

int size(pNode t) {
    return t ? t->sz : 0;
}

void pull(pNode t) {
    if (!t) return;
    t->sz = size(t->l) + size(t->r) + t->cnt;
}

/*
split by value:
    a <= x
    b > x
*/
void split(pNode t, int x, pNode &a, pNode &b) {
    if (!t) {
        a = b = nullptr;
        return;
    }

    if (t->val <= x) {
        a = t;
        split(t->r, x, a->r, b);
        pull(a);
    } else {
        b = t;
        split(t->l, x, a, b->l);
        pull(b);
    }
}

pNode merge(pNode a, pNode b) {
    if (!a || !b) return a ? a : b;

    if (a->pri < b->pri) {
        a->r = merge(a->r, b);
        pull(a);
        return a;
    } else {
        b->l = merge(a, b->l);
        pull(b);
        return b;
    }
}

void ins(pNode &root, int x) {
    pNode a, b, c;

    split(root, x, a, c);
    split(a, x - 1, a, b);

    if (b) {
        b->cnt++;
        pull(b);
    } else {
        b = new Node(x);
    }

    root = merge(merge(a, b), c);
}

void del(pNode &root, int x) {
    pNode a, b, c;

    split(root, x, a, c);
    split(a, x - 1, a, b);

    if (b) {
        if (b->cnt > 1) {
            b->cnt--;
            pull(b);
        } else {
            b = merge(b->l, b->r);
        }
    }

    root = merge(merge(a, b), c);
}

int kth(pNode t, int k) {
    if (!t) return -1;

    if (k <= size(t->l)) {
        return kth(t->l, k);
    }

    k -= size(t->l);

    if (k <= t->cnt) {
        return t->val;
    }

    return kth(t->r, k - t->cnt);
}

int rank_of(pNode &root, int x) {
    pNode a, b;

    split(root, x - 1, a, b);

    int ans = size(a) + 1;

    root = merge(a, b);

    return ans;
}

}


/*
========================================================
6. QUICK USAGE
========================================================

--------------------------------------------------------
A. Ordered multiset FHQ
--------------------------------------------------------

fhq_set::init();

fhq_set::ins(x);
fhq_set::del(x);

int r = fhq_set::rank_of(x);
int x = fhq_set::kth(fhq_set::root, k);
int p = fhq_set::pre(x);
int s = fhq_set::suf(x);

--------------------------------------------------------
B. Implicit sequence FHQ
--------------------------------------------------------

fhq_seq::init();

vector<ll> a = {1, 2, 3, 4, 5};
fhq_seq::build(a);

fhq_seq::ins(pos, x);          // chèn x sau pos phần tử đầu
fhq_seq::del(l, r);            // xóa đoạn [l, r]
fhq_seq::reverse(l, r);        // đảo đoạn [l, r]
fhq_seq::addRange(l, r, x);    // cộng x vào đoạn [l, r]
fhq_seq::setRange(l, r, x);    // gán đoạn [l, r] = x

auto [sum, mn, mx] = fhq_seq::get(l, r);

ll val = fhq_seq::kth(pos);

vector<ll> cur = fhq_seq::getAll();

--------------------------------------------------------
C. Unique set FHQ
--------------------------------------------------------

fhq_unique::init();

fhq_unique::ins(x);
fhq_unique::del(x);
int r = fhq_unique::rank_of(x);
int val = fhq_unique::kth(fhq_unique::root, k);

--------------------------------------------------------
D. Persistent FHQ
--------------------------------------------------------

persistent_fhq::init();

vector<int> root(q + 1);
root[0] = 0;

root[i] = persistent_fhq::ins(root[i - 1], x);
root[i] = persistent_fhq::del(root[i - 1], x);

int val = persistent_fhq::kth(root[i], k);
int r = persistent_fhq::rank_of(root[i], x);

--------------------------------------------------------
E. Pointer Treap
--------------------------------------------------------

ptr_treap::pNode root = nullptr;

ptr_treap::ins(root, x);
ptr_treap::del(root, x);

int val = ptr_treap::kth(root, k);
int r = ptr_treap::rank_of(root, x);

========================================================
*/


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /*
    Đây là file template tổng hợp Treap / FHQ Treap.

    Khi làm bài thật:
        - ordered set/multiset: dùng fhq_set
        - dãy theo vị trí: dùng fhq_seq
        - giá trị unique: dùng fhq_unique
        - nhiều version: dùng persistent_fhq
        - thích pointer: dùng ptr_treap
    */

    return 0;
}
