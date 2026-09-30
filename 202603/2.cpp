#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct FlexibleTask {
    double a, b;
    double ratio;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    double initial_time = 0.0;
    vector<pair<int, double>> normal_items;
    vector<FlexibleTask> flex_items;

    for (int i = 0; i < n; ++i) {
        int o;
        double t, a, b;
        cin >> o >> t >> a >> b;
        initial_time += t;
        if (o == 1) {
            normal_items.push_back({(int)a, b});
        } else {
            flex_items.push_back({a, b, b / a});
        }
    }

    // 灵活型任务按单位收益降序排列
    sort(flex_items.begin(), flex_items.end(), [](const FlexibleTask& x, const FlexibleTask& y) {
        return x.ratio > y.ratio;
    });

    // 普通型任务的 0-1 背包 DP
    vector<double> dp(m + 1, 0.0);
    for (const auto& item : normal_items) {
        int w = item.first;
        double v = item.second;
        for (int j = m; j >= w; --j) {
            dp[j] = max(dp[j], dp[j - w] + v);
        }
    }

    // 计算 C 杯咖啡在灵活型任务上的最大收益
    auto calc_flex = [&](double C) {
        double gain = 0.0;
        for (const auto& ft : flex_items) {
            if (C <= 0) break;
            double take = min(C, ft.a);
            gain += take * ft.ratio;
            C -= take;
        }
        return gain;
    };

    double max_reduction = 0.0;
    for (int j = 0; j <= m; ++j) {
        double cur_reduction = dp[j] + calc_flex(m - j);
        max_reduction = max(max_reduction, cur_reduction);
    }

    cout << fixed << setprecision(6) << (initial_time - max_reduction) << "\n";

    return 0;
}
