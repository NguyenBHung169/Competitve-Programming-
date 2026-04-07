#include <bits/stdc++.h>
using namespace std;

#define L(i, a, b) for (int i = (a); i <= (b); ++i)

const int N = 200000 + 5;
const int INF = 1e9;

mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

int n, m, k, total_nodes;

/*
    Mỗi ô có zombie sẽ được nén thành 1 id cơ sở từ 1..total_nodes.

    Nhưng trong bài này, sau khi đụng zombie đầu tiên thì hạt dẻ sẽ đi chéo.
    Hướng đi chéo phụ thuộc vào việc nó đang ở "pha" nào của quỹ đạo phản xạ.

    Vì thế mỗi zombie/ô sẽ được tách thành 2 state:
    - state L : một kiểu pha/quỹ đạo
    - state R : kiểu pha/quỹ đạo còn lại

    Ta đánh số:
    - 1 .. total_nodes                  : state L
    - total_nodes+1 .. 2*total_nodes   : state R

    Như vậy một "chain phản xạ" thực chất là một dãy các state,
    và hạt dẻ luôn di chuyển dọc theo chain đó từ trái sang phải.
*/

struct Zombie {
    int col, row;   // vị trí ô (x, y)
} zombie[N << 1];

int remain_cnt[N];                 // số zombie còn lại trong ô đại diện bởi id
int next_state[N << 1], prev_state[N << 1];

/*
    first_in_row[row]:
    map theo cột -> zombie id còn sống ngoài cùng bên trái của hàng đó.

    Vì khi ném vào một hàng, hạt dẻ lăn ngang từ trái sang phải,
    nên zombie đầu tiên nó chạm phải chính là zombie ngoài cùng bên trái còn sống của hàng.

    first_in_row[row].begin()->second sẽ cho ta id đó.
*/
map<int, int> first_in_row[20005];

/*
    cell_cnt[(x, y)] = số zombie tại đúng ô đó.
    Đề cho phép nhiều zombie đứng cùng một ô, nên phải gộp lại.
*/
map<pair<int,int>, int> cell_cnt;

/*
    diag_group[id] chứa các state nằm trên cùng một "đường phản xạ".

    Sau khi đụng zombie đầu tiên, hạt dẻ đi chéo 45 độ, bật tường rồi lại đi tiếp.
    Nếu ta "trải phẳng" chuyển động phản xạ này ra, thì quỹ đạo của hạt dẻ trở thành
    một đường thẳng đi sang phải, đi qua một dãy state theo thứ tự tăng dần theo cột.

    Mỗi diag_group chính là một chain như vậy trước khi nối bằng next_state.
*/
vector<int> diag_group[N];


// ==========================================================
// Segment tree: lưu hàng tốt nhất hiện tại
// ==========================================================

/*
    best_value[p] = nếu xét đoạn hàng mà node p quản lý,
                    giá trị lớn nhất (số zombie có thể giết) là bao nhiêu

    best_row[p]   = hàng đạt giá trị đó

    Quy tắc đề bài:
    - luôn chọn hàng giết được nhiều zombie nhất
    - nếu hòa thì chọn hàng nhỏ nhất

    Vì thế trong pull_seg:
    - nếu 2 con bằng nhau, ta ưu tiên nhánh trái (hàng nhỏ hơn)
*/
int best_value[N << 2], best_row[N << 2];

inline void pull_seg(int p) {
    if (best_value[p << 1] >= best_value[p << 1 | 1]) {
        best_value[p] = best_value[p << 1];
        best_row[p] = best_row[p << 1];
    } else {
        best_value[p] = best_value[p << 1 | 1];
        best_row[p] = best_row[p << 1 | 1];
    }
}

void build_seg(int p, int l, int r) {
    if (l == r) {
        best_value[p] = 0;
        best_row[p] = l;
        return;
    }
    int mid = (l + r) >> 1;
    build_seg(p << 1, l, mid);
    build_seg(p << 1 | 1, mid + 1, r);
    pull_seg(p);
}

void update_seg(int p, int l, int r, int pos, int val) {
    if (l == r) {
        best_value[p] = val;
        return;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid) update_seg(p << 1, l, mid, pos, val);
    else update_seg(p << 1 | 1, mid + 1, r, pos, val);
    pull_seg(p);
}


// ==========================================================
// Implicit FHQ Treap
// ==========================================================

/*
    Mỗi chain phản xạ sẽ được lưu bằng 1 implicit FHQ treap.

    Vì sao dùng FHQ?
    - Ta cần cắt chain tại một vị trí bất kỳ (split theo thứ tự)
    - Ta cần nối lại 2 chain sau khi zombie bị xóa (merge)
    - Ta cần biết vị trí của một state trong chain
    - Ta cần trên mỗi suffix tìm "zombie còn hiệu lực đầu tiên"
      hoặc "key đầu tiên" rất nhanh

    Những việc này FHQ làm rất hợp:
    - split/merge trung bình O(log n)
    - lưu thêm thông tin prefix-min trên cây con
*/

/*
    Cấu trúc cây nhị phân của implicit treap
*/
int left_child[N << 1], right_child[N << 1], parent_node[N << 1], priority_key[N << 1];

/*
    subtree_size[u] = số node trong cây con u
    subtree_sum[u]  = tổng số zombie "còn sống" trong cây con u

    Ở đây mỗi state đại diện cho 1 ô đã nén.
    Nếu ô đó còn zombie thì đóng góp 1 đơn vị vào chain.
    subtree_sum của một suffix chính là số lần hạt dẻ còn có thể giết tiếp
    khi nó đã đi vào suffix đó.
*/
int subtree_size[N << 1], subtree_sum[N << 1];

/*
    Hai bộ biến quan trọng nhất của lời giải:

    1) active_pos / active_min
       Dùng cho thao tác collect():
       - Trong suffix hiện tại, tìm state đầu tiên mà hạt dẻ thực sự sẽ chạm tới và giết.
       - Sau khi giết xong một zombie trên chain, hạt dẻ tiếp tục đi sang phải và lại
         tìm zombie active tiếp theo, lặp lại đến khi không còn.

    2) key_pos / key_min
       Dùng cho việc "đóng góp" của cả chain lên segment tree:
       - Mỗi chain chỉ có đúng một state "key đầu tiên" đại diện cho việc:
         nếu ta bắn vào hàng tương ứng với state này, thì hạt dẻ sẽ đi vào chain đó.
       - Từ state key đầu tiên đến cuối chain, tổng số alive chính là số zombie
         mà hàng đó hiện có thể giết.

    is_alive[u]:
       - state này/ô này hiện còn ít nhất 1 zombie hay không
       - nếu không còn zombie, state vẫn có thể tồn tại trong chain, nhưng không còn đóng góp giết nữa
*/
int active_pos[N << 1], active_min[N << 1];
int key_pos[N << 1], key_min[N << 1];
int is_alive[N << 1];

inline int get_size(int u) { return u ? subtree_size[u] : 0; }
inline int get_sum(int u)  { return u ? subtree_sum[u] : 0; }
inline int get_active_min(int u) { return u ? active_min[u] : INF; }
inline int get_key_min(int u)    { return u ? key_min[u] : INF; }

/*
    pull(u):
    cập nhật toàn bộ thông tin tổng hợp của node u từ 2 con.

    subtree_size:
      kích thước cây con

    subtree_sum:
      tổng số state còn sống trong cây con

    active_min:
      vị trí nhỏ nhất trong thứ tự trung tố của một state active trong cây con

    key_min:
      vị trí nhỏ nhất trong thứ tự trung tố của một state key trong cây con

    Vì là implicit treap, "vị trí" không phải key cố định,
    mà được tính theo:
    - toàn bộ subtree trái đứng trước
    - rồi đến chính node
    - rồi đến subtree phải
*/
inline void pull(int u) {
    if (!u) return;

    if (left_child[u])  parent_node[left_child[u]] = u;
    if (right_child[u]) parent_node[right_child[u]] = u;

    subtree_size[u] = get_size(left_child[u]) + get_size(right_child[u]) + 1;
    subtree_sum[u]  = get_sum(left_child[u]) + get_sum(right_child[u]) + is_alive[u];

    active_min[u] = min({
        get_active_min(left_child[u]),
        get_size(left_child[u]) + active_pos[u],
        get_size(left_child[u]) + 1 + get_active_min(right_child[u])
    });

    key_min[u] = min({
        get_key_min(left_child[u]),
        get_size(left_child[u]) + key_pos[u],
        get_size(left_child[u]) + 1 + get_key_min(right_child[u])
    });
}

/*
    Tạo một treap node cho state id.

    alive_flag:
      = 1 nếu ô này hiện còn zombie
      = 0 nếu ô này đã trống

    key_flag:
      = 1 nếu đây là state "đầu vào" của một hàng,
           tức là hàng đó nếu bắn vào sẽ đi vào chain này từ đây
      = 0 nếu không

    Khi node không active / không key thì ta gán INF
    để nó không ảnh hưởng khi lấy min vị trí.
*/
int new_node(int id, int alive_flag, int key_flag) {
    priority_key[id] = (int)rng();
    left_child[id] = right_child[id] = parent_node[id] = 0;

    subtree_size[id] = 1;
    is_alive[id] = alive_flag;
    subtree_sum[id] = alive_flag;

    active_pos[id] = alive_flag ? 1 : INF;
    active_min[id] = active_pos[id];

    key_pos[id] = key_flag ? 1 : INF;
    key_min[id] = key_pos[id];

    return id;
}

/*
    split_treap(u, k, a, b):
    tách treap u thành:
    - a chứa k node đầu tiên theo thứ tự trung tố
    - b chứa phần còn lại

    Vì chain được hiểu là một dãy state theo thứ tự hạt dẻ đi qua,
    nên split theo "số node đầu" chính là cắt chain tại một vị trí.
*/
void split_treap(int u, int k, int &a, int &b) {
    if (!u) {
        a = b = 0;
        return;
    }

    if (get_size(left_child[u]) >= k) {
        split_treap(left_child[u], k, a, left_child[u]);
        if (left_child[u]) parent_node[left_child[u]] = u;
        b = u;
        parent_node[b] = 0;
        pull(b);
    } else {
        split_treap(right_child[u], k - get_size(left_child[u]) - 1, right_child[u], b);
        if (right_child[u]) parent_node[right_child[u]] = u;
        a = u;
        parent_node[a] = 0;
        pull(a);
    }
}

/*
    merge_treap(a, b):
    ghép 2 chain a rồi b lại thành một chain mới.

    Điều kiện đúng:
    - mọi node trong a phải đứng trước mọi node trong b trong thứ tự chain
*/
int merge_treap(int a, int b) {
    if (!a || !b) {
        int t = a ? a : b;
        if (t) parent_node[t] = 0;
        return t;
    }

    if (priority_key[a] < priority_key[b]) {
        right_child[a] = merge_treap(right_child[a], b);
        if (right_child[a]) parent_node[right_child[a]] = a;
        pull(a);
        parent_node[a] = 0;
        return a;
    } else {
        left_child[b] = merge_treap(a, left_child[b]);
        if (left_child[b]) parent_node[left_child[b]] = b;
        pull(b);
        parent_node[b] = 0;
        return b;
    }
}

inline int get_root(int u) {
    while (parent_node[u]) u = parent_node[u];
    return u;
}

/*
    get_pos(u):
    trả về vị trí của state u trong chain hiện tại.

    Rất quan trọng khi ta muốn:
    - cắt chain ngay trước u
    - hoặc tách đúng node u ra để sửa trạng thái
*/
inline int get_pos(int u) {
    int pos = get_size(left_child[u]) + 1;
    while (parent_node[u]) {
        if (u == right_child[parent_node[u]]) {
            pos += get_size(left_child[parent_node[u]]) + 1;
        }
        u = parent_node[u];
    }
    return pos;
}

inline int leftmost(int u) {
    while (left_child[u]) u = left_child[u];
    return u;
}

inline int rightmost(int u) {
    while (right_child[u]) u = right_child[u];
    return u;
}

/*
    base_id(state_id):
    từ state L/R quay về id zombie cơ sở.
*/
inline int base_id(int state_id) {
    return (state_id > total_nodes ? state_id - total_nodes : state_id);
}

/*
    zombie đầu tiên còn sống của hàng row
*/
inline int first_id_in_row(int row) {
    if (first_in_row[row].empty()) return 0;
    return first_in_row[row].begin()->second;
}

/*
    mod_node(u, alive_flag, key_flag):
    tách riêng đúng state u ra khỏi treap,
    rồi sửa lại 2 thuộc tính:
    - còn sống không
    - có phải key không

    Đây là primitive nền tảng dùng khi:
    - một ô bị xóa hết zombie => alive = 0
    - một ô trở thành zombie đầu tiên mới của hàng => key = 1
*/
void mod_node(int u, int alive_flag, int key_flag) {
    int root = get_root(u), pos = get_pos(u);
    int left_part, mid_part, right_part;

    split_treap(root, pos - 1, left_part, mid_part);
    split_treap(mid_part, 1, mid_part, right_part);

    is_alive[mid_part] = alive_flag;
    subtree_sum[mid_part] = alive_flag;

    active_pos[mid_part] = alive_flag ? 1 : INF;
    active_min[mid_part] = active_pos[mid_part];

    key_pos[mid_part] = key_flag ? 1 : INF;
    key_min[mid_part] = key_pos[mid_part];

    pull(mid_part);
    root = merge_treap(left_part, merge_treap(mid_part, right_part));
}

/*
    del_contri(root):
    Xóa contribution cũ của cả chain khỏi segment tree.

    Một chain chỉ đóng góp cho đúng 1 hàng:
    - đó là hàng của state key đầu tiên trong chain

    Nếu state key đầu tiên nằm ở vị trí p,
    thì số zombie hàng đó bắn được chính là:
    tổng số alive từ p đến cuối chain.

    Trước khi sửa chain, ta phải xóa contribution cũ này đi.
*/
void del_contri(int root) {
    if (!root) return;

    int pos = key_min[root];
    if (pos > subtree_size[root]) return;

    int left_part, right_part;
    split_treap(root, pos, left_part, right_part);

    int u = rightmost(left_part);
    update_seg(1, 1, n, zombie[base_id(u)].row, 0);

    root = merge_treap(left_part, right_part);
}

/*
    add_contri(root):
    Sau khi sửa chain xong, tính lại contribution mới của chain
    rồi cập nhật lên segment tree.

    Nếu p là vị trí key đầu tiên, thì suffix từ p trở đi chính là
    toàn bộ quãng mà hạt dẻ sẽ đi và có thể giết zombie.
    subtree_sum của suffix đó chính là số zombie hiện có thể giết.
*/
void add_contri(int root) {
    if (!root) return;

    int pos = key_min[root];
    if (pos > subtree_size[root]) return;

    int left_part, right_part;
    split_treap(root, pos - 1, left_part, right_part);

    int u = leftmost(right_part);
    update_seg(1, 1, n, zombie[base_id(u)].row, subtree_sum[right_part]);

    root = merge_treap(left_part, right_part);
}

int killed_list[N << 1], killed_top;

/*
    collect(root):
    Giả sử root là suffix của chain bắt đầu từ điểm hạt dẻ đã đi vào.

    Hàm này sẽ:
    - tìm zombie active đầu tiên trong suffix
    - thêm nó vào danh sách bị giết
    - tiếp tục đệ quy sang phần bên phải sau nó

    Ý nghĩa theo đề:
    hạt dẻ cứ tiếp tục lăn sang phải theo quỹ đạo phản xạ,
    mỗi khi gặp zombie tiếp theo thì giết 1 con, rồi đi tiếp,
    cho đến khi không gặp thêm zombie nào nữa.

    active_min giúp ta tìm rất nhanh zombie active đầu tiên trong suffix đó.
*/
int collect(int root) {
    if (!root) return root;

    int pos = active_min[root];
    if (pos > subtree_size[root]) return root;

    int left_part, right_part;
    split_treap(root, pos, left_part, right_part);

    int u = rightmost(left_part);
    killed_list[++killed_top] = base_id(u);

    right_part = collect(right_part);
    return merge_treap(left_part, right_part);
}

/*
    del_zb(u):
    Xóa hẳn ô u khỏi cấu trúc khi remain_cnt[u] giảm về 0.

    Đây là đoạn khó nhất của cả lời giải.

    Những việc cần làm:
    1) tìm zombie đầu tiên cũ của hàng này
    2) xóa u khỏi map hàng
    3) tìm zombie đầu tiên mới của hàng (nếu có)
    4) xóa contribution cũ của 2 chain chứa 2 state của u
    5) set cả 2 state của u thành dead + not key
    6) nếu u từng là first của hàng, thì gán key cho zombie first mới
    7) nếu u không nằm ở biên trên/dưới, thì khi bỏ nó đi,
       2 chain ở 2 state sẽ bị "đổi đầu mút" và phải nối chéo lại
    8) add contribution mới của các chain vừa thay đổi

    Vì sao phải nối chéo?
    - khi còn u, quỹ đạo phản xạ đi qua u làm cầu nối giữa 2 phần đường đi
    - khi xóa u khỏi hoàn toàn, quỹ đạo phản xạ ở 2 state của u không còn bám vào u nữa,
      nên 2 nửa chain phải ghép lại với nhau theo cấu trúc đối ứng
*/
void del_zb(int u) {
    int row = zombie[u].row;
    int old_first = first_id_in_row(row);

    first_in_row[row].erase(zombie[u].col);

    int new_first = first_id_in_row(row);
    if (row <= n / 2 && new_first) new_first += total_nodes;

    int root_left = get_root(u);
    int root_right = get_root(u + total_nodes);

    del_contri(root_left);
    del_contri(root_right);

    mod_node(u, 0, 0);
    mod_node(u + total_nodes, 0, 0);

    if (old_first == u && new_first) mod_node(new_first, 1, 1);

    /*
        Nếu u nằm ở hàng giữa (không phải biên 1 hay n),
        bỏ u đi sẽ làm 2 chain ở 2 state phải nối chéo lại.

        Ta cắt:
        - chain trái tại ngay trước u
        - chain phải tại ngay trước state đối ứng của u

        Rồi ghép:
        - nửa trước của chain trái + nửa sau của chain phải
        - nửa trước của chain phải + nửa sau của chain trái
    */
    if (row != 1 && row != n) {
        root_left = get_root(u);
        root_right = get_root(u + total_nodes);

        int pos_left = get_pos(u);
        int pos_right = get_pos(u + total_nodes);

        int a1, a2, b1, b2;
        split_treap(root_left, pos_left - 1, a1, a2);
        split_treap(root_right, pos_right - 1, b1, b2);

        root_left = merge_treap(a1, b2);
        root_right = merge_treap(b1, a2);
    }

    add_contri(get_root(u));
    add_contri(get_root(u + total_nodes));

    if (old_first == u && new_first) add_contri(get_root(new_first));
}

/*
    solve(start_id):
    Mô phỏng đúng 1 lượt ném hạt dẻ khi ta đã chọn được hàng tốt nhất.

    start_id là zombie đầu tiên của hàng đó.

    Theo đề:
    - nếu hàng nằm ở nửa trên thì sau cú va chạm đầu tiên, hạt dẻ đi chéo xuống phải
    - nếu ở nửa dưới thì đi chéo lên phải

    Vì vậy start_id phải được chuyển sang đúng state tương ứng.
*/
void solve(int start_id) {
    if (zombie[start_id].row <= n / 2) start_id += total_nodes;

    killed_top = 0;

    /*
        Cắt chain ngay trước state start_id.
        Phần right_part chính là đoạn đường đi thực sự của hạt dẻ sau khi nó
        đã đụng zombie đầu tiên và bắt đầu lăn chéo.
    */
    int root = get_root(start_id), pos = get_pos(start_id);
    int left_part, right_part;
    split_treap(root, pos - 1, left_part, right_part);

    /*
        Thu thập toàn bộ các ô bị giết trong lần ném này.
    */
    right_part = collect(right_part);
    root = merge_treap(left_part, right_part);

    /*
        Mỗi ô trong killed_list giảm đi 1 zombie.
        Nếu ô nào hết zombie thì phải xóa hẳn khỏi toàn bộ cấu trúc.
    */
    L(i, 1, killed_top) {
        int u = killed_list[i];
        remain_cnt[u]--;
        if (!remain_cnt[u]) del_zb(u);
    }
}


// ==========================================================
// Build initial chains
// ==========================================================

/*
    Sắp các state theo cột tăng dần trong cùng một chain phản xạ.
    Vì hạt dẻ luôn đi từ trái sang phải, thứ tự đúng trên chain phải theo cột.
*/
inline bool cmp_state(int a, int b) {
    if (a > total_nodes) a -= total_nodes;
    if (b > total_nodes) b -= total_nodes;
    return zombie[a].col < zombie[b].col;
}

/*
    build_chain(start_state):
    Từ state đầu chain, lần theo next_state để lấy toàn bộ dãy state,
    rồi merge thành một treap.

    Sau đó tìm state key đầu tiên của chain.
    Nếu state key đó nằm ở vị trí i trong chain độ dài len,
    thì số zombie hàng tương ứng bắn được ban đầu là len - i + 1
    (chính là số alive trong suffix từ key đó trở đi; ở thời điểm đầu mọi ô đều alive).
*/
void build_chain(int start_state) {
    killed_top = 0;
    int cur = start_state;
    while (cur) {
        killed_list[++killed_top] = cur;
        cur = next_state[cur];
    }

    int root = killed_list[1];
    L(i, 2, killed_top) root = merge_treap(root, killed_list[i]);

    L(i, 1, killed_top) {
        int state_id = killed_list[i];
        if (key_pos[state_id] == 1) {
            update_seg(1, 1, n, zombie[base_id(state_id)].row, killed_top - i + 1);
            return;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> k;

    build_seg(1, 1, n);

    /*
        B1. Đọc input và gộp các zombie trùng ô.
        Đề cho phép nhiều zombie cùng 1 ô, nhưng mỗi va chạm chỉ giết 1 con.
        Vì vậy mỗi ô chỉ cần lưu số lượng.
    */
    L(i, 1, m) {
        int x, y;
        cin >> x >> y;
        cell_cnt[{x, y}]++;
    }

    /*
        B2. Nén các ô có zombie thành id 1..total_nodes.
        Đồng thời xây map first_in_row để biết zombie ngoài cùng bên trái từng hàng.
    */
    for (auto &it : cell_cnt) {
        ++total_nodes;
        zombie[total_nodes] = {it.first.first, it.first.second};
        remain_cnt[total_nodes] = it.second;
        first_in_row[zombie[total_nodes].row][zombie[total_nodes].col] = total_nodes;
    }

    /*
        B3. Copy thông tin sang state còn lại.
        Hai state cùng đại diện cho cùng một ô vật lý,
        chỉ khác "pha quỹ đạo phản xạ".
    */
    L(i, 1, total_nodes) zombie[i + total_nodes] = zombie[i];

    /*
        B4. Đánh dấu state key ban đầu cho từng hàng.

        Nếu bắn vào một hàng, hạt dẻ lăn ngang trước, đụng zombie đầu tiên của hàng.
        Sau cú va chạm đó, nó chuyển vào đúng một state tùy hàng nằm ở nửa trên hay nửa dưới.

        Vì thế:
        - với hàng nửa trên, first zombie của hàng sẽ dùng state R
        - với hàng nửa dưới, first zombie của hàng sẽ dùng state L
    */
    static int initial_key[N << 1];
    L(row, 1, n) {
        int id = first_id_in_row(row);
        if (!id) continue;

        if (row <= n / 2) initial_key[id + total_nodes] = 1;
        else initial_key[id] = 1;
    }

    /*
        B5. Gom các state vào từng chain phản xạ.

        Hai công thức modulo ở đây chính là cách "trải phẳng" quỹ đạo phản xạ
        của đường chéo bật giữa 2 biên trên/dưới.

        Mỗi state chỉ tham gia vào chain nếu quỹ đạo tương ứng là hợp lệ:
        - state kiểu thứ nhất chỉ có nếu không ở biên trên
        - state kiểu thứ hai chỉ có nếu không ở biên dưới
    */
    L(i, 1, total_nodes) {
        int x = zombie[i].col;
        int y = zombie[i].row;

        if (y != 1) {
            int id = (x + y - 1) % (n * 2 - 2);
            diag_group[id].push_back(i);
        }
        if (y != n) {
            int id = ((x - y + 1) % (n * 2 - 2) + (n * 2 - 2)) % (n * 2 - 2);
            diag_group[id].push_back(i + total_nodes);
        }
    }

    /*
        B6. Trên mỗi chain, sắp theo cột tăng dần rồi nối next_state.

        Sau khi nối xong, next_state mô tả đúng "state tiếp theo mà hạt dẻ có thể gặp"
        nếu cứ tiếp tục đi sang phải trên quỹ đạo phản xạ.

        Đoạn đổi:
            v += (v <= total_nodes ? total_nodes : -total_nodes)
        là chuyển state của điểm kế tiếp sang pha đúng sau khi hạt dẻ chạm/bật.
    */
    L(i, 0, n * 2 - 3) {
        sort(diag_group[i].begin(), diag_group[i].end(), cmp_state);

        int sz = (int)diag_group[i].size();
        for (int j = 0; j + 1 < sz; ++j) {
            int u = diag_group[i][j];
            int v = diag_group[i][j + 1];

            if (zombie[base_id(v)].row != 1 && zombie[base_id(v)].row != n) {
                v += (v <= total_nodes ? total_nodes : -total_nodes);
            }

            next_state[u] = v;
            prev_state[v] = u;
        }
    }

    /*
        B7. Tạo toàn bộ treap node.
        Ban đầu ô nào có zombie thì alive = 1.
        Chỉ những state key đầu hàng mới có key = 1.
    */
    L(i, 1, total_nodes) {
        new_node(i, remain_cnt[i] > 0 ? 1 : 0, initial_key[i]);
        new_node(i + total_nodes, remain_cnt[i] > 0 ? 1 : 0, initial_key[i + total_nodes]);
    }

    /*
        B8. Với mỗi chain độc lập (state không có prev_state),
        build thành treap và ghi contribution ban đầu lên segment tree.
    */
    L(i, 1, total_nodes * 2) {
        if (!prev_state[i]) build_chain(i);
    }

    int answer = 0;

    /*
        B9. Thực hiện đúng K lượt:
        - segment tree cho ta hàng tốt nhất hiện tại
        - cộng số zombie giết được
        - nếu hàng đó còn bắn được ít nhất 1 zombie thì mô phỏng 1 lượt solve
    */
    while (k--) {
        cout << best_row[1] << ' ' << best_value[1] << '\n';

        answer += best_value[1];

        if (best_value[1] >= 1) {
            int start_id = first_id_in_row(best_row[1]);
            if (start_id) solve(start_id);
        }
    }

    cout << answer << '\n';
    return 0;
}
