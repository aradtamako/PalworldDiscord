# 機能
- パルワールド内のメッセージをDiscordに送信する
- Discordのメッセージをパルワールド内に送信する

# 参考動画
https://github.com/aradtamako/PalworldDiscord/assets/40272766/f2980863-9aec-440b-acee-935a4005f356


# ダウンロード
https://github.com/aradtamako/PalworldDiscord/releases

# 前提条件
- Windowsのパルワールドサーバのみで動作
- Linuxのパルワールドサーバでは動作しない
- 2024/01/25 時点の最新サーバ／クライアントで動作確認

# 使い方
1. 以下4つのファイルを以下ディレクトリにコピーする
- cpprest_2_10.dll
- minhook.x64.dll
- version.dll
- DiscordPluginConfig.json

```
steamapps\common\PalServer\Pal\Binaries\Win64\
```

2. DiscordPluginConfig.json を編集する

3. PalServer.exe を起動する

以上

# その他
- main.py はDiscordで入力したメッセージをパルワールド内に送信したい場合に必要
- パルワールド内のメッセージをDiscordに送信するだけであれば不要
- 簡単なので説明は省略
