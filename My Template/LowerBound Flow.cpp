struct Dinic {
    struct edge {
        int to, rev, id;
        long long flow, w;
    };

    int n, s, t, mxid;
    vector<int> d, done, flow_through;
    vector<vector<edge>> g;

    Dinic() {}

    Dinic(int _n) {
        n = _n + 10;
        mxid = 0;
        g.resize(n);
    }

    void add_edge(int u, int v, long long w, int id = -1) {
        edge a = {v, (int)g[v].size(), id, 0, w};
        edge b = {u, (int)g[u].size(), -2, 0, 0};
        g[u].push_back(a);
        g[v].push_back(b);
        mxid = max(mxid, id);
    }

    bool bfs() {
        d.assign(n, -1);
        d[s] = 0;
        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : g[u]) {
                int v = e.to;
                if (d[v] == -1 && e.flow < e.w) {
                    d[v] = d[u] + 1;
                    q.push(v);
                }
            }
        }
        return d[t] != -1;
    }

    long long dfs(int u, long long flow) {
        if (u == t) return flow;

        for (int &i = done[u]; i < (int)g[u].size(); i++) {
            edge &e = g[u][i];
            if (e.w <= e.flow) continue;

            int v = e.to;
            if (d[v] != d[u] + 1) continue;

            long long nw = dfs(v, min(flow, e.w - e.flow));
            if (nw > 0) {
                e.flow += nw;
                g[v][e.rev].flow -= nw;
                return nw;
            }
        }
        return 0;
    }

    long long max_flow(int _s, int _t) {
        s = _s;
        t = _t;
        long long flow = 0;
        const long long inf = (long long)4e18;

        while (bfs()) {
            done.assign(n, 0);
            while (long long nw = dfs(s, inf)) {
                flow += nw;
            }
        }

        flow_through.assign(mxid + 1, 0);
        for (int i = 0; i < n; i++) {
            for (auto &e : g[i]) {
                if (e.id >= 0) {
                    flow_through[e.id] = e.flow;
                }
            }
        }
        return flow;
    }
};

// ===================== Lớp bọc lower-bound flow =====================
// Thay cho namespace Dinic trong code cũ
struct LowerBoundFlow {
    int N;
    vector<long long> demand;
    vector<unordered_set<int>> st;
    Dinic mf;

    LowerBoundFlow() {}

    LowerBoundFlow(int _n) {
        init(_n);
    }

    void init(int _n) {
        N = _n;
        demand.assign(N + 5, 0);
        st.assign(N + 5, {});
        mf = Dinic(N + 5);
    }

    // thay cho Dinic::AddEdge(u, v, l, r) trong code cũ
    void AddEdge(int u, int v, long long l, long long r) {
        if (st[u].count(v)) return;   // giữ nguyên logic chống cạnh trùng
        if (l > 0) st[u].insert(v);

        demand[u] -= l;
        demand[v] += l;
        mf.add_edge(u, v, r - l);
    }

    // thay cho Dinic::solve(s, t) trong code cũ
    long long solve(int s, int t) {
        int SS = t + 1;
        int TT = t + 2;

        long long need = 0;
        for (int i = s; i <= t; i++) {
            if (demand[i] > 0) {
                mf.add_edge(SS, i, demand[i]);
                need += demand[i];
            }
            if (demand[i] < 0) {
                mf.add_edge(i, TT, -demand[i]);
            }
        }

        long long pushed = mf.max_flow(SS, TT);
        return need - pushed;
    }
};

/*
    ========================= LOWER BOUND FLOW - CÁCH DÙNG =========================

    Template này xử lý bài toán flow có cận dưới trên cạnh:

        l <= f(u, v) <= r

    Ý tưởng:
    - Mỗi cạnh [l, r] được đổi thành cạnh thường có capacity = r - l
    - Phần cận dưới l được "ép trước", nên làm lệch cân bằng luồng ở các đỉnh
    - Ta dùng mảng demand[] để ghi lại độ lệch đó:
            demand[u] -= l
            demand[v] += l
    - Sau đó thêm:
            SS -> i   với capacity = demand[i]   nếu demand[i] > 0
            i  -> TT  với capacity = -demand[i]  nếu demand[i] < 0
    - Chạy maxflow từ SS sang TT để kiểm tra có bù hết demand hay không

    -------------------------------------------------------------------------------
    1. KHỞI TẠO
    -------------------------------------------------------------------------------
        LowerBoundFlow LBF(n + 5);

    hoặc:
        LBF.init(n + 5);

    trong đó n là số đỉnh gốc của bài.

    -------------------------------------------------------------------------------
    2. THÊM CẠNH CÓ CẬN DƯỚI / CẬN TRÊN
    -------------------------------------------------------------------------------
        LBF.AddEdge(u, v, l, r);

    nghĩa là cạnh u -> v phải thỏa:
        l <= flow <= r

    Ví dụ:
        LBF.AddEdge(1, 3, 2, 7);
    tức là flow trên cạnh 1 -> 3 phải nằm trong [2, 7].

    -------------------------------------------------------------------------------
    3. CHỈ KIỂM TRA "CÓ TỒN TẠI LUỒNG HỢP LỆ HAY KHÔNG"
    -------------------------------------------------------------------------------
        long long remain = LBF.solve(1, n);

    Ở đây solve(L, R) sẽ duyệt các đỉnh từ L đến R để nối với SS, TT.
    Thường nếu các đỉnh được đánh số từ 1..n thì gọi:
        LBF.solve(1, n)

    Ý nghĩa kết quả:
    - remain == 0  : tồn tại feasible flow
    - remain > 0   : không tồn tại feasible flow

    Mẫu:
        LowerBoundFlow LBF(n + 5);
        for (...) {
            LBF.AddEdge(u, v, l, r);
        }
        if (LBF.solve(1, n) == 0) {
            // có nghiệm
        } else {
            // vô nghiệm
        }

    -------------------------------------------------------------------------------
    4. NẾU BÀI TOÁN LÀ s-t FLOW CÓ CẬN DƯỚI
    -------------------------------------------------------------------------------
    Trước khi gọi solve(), phải thêm cạnh:
        t -> s với [0, INF]

    Ví dụ:
        const long long INF = (long long)4e18;
        LBF.AddEdge(t, s, 0, INF);

    rồi mới:
        LBF.solve(1, n);

    Đây là bước chuẩn để biến bài toán s-t flow có cận dưới
    thành bài toán circulation có cận dưới.

    -------------------------------------------------------------------------------
    5. LƯU Ý QUAN TRỌNG
    -------------------------------------------------------------------------------
    - Hàm solve(L, R) trong template này KHÔNG nhận source/sink thật của bài.
    - Nó chỉ nhận đoạn chỉ số đỉnh [L..R] cần đem đi cân bằng demand.
    - Nếu bài đánh số đỉnh từ 1..n, gần như luôn gọi:
            solve(1, n)

    -------------------------------------------------------------------------------
    6. TEMPLATE NÀY DÙNG TỐT NHẤT CHO VIỆC GÌ?
    -------------------------------------------------------------------------------
    - Kiểm tra feasible lower bound flow
    - Làm bước dựng ban đầu cho các bài:
        + s-t flow có cận dưới
        + min flow / max flow có cận dưới

    Lưu ý:
    - Template hiện tại chủ yếu giải phần "feasibility"
    - Nếu bài yêu cầu minflow / maxflow có cận dưới,
      sau bước feasible còn phải xử lý thêm trên residual graph.

    -------------------------------------------------------------------------------
    7. TÓM TẮT CÁCH NHỚ
    -------------------------------------------------------------------------------
    Bài có cạnh [l, r]:
        -> gọi AddEdge(u, v, l, r)

    Chỉ hỏi có nghiệm không:
        -> solve(1, n), kiểm tra có bằng 0 không

    Bài là s-t flow có cận dưới:
        -> add thêm cạnh (t -> s, [0, INF])
        -> solve(1, n)

    =============================================================================== 
*/
