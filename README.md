# after-school

高専ロボコン 2026 松江 B の制御用リポジトリです。

現在のブランチには、PIC18F26Q84 でロータリーエンコーダを読み取り、UART でカウント値を送信する MPLAB X プロジェクトを置いています。

## プロジェクト

- `pic_sender_rotalyencoder.X/` : ロータリーエンコーダ送信側の MPLAB X プロジェクト
- `pic_sender_rotalyencoder.X/main.c` : エンコーダ読み取り、カウント更新、UART 送信処理
- `pic_sender_rotalyencoder.X/mcc_generated_files/` : MCC 生成コード
- `pic_sender_rotalyencoder.X/nbproject/` : MPLAB X プロジェクト設定
- `pic_sender_rotalyencoder.X/pic_sender_rotalyencoder.mc3` : MCC 設定

## 動作概要

起動時にエンコーダ値 `0` を送信し、その後は A/B 相の状態変化を 1 ms 周期でポーリングします。

有効な状態遷移があった場合はカウント値を `+1` または `-1` し、更新後の値を UART1 から送信します。

## UART 送信フォーマット

UART1 は 115200 bps で使用します。

送信する文字列は次の形式です。

```text
ENC,<count>\r\n
```

例:

```text
ENC,0
ENC,1
ENC,2
ENC,1
```

## ピン割り当て

| 用途 | PIC ピン | 備考 |
| --- | --- | --- |
| エンコーダ A 相 | RA0 | デジタル入力、内部プルアップ有効 |
| エンコーダ B 相 | RA1 | デジタル入力、内部プルアップ有効 |
| UART1 TX | RC6 | `U1TX` |
| UART1 RX | RC7 | `U1RX`、現在のアプリ本体では未使用 |

## 開発環境

プロジェクト設定上の主な環境は次の通りです。

- MCU: `PIC18F26Q84`
- Toolchain: `XC8 3.10`
- Device Family Pack: `PIC18F-Q_DFP 1.28.451`
- クロック: HFINTOSC 64 MHz

## ビルド

MPLAB X IDE で `pic_sender_rotalyencoder.X` を開き、`default` 構成で Clean and Build してください。

コマンドラインでビルドする場合は、MPLAB X / XC8 のパスが通っている環境で次を実行します。

```sh
cd pic_sender_rotalyencoder.X
make CONF=default build
```

生成される HEX のパスは次の通りです。

```text
pic_sender_rotalyencoder.X/dist/default/production/pic_sender_rotalyencoder.X.production.hex
```
