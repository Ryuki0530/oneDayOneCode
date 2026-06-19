% sdv_latency_sim.m
% SDV Latency Budget Simulator
%
% 目的:
% 分散ECU構成と統合ECU構成の処理遅延をMATLAB上で簡易シミュレーションし、
% deadline miss率や遅延分布を比較する。

clear;
clc;
close all;

project_dir = "C:\Users\ryuki\Desktop\oneDayOneCode\2026\06\2026-06-18_Matlab_SDV-Latency-Budget-Simulator";
results_dir = fullfile(project_dir, "results");

if ~isfolder(project_dir)
    mkdir(project_dir);
end

if ~isfolder(results_dir)
    mkdir(results_dir);
end

% MATLABの作業フォルダをプロジェクトフォルダへ移動
cd(project_dir);


% 試行回数
N = 1000;

% システム全体の許容遅延 [ms]
deadline_ms = 25;

% 乱数シード固定
% 毎回同じ結果にしたい場合は固定する。
% 完全にランダムにしたい場合は rng("shuffle") に変更する。
rng(42);

%% ============================================================
%  3. Distributed ECU 構成の遅延生成
% ============================================================
%
% Sensor ECU
%   ↓ 通信1
% Perception ECU
%   ↓ 通信2
% Planning ECU
%   ↓ 通信3
% Control ECU
%   ↓
% Actuator
%
% 分散ECU構成では、各処理は比較的軽いが、
% ECU間通信が複数回入る。

dist_sensor      = make_delay(1.0, 0.2, N);
dist_comm_1      = make_delay(2.0, 0.8, N);
dist_perception  = make_delay(8.0, 1.5, N);
dist_comm_2      = make_delay(2.0, 0.8, N);
dist_planning    = make_delay(4.0, 1.0, N);
dist_comm_3      = make_delay(2.0, 0.8, N);
dist_control     = make_delay(2.0, 0.5, N);
dist_actuator    = make_delay(1.0, 0.2, N);

distributed_total = ...
    dist_sensor + ...
    dist_comm_1 + ...
    dist_perception + ...
    dist_comm_2 + ...
    dist_planning + ...
    dist_comm_3 + ...
    dist_control + ...
    dist_actuator;

%% ============================================================
%  4. Integrated ECU 構成の遅延生成
% ============================================================
%
% Integrated ECU
%   - Perception
%   - Planning
%   - Control
%
% 統合ECU構成では、ECU間通信は減る。
% 一方で、同一ECU内に処理が集中するため、
% スケジューリング遅延や負荷変動が問題になる。

int_sensor       = make_delay(1.0, 0.2, N);
int_internal     = make_delay(0.5, 0.2, N);
int_perception   = make_delay(9.0, 2.0, N);
int_planning     = make_delay(5.0, 1.5, N);
int_control      = make_delay(3.0, 1.0, N);
int_scheduling   = make_delay(3.0, 2.0, N);
int_actuator     = make_delay(1.0, 0.2, N);

integrated_total = ...
    int_sensor + ...
    int_internal + ...
    int_perception + ...
    int_planning + ...
    int_control + ...
    int_scheduling + ...
    int_actuator;

%% ============================================================
%  5. 統計量の計算
% ============================================================

distributed_mean = mean(distributed_total);
distributed_max  = max(distributed_total);
distributed_std  = std(distributed_total);
distributed_miss_rate = mean(distributed_total > deadline_ms) * 100;

integrated_mean = mean(integrated_total);
integrated_max  = max(integrated_total);
integrated_std  = std(integrated_total);
integrated_miss_rate = mean(integrated_total > deadline_ms) * 100;

Architecture = [
    "Distributed ECU"
    "Integrated ECU"
];

Mean_ms = [
    distributed_mean
    integrated_mean
];

Max_ms = [
    distributed_max
    integrated_max
];

Std_ms = [
    distributed_std
    integrated_std
];

MissRate_percent = [
    distributed_miss_rate
    integrated_miss_rate
];

result_table = table( ...
    Architecture, ...
    Mean_ms, ...
    Max_ms, ...
    Std_ms, ...
    MissRate_percent ...
);

%% ============================================================
%  6. 結果表示
% ============================================================

disp("=== SDV Latency Simulation Result ===");
disp(result_table);

%% ============================================================
%  7. CSV保存
% ============================================================

result_csv_path = fullfile(results_dir, "sdv_latency_result.csv");
writetable(result_table, result_csv_path);

% 各試行の生データも保存する。
% 後からExcelやMATLABで再分析できるようにするため。
Sample = (1:N)';

raw_latency_table = table( ...
    Sample, ...
    distributed_total, ...
    integrated_total, ...
    distributed_total > deadline_ms, ...
    integrated_total > deadline_ms, ...
    'VariableNames', { ...
        'Sample', ...
        'DistributedLatency_ms', ...
        'IntegratedLatency_ms', ...
        'DistributedDeadlineMiss', ...
        'IntegratedDeadlineMiss' ...
    } ...
);

raw_csv_path = fullfile(results_dir, "raw_latency_samples.csv");
writetable(raw_latency_table, raw_csv_path);

%% ============================================================
%  8. ヒストグラム表示・保存
% ============================================================

fig = figure("Name", "SDV Latency Budget Simulator");

histogram(distributed_total, ...
    "BinWidth", 0.5, ...
    "FaceAlpha", 0.6);

hold on;

histogram(integrated_total, ...
    "BinWidth", 0.5, ...
    "FaceAlpha", 0.6);

xline(deadline_ms, "--", ...
    "Deadline = 25 ms", ...
    "LineWidth", 2);

grid on;

title("SDV Latency Distribution");
xlabel("End-to-End Latency [ms]");
ylabel("Count");

legend( ...
    "Distributed ECU", ...
    "Integrated ECU", ...
    "Deadline", ...
    "Location", "best" ...
);

histogram_path = fullfile(results_dir, "latency_histogram.png");

% 新しめのMATLABでは exportgraphics を使う。
% 環境によって失敗した場合は saveas にフォールバックする。
try
    exportgraphics(fig, histogram_path, "Resolution", 300);
catch
    saveas(fig, histogram_path);
end

%% ============================================================
%  9. 保存先表示
% ============================================================

disp(" ");
disp("=== Saved Files ===");
disp("Result CSV:");
disp(result_csv_path);
disp("Raw Latency CSV:");
disp(raw_csv_path);
disp("Histogram PNG:");
disp(histogram_path);



%% ============================================================
%  ローカル関数
% ============================================================

function delay = make_delay(mean_ms, std_ms, N)
% make_delay
%
% 平均 mean_ms、標準偏差 std_ms の正規分布乱数から、
% N個の遅延データを生成する。
%
% 遅延時間が負になるのは物理的に不自然なので、
% max(delay, 0) によって0未満を0に丸める。

    delay = mean_ms + std_ms .* randn(N, 1);
    delay = max(delay, 0);

end