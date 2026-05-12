#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 200000 + 5;
const int MAXX = 1000 + 5;
const int MAXY = 1000 + 5;

/*
========================================================
                    FENWICK TREE / BIT
========================================================

BIT dùng tốt cho các dạng:

1. Point Add + Prefix Sum
   - add(pos, val): a[pos] += val
   - sumPrefix(pos): a[1] + ... + a[pos]
   - sumRange(l, r): a[l] + ... + a[r]

2. Set Value
   - setVal(pos, val): a[pos] = val
   - cần lưu thêm mảng a[]

3. K-th prefix
   - kth(k): tìm vị trí nhỏ nhất pos sao cho sumPrefix(pos) >= k
   - chỉ dùng khi các giá trị trong BIT không âm

4. Range Add + Point Query
   - addRange(l, r, val): a[l..r] += val
   - getPoint(pos): lấy a[pos]

5. Range Add + Range Sum
   - addRange(l, r, val): a[l..r] += val
   - sumRange(l, r): lấy tổng a[l..r]
   - dùng 2 BIT

6. BIT 2D
   - add(x, y, val)
   - sumRect(x1, y1, x2, y2)

========================================================
*/


/*
========================================================
        1. BIT CƠ BẢN: POINT ADD + RANGE SUM
========================================================

Ý tưởng:

bit[i] lưu tổng của một đoạn kết thúc tại i.
Độ dài đoạn đó là lowbit(i).

Ví dụ:
i = 12 = 1100b
lowbit(12) = 4
=> bit[12] quản lý đoạn [9..12]

Các thao tác:
- add(pos, val): cộng val vào a[pos]
- sumPrefix(pos): tính tổng a[1..pos]
- sumRange(l, r): tính tổng a[l..r]
- kth(k): tìm vị trí nhỏ nhất có prefix sum >= k

Điều kiện kth:
- các giá trị trong BIT phải không âm
- thường dùng khi BIT lưu tần suất
*/

struct BIT {
    int n;
    ll bit[MAXN];

    // lowbit(x) lấy ra phần 2^k thấp nhất của x
    //
    // Ví dụ:
    // x = 12 = 1100b => lowbit = 0100b = 4
    // x = 10 = 1010b => lowbit = 0010b = 2
    int lowbit(int x) {
        return x & -x;
    }

    // Khởi tạo BIT với size n
    //
    // Phải gọi init(n) trước khi dùng.
    void init(int _n) {
        n = _n;

        for (int i = 1; i <= n; i++) {
            bit[i] = 0;
        }
    }

    // Cộng val vào vị trí idx:
    //
    // a[idx] += val
    //
    // Khi a[idx] thay đổi, ta phải cập nhật tất cả bit[i]
    // mà đoạn quản lý của bit[i] có chứa idx.
    //
    // Các i đó được sinh bằng:
    // idx, idx + lowbit(idx), ...
    void add(int idx, ll val) {
        for (int i = idx; i <= n; i += lowbit(i)) {
            bit[i] += val;
        }
    }

    // Tính tổng prefix:
    //
    // return a[1] + a[2] + ... + a[idx]
    //
    // Ta tách prefix [1..idx] thành các đoạn rời nhau
    // được quản lý bởi BIT.
    //
    // Ví dụ idx = 13:
    // 13 -> 12 -> 8 -> 0
    //
    // sumPrefix(13) = bit[13] + bit[12] + bit[8]
    ll sumPrefix(int idx) {
        ll res = 0;

        for (int i = idx; i > 0; i -= lowbit(i)) {
            res += bit[i];
        }

        return res;
    }

    // Tính tổng đoạn [l, r]
    //
    // sum(l, r) = prefix(r) - prefix(l - 1)
    ll sumRange(int l, int r) {
        if (l > r) return 0;

        return sumPrefix(r) - sumPrefix(l - 1);
    }

    // Tìm vị trí nhỏ nhất pos sao cho:
    //
    // sumPrefix(pos) >= k
    //
    // Điều kiện:
    // - BIT lưu các giá trị không âm
    // - tổng toàn mảng phải >= k
    //
    // Nếu tổng toàn mảng < k thì trả về n + 1.
    //
    // Ứng dụng:
    // - tìm phần tử thứ k trong multiset
    // - tìm k-th order statistic sau khi nén tọa độ
    // - tìm vị trí theo prefix sum
    int kth(ll k) {
        if (sumPrefix(n) < k) return n + 1;

        int pos = 0;

        // pw là lũy thừa 2 lớn nhất <= n
        int pw = 1;
        while ((pw << 1) <= n) pw <<= 1;

        // Binary lifting trên cây BIT
        //
        // Ý tưởng:
        // pos đang là vị trí lớn nhất biết rằng prefix(pos) < k.
        // Thử nhảy thêm step.
        // Nếu prefix(pos + step) vẫn < k,
        // thì đáp án chắc chắn nằm bên phải pos + step.
        for (int step = pw; step > 0; step >>= 1) {
            int nxtPos = pos + step;

            if (nxtPos <= n && bit[nxtPos] < k) {
                pos = nxtPos;
                k -= bit[nxtPos];
            }
        }

        return pos + 1;
    }
};


/*
========================================================
        2. BIT CÓ HỖ TRỢ SET VALUE
========================================================

BIT cơ bản chỉ hỗ trợ:
    a[pos] += val

Nếu muốn:
    a[pos] = val

thì ta cần lưu mảng a[pos] hiện tại.

Khi set:
    delta = val - a[pos]
    a[pos] = val
    BIT.add(pos, delta)
*/

struct BITSet {
    int n;
    ll a[MAXN];
    BIT fw;

    // Khởi tạo
    void init(int _n) {
        n = _n;
        fw.init(n);

        for (int i = 1; i <= n; i++) {
            a[i] = 0;
        }
    }

    // Build từ mảng ban đầu arr[1..n]
    void build(vector<ll> arr) {
        n = (int)arr.size() - 1;
        init(n);

        for (int i = 1; i <= n; i++) {
            a[i] = arr[i];
            fw.add(i, a[i]);
        }
    }

    // Cộng val vào a[pos]
    void add(int pos, ll val) {
        a[pos] += val;
        fw.add(pos, val);
    }

    // Gán a[pos] = val
    void setVal(int pos, ll val) {
        ll delta = val - a[pos];

        a[pos] = val;
        fw.add(pos, delta);
    }

    // Lấy a[pos]
    ll getVal(int pos) {
        return a[pos];
    }

    // Tổng prefix [1..pos]
    ll sumPrefix(int pos) {
        return fw.sumPrefix(pos);
    }

    // Tổng đoạn [l..r]
    ll sumRange(int l, int r) {
        return fw.sumRange(l, r);
    }
};


/*
========================================================
        3. RANGE ADD + POINT QUERY
========================================================

Dạng bài:
    update l r val:
        a[l] += val
        a[l + 1] += val
        ...
        a[r] += val

    query pos:
        hỏi a[pos]

Ý tưởng dùng mảng hiệu diff:

Nếu:
    diff[l] += val
    diff[r + 1] -= val

thì:
    a[pos] = diff[1] + diff[2] + ... + diff[pos]

Vậy ta dùng BIT để lưu diff.

Thao tác:
- addRange(l, r, val)
- getPoint(pos)
*/

struct BITRangePoint {
    int n;
    ll bit[MAXN];

    int lowbit(int x) {
        return x & -x;
    }

    void init(int _n) {
        n = _n;

        for (int i = 1; i <= n; i++) {
            bit[i] = 0;
        }
    }

    // Cộng val vào diff[idx]
    void addPoint(int idx, ll val) {
        for (int i = idx; i <= n; i += lowbit(i)) {
            bit[i] += val;
        }
    }

    // Cộng val vào đoạn [l, r]
    //
    // diff[l] += val
    // diff[r + 1] -= val
    void addRange(int l, int r, ll val) {
        if (l > r) return;

        addPoint(l, val);

        if (r + 1 <= n) {
            addPoint(r + 1, -val);
        }
    }

    // Lấy a[idx] thật
    //
    // a[idx] = diff[1] + ... + diff[idx]
    ll getPoint(int idx) {
        ll res = 0;

        for (int i = idx; i > 0; i -= lowbit(i)) {
            res += bit[i];
        }

        return res;
    }
};


/*
========================================================
        4. RANGE ADD + RANGE SUM
========================================================

Dạng bài:
    update l r val:
        a[l..r] += val

    query l r:
        hỏi tổng a[l..r]

Dùng 2 BIT.

Công thức quan trọng:

Sau các cập nhật trên mảng hiệu, prefix sum thật của a[1..x]:

    sumPrefix(x) = x * sum(bit1, x) - sum(bit2, x)

Khi muốn cộng val vào đoạn [l, r], ta làm:

    addPrefix(l, val)
    addPrefix(r + 1, -val)

Trong đó:

    addPrefix(pos, val):
        bit1[pos] += val
        bit2[pos] += val * (pos - 1)

Sau đó:

    sumRange(l, r) = sumPrefix(r) - sumPrefix(l - 1)

Đây là phiên bản BIT mạnh nhất trong 1D thường dùng.
*/

struct BITRangeSum {
    int n;
    ll bit1[MAXN], bit2[MAXN];

    int lowbit(int x) {
        return x & -x;
    }

    void init(int _n) {
        n = _n;

        for (int i = 1; i <= n; i++) {
            bit1[i] = 0;
            bit2[i] = 0;
        }
    }

    // Cộng val vào một BIT tại idx
    void add(ll bit[], int idx, ll val) {
        for (int i = idx; i <= n; i += lowbit(i)) {
            bit[i] += val;
        }
    }

    // Lấy prefix sum của một BIT
    ll sum(ll bit[], int idx) {
        ll res = 0;

        for (int i = idx; i > 0; i -= lowbit(i)) {
            res += bit[i];
        }

        return res;
    }

    // Hàm nội bộ.
    //
    // Dùng để cập nhật ảnh hưởng từ idx trở đi.
    //
    // Nếu gọi addPrefix(idx, val), thì với mọi x >= idx,
    // prefix sum sẽ bị ảnh hưởng thêm:
    //
    // val * (x - idx + 1)
    //
    // Công thức được biến đổi thành:
    // x * sum(bit1, x) - sum(bit2, x)
    void addPrefix(int idx, ll val) {
        add(bit1, idx, val);
        add(bit2, idx, val * (idx - 1));
    }

    // Cộng val vào đoạn [l, r]
    //
    // addPrefix(l, val)
    // addPrefix(r + 1, -val)
    void addRange(int l, int r, ll val) {
        if (l > r) return;

        addPrefix(l, val);

        if (r + 1 <= n) {
            addPrefix(r + 1, -val);
        }
    }

    // Tính tổng prefix thật:
    //
    // a[1] + a[2] + ... + a[idx]
    ll sumPrefix(int idx) {
        ll s1 = sum(bit1, idx);
        ll s2 = sum(bit2, idx);

        return s1 * idx - s2;
    }

    // Tính tổng đoạn [l, r]
    ll sumRange(int l, int r) {
        if (l > r) return 0;

        return sumPrefix(r) - sumPrefix(l - 1);
    }
};


/*
========================================================
        5. BIT 2D: POINT ADD + RECTANGLE SUM
========================================================

Dạng bài trên lưới:

    update x y val:
        a[x][y] += val

    query x1 y1 x2 y2:
        hỏi tổng hình chữ nhật:
        x1 <= x <= x2
        y1 <= y <= y2

BIT 2D là mở rộng của BIT 1D.

Complexity:
    add:      O(log n * log m)
    sumRect:  O(log n * log m)

Lưu ý:
- BIT 2D dùng mảng lớn khá tốn bộ nhớ.
- Nếu tọa độ lớn nhưng số điểm ít, nên dùng nén tọa độ hoặc BIT 2D dạng vector/map.
*/

struct BIT2D {
    int n, m;
    ll bit[MAXX][MAXY];

    int lowbit(int x) {
        return x & -x;
    }

    void init(int _n, int _m) {
        n = _n;
        m = _m;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                bit[i][j] = 0;
            }
        }
    }

    // Cộng val vào điểm (x, y)
    void add(int x, int y, ll val) {
        for (int i = x; i <= n; i += lowbit(i)) {
            for (int j = y; j <= m; j += lowbit(j)) {
                bit[i][j] += val;
            }
        }
    }

    // Tổng hình chữ nhật [1..x][1..y]
    ll sumPrefix(int x, int y) {
        ll res = 0;

        for (int i = x; i > 0; i -= lowbit(i)) {
            for (int j = y; j > 0; j -= lowbit(j)) {
                res += bit[i][j];
            }
        }

        return res;
    }

    // Tổng hình chữ nhật [x1..x2][y1..y2]
    //
    // Dùng nguyên lý bao hàm - loại trừ:
    //
    // sum(x1, y1, x2, y2)
    // = pref(x2, y2)
    // - pref(x1 - 1, y2)
    // - pref(x2, y1 - 1)
    // + pref(x1 - 1, y1 - 1)
    ll sumRect(int x1, int y1, int x2, int y2) {
        if (x1 > x2 || y1 > y2) return 0;

        return sumPrefix(x2, y2)
             - sumPrefix(x1 - 1, y2)
             - sumPrefix(x2, y1 - 1)
             + sumPrefix(x1 - 1, y1 - 1);
    }
};


/*
========================================================
        6. NÉN TỌA ĐỘ
========================================================

BIT thường cần chỉ số từ 1..n.

Nếu giá trị lớn:
    a[i] có thể tới 1e9, 1e18

thì cần nén tọa độ.

Ví dụ:
    a = [100, 5, 1000000000]

Sau nén:
    5          -> 1
    100        -> 2
    1000000000 -> 3

Dùng trong:
- đếm inversion
- đếm số phần tử nhỏ hơn/lớn hơn
- offline queries
- kth order statistic
*/

vector<int> compressVector(vector<ll> vals) {
    sort(vals.begin(), vals.end());

    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    vector<int> res;
    return res;
}

// Lấy id nén của x trong vector vals đã sort unique.
//
// Lưu ý:
// vals phải là vector đã được sort và unique trước.
int getCompressedId(const vector<ll> &vals, ll x) {
    return lower_bound(vals.begin(), vals.end(), x) - vals.begin() + 1;
}


/*
========================================================
        7. ĐẾM INVERSION BẰNG BIT
========================================================

Inversion là cặp:
    i < j và a[i] > a[j]

Cách làm:

Duyệt từ phải sang trái.
Khi đứng ở i, các phần tử bên phải đã được thêm vào BIT.

Số inversion với i là:
    số phần tử bên phải < a[i]

Nếu x là id nén của a[i], số phần tử nhỏ hơn a[i] là:
    fw.sumPrefix(x - 1)

Sau đó thêm a[i] vào BIT:
    fw.add(x, 1)
*/

ll countInversion(vector<ll> a) {
    // a dùng 1-index
    int n = (int)a.size() - 1;

    vector<ll> vals;

    for (int i = 1; i <= n; i++) {
        vals.push_back(a[i]);
    }

    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    BIT fw;
    fw.init((int)vals.size());

    ll inv = 0;

    for (int i = n; i >= 1; i--) {
        int x = getCompressedId(vals, a[i]);

        // Đếm số phần tử nhỏ hơn a[i] ở bên phải
        inv += fw.sumPrefix(x - 1);

        // Thêm a[i] vào cấu trúc
        fw.add(x, 1);
    }

    return inv;
}


/*
========================================================
        8. ĐẾM SỐ PHẦN TỬ < x, <= x, > x, >= x
========================================================

Giả sử BIT đang lưu tần suất các giá trị sau khi nén.

Với x là giá trị gốc, vals là mảng giá trị đã sort unique:

- số phần tử < x:
    id = lower_bound(vals, x)
    lấy sumPrefix(id)

- số phần tử <= x:
    id = upper_bound(vals, x)
    lấy sumPrefix(id)

Do BIT dùng 1-index:
    lower_bound trả số lượng phần tử < x
    upper_bound trả số lượng phần tử <= x

Các hàm dưới đây dùng cho bài offline hoặc dynamic frequency.
*/

ll countLess(BIT &fw, const vector<ll> &vals, ll x) {
    int id = lower_bound(vals.begin(), vals.end(), x) - vals.begin();

    // id chính là số lượng giá trị nén <= vị trí id
    // vì BIT 1-index nên prefix(id) là số phần tử có value < x
    return fw.sumPrefix(id);
}

ll countLessEqual(BIT &fw, const vector<ll> &vals, ll x) {
    int id = upper_bound(vals.begin(), vals.end(), x) - vals.begin();

    return fw.sumPrefix(id);
}

ll countGreater(BIT &fw, const vector<ll> &vals, ll x) {
    ll total = fw.sumPrefix((int)vals.size());

    return total - countLessEqual(fw, vals, x);
}

ll countGreaterEqual(BIT &fw, const vector<ll> &vals, ll x) {
    ll total = fw.sumPrefix((int)vals.size());

    return total - countLess(fw, vals, x);
}


/*
========================================================
        9. CÁCH DÙNG TỪNG LOẠI BIT
========================================================

Bạn có thể đọc phần này như sheet hướng dẫn sử dụng.

--------------------------------------------------------
A. Point Add + Range Sum
--------------------------------------------------------

BIT fw;
fw.init(n);

fw.add(pos, val);             // a[pos] += val
fw.sumPrefix(pos);            // tổng a[1..pos]
fw.sumRange(l, r);            // tổng a[l..r]

--------------------------------------------------------
B. Set Value
--------------------------------------------------------

BITSet fw;
fw.init(n);

fw.setVal(pos, val);          // a[pos] = val
fw.add(pos, delta);           // a[pos] += delta
fw.getVal(pos);               // lấy a[pos]
fw.sumRange(l, r);            // tổng a[l..r]

--------------------------------------------------------
C. K-th Prefix
--------------------------------------------------------

BIT fw;
fw.init(n);

fw.add(x, 1);                 // thêm một phần tử có id x
fw.add(x, -1);                // xóa một phần tử có id x

int pos = fw.kth(k);          // vị trí nhỏ nhất sao cho prefix(pos) >= k

Điều kiện:
- tần suất không âm
- tổng số phần tử >= k

--------------------------------------------------------
D. Range Add + Point Query
--------------------------------------------------------

BITRangePoint fw;
fw.init(n);

fw.addRange(l, r, val);       // a[l..r] += val
fw.getPoint(pos);             // hỏi a[pos]

--------------------------------------------------------
E. Range Add + Range Sum
--------------------------------------------------------

BITRangeSum fw;
fw.init(n);

fw.addRange(l, r, val);       // a[l..r] += val
fw.sumPrefix(pos);            // tổng a[1..pos]
fw.sumRange(l, r);            // tổng a[l..r]

--------------------------------------------------------
F. BIT 2D
--------------------------------------------------------

BIT2D fw;
fw.init(n, m);

fw.add(x, y, val);            // a[x][y] += val
fw.sumPrefix(x, y);           // tổng hình chữ nhật [1..x][1..y]
fw.sumRect(x1, y1, x2, y2);   // tổng hình chữ nhật bất kỳ

--------------------------------------------------------
G. Inversion
--------------------------------------------------------

vector<ll> a(n + 1);
for(int i = 1; i <= n; i++) cin >> a[i];

cout << countInversion(a) << '\n';

========================================================
*/


/*
========================================================
        10. MAIN DEMO
========================================================

Phần main này chỉ minh họa cách gọi.
Khi làm bài thật, bạn giữ template phía trên và tự viết main theo đề.
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /*
    ====================================================
    DEMO 1: BIT cơ bản
    ====================================================

    Input ví dụ:
    n = 5
    a = 1 2 3 4 5

    Query:
    sumRange(2, 4) = 2 + 3 + 4 = 9
    add(3, 10) => a[3] = 13
    sumRange(2, 4) = 2 + 13 + 4 = 19
    */

    int n = 5;

    BIT fw;
    fw.init(n);

    vector<ll> a = {0, 1, 2, 3, 4, 5};

    for (int i = 1; i <= n; i++) {
        fw.add(i, a[i]);
    }

    // cout << fw.sumRange(2, 4) << '\n'; // 9

    fw.add(3, 10);

    // cout << fw.sumRange(2, 4) << '\n'; // 19


    /*
    ====================================================
    DEMO 2: Range Add + Point Query
    ====================================================
    */

    BITRangePoint rp;
    rp.init(n);

    rp.addRange(2, 4, 10);

    // a[2], a[3], a[4] được cộng 10
    // cout << rp.getPoint(3) << '\n'; // 10
    // cout << rp.getPoint(5) << '\n'; // 0


    /*
    ====================================================
    DEMO 3: Range Add + Range Sum
    ====================================================
    */

    BITRangeSum rs;
    rs.init(n);

    rs.addRange(2, 4, 10);

    // cout << rs.sumRange(1, 5) << '\n'; // 30
    // cout << rs.sumRange(3, 4) << '\n'; // 20


    /*
    ====================================================
    DEMO 4: Đếm inversion
    ====================================================
    */

    vector<ll> b = {0, 3, 1, 2};

    // inversion: (3,1), (3,2) => 2
    // cout << countInversion(b) << '\n'; // 2


    return 0;
}
