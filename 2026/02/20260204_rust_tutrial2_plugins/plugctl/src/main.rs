mod cli;
mod config;

use anyhow::Result;
use clap::Parser;

fn main() {
    // anyhow を使って main を簡潔に（エラーは人間向けに表示）
    if let Err(e) = real_main() {
        eprintln!("Error: {:#}", e);
        std::process::exit(1);
    }
}

fn real_main() -> Result<()> {
    let args = cli::Args::parse();

    // カレントディレクトリの Cargo.toml を読む
    let cfg = config::load_from_cwd_cargo_toml()?;

    match args.command {
        cli::Command::List => {
            for p in &cfg.plugins {
                println!("{}", p.name);
            }
        }
        cli::Command::Run { name } => {
            let target_name = match name {
                Some(n) => n,
                None => cfg
                    .default
                    .clone()
                    .ok_or_else(|| anyhow::anyhow!("run で name 省略時は default が必要です"))?,
            };

            let p = cfg
                .find_plugin(&target_name)
                .ok_or_else(|| anyhow::anyhow!("plugin '{}' が見つかりません", target_name))?;

            for _ in 0..p.repeat {
                println!("{}", p.message);
            }
        }
        cli::Command::Show { name } => {
            let p = cfg
                .find_plugin(&name)
                .ok_or_else(|| anyhow::anyhow!("plugin '{}' が見つかりません", name))?;

            // 表示形式は自由なので、分かりやすい形で出す
            println!("name   : {}", p.name);
            println!("message: {}", p.message);
            println!("repeat : {}", p.repeat);
        }
        cli::Command::Validate => {
            // load 時点で validate 済み（=ここに来たら OK）
            println!("OK");
        }
    }

    Ok(())
}
