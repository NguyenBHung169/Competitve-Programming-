namespace SAT2 {
	const int N = 20005;

	VI e[N];
	int n, cnt, dfn[N], low[N], st[N], bel[N], top, ind;
	bool ins[N];

	// val[i] = giá trị của biến i (0/1)
	// giả sử mỗi biến i có 2 đỉnh:
	// i<<1     : false
	// i<<1 | 1 : true
	bool val[N >> 1];

	void init(int ct) {
		n = ct;              // tổng số đỉnh của implication graph = 2 * số biến
		cnt = 0;
		top = 0;
		ind = 0;
		L(i, 0, n) {
			e[i].clear();
			dfn[i] = low[i] = bel[i] = 0;
			ins[i] = 0;
		}
	}

	void add(int u, int v) {
		e[u].pb(v);
	}

	// phủ định của literal x
	int neg(int x) {
		return x ^ 1;
	}

	// thêm mệnh đề kéo theo: u -> v
	void add_imp(int u, int v) {
		add(u, v);
	}

	// thêm mệnh đề (u OR v)
	// tương đương (!u -> v) và (!v -> u)
	void add_or(int u, int v) {
		add(neg(u), v);
		add(neg(v), u);
	}

	// ép literal u = true
	// tức là (!u -> u)
	void add_true(int u) {
		add(neg(u), u);
	}

	// ép literal u = false
	// tức là (u -> !u)
	void add_false(int u) {
		add(u, neg(u));
	}

	// u và v phải cùng giá trị
	// (u == v) <=> (u -> v) & (v -> u) & (!u -> !v) & (!v -> !u)
	void add_equal(int u, int v) {
		add(u, v);
		add(v, u);
		add(neg(u), neg(v));
		add(neg(v), neg(u));
	}

	// u và v phải khác giá trị
	// (u != v) <=> (u OR v) & (!u OR !v)
	void add_xor(int u, int v) {
		add_or(u, v);
		add_or(neg(u), neg(v));
	}

	void tarjan(int u) {
		dfn[u] = low[u] = ++ind;
		ins[u] = 1;
		st[++top] = u;

		L(i, 0, SZ(e[u])) {
			int v = e[u][i];
			if (!dfn[v]) {
				tarjan(v);
				low[u] = min(low[u], low[v]);
			}
			else if (ins[v]) {
				low[u] = min(low[u], dfn[v]); // Tarjan chuẩn: phải là dfn[v]
			}
		}

		if (dfn[u] == low[u]) {
			++cnt;
			while (1) {
				bel[st[top]] = cnt;
				ins[st[top]] = 0;
				if (st[top--] == u) break;
			}
		}
	}

	// chạy SCC
	void solve() {
		L(i, 0, n) {
			if (!dfn[i]) tarjan(i);
		}
		// Với Tarjan kiểu này:
		// nếu có cạnh SCC A -> SCC B thì bel[A] >= bel[B]
	}

	// vars = số biến
	// trả về true nếu có nghiệm, đồng thời sinh nghiệm vào val[]
	bool satisfiable(int vars) {
		solve();

		L(i, 0, vars) {
			if (bel[i << 1] == bel[i << 1 | 1]) return 0; // x và !x cùng SCC => vô nghiệm
		}

		L(i, 0, vars) {
			// SCC có topo đứng "trước" hơn sẽ quyết định giá trị
			// với quy ước bel lớn hơn đi về bel nhỏ hơn,
			// cách gán chuẩn là:
			val[i] = (bel[i << 1] < bel[i << 1 | 1]);
		}
		return 1;
	}
};

