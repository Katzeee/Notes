2023.07.24
---

# Install neovim(through ppa)

```bash
sudo apt-get install software-properties-common
sudo add-apt-repository ppa:neovim-ppa/unstable
sudo apt-get update
sudo apt-get install neovim
```

# Install nerd fonts

```bash
#/bin/bash
#https://www.nerdfonts.com/font-downloads
echo "[-] Download fonts [-]"
echo "https://github.com/ryanoasis/nerd-fonts/releases/download/v3.0.2/JetBrainsMono.zip"
wget https://github.com/ryanoasis/nerd-fonts/releases/download/v3.0.2/JetBrainsMono.zip
unzip DroidSansMono.zip -d ~/.local/share/fonts/
fc-cache -fv
echo "done!"
```

Remember to restart a terminal.

use `fc-list` to show all fonts installed.
