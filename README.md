# Cinderella Screen
デレステをフルスクリーンにします

## 免責事項
Windows Defenderにウイルス・マルウェアとして誤検知されることがあります。

誤検知を回避するには [保護履歴](https://support.microsoft.com/ja-jp/topic/%E4%BF%9D%E8%AD%B7%E5%B1%A5%E6%AD%B4-f1e5fd95-09b4-46d1-b8c7-1059a1e09708) > 許可 を選択します。

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
* 軽量・シンプル
    * 実行ファイルは150KB程度です
* フルスクリーン化
    * 4Kモニターに対応しています
* デスクトップ壁紙モード
    * デレステのウィンドウをデスクトップ背景に配置します
* キーショートカット
    * F11を押してフルスクリーンに即座に切り替えることができます
* 自動カーソル非表示
    * フルスクリーン時にカーソルを自動的に非表示にします
* スクリーンショット
    * PNG形式の画像を保存できます
* ワイドモニター対応
    * 16:9モニター以外でも動作します
* 縦画面対応
    * 縦画面ゲームモードでもフルスクリーンに設定できます

## 開発
1. Download [VSCode](https://code.visualstudio.com/)
1. Install [VSCode C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
1. Install Microsoft Visual C++ (MSVC) compiler toolset via Build Tools for Visual Studio 2022
    * See https://code.visualstudio.com/docs/cpp/config-msvc
    * From the Visual Studio [Downloads](https://visualstudio.microsoft.com/ja/downloads/#build-tools-for-visual-studio-2022) page, scroll down until you see Tools for Visual Studio under the All Downloads section and select the download for Build Tools for Visual Studio 2022.
1. Install Windows 10 SDK 10.0.19041.0 via Build Tools for Visual Studio 2022
1. Install CMake tools for Windows
    * https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170
1. Open Developer PowerShell for VS 2022
1. Run the following commands
```
cd <this-repository-folder>
mkdir build
cd build
cmake .. -G "Visual Studio 16 2022" -A x64
cmake --build .
Debug/imascs.exe
```
