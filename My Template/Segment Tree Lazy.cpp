struct ST {
  #define lc (n << 1)
  #define rc ((n << 1) | 1)
  long long t[4 * N], lazy[4 * N];

  ST() {
    memset(t,    0, sizeof t);
    memset(lazy, 0, sizeof lazy);
  }

  // Đẩy tag lazy xuống con và cập nhật giá trị nút
  inline void push(int n, int b, int e) {
    if (lazy[n] == 0) return;
    t[n] += lazy[n] * (e - b + 1);
    if (b != e) {
      lazy[lc] += lazy[n];
      lazy[rc] += lazy[n];
    }
    lazy[n] = 0;
  }

  // Gộp kết quả hai con (sum)
  inline long long combine(long long L, long long R) {
    return L + R;
  }

  // Cập nhật giá trị nút từ hai con
  inline void pull(int n) {
    t[n] = t[lc] + t[rc];
  }

  // Xây cây từ a[b..e]
  void build(int n, int b, int e) {
    lazy[n] = 0;
    if (b == e) {
      t[n] = a[b];
      return;
    }
    int mid = (b + e) >> 1;
    build(lc, b, mid);
    build(rc, mid + 1, e);
    pull(n);
  }

  // Cộng v vào toàn bộ đoạn [i..j]
  void upd(int n, int b, int e, int i, int j, long long v) {
    push(n, b, e);
    if (j < b || e < i) return;        // không giao nhau
    if (i <= b && e <= j) {
      lazy[n] = v;                      // đánh dấu lazy
      push(n, b, e);
      return;
    }
    int mid = (b + e) >> 1;
    upd(lc, b, mid, i, j, v);
    upd(rc, mid + 1, e, i, j, v);
    pull(n);
  }

  // Truy vấn tổng trên đoạn [i..j]
  long long query(int n, int b, int e, int i, int j) {
    push(n, b, e);
    if (j < b || e < i) return 0;      // phần tử trung tính của sum
    if (i <= b && e <= j) return t[n];
    int mid = (b + e) >> 1;
    return combine(
      query(lc, b, mid, i, j),
      query(rc, mid + 1, e, i, j)
    );
  }
};
