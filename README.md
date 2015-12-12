
# 京極入致 (manakonso)

Windowsで動作する[凪霧](http://maykloaayukawa.wix.com/nagili "人工言語 凪霧")のIMEを開発中です。

[CorvusSKK](https://github.com/nathancorvussolis/corvusskk) を基にして開発しています。


## ライセンス

Microsoft Public License (MS-PL)

詳細は LICENSE.TXT を見てください。


## インストール

### 対応OS

* Windows Vista (32-bit / 64-bit) SP2
* Windows 7     (32-bit / 64-bit)
* Windows 8     (32-bit / 64-bit)
* Windows 8.1   (32-bit / 64-bit)
* Windows 10    (32-bit / 64-bit)


### DirectX 9 以上をサポートしたGPU (推奨)

表示オプションでDirect2Dを使用するには、DirectX 9 以上をサポートしたGPUが必要です。

サポートしていない場合は自動的にGDIで描画されます。(多分)


### Vista 用更新プログラムのインストール

Windows Vista SP2 では、以下のプラットフォーム更新プログラムのいずれかを予めインストールしてください。

Windows 7 以降ではインストールは不要です。

* KB971644 (Windows Update から)
* KB971512

    (x64) https://www.microsoft.com/ja-JP/download/details.aspx?id=4390

    (x86) https://www.microsoft.com/ja-JP/download/details.aspx?id=3274


### manakonso のインストール

manakonso-X.Y.Z.zip を展開し、32bit版 Windows では manakonso-X.Y.Z-x86.msi、64bit版 Windows では manakonso-X.Y.Z-x64.msi を実行してください。 (X, Y, Z はバージョン番号)

32bit版 Windows では %SystemRoot%\System32\IME\IMMANAKONSO、64bit版 Windows では %SystemRoot%\System32\IME\IMMANAKONSO および %SystemRoot%\SysWOW64\IME\IMMANAKONSO にインストールされます。

アップデートの後はOSを再起動しておくと安全です。


### manakonso のアンインストール

コントロールパネルの「プログラムと機能」からアンインストールしてください。

または、インストールに使用した msi ファイルを再度実行し「Remove」を選択してください。

アンインストールの後はOSを再起動しておくと安全です。


### 初期設定

任意のユーザーアカウントで初めて使用するときは、設定ダイアログでOKボタンを押して初期設定を保存してください。

設定ダイアログで保存した後は、IME OFF → ON で新しい設定が反映されます。


### Windows ストアアプリ、Microsoft Edge

Windows ストアアプリ、Microsoft Edge 等で使用するには、設定ファイル (config.xml) にアクセス許可を付加させる必要があります。

Windows 8 以降で動作する設定ダイアログでOKボタンを押して保存を行うとアクセス許可の付加が自動的におこなわれます。

また、Windows 8 以前から Windows 8 以降へ Windows をアップグレードしたときは、manakonsoを一旦アンインストールした上でインストールし直してください。

インストール時に、Windows ストアアプリ、Microsoft Edge 等で使用可能となるようにIMEの登録がおこなわれます。


### Adobe Reader X, XI, Acrobat Reader DC

Adobe Reader X 以降で使用するには、設定ファイル (config.xml) にアクセス許可を付加させる必要があります。

設定ダイアログでOKボタンを押して保存を行うとアクセス許可の付加が自動的におこなわれます。



## 設定

### 設定ダイアログ

設定ダイアログは、以下のいずれかの操作で開くことができます。

* 言語バーを右クリックし、メニューから「設定」を選択する。「テキストサービスと入力言語」ダイアログの「インストールされているサービス」からmanakonsoを選択し、プロパティボタンを押す。
* 言語バーの入力モードアイコンを左クリックし、メニューから「設定」を選択する。
* 言語バーのヘルプボタンを左クリックし、メニューから「設定」を選択する。
* Windows 8 以降の通知領域の入力モードアイコンを右クリックし、メニューから「設定」を選択する。

設定ダイアログで保存した後は、IME OFF → ON で新しい設定が反映されます。

IME ON/OFF のキーのみ、IME ON → OFF のときにも反映されます。



## 開発環境

Visual Studio Express 2015 for Windows Desktop Update 1

WiX Toolset v3.10.1
