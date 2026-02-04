use clap::{Parser, Subcommand};

#[derive(Debug, Parser)]
#[command(name = "plugctl")]
#[command(about = "Cargo.toml (package.metadata.plugctl) 駆動のプラグイン式CLI", long_about = None)]
pub struct Args {
    #[command(subcommand)]
    pub command: Command,
}

#[derive(Debug, Subcommand)]
pub enum Command {
    /// 定義されている plugin 名を一覧表示
    List,

    /// 指定 plugin の message を repeat 回だけ表示（省略時は default）
    Run {
        /// plugin 名（省略すると default を使用）
        name: Option<String>,
    },

    /// 指定 plugin の設定を表示
    Show {
        /// plugin 名
        name: String,
    },

    /// 設定を検証し、問題なければ OK を表示
    Validate,
}
