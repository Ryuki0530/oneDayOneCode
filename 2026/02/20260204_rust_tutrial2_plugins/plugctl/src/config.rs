use anyhow::{bail, Context, Result};
use serde::Deserialize;
use std::{collections::HashSet, fs, path::Path};

/// Cargo.toml 全体を全部構造体で表現するのは大変なので、
/// 最低限 package -> metadata -> plugctl だけを拾う構造にする。
#[derive(Debug, Deserialize)]
struct CargoTomlRoot {
    package: Option<CargoPackage>,
}

#[derive(Debug, Deserialize)]
struct CargoPackage {
    metadata: Option<CargoMetadata>,
}

#[derive(Debug, Deserialize)]
struct CargoMetadata {
    plugctl: Option<PlugctlConfig>,
}

/// アプリが欲しい設定の形（package.metadata.plugctl）
#[derive(Debug, Clone, Deserialize)]
pub struct PlugctlConfig {
    pub default: Option<String>,
    #[serde(default)]
    pub plugins: Vec<Plugin>,
}

/// 個々の plugin 定義
#[derive(Debug, Clone, Deserialize)]
pub struct Plugin {
    pub name: String,
    pub message: String,
    pub repeat: u32,
}

impl PlugctlConfig {
    /// 名前で plugin を引く
    pub fn find_plugin(&self, name: &str) -> Option<&Plugin> {
        self.plugins.iter().find(|p| p.name == name)
    }

    /// 要求仕様に基づくバリデーション
    pub fn validate(&self) -> Result<()> {
        // plugin が 0 件はエラー
        if self.plugins.is_empty() {
            bail!("plugin が 0 件です。[[package.metadata.plugctl.plugins]] を1つ以上定義してください。");
        }

        // name 重複禁止
        let mut seen = HashSet::new();
        for p in &self.plugins {
            if !seen.insert(p.name.as_str()) {
                bail!("plugin name '{}' が重複しています。", p.name);
            }
        }

        // repeat は 1 以上
        for p in &self.plugins {
            if p.repeat < 1 {
                bail!("plugin '{}' の repeat は 1 以上である必要があります。", p.name);
            }
        }

        // default があるなら存在確認
        if let Some(def) = &self.default {
            if self.find_plugin(def).is_none() {
                bail!("default '{}' が設定されていますが、その名前の plugin が存在しません。", def);
            }
        }

        Ok(())
    }
}

/// カレントディレクトリの Cargo.toml から設定を読む
pub fn load_from_cwd_cargo_toml() -> Result<PlugctlConfig> {
    load_from_path(Path::new("Cargo.toml"))
}

/// 任意パスから読む（拡張しやすいように関数を分けておく）
fn load_from_path(path: &Path) -> Result<PlugctlConfig> {
    let s = fs::read_to_string(path)
        .with_context(|| format!("Cargo.toml を読み込めません: {}", path.display()))?;

    let root: CargoTomlRoot =
        toml::from_str(&s).context("Cargo.toml のTOMLパースに失敗しました")?;

    let cfg = root
        .package
        .and_then(|p| p.metadata)
        .and_then(|m| m.plugctl)
        .ok_or_else(|| {
            anyhow::anyhow!(
                "Cargo.toml に [package.metadata.plugctl] が見つかりません。設定を追加してください。"
            )
        })?;

    // validate は読み込み時点で必ず実施（validate コマンドはここまで来ればOK）
    cfg.validate()?;

    Ok(cfg)
}
