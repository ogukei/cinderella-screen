# Cinderella Screen
デレステをフルスクリーンにします

## 使い方
実行環境はWindows 10を想定しています

1. [Releases](https://github.com/ogukei/cinderella-screen/releases) から `imascs.exe.zip` をダウンロード
1. `imascs.exe` を実行
1. Fullscreen (F11) でフルスクリーンに変更
3. Desktop Wallpaper (F10) で壁紙モードに変更
4. Default (F9) で通常モードに変更

F9もしくはAlt+Tabでフルスクリーンから抜け出せます

## 開発
1. Download [VSCode](https://code.visualstudio.com/)
1. Install [VSCode C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
1. Install Microsoft Visual C++ (MSVC) compiler toolset via Build Tools for Visual Studio 2019
    1. see https://code.visualstudio.com/docs/cpp/config-msvc
1. Install Windows 10 SDK 10.0.19041.0 via Build Tools for Visual Studio 2019
1. Open Developer PowerShell for VS 2019
1. Run `cd <this-repository-folder>`
1. Run `cmake .. -G "Visual Studio 16 2019" -A x64`
1. Run `cmake --build . --config Release`
1. Run `Release/imascs.exe`
