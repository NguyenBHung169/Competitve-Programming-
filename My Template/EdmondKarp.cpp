template<class T>
struct MCMF {
    struct edge {
        int u, v, id;
        T cap, cost;
        edge(int _u, int _v, T _cap, T _cost, int _id)
            : u(_u), v(_v), id(_id), cap(_cap), cost(_cost) {}
    };

    const T inf = numeric_limits<T>::max() / 4;

    int n, s, t, mxid;
    T flow, cost;
    bool neg;

    vector<vector<int>> g;
    vector<edge> e;
    vector<T> d, potential, flow_through;
    vector<int> par;

    MCMF() {}

    MCMF(int _n) {
        n = _n + 10;
        g.assign(n, vector<int>());
        neg = false;
        mxid = 0;
    }

    void add_edge(int u, int v, T cap, T cost, int id = -1, bool directed = true) {
        if (cost < 0) neg = true;

        g[u].push_back((int)e.size());
        e.emplace_back(u, v, cap, cost, id);

        g[v].push_back((int)e.size());
        e.emplace_back(v, u, 0, -cost, -1);

        mxid = max(mxid, id);

        if (!directed) {
            add_edge(v, u, cap, cost, -1, true);
        }
    }

    // Shortest path trên reduced cost
    bool dijkstra() {
        par.assign(n, -1);
        d.assign(n, inf);

        priority_queue<pair<T,int>, vector<pair<T,int>>, greater<pair<T,int>>> pq;
        d[s] = 0;
        pq.push({0, s});

        while (!pq.empty()) {
            auto [dist_u, u] = pq.top(); pq.pop();
            if (dist_u != d[u]) continue;

            for (int id : g[u]) {
                if (e[id].cap <= 0) continue;
                int v = e[id].v;
                T w = e[id].cost + potential[u] - potential[v];
                if (d[u] + w < d[v]) {
                    d[v] = d[u] + w;
                    par[v] = id;
                    pq.push({d[v], v});
                }
            }
        }

        for (int i = 0; i < n; i++) {
            if (d[i] < inf) d[i] += potential[i] - potential[s];
        }
        for (int i = 0; i < n; i++) {
            if (d[i] < inf) potential[i] = d[i];
        }

        return d[t] != inf;
    }

    // Trace ngược và đẩy flow
    T send_flow(int v, T cur) {
        if (par[v] == -1) return cur;

        int id = par[v];
        int u = e[id].u;

        T pushed = send_flow(u, min(cur, e[id].cap));
        if (pushed == 0) return 0;

        cost += pushed * e[id].cost;
        e[id].cap -= pushed;
        e[id ^ 1].cap += pushed;

        return pushed;
    }

    pair<T, T> solve(int _s, int _t, T goal = numeric_limits<T>::max() / 4) {
        s = _s;
        t = _t;
        flow = 0;
        cost = 0;
        potential.assign(n, 0);

        // Khởi tạo potential nếu có cạnh âm
        if (neg) {
            d.assign(n, inf);
            d[s] = 0;
            bool relax_any = true;

            for (int iter = 0; iter < n && relax_any; iter++) {
                relax_any = false;
                for (int u = 0; u < n; u++) {
                    if (d[u] == inf) continue;
                    for (int id : g[u]) {
                        if (e[id].cap > 0 && d[u] + e[id].cost < d[e[id].v]) {
                            d[e[id].v] = d[u] + e[id].cost;
                            relax_any = true;
                        }
                    }
                }
            }

            for (int i = 0; i < n; i++) {
                if (d[i] < inf) potential[i] = d[i];
            }
        }

        while (flow < goal && dijkstra()) {
            T pushed = send_flow(t, goal - flow);
            if (pushed == 0) break;
            flow += pushed;
        }

        flow_through.assign(mxid + 10, 0);
        for (int u = 0; u < n; u++) {
            for (int id : g[u]) {
                if (e[id].id >= 0) {
                    flow_through[e[id].id] = e[id ^ 1].cap;
                }
            }
        }

        return {flow, cost};
    }
};
