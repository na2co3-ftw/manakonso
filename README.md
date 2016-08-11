
# 京極入致 (manakonso) ver. 0.1.1

Windowsで動作する[凪霧](http://maykloaayukawa.wix.com/nagili "人工言語 凪霧")のIMEを開発中です。

[CorvusSKK](https://github.com/nathancorvussolis/corvusskk) を基にして開発しています。

京字が入力できるだけで極字変換はできませんが、既存のIMEを使わずに京字が入力できるだけでも利便性があるということなので一旦リリースします。

## インストール


### 対応OS

* Windows Vista (32-bit / 64-bit)

    + Service Pack 2
    + プラットフォーム更新プログラム ( [KB971644](https://support.microsoft.com/en-us/kb/971644) または [KB971512](https://support.microsoft.com/en-us/kb/971512) )

* Windows 7     (32-bit / 64-bit)

    + Service Pack 1

* Windows 8     (32-bit / 64-bit)

* Windows 8.1   (32-bit / 64-bit)

    + S14 Update ( [KB2919355](https://support.microsoft.com/en-us/kb/2919355) )

* Windows 10    (32-bit / 64-bit)


### インストール

manakonso-X.Y.Z.exe を実行してください。 (X, Y, Z はバージョン番号)

アップデートの後はOSを再起動しておくと安全です。

Windows Vista/7 では、インストールに極端に時間がかかるときがあります。
その場合は、時間をおいて再度インストールを試みてください。

インストール先

* 32bit版 Windows

    + %SystemRoot%\System32\IME\IMMANAKONSO

* 64bit版 Windows

    + %SystemRoot%\System32\IME\IMMANAKONSO
    + %SystemRoot%\SysWOW64\IME\IMMANAKONSO

* 共通

    + %SystemRoot%\IME\IMMANAKONSO


### アンインストール

コントロールパネルの「プログラムと機能」からアンインストールしてください。

または、インストールに使用した exe ファイルを再度実行し「Uninstall」を選択してください。

アンインストールの後はOSを再起動しておくと安全です。


### Windows ストアアプリ、Microsoft Edge

Windows ストアアプリ、Microsoft Edge 等で使用するには、設定ファイル (config.xml) にアクセス許可を付加させる必要があります。

Windows 8 以降で動作する設定ダイアログでOKボタンを押して保存をおこなうとアクセス許可の付加が自動的におこなわれます。

また、Windows 8 以前から Windows 8 以降へ Windows をアップグレードしたときは、manakonsoを一旦アンインストールした上でインストールし直してください。

インストール時に、Windows ストアアプリ、Microsoft Edge 等で使用可能となるようにIMEの登録がおこなわれます。


### Adobe Reader X, XI, Acrobat Reader DC

Adobe Reader X 以降で使用するには、設定ファイル (config.xml) にアクセス許可を付加させる必要があります。

設定ダイアログでOKボタンを押して保存をおこなうとアクセス許可の付加が自動的におこなわれます。



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

Visual Studio Express 2015 for Windows Desktop Update 2

WiX Toolset v3.10.3

pandoc 1.16.0.2


### ビルド手順

ビルド

    > installer\_solution_build.cmd
    > installer\_build.cmd

ビルド ＆ 署名

    > installer\_solution_build.cmd
    > installer\_sign.cmd <URL 1> <pfx file 1> <password 1> <URL 2> <pfx file 2> <password 2>

	    * <URL 1> : SHA-1 Authenticode timestamp server
		* <pfx file 1> : pfx file for SHA-1 file digest algorithm
	    * <URL 2> : SHA-256 RFC-3161 timestamp server
		* <pfx file 2> : pfx file for SHA-256 file digest algorithm

クリア

    > installer\_solution_clean.cmd
    > installer\_clean.cmd

---

Copyright (C) 2015-2016 na2co3

[@na2co3_ftw](https://twitter.com/na2co3_ftw)
