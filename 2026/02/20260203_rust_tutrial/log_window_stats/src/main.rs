use std::collections::{HashMap, VecDeque};
use std::io::{self, Read};


fn main() {
    //高速入力
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).unwrap();
    let mut it = input.split_whitespace();

    // 1行目
    let n: usize = match it.next() {
        Some(v) => v.parse().unwrap(),
        None => return, // 入力が空なら終了
    };
    let k: i64 = it.next().unwrap().parse().unwrap();

    // window: 直近K秒に含まれるログを（時刻, ユーザー）で保持するキュー
    let mut window: VecDeque<(i64, String)> = VecDeque::new();

    // counts: window内での「ユーザーID -> 出現回数」
    // 出現回数が0になったらremoveすることで、counts.len()がユニーク数になる
    let mut counts: HashMap<String, usize> = HashMap::new();

    // 出力をまとめて作って最後に一括でprint（printlnをN回やるより速い）
    let mut out = String::new();
    out.reserve(n * 3);

    for _ in 0..n {
        // 各ログ: t user
        let t: i64 = it.next().unwrap().parse().unwrap();
        let user: String = it.next().unwrap().to_string();

        // 1) 今回のログをwindowに追加し、ユーザー出現回数を増やす
        window.push_back((t, user.clone()));
        *counts.entry(user).or_insert(0) += 1;

        // 2) 範囲 [t-K+1, t] より古いログをwindow先頭から捨てる
        //    startが負になる場合もあるが、その場合「front_t < start」は基本的に成立しないので問題ない
        let start = t - k + 1;

        while let Some((front_t, _)) = window.front() {
            // 先頭が範囲内なら、それ以降も時刻順なので打ち切り
            if *front_t >= start {
                break;
            }

            // 先頭（古すぎるログ）を取り出して、countsを減らす
            let (_old_t, old_user) = window.pop_front().unwrap();
            if let Some(cnt) = counts.get_mut(&old_user) {
                *cnt -= 1;
                // 出現回数が0ならHashMapから削除（ユニーク数に効く）
                if *cnt == 0 {
                    counts.remove(&old_user);
                }
            }
        }

        // 3) 現時点のユニークユーザー数（= HashMapのキー数）を出力
        out.push_str(&counts.len().to_string());
        out.push('\n');
    }

    // 最後にまとめて出力
    print!("{}", out);
}