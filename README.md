# Cinderella Screen
デレステをフルスクリーンにします

## 使い方
実行環境はWindows 10を想定しています

1. [Releases](https://github.com/ogukei/cinderella-screen/releases) から `imascs.exe.zip` をダウンロード
1. `imascs.exe` を実行
1. Fullscreen (F11) でフルスクリーンに変更
1. Desktop Wallpaper (F10) でデスクトップ壁紙モードに変更
1. Default (F9) で通常モードに変更
1. Screenshot (F12) でスクリーンショット

F9もしくはAlt+Tabでフルスクリーンから抜け出せます

スクリーンショットは `C:/Users/<User>/Pictures/Screenshots` に保存されます

## 機能
1. 軽量・シンプル
    1. 実行ファイルは150KB程度です
1. フルスクリーン化
    1. 4Kモニターに対応しています
1. デスクトップ壁紙モード
    1. デレステのウィンドウをデスクトップ背景に配置します
1. キーショートカット
    1. F11を押してフルスクリーンに即座に切り替えることができます
1. 自動カーソル非表示
    1. フルスクリーン時にカーソルを自動的に非表示にします
1. スクリーンショット
    1. PNG形式の画像を保存できます
1. ワイドモニター対応
    1. 16:9モニター以外でも動作します
1. 縦画面対応
    1. 縦画面ゲームモードでもフルスクリーンに設定できます

## 開発
1. Download [VSCode](https://code.visualstudio.com/)
1. Install [VSCode C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
1. Install Microsoft Visual C++ (MSVC) compiler toolset via Build Tools for Visual Studio 2019
    1. see https://code.visualstudio.com/docs/cpp/config-msvc
1. Install Windows 10 SDK 10.0.19041.0 via Build Tools for Visual Studio 2019
1. Open Developer PowerShell for VS 2019
1. Run the following commands
```
cd <this-repository-folder>
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build .
Debug/imascs.exe
```
