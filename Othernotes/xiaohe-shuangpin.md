2022.5.25
---

# Windows

There is an easy way to use XiaoheShuanpin via Microsoft pinyin.

Press <kbd>win + R</kbd>, input `regedit` to open register table, find key `计算机\HKEY_CURRENT_USER\Software\Microsoft\InputMethod\Settings\CHS`, new a string named `UserDefinedDoublePinyinScheme0`, whose value is `小鹤双拼*2*^*iuvdjhcwfg^xmlnpbksqszxkrltvyovt`

Then you can set `小鹤双拼` as the defualt method in Microsoft shuangpin.

2023.07.21
---

# Linux

> https://raawaa.github.io/2016/04/04/how-to-enable-flypy-in-rime-on-linux/

## Add double pin input method using ibus

- On Ubuntu

```bash
# 安装 ibus-rime ，据说 fcitx-rime 不是官方维护的。
$ sudo apt-get install ibus-rime

# 安装双拼方案
$ sudo apt-get insatll librime-data-double-pinyin
```

- On Archlinux

```bash
$ sudo pacman -S ibus-rime  # Archlinux 下已自带双拼方案，无需另外安装
```

Use `ibus-setup` to add Chinese(rime)

## Add config file for ibus

Create `~/.config/ibus/rime/default.custom.yaml`

```yaml
patch:
  schema_list:
    - schema: luna_pinyin          # 朙月拼音
    - schema: luna_pinyin_simp     # 朙月拼音 简化字模式
    - schema: luna_pinyin_tw       # 朙月拼音 臺灣正體模式
    - schema: terra_pinyin         # 地球拼音 dì qiú pīn yīn
    - schema: bopomofo             # 注音
    - schema: jyutping             # 粵拼
    - schema: cangjie5             # 倉頡五代
    - schema: cangjie5_express     # 倉頡 快打模式
    - schema: quick5               # 速成
    - schema: wubi86               # 五笔 86
    - schema: wubi_pinyin          # 五笔拼音混合輸入
    - schema: double_pinyin        # 自然碼雙拼
    - schema: double_pinyin_mspy   # 微軟雙拼
    - schema: double_pinyin_abc    # 智能 ABC 雙拼
    - schema: double_pinyin_flypy  # 小鶴雙拼
    - schema: wugniu               # 吳語上海話（新派）
    - schema: wugniu_lopha         # 吳語上海話（老派）
    - schema: sampheng             # 中古漢語三拼
    - schema: zyenpheng            # 中古漢語全拼
    - schema: ipa_xsampa           # X-SAMPA 國際音標
    - schema: emoji                # emoji 表情
```

Restart ibus by running command `ibus restart` .

Then, you can press `Ctrl+\`` when you are typing using rime to config the rime input method;
