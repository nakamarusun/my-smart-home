# SmartHome web server untuk Raspberry PI

## Panduan:

Pertama, mari kita setel server SQL di Raspberry pi kita. Untuk langkah selanjutnya sudah diasumsi anda berhasil login ke terminal Raspberry Pi melalui SSH / Serial / Layar yang terhubung.

1. Install MariaDB Database di RPI
```shell
sudo apt update
sudo apt install mariadb-server
```

2. Setel password dan akses rootnya
```shell
sudo mysql_secure_installation
```

3. Login kedalam MariaDB
```shell
sudo mariadb -u root -p
```

4. Buat MariaDB user baru khusus untuk smart homemu.
```SQL
CREATE USER '<NAMA_USERMU>'@'localhost' IDENTIFIED BY '<PASSWORD_BARU>';
```

5. Buat database baru untuk smarthome kita, dan beri akses full ke user yang baru kita buat
```SQL
CREATE DATABASE my_smarthome;
GRANT ALL PRIVILEGES ON my_smarthome.* TO '<NAMA_USERMU>'@'localhost';
FLUSH PRIVILEGES;
COMMIT;
EXIT;
```

Mulai dari sini, semua konfigurasi di sisi MariaDB sudah selesai, dan kita bisa lanjut ke aplikasi Flask kita.

Sekarang, kita bisa setel virtual environment python kita untuk bisa menjalankan aplikasi Flasknya.

1. Buat virtual environment baru
```shell
python -m virtualenv venv
```

2. Masuk kedalam virtual environment, dan install semua library yang diperlukan
```shell
venv/Scripts/activate
pip install requirements.txt
```