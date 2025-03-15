# OSP2-BE: The Enhanced Quake 3 Mod

OSP2-BE is an enhanced version of the classic OSP mod, built on the open-source OSP2 and detailed analysis of Quake 3. This project integrates contributions from well-known sources in the Quake community to bring modern improvements.


## 🔧 Contributions

OSP2-BE incorporates code from several notable Quake mods and projects:
- **OSP2 by snems**: [Github Link](https://github.com/snems/OSP2)
- **baseq3a by Cyrax**: [GitHub Link](https://github.com/ec-/baseq3a)
- **XQ3E Engine**: [GitHub Link](https://github.com/xq3e/engine)
- **Unlagged Mod by Nail Toronto**
- **Ratmod**: [GitHub Link](https://github.com/rdntcntrl/ratoa_gamecode)

## ✨ Features

OSP2-BE features:

- Additional background for shud obituaries 
- Temporal LG Accuracy for superhud

- cg_itemsRespawnAnimation 0 - disable respawn animations for 3D items
- cg_shud_fightSound 0 - disable fight sound at respawn for freeze mod
- cg_drawCenterMessages 0 - disable center messages (welcome server msg, frag msg etc.)
- cg_drawDamage 3/4 - enable red border when damage recieve. (3 - classic + border, 4 - border only)
- cg_drawGun reworked. bitmask: 2 - no movement animation, 4 - no switch animation, 8 - no fire animation, 16 - ghost shader. 3 still Quake Live analog (ghost shader + no movement animation)  

![shot-Dec-17-2024_17 11 28](https://github.com/user-attachments/assets/97452e84-d981-4458-8fb8-92b88cd6dad1)

OSP2 offers an array of exciting features that enhance gameplay:

- **VQ3/Promode physics support** – Choose your style of play with physics options tailored for each mode.
- **CPMA-like SuperHUD** – Experience an advanced HUD, reminiscent of CPMA, for clearer visuals.
- **Proportional Fonts** – Enjoy improved readability and visual appeal with proportional fonts.
- **Multilevel Hitsounds & Fragsounds** – Customizable sounds for hits and frags that add depth to the gameplay.
- **Crosshair Decorations & Animations** – A variety of decorative and animated crosshairs to enhance your visual experience.
- **Client-side /mute** – Mute players directly on your client to avoid distractions during gameplay.
- **And many other small improvements and tweaks** – A polished experience with multiple optimizations.

![OSP2 Screenshot](https://github.com/snems/OSP2/blob/master/docs/shot.jpg?raw=true)


## 🔄 Mod State

Currently, the mod includes the **cgame** component only. More features will be added over time.

## 📞 Contacts

Join the OSP2-BE community on Telegram for updates, discussions, and support:
- [Telegram Channel](https://t.me/q3osp2)
- [Telegram Chat](https://t.me/q3_osp2)

## 🛠 New Commands

See [docs/commands.md](docs/commands.md) for a comprehensive list of new commands and how to use them.

Example configuration file: `OSP2.cfg` located inside the mod directory.

## 📥 Installation

To install OSP2-BE:

1. **Install Quake 3** and the **OSP mod v1.03**.
2. Copy the mod files (pack or library) into the `osp` folder within your Quake 3 directory.

## 🏗 Building the Mod

Currently, only **Linux QVM builds** are supported.

### To Build for Linux QVM:

1. Navigate to the build directory:
    ```bash
    cd build/linux-qvm
    ```

2. Run the `make` command to compile the mod:
    ```bash
    make
    ```

Once the build process is complete, you can run the mod on your Quake 3 server or client.
