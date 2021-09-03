# Tetris_v3

- Tetris_v2 を書き直したもの
- ゲームはタイトル画面、ゲーム画面、ポーズ画面で構成

## 実行環境

- os: Ubuntu 20.04.1 LTS (64-bit)
- コンパイラ: g++\-9 (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0

## 外部ライブラリ

- sdl
    - libsdl2-dev:amd64 2.0.10+dfsg1-3 amd64 Simple DirectMedia Layer development files
    - libsdl2-image-dev:amd64 2.0.5+dfsg1-2 amd64 Image loading library for Simple DirectMedia Layer 2, development files
    - libsdl2-ttf-dev:amd64 2.0.15+dfsg1-1 amd64 TrueType Font library for Simple DirectMedia Layer 2, development files

## 操作方法

|key|内容|
|:--:|:--|
|w|上|
|a|右|
|s|下|
|d|左|
|j|決定、左周り|
|k|右回り|
|h|ホールド|
|space|ポーズ|
|escape|ゲーム終了|

## 実行結果

![demo](https://github.com/leon0128/Tetris_v3/blob/media/game.gif)