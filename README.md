# Sistem Monitoring Penggunaan Daya Listrik Secara Real-Time Berbasis Internet of Things (IoT) dengan Protokol MQTT QoS Level 1

**Penulis :** Indro Dwi Saputro
**NIM :** 2009106099
**Program Studi :** Informatika, Fakultas Teknik
**Universitas MULAWARMAN – 2025**

---

## 🧠 Deskripsi Singkat

Proyek ini merupakan hasil penelitian skripsi yang bertujuan untuk **merancang dan membangun sistem monitoring penggunaan daya listrik secara real-time berbasis Internet of Things (IoT)**.
Sistem dikembangkan menggunakan **protokol MQTT dengan Quality of Service (QoS) Level 1** untuk memastikan pengiriman data minimal satu kali antara perangkat dan server.

Perangkat utama:

* **ESP32** → Mikrokontroler pengirim data (*publisher*)
* **Sensor PZEM-004T** → Pengukur tegangan, arus, daya, dan energi listrik
* **Mosquitto Broker** → Server MQTT lokal untuk komunikasi data
* **FRP (Fast Reverse Proxy)** → Menghubungkan broker lokal ke jaringan publik
* **Aplikasi Android (Kodular)** → Menampilkan hasil monitoring secara real-time

---

## 🎯 Tujuan

1. Mengimplementasikan **metode QoS Level 1** pada protokol MQTT untuk meningkatkan keandalan transmisi data.
2. Merancang dan membangun **prototipe sistem IoT** yang dapat memantau konsumsi listrik rumah tangga secara real-time.
3. Menganalisis performa jaringan berdasarkan parameter **IP Packet Transfer Delay (IPTD)** dan **IP Packet Loss Ratio (IPLR)** sesuai standar **ITU-T Y.1541**.

---

## ⚙️ Arsitektur Sistem

```
[PZEM-004T] ⇄ [ESP32] ⇄ [Router Wi-Fi]
         ↓
     [Broker MQTT (Mosquitto)]
         ↓
     [FRP Server Publik]
         ↓
  [Aplikasi Android (Kodular)]
![alt text]https://drive.google.com/file/d/1y2_jFxCM9v0zlLWh-fi-jEPuN4ESc05Y/view?usp=drive_linkhttps://drive.google.com/file/d/1y2_jFxCM9v0zlLWh-fi-jEPuN4ESc05Y/view?usp=drive_link
```

* Sensor PZEM-004T mengukur tegangan, arus, daya, dan energi.
* ESP32 mengirim data ke broker MQTT lokal dengan QoS 1.
* Broker dihubungkan ke internet melalui FRP untuk akses jarak jauh.
* Aplikasi Android berlangganan (*subscribe*) topik MQTT dan menampilkan data pengguna.

---

## 💻 Teknologi yang Digunakan

| Komponen            | Teknologi / Perangkat       |
| ------------------- | --------------------------- |
| Mikrokontroler      | ESP32                       |
| Sensor              | PZEM-004T                   |
| Protokol Komunikasi | MQTT QoS Level 1            |
| Broker              | Mosquitto                   |
| Proxy / Tunnel      | Fast Reverse Proxy (FRP)    |
| Aplikasi Mobile     | Kodular (block programming) |
| Tools Analisis      | Wireshark                   |
| Standar Evaluasi    | ITU-T Y.1541                |

---

## 🧩 Metodologi

Metode penelitian: **Research and Development (R&D)** dengan pendekatan eksperimental.
Tahapan:

1. Identifikasi masalah & studi literatur
2. Analisis kebutuhan
3. Perancangan sistem (perangkat keras dan lunak)
4. Implementasi dan integrasi komponen
5. Pengujian QoS (MQTT QoS Level 1)
6. Evaluasi hasil berdasarkan standar ITU-T Y.1541

---

## 🔍 Hasil Pengujian

| Parameter                       | Jarak Dekat | Jarak Jauh | Kategori ITU-T Y.1541   |
| ------------------------------- | ----------- | ---------- | ----------------------- |
| IP Packet Transfer Delay (IPTD) | 4,65 ms     | 10,48 ms   | Sangat Baik (≤ 100 ms)  |
| IP Packet Loss Ratio (IPLR)     | 0,0014 %    | 0,0044 %   | Sangat Baik (≤ 0,025 %) |

**Kesimpulan :** Transmisi data MQTT QoS Level 1 stabil dan efisien. Sistem berhasil menampilkan pemakaian daya listrik secara real-time melalui aplikasi Android.

---

## 📱 Tampilan Aplikasi

Antarmuka dibangun menggunakan **Kodular**, menampilkan:

* Tegangan (V)
* Arus (A)
* Daya (W)
* Energi (kWh)
* Total biaya konsumsi listrik

*(Gambar antarmuka dapat disesuaikan pada folder `/images` proyek.)*

---

## 🚀 Cara Instalasi

### Persiapan

1. Unduh dan instal Arduino IDE / PlatformIO.
2. Siapkan library ESP32 dan PZEM-004T.
3. Siapkan broker Mosquitto di server lokal.
4. Konfigurasikan FRP agar broker dapat diakses publik.

### Langkah

```bash
# Clone repository
git clone https://github.com/IndroMan/sistem-monitoring-daya-listrik.git
cd sistem-monitoring-daya-listrik

# Upload program ke ESP32
# (gunakan file .ino yang telah disesuaikan)

# Jalankan broker MQTT
mosquitto -v

# Jalankan FRP
frpc -c frpc.ini
```

### Jalankan Aplikasi

1. Buka aplikasi Android yang dibuat dengan Kodular.
2. Aplikasi akan langsung menampilkan output dari sensor yang berupa :
  - Tegangan (V)
  - Arus (A)
  - Daya (W)
  - Energi (kWh)
  - Total biaya konsumsi listrik

---

## 📂 Struktur Folder

```
/sistem-monitoring-daya-listrik
│
├─ /arduino_code        # Program ESP32 + PZEM-004T
├─ /broker_config       # Konfigurasi Mosquitto
├─ /frp_config          # File konfigurasi FRP (frpc.ini, frps.ini)
├─ /android_app         # File .aia / .apk dari Kodular
├─ /analysis            # Hasil pengujian Wireshark, IPTD & IPLR
├─ /images              # Gambar diagram & antarmuka aplikasi
└─ README.md
```

---

## 📊 Analisis Standar ITU-T Y.1541

Kategori penilaian QoS berdasarkan standar ITU-T Y.1541:

| Kategori    | IP Packet Transfer Delay | IP Packet Loss Ratio |
| ----------- | ------------------------ | -------------------- |
| Sangat Baik | ≤ 100 ms                 | ≤ 0,025 %            |
| Baik        | 133 – 200 ms             | 0,025 – 0,05 %       |
| Sedang      | 201 – 400 ms             | 0,05 – 0,1 %         |
| Buruk       | > 400 ms                 | > 0,1 %              |

Hasil pengujian menunjukkan sistem berada pada kategori **Sangat Baik** untuk semua parameter.

---

## 🧾 Kesimpulan

* Sistem IoT berhasil memantau daya listrik secara real-time dengan akurasi tinggi.
* QoS Level 1 menjamin keandalan pengiriman data melalui MQTT.
* Nilai delay dan loss berada dalam kategori **sangat baik** berdasarkan standar ITU-T Y.1541.

---

## 🛠️ Saran

* Menambah fitur multi-node monitoring.
* Menyediakan notifikasi penggunaan daya berlebih.
* Integrasi kontrol beban otomatis.
* Pengembangan dashboard berbasis web.

---

## ✉️ Kontak

**Indro Dwi Saputro**
📧 [indrodwi2@gmail.com](mailto:indrodwi2@gmail.com)
🏫 Universitas Mulawarman – Program Studi Informatika
📍 Samarinda, Kalimantan Timur
🌐 [https://github.com/IndroMan](https://github.com/IndroMan)

---
