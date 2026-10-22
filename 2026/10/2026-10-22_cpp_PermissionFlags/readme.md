# Permission Flags

使用言語: C++17
目安: 15〜20分

## 課題
ビット演算を使い、「読み取り・書き込み・実行」の権限を管理するクラス PermissionFlags を作成してください。

## 要件
1. 権限を次の値で定義してください。
   - READ = 1
   - WRITE = 2
   - EXECUTE = 4

2. PermissionFlags は、初期状態ではすべての権限を無効にしてください。

3. 次のメソッドを実装してください。
   - void grant(unsigned int flags)
     指定された権限を有効にします。
   - void revoke(unsigned int flags)
     指定された権限を無効にします。
   - bool has(unsigned int flags) const
     指定された権限がすべて有効なら true を返します。

4. READ | WRITE のように、複数の権限をまとめて指定できるようにしてください。
   引数には1〜7の値だけが渡されるものとします。

5. 権限の保存には1つの unsigned int を使い、判定と更新にはビット演算を使ってください。

## 動作確認
main 関数で次の操作を順番に実行し、判定結果を表示してください。
bool の表示には std::boolalpha を使用してください。

操作:
1. 初期状態で has(READ)
2. grant(READ | WRITE)
3. has(READ)
4. has(READ | EXECUTE)
5. grant(EXECUTE)
6. has(READ | WRITE | EXECUTE)
7. revoke(WRITE)
8. has(WRITE)
9. has(READ | EXECUTE)

期待出力:
false
true
false
true
false
true

## 完了条件
期待出力と一致し、同じ権限を繰り返し grant または revoke しても、正しい状態を維持できれば完成です。