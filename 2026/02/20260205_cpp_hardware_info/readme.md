# 今日のC++課題：Linuxの「見えてるハード」を覗くCLI（syscall縛り）

## ねらい
LinuxがOSとして認識している「ハードウェア情報」を、**できるだけシステムコール（syscall）ベース**で集めて表示するCLIツールをC++で作る。

`/proc` と `/sys` は「カーネルが見せてる仮想ファイル」なので、実質“カーネルとのI/O”です。  
今回は **libudev などの外部ライブラリ禁止**で、`openat/read/getdents64` などを使って掘ります。

---

## 成果物
プロジェクト名：`hwpeek`

実行例：
```bash
./hwpeek --summary
./hwpeek --cpu
./hwpeek --block
./hwpeek --net
./hwpeek --pci
./hwpeek --all