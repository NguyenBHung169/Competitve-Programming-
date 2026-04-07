#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

struct node {
  node *l, *r;
  int key, prior;

  node(int _key) {
    l = r = nullptr;
    key = _key;          // key ở đây chính là giá trị BST, ví dụ vị trí i
    prior = rnd();       // priority ngẫu nhiên để giữ cân bằng treap
  }
};

struct FHQTreap {
  node *root;
  vector<int> vec;

  FHQTreap() {
    root = nullptr;
  }

  /*
    split(t, pos, l, r):
    Tách cây t thành 2 cây:
      - l chứa toàn bộ node có key <= pos
      - r chứa toàn bộ node có key > pos

    Đây là thao tác cốt lõi nhất của FHQ treap.

    Ý tưởng:
    - Nếu t->key <= pos thì bản thân t chắc chắn thuộc cây trái.
      Chỉ cần tiếp tục tách cây con phải của t.
    - Nếu t->key > pos thì bản thân t chắc chắn thuộc cây phải.
      Chỉ cần tiếp tục tách cây con trái của t.
  */
  void split(node *t, int pos, node *&l, node *&r) {
    if (!t) {
      l = r = nullptr;
      return;
    }

    if (t->key <= pos) {
      split(t->r, pos, t->r, r);
      l = t;
    } else {
      split(t->l, pos, l, t->l);
      r = t;
    }
  }

  /*
    merge(l, r):
    Ghép 2 cây l và r thành một cây duy nhất.
    Điều kiện bắt buộc:
      mọi key trong l < mọi key trong r

    Vì đã đảm bảo thứ tự key như vậy, ta chỉ cần dùng priority để quyết định ai làm gốc.
    Ở đây ta chọn node có prior nhỏ hơn làm gốc.
  */
  node* merge(node *l, node *r) {
    if (!l || !r) return l ? l : r;

    if (l->prior < r->prior) {
      l->r = merge(l->r, r);
      return l;
    } else {
      r->l = merge(l, r->l);
      return r;
    }
  }

  /*
    insert(id):
    Chèn một key mới vào treap.

    Cách làm chuẩn của FHQ:
    - Tách root thành:
        l = key <= id
        r = key > id
    - rồi ghép:
        l + new node(id) + r

    Lưu ý:
    - Template này giả sử bạn không chèn trùng key.
    - Nếu có thể trùng key thì phải xử lý thêm theo bài toán.
  */
  void insert(int id) {
    node *l, *r;
    split(root, id, l, r);
    root = merge(merge(l, new node(id)), r);
  }

  /*
    erase_range(L, R):
    Cắt toàn bộ các node có key thuộc đoạn [L, R] ra khỏi treap hiện tại.
    Trả về cây chứa đúng đoạn đó.

    Quy trình:
    - split(root, L-1) => tách thành:
        l : key <= L-1
        r : key >= L
    - split(r, R) => tách tiếp thành:
        mid : L <= key <= R
        mr  : key > R
    - root mới = merge(l, mr)
    - trả về mid

    Đây chính là kiểu thao tác rất hay dùng khi treap lưu các vị trí.
  */
  node* erase_range(int L, int R) {
    node *l, *r, *mid, *mr;
    split(root, L - 1, l, r);
    split(r, R, mid, mr);
    root = merge(l, mr);
    return mid;
  }

  /*
    merge_treap(a, b):
    Union 2 treap bất kỳ lại thành 1 treap.

    Khác với merge(l, r):
    - merge(l, r) yêu cầu mọi key ở l < mọi key ở r
    - merge_treap(a, b) thì không cần điều đó, 2 cây có thể xen kẽ key với nhau

    Ý tưởng union:
    - Chọn node gốc có priority tốt hơn
    - Tách cây còn lại theo key của node gốc
    - Phần <= key ghép vào cây trái
    - Phần > key ghép vào cây phải

    Độ phức tạp trung bình tốt khi dùng để union 2 BST ngẫu nhiên kiểu treap.
  */
  node* merge_treap(node *a, node *b) {
    if (!a) return b;
    if (!b) return a;

    // Thống nhất với merge(): priority nhỏ hơn là tốt hơn
    if (a->prior > b->prior) swap(a, b);

    node *L, *R;
    split(b, a->key, L, R);

    a->l = merge_treap(a->l, L);
    a->r = merge_treap(a->r, R);
    return a;
  }

  /*
    combine(x):
    Ghép toàn bộ cây x vào root hiện tại.
    Đây là wrapper cho merge_treap.
  */
  void combine(node *x) {
    root = merge_treap(root, x);
  }

  /*
    inorder:
    Duyệt inorder để lấy các key theo thứ tự tăng dần.
    Vì treap này là BST theo key nên inorder sẽ cho dãy đã sort.
  */
  void inorder(node *cur) {
    if (!cur) return;
    inorder(cur->l);
    vec.push_back(cur->key);
    inorder(cur->r);
  }

  /*
    get_all_keys():
    Trả về toàn bộ key trong treap theo thứ tự tăng dần.
  */
  vector<int> get_all_keys() {
    vec.clear();
    inorder(root);
    return vec;
  }

  /*
    contains(pos):
    Kiểm tra xem key = pos có tồn tại trong treap hay không.
    Đây chỉ là hàm phụ, không bắt buộc.
  */
  bool contains(node *cur, int pos) {
    if (!cur) return false;
    if (cur->key == pos) return true;
    if (pos < cur->key) return contains(cur->l, pos);
    return contains(cur->r, pos);
  }

  bool contains(int pos) {
    return contains(root, pos);
  }
};

/*
  =========================
  CÁCH HIỂU TEMPLATE NÀY
  =========================

  1) Đây là FHQ treap theo KEY
     - Mỗi node lưu một key
     - Cấu trúc BST theo key
     - Cân bằng ngẫu nhiên bằng priority

  2) Hai thao tác nền tảng:
     - split
     - merge

  3) Mọi thao tác khác đều xây từ 2 thao tác này:
     - insert
     - xóa đoạn key [L..R]
     - tách / ghép / union 2 cây

  4) Cách dùng rất hợp trong bài:
     "Mỗi màu / mỗi giá trị giữ một tập các vị trí xuất hiện"
     Khi đó:
       treap[color] sẽ chứa toàn bộ vị trí i mà a[i] = color

     Ví dụ:
       a = [1, 2, 1, 3, 2]
       treap[1] chứa {1, 3}
       treap[2] chứa {2, 5}
       treap[3] chứa {4}

     Nếu truy vấn:
       chuyển mọi vị trí thuộc đoạn [l, r] có giá trị x sang giá trị y

     Ta làm:
       node *p = treap[x].erase_range(l, r);
       treap[y].combine(p);

     Vì:
       - erase_range(l, r) cắt đúng các vị trí trong [l, r] của màu x
       - combine(p) ghép toàn bộ các vị trí đó sang màu y
*/

FHQTreap t[N];
int ans[N];

int32_t main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  /*
    Ví dụ đúng theo bài gốc của bạn:

    Input:
    - n
    - dãy a[1..n]
    - q truy vấn dạng:
        l r x y
      nghĩa là:
      lấy mọi vị trí i trong [l, r] đang có giá trị x, chuyển thành y
  */

  int n;
  cin >> n;

  for (int i = 1; i <= n; i++) {
    int k;
    cin >> k;
    t[k].insert(i);   // giá trị k xuất hiện ở vị trí i
  }

  int q;
  cin >> q;

  while (q--) {
    int l, r, x, y;
    cin >> l >> r >> x >> y;

    if (x == y) continue;   // cùng màu thì không cần làm gì

    node *p = t[x].erase_range(l, r); // lấy các vị trí thuộc [l, r] của màu x
    t[y].combine(p);                  // nhập chúng vào màu y
  }

  /*
    Khôi phục mảng kết quả:
    với mỗi giá trị v, duyệt toàn bộ vị trí trong treap[v],
    rồi gán ans[pos] = v
  */
  for (int v = 1; v < N; v++) {
    auto pos = t[v].get_all_keys();
    for (auto id : pos) ans[id] = v;
  }

  for (int i = 1; i <= n; i++) {
    cout << ans[i] << " \n"[i == n];
  }

  return 0;
}
