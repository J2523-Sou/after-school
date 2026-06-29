高専ロボコン2026 松江Bのリポジトリ

このブランチは、PIC マイコンで UART を受信して PWM 出力へ反映する MPLAB X プロジェクトです。

## ファイル構成

- `pic_receiver.X/main.c` : UART 受信処理と PWM 反映処理
- `pic_receiver.X/mcc_generated_files/` : MCC が生成した周辺機能コード
- `pic_receiver.X/nbproject/` : MPLAB X プロジェクト設定
- `pic_receiver.X/pic_receiver.mc3` : MCC 設定

## 受信フレーム

9 バイトのフレームを受信します。

1. ヘッダ: `0xAA`
2. ペイロード: 8 バイト

ペイロード 8 バイトは、各 PWM 出力の duty にスケーリングして反映します。
