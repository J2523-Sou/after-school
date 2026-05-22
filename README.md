高専ロボコン2026 松江Bのリポジトリ

基本的にブランチを生やして使いましょう。

このブランチは、PIC へ UART 送信するための `afs_uart` ライブラリを含みます。

## ファイル構成

- `function_text/afs_uart.py` : PIC 向け UART 送信ライブラリ
- `src/uart_sender.py` : UART送信プログラム
- `src/uart_for.py` : 0xAA + 8バイトを繰り返し送信するプログラム（ボーレート 9600 固定）

## 仕様

送信フレームは以下の9バイトです。

1. ヘッダ: `0xAA`
2. ペイロード: 8バイト（各値 `0..255`）

## 使い方

```bash
python3 src/uart_sender.py --port /dev/ttyAMA0 --baudrate 9600 1 2 3 4 5 6 7 8
```

`uart_for.py` を使う場合は、ボーレート 9600 固定で次のように実行します。

```bash
python3 src/uart_for.py --port /dev/ttyAMA0
```

`/dev/ttyAMA0` にアクセスできない場合は、`sudo` で実行するか、ユーザーを `dialout` グループに追加してください。

## オプション

- `--port` : シリアルポート（デフォルト: `/dev/ttyAMA0`）
- `--timeout` : タイムアウト秒（デフォルト: `1.0`）
- `--dry-run` : 実送信せずにフレーム内容を表示

### dry-run例

```bash
python3 src/uart_sender.py --dry-run 1 2 3 4 5 6 7 8
# frame(hex)= AA 01 02 03 04 05 06 07 08
```
