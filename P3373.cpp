#include <bits/stdc++.h>
#define int long long
using namespace std;

const int MAXN = 100005;
int P;

struct Node {
    int l, r;
    int sum;
    int add_tag, mul_tag;
} tr[MAXN * 4];

int a[MAXN];

void pushup(int u) {
    tr[u].sum = (tr[u << 1].sum + tr[u << 1 | 1].sum) % P;
}

void eval(int u, int add, int mul) {
    tr[u].sum = (tr[u].sum * mul % P + (tr[u].r - tr[u].l + 1) * add % P) % P;
    tr[u].mul_tag = (tr[u].mul_tag * mul) % P;
    tr[u].add_tag = (tr[u].add_tag * mul % P + add) % P;
}

void pushdown(int u) {
    eval(u << 1, tr[u].add_tag, tr[u].mul_tag);
    eval(u << 1 | 1, tr[u].add_tag, tr[u].mul_tag);
    tr[u].add_tag = 0;
    tr[u].mul_tag = 1;
}

void build(int u, int l, int r) {
    tr[u] = {l, r, 0, 0, 1};
    if (l == r) {
        tr[u].sum = a[l] % P;
        return;
    }
    int mid = l + r >> 1;
    build(u << 1, l, mid);
    build(u << 1 | 1, mid + 1, r);
    pushup(u);
}

void update_add(int u, int l, int r, int d) {
    if (tr[u].l >= l && tr[u].r <= r) {
        eval(u, d, 1);
        return;
    }
    pushdown(u);
    int mid = tr[u].l + tr[u].r >> 1;
    if (l <= mid) update_add(u << 1, l, r, d);
    if (r > mid) update_add(u << 1 | 1, l, r, d);
    pushup(u);
}

void update_mul(int u, int l, int r, int d) {
    if (tr[u].l >= l && tr[u].r <= r) {
        eval(u, 0, d);
        return;
    }
    pushdown(u);
    int mid = tr[u].l + tr[u].r >> 1;
    if (l <= mid) update_mul(u << 1, l, r, d);
    if (r > mid) update_mul(u << 1 | 1, l, r, d);
    pushup(u);
}

int query(int u, int l, int r) {
    if (tr[u].l >= l && tr[u].r <= r) {
        return tr[u].sum;
    }
    pushdown(u);
    int mid = tr[u].l + tr[u].r >> 1;
    int res = 0;
    if (l <= mid) res = (res + query(u << 1, l, r)) % P;
    if (r > mid) res = (res + query(u << 1 | 1, l, r)) % P;
    return res;
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m >> P)) return 0;
    for (int i = 1; i <= n; i++) cin >> a[i];
    build(1, 1, n);
    while (m--) {
        int op, l, r, k; cin >> op;
        if (op == 1) { cin >> l >> r >> k; update_mul(1, l, r, k); }
        else if (op == 2) { cin >> l >> r >> k; update_add(1, l, r, k); }
        else { cin >> l >> r; cout << query(1, l, r) << "\n"; }
    }

    return 0;
}
