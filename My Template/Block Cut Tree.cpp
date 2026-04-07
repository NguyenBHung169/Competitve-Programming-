#include<bits/stdc++.h>
using namespace std;

struct graph
{
	int n;
	vector<vector<int>> adj;

	graph(int n = 0) : n(n), adj(n) {}

	void add_edge(int u, int v)
	{
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	int add_node()
	{
		adj.push_back({});
		return n++;
	}

	vector<int>& operator[](int u) { return adj[u]; }
};

/*
	Template block-cut tree hoàn chỉnh.

	Những gì struct này trả về:
	- comps:
		danh sách các biconnected component (theo đỉnh)
		comps[i] là list các đỉnh gốc thuộc block i

	- art[u]:
		= 1 nếu u là articulation point
		= 0 nếu không

	- tree:
		block-cut tree sau khi build xong

	- id[u]:
		node trên block-cut tree tương ứng với đỉnh gốc u
		+ nếu u là articulation point -> id[u] là node "art" riêng của u
		+ nếu u không phải articulation point -> id[u] là node block chứa u

	- art_node[u]:
		nếu u là articulation point thì đây là node của u trong BCT
		ngược lại = -1

	- block_node[i]:
		node trên BCT tương ứng với comps[i]

	Ý tưởng block-cut tree:
	- mỗi articulation point là một node riêng
	- mỗi biconnected component là một node riêng
	- nối block node với art node nếu articulation đó nằm trong block đó
*/
struct block_cut_tree
{
	vector<vector<int>> comps;
	vector<int> num, low, art, id, art_node, block_node;
	graph tree;
};

/*
	Hàm này giữ đúng "tinh thần" template cũ:
	- nhận vào graph
	- chạy Tarjan tìm biconnected components
	- build luôn block-cut tree
	- trả về đầy đủ thông tin để dùng tiếp
*/
block_cut_tree biconnected_components(graph &adj)
{
	int n = adj.n;

	block_cut_tree res;
	res.num.assign(n, 0);
	res.low.assign(n, 0);
	res.art.assign(n, 0);
	res.id.assign(n, -1);
	res.art_node.assign(n, -1);

	vector<pair<int,int>> stk;   // stack cạnh để tách BCC chuẩn Tarjan
	int timer = 0;

	/*
		add_component_until(stop_u, stop_v):
		pop cạnh khỏi stack cho tới khi pop được cạnh (stop_u, stop_v),
		toàn bộ các đỉnh xuất hiện trong số cạnh vừa pop tạo thành 1 BCC.
	*/
	auto add_component_until = [&](int stop_u, int stop_v)
	{
		vector<int> comp;
		while (true)
		{
			auto e = stk.back();
			stk.pop_back();

			comp.push_back(e.first);
			comp.push_back(e.second);

			if (e.first == stop_u && e.second == stop_v) break;
		}

		// unique các đỉnh trong component
		sort(comp.begin(), comp.end());
		comp.erase(unique(comp.begin(), comp.end()), comp.end());

		res.comps.push_back(comp);
	};

	function<void(int,int)> dfs = [&](int u, int p)
	{
		res.num[u] = res.low[u] = ++timer;
		int child = 0;

		for (int v : adj[u])
		{
			if (v == p) continue;

			if (!res.num[v])
			{
				stk.push_back({u, v});
				dfs(v, u);
				res.low[u] = min(res.low[u], res.low[v]);
				++child;

				/*
					Điều kiện low[v] >= num[u]:
					subtree của v không quay lên trên u được,
					nên các cạnh từ (u,v) trở xuống tạo thành 1 BCC.
				*/
				if (res.low[v] >= res.num[u])
				{
					// articulation point:
					// - u không phải root: chỉ cần có 1 con như vậy
					// - u là root: phải có từ 2 con DFS trở lên
					if (p != -1 || child >= 2) res.art[u] = 1;

					add_component_until(u, v);
				}
			}
			else if (res.num[v] < res.num[u]) // back-edge đi lên tổ tiên
			{
				stk.push_back({u, v});
				res.low[u] = min(res.low[u], res.num[v]);
			}
		}

		/*
			Nếu u là đỉnh cô lập (isolated vertex), nó vẫn tạo thành 1 block riêng.
			Vì không có cạnh nào nên Tarjan bằng stack cạnh sẽ không tự sinh ra comp,
			ta phải thêm tay.
		*/
		if (p == -1 && child == 0)
		{
			res.comps.push_back({u});
		}
	};

	for (int u = 0; u < n; ++u)
	{
		if (!res.num[u])
		{
			dfs(u, -1);

			// Bình thường sau mỗi connected component thì stack cạnh phải rỗng.
			// Đoạn này để an toàn nếu còn sót cạnh do biến thể cài đặt.
			if (!stk.empty())
			{
				vector<int> comp;
				while (!stk.empty())
				{
					comp.push_back(stk.back().first);
					comp.push_back(stk.back().second);
					stk.pop_back();
				}
				sort(comp.begin(), comp.end());
				comp.erase(unique(comp.begin(), comp.end()), comp.end());
				if (!comp.empty()) res.comps.push_back(comp);
			}
		}
	}

	/*
		========================
		BUILD BLOCK-CUT TREE
		========================

		Quy ước:
		- mỗi articulation point có một node riêng trong tree
		- mỗi BCC có một node riêng trong tree

		id[u]:
		- nếu u là articulation point -> id[u] = node art của u
		- nếu u không phải articulation point -> id[u] = node block chứa u
	*/

	graph tree(0);

	// B1. Tạo node riêng cho mọi articulation point
	for (int u = 0; u < n; ++u)
	{
		if (res.art[u])
		{
			res.art_node[u] = tree.add_node();
			res.id[u] = res.art_node[u];
		}
	}

	// B2. Với mỗi BCC, tạo 1 block node
	int bcc_cnt = (int)res.comps.size();
	res.block_node.assign(bcc_cnt, -1);

	for (int i = 0; i < bcc_cnt; ++i)
	{
		int node = tree.add_node();
		res.block_node[i] = node;

		for (int u : res.comps[i])
		{
			if (res.art[u])
			{
				// articulation point: nối block node với art node
				tree.add_edge(node, res.art_node[u]);
			}
			else
			{
				// non-articulation: đỉnh này chỉ thuộc đúng 1 block
				res.id[u] = node;
			}
		}
	}

	res.tree = tree;
	return res;
}

/*
	========================
	CÁCH SỬ DỤNG TEMPLATE
	========================

	1) Khởi tạo đồ thị:
		graph g(n);
		g.add_edge(u, v);

	2) Build:
		auto bc = biconnected_components(g);

	3) Kết quả nhận được:

	- bc.comps
		Danh sách các block.
		Mỗi phần tử là danh sách đỉnh gốc thuộc block đó.

	- bc.art[u]
		Cho biết u có phải articulation point hay không.

	- bc.tree
		Block-cut tree.
		Node trong tree gồm 2 loại:
		+ node articulation
		+ node block

	- bc.id[u]
		Node của đỉnh gốc u trên block-cut tree:
		+ nếu u là articulation -> id[u] là node art riêng của u
		+ nếu u không phải articulation -> id[u] là block node chứa u

	- bc.art_node[u]
		Nếu u là articulation thì đây là node của u trong BCT, ngược lại = -1

	- bc.block_node[i]
		Node của block i (tức là comps[i]) trên BCT

	4) Dùng như thế nào trong bài toán?

	- Nếu cần đưa 2 đỉnh gốc u, v lên cây:
		int U = bc.id[u];
		int V = bc.id[v];

	- Sau đó có thể làm:
		+ DFS trên bc.tree
		+ LCA trên bc.tree
		+ distance on tree
		+ đếm số articulation point / số block trên đường đi
		+ DP trên block-cut tree

	5) Ghi nhớ bản chất:
	- block-cut tree xử lý bài toán liên quan tới "xóa đỉnh / articulation point"
	- nếu bài liên quan tới "xóa cạnh / bridge" thì thường dùng bridge tree, không phải BCT
*/

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	/*
		Ví dụ dùng:

		Input:
		n m
		m cạnh 0-indexed
	*/

	int n, m;
	cin >> n >> m;

	graph g(n);
	for (int i = 0; i < m; ++i)
	{
		int u, v;
		cin >> u >> v;
		g.add_edge(u, v);
	}

	auto bc = biconnected_components(g);

	// In các articulation point
	cout << "Articulation points:\n";
	for (int u = 0; u < n; ++u)
	{
		if (bc.art[u]) cout << u << ' ';
	}
	cout << "\n";

	// In các biconnected components
	cout << "Biconnected components:\n";
	for (int i = 0; i < (int)bc.comps.size(); ++i)
	{
		cout << "comp " << i << ": ";
		for (int u : bc.comps[i]) cout << u << ' ';
		cout << "\n";
	}

	// In mapping đỉnh gốc -> node trên block-cut tree
	cout << "id[u] on block-cut tree:\n";
	for (int u = 0; u < n; ++u)
	{
		cout << "vertex " << u << " -> tree node " << bc.id[u] << "\n";
	}

	// In cây block-cut tree
	cout << "Block-cut tree:\n";
	for (int u = 0; u < bc.tree.n; ++u)
	{
		cout << u << ": ";
		for (int v : bc.tree[u]) cout << v << ' ';
		cout << "\n";
	}

	return 0;
}
