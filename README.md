# SA-MP Map Editor

Map Editor ini dirancang khusus untuk para mapper, terutama pemain **Jogjagamers Roleplay**, agar dapat menggunakan map editor dengan kemampuan yang mirip seperti yang digunakan di dalam server JGRP. Editor ini dilengkapi fitur pemetaan objek, pengelompokan, export/import map, dan masih banyak lagi.

## 📦 Installation

### 📋 Prerequisites

Sebelum melanjutkan instalasi, pastikan hal berikut:

- **sampctl**  
  Tool utama untuk mengelola dan membangun project ini.  
  📖 [Cara install sampctl](https://github.com/Southclaws/sampctl#installing)

- **Git** (opsional)  
  Digunakan untuk meng-clone repository dari GitHub. Jika tidak ingin menggunakan Git, Anda bisa langsung download file ZIP dari GitHub.

### 📥 Source Code Retreival

Anda bisa mendapatkan source code proyek ini dengan dua cara:

#### 1. Download ZIP
Download source code sebagai ZIP, ekstrak hasilnya di tempat penyimpanan yang Anda inginkan

#### 2. Menggunakan Git (opsional)
Jika Anda tidak memiliki Git, Anda bisa:
- Kunjungi halaman repositori: ([samp-map-editor](https://github.com/BabyJnL/samp-map-editor))
- Klik tombol "**Code**" -> "**Download ZIP**"
- Ekstrak file ZIP
- Buka folder hasil ekstraksi

## ✅ Setup

Setelah mengunduh atau meng-clone project ini, lakukan langkah berikut:

1. Buat folder `map` yang nantinya folder ini akan digunakan untuk menyimpan file map hasil export atau untuk keperluan import
2. Buat folder `internal` di dalam folder `scriptfiles` untuk penyimpanan data player
    > Note: tanpa folder internal ini, server tidak bisa membuat file database untuk menyimpan data player
3. Install Depedencies, Build & Jalankan Gamemode:
    ```bash
    sampctl ensure
    sampctl build
    sampctl run
    ```

## 💡 In-Game Usage

Setelah server berjalan, buka client San Andreas Multiplayer kemudian tambahkan new server `localhost:7777` dan masuk ke dalam servernya


### 📋 List Command

| Command | Deskripsi |
|--------|-----------|
| `/help` | Menampilkan command yang tersedia di server
| `/object` / `/ob` | Menampilkan menu utama object editor |
| `/object create [model]` | Membuat objek berdasarkan parameter model yang diberikan |
| `/ob remove [slot]` | Menghapus object berdasarkan slot yang diberikan |
| `/ob clear` | Menghapus semua object yang dibuat |
| `/ob copy [slot]` | Membuat clone dari object slot yang diberikan |
| `/ob move [slot] [direction (N/S/E/W/U(p)/D(own))] [amount] [opt:speed = 5.0]` | Memindahkan object ke arah tertentu sebanyak amount yang diberikan dan dapat diatur kecepatan perpindahannya |
| `/ob rot [slot] [slot] [rot X] [rot Y] [rot Z]` | Mengubah rotasi pada object slot dan titik rotasi yang diberikan |
| `/ob select [opt: slot]` | Memilih object untuk diedit (parameter slot optional apabila ingin memilih objek dengan mengklik objek yang dituju) |
| `/ob tele [slot]` | Teleportasi ke posisi object slot yang diberikan |
| `/ob texture [slot] [index] [model] [txdname] [texture] [opt: alpha] [opt: red]  [opt: green]  [opt: blue]` | Memberikan texture pada object slot yang diberikan disertai dengan parameter pendukung untuk texture yang diinginkan (Texture dapat ditemukan di: https://textures.xyin.ws/?page=textures) |
| `/ob resetmaterial [slot]` | Mengatur ulang semua texture / material object slot menjadi default |
| `/ob textprop [slot] [index = 0]` | Menampilkan menu untuk mengatur material teks pada object slot dan index yang diberikan (default index adalah 0) |
| `/ob paintbrush [material index] [model] [txdname] [texture] [opt: alpha] [opt: red]  [opt: green]  [opt: blue]` | Hampir sama dengan `/ob texture` namun alih-alih mengubah texture object slot slot secara langsung, command ini dapat mengubah texture banyak object dengan cara menembak object yang dituju |
| `/ob model [slot] [model]` | Mengubah model pada object slot yang diberikan |
| `/ob export [map name]` | Menyimpan object yang telah dibuat menjadi file .txt dengan nama map yang diberikan |
| `/ob load [map name]` | Memuat object beserta texturenya dari file .txt dengan nama map yang diberikan di direktori map  |
| `/ob group [slot] [group]` | Mengatur group dari object slot yang diberikan  |
| `/ob gclone [group]` | Membuat clone dari group id yang diberikan  |
| `/ob gmove [group] [direction (N/S/E/W/U(p)/D(own))] [amount] [opt:speed = 5.0]` | Memindahkan group object ke arah tertentu sebanyak amount yang diberikan dan dapat diatur kecepatan perpindahannya  |
| `/ob grotate [group] [rot X] [rot Y] [rot Z]` | Mengubah rotasi pada object group dan titik rotasi yang diberikan  |
| `/ob gremove [group]` | Menghapus object pada group yang diberikan  |

## 🙏 Credits
Special Thanks to :
- Southclaws for sampctl and tooling
- Y_Less for YSI & sscanf
- Incognito for Streamer Plugin
- JaTochNietDan for FileManager Plugin
- tianmetal for Gamemode Base Script
- BabyJnL for Bug Fixes and Enhancements

## 📄 License

This project is open-source. You are free to use, modify, and distribute with proper credits.  
