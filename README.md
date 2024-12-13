# Chat-Server Program

Selamat datang di **Chat-Server**, sebuah program yang dirancang untuk memenuhi tugas besar mata kuliah *Praktikum Sistem Operasi*. Program ini memungkinkan komunikasi real-time antar beberapa klien dengan fitur **multi-client**, performa yang andal, dan keamanan data yang terjamin.

---

## ✨ Fitur Utama
- **Multi-client Support**: Mendukung lebih dari 6 klien secara simultan.
- **Keamanan Terjamin**: Data komunikasi dilindungi untuk mencegah kebocoran.
- **Kinerja Optimal**: Menggunakan socket programming untuk koneksi yang stabil dan efisien.
- Dibangun dengan bahasa C yang ringan dan cepat.

---

## 👩‍💻 Pembuat
- **Farida Aryani** (NIM: 231524005)  
- **Ratna Puspitasari** (NIM: 231524022)

---

## 🛠️ Persyaratan Sistem
Pastikan sistem Anda memenuhi persyaratan berikut sebelum menjalankan program:

- **Sistem Operasi**: Linux/Unix atau Windows (dengan *socket support*).
- **Compiler**: GCC atau kompatibel.
- **Library yang Dibutuhkan**:
  - `sys/socket.h`
  - `arpa/inet.h`
- Koneksi jaringan lokal atau internet.

---

## ⚙️ Langkah-Langkah Penggunaan

### 1. Clone Repository
Clone repository ini ke dalam komputer Anda:
```
git clone https://github.com/username/chat-server.git
```

### 2. Kompilasi Program
Gunakan GCC untuk mengompilasi program:
```
gcc -o server server.c
gcc -o client client.c
```

### 3. Jalankan Server
Pada terminal, jalankan server terlebih dahulu:
```
./server
```

### 4. Jalankan Client
Pada terminal lain, jalankan client:
```
./client
```

### 5. Mulai Komunikasi
Setelah client terhubung, Anda dapat memulai percakapan antar klien.

---

## 🧩 Cara Kerja Program
1. **Server**:
   - Menunggu koneksi dari klien menggunakan socket.
2. **Client**:
   - Mengirim dan menerima pesan dari server.
   - Mendukung komunikasi real-time antar klien yang terhubung.

---

## 💡 Tips dan Trik
- Gunakan port standar (misalnya 8080) untuk kemudahan pengaturan.
- Pastikan firewall Anda mengizinkan akses ke port yang digunakan.
- Jika terjadi masalah koneksi, periksa konfigurasi IP dan port.

---

## 🚀 Kontribusi
Kami menerima masukan dan saran! Jangan ragu untuk membuat *issue* atau *pull request* di repository ini.

---

## 📜 Lisensi
Proyek ini dibuat untuk tujuan edukasi. Silakan gunakan dengan bijak.
