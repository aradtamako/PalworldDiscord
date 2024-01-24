# 参考動画
https://streamable.com/nfgbe8

# ダウンロード
https://github.com/aradtamako/PalworldDiscord/releases

# 前提条件
- Windowsのパルワールドサーバのみで動作

- Linuxのパルワールドサーバでは動作しない
- 2024/01/25 時点の最新クライアントで動作確認

# 使い方1
1. 以下4つのファイルを以下ディレクトリにコピーする
- cpprest_2_10.dll
- minhook.x64.dll
- version.dll
- DiscordPluginConfig.json

```
steamapps\common\PalServer\Pal\Binaries\Win64\
```

2. PalServer.exe を起動する

以上


# その他
- main.py はDiscordで入力したメッセージをパルワールド内に送信したい場合に必要
- パルワールドのメッセージをDiscordに送信するだけであれば不要
- 簡単なので説明は省略
