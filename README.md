# Soal Shift Modul 3 (Kelompok E6)

###### Nama Anggota :
1. Ifta Jihan Nabila (05111740000034)
2. Komang Yogananda MW (05111740000114)

**Penjelasan Soal Shift Modul 3 Sistem Operasi 2019:**
* [Soal 1](#Soal-1)
* [Soal 2](#Soal-2)
* [Soal 3](#Soal-3)
* [Soal 4](#Soal-4)
* [Soal 5](#Soal-5)

## Soal-1
Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan  
Contoh:  
    ./faktorial 5 3 4  
    3! = 6  
    4! = 24  
    5! = 120 
      
**_Jawaban:_** 

```c
int arg[500];
pthread_t thread[500];
int status;
```
Pendeklarasian variabel **arg**, **thread**, dan **status**. Variable **arg** untuk menampung argument program, **thread** adalah thread yang digunakan untuk menghitung nilai faktorial masing masing dan **status** digunakan sebagai penanda agar saat mencetak output terurut.

```c
void *factorial(void *num){
	long long ans = 1;
	int fak = *((int *)num);
	pthread_t id = pthread_self();
	for (int i = 1; i <= fak; i++){
		ans = ans * i;
	}
	while (!pthread_equal(id, thread[status]));
	printf("%d! = %lld\n",fak, ans);
	status++;
}
```
Fungsi **faktorial** digunakan untuk menghitung nilai faktorial dari **num**. Faktorial dihitung menggunakan perulangan dari 1 hingga n.  
`while (!pthread_equal(id, thread[status]));` potongan kode tersebut digunakan agar setiap thread yang berjalan paralel mencetak output sesuai dengan antriannya (dari yang paling kecil) dengan memanfaatkan variabel **status**.

```c
int main(int argc, char **argv){
    	for (int i = 1; i < argc; ++i){
		arg[i-1] = atoi(argv[i]);
	}
	status = 0;
	qsort(arg, argc-1, sizeof(int), cmpfunc);
	for (int i = 0; i < argc-1; i++) {
		int err = pthread_create(&(thread[i]), NULL, &factorial, &(arg[i]));
		if (err != 0){
			printf("\n can't create thread for factorial %d : [%s]",arg[i], strerror(err));
		}
	}
	for (int i = 0; i < argc-1; i++) pthread_join(thread[i], NULL);
	return 0;
}
```
- Fungsi `atoi` digunakan untuk mengkonversi string menjadi integer.
- Fungsi `qsort(arg, argc-1, sizeof(int), cmpfunc)` digunakan untuk mengurutkan argument agar terurut dari kecil ke besar. `qsort` mengambil argument `cmpfunc` yang berupa comparator.
- Fungsi `pthread_create(&(thread[i]), NULL, &factorial, &(arg[i]))` digunakan untuk mengassign sebuah thread untuk menghitung faktorial dari sebuah argument. Setiap argument memiliki 1 thread untuk menghitung nilai faktorialnya.

**_Hasil_**  
![faktorial](soal1/hasil.PNG)


## Soal-2
Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:  
- Terdapat 2 server: server penjual dan server pembeli
- 1 server hanya bisa terkoneksi dengan 1 client  
- Server penjual dan server pembeli memiliki stok barang yang selalu sama  
- Client yang terkoneksi ke server penjual hanya bisa menambah stok  
    - Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1  
- Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok  
    - Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1  
- Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok  
    - Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”  
    - Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”  
- Server penjual akan mencetak stok saat ini setiap 5 detik sekali  
- Menggunakan thread, socket, shared memory

**_Jawaban_**  
Terdapat 4 program jawaban yaitu
1. Program Server Penjual
2. Program Server Pembeli
3. Program Client Penjual
4. Program Client Pembeli

**Program Server Penjual**
```c
#define PORT 8080
```
Server dan client pembeli menggunakan port `8080`
```c
void *message(void *arg){
    char message[100];
    int socket = *((int *)arg);
    int valread;
    while( valread = recv(socket, message, 100, 0) > 0){
        // printf("%s\n", message);
        if (strcmp(message, "tambah") == 0){
            *value = *value + 1;
            send(socket, "transaksi berhasil", sizeof("transaksi berhasil"), 0);
        }else{
            send(socket, "transaksi gagal", sizeof("transaksi gagal"), 0);
        }
        memset(message, 0, sizeof(message));
    }
    if (valread == 0){
        printf("Client disconnects.\n");
    }
    total_connection--;
}
```
- Fungsi **message** digunakan untuk menghandle connection dari client.
- **total_connection** merupakan variabel bantu untuk menghitung jumlah koneksi yang masuk ke server.
- **variabel message** merupakan variabel untuk menampung pesan yang diterima oleh server.
- Setelah menerima pesan dari client, server pasti mengirim pesan balik ke client sesuai dengan kondisinya. Server mengirim `"-1"` apabila connectionnya direject.

```c
void *fiveSecondTimer(void *arg){
    while(1){
        int a = *((int *)arg);
        printf("Stock saat ini: %d\n", a);
        sleep(5);
    }
}
```
- Fungsi **fiveSecondTimer** digunakan untuk menampilkan stock setiap 5 detik. Fungsi ini berjalan pada sebuah thread.

```c
while(1){
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    if (total_connection == 1){
        send(new_socket, reject, sizeof(reject), 0);
        close(new_socket);
    }else{
        printf("New Clients detected.\n");
        total_connection++;
        int *socket = (int*)malloc(sizeof(int));
        *socket = new_socket;
        pthread_t messageHandler;
        pthread_create(&messageHandler, NULL, &message, (void *)socket);
    }
}
```
- Setiap saat server menerima koneksi socket dari client. Namun ketika **total_connection** active sudah 1 maka socket baru tersebut langsung ditutup.
- Apabila **total_connection** masih 0 maka dijalankan thread untuk menghandle socket tersebut.

**Program Server Pembeli**
```c
#define PORT 8000
```
Server dan client pembeli menggunakan port `8000`
```c
void *message(void *arg){
    char message[100];
    int socket = *((int *)arg);
    int valread;
    while( valread = recv(socket, message, 100, 0) > 0){
        // printf("%s\n", message);
        if (strcmp(message, "beli") == 0){
            if (*value > 0){
                *value = *value - 1;
                send(socket, "transaksi berhasil", sizeof("transaksi berhasil"), 0);
            }else{
                send(socket, "transaksi gagal", sizeof("transaksi gagal"), 0);
            }
        }else{
            send(socket, "transaksi gagal", sizeof("transaksi gagal"), 0);
        }
        memset(message, 0, sizeof(message));
    }
    if (valread == 0){
        printf("Client disconnects.\n");
    }
    total_connection--;
}
```
- Fungsi **message** pada Server pembeli sama seperti pada fungsi server penjual, perbedaannya pada `strcmp(message, "beli")`
- Fungsi utama pada Server Pembeli juga sama seperti Server Penjual.

**Program Client Penjual**
```c
#define PORT 8080
```
Client penjual menggunakan port 8080.
```c
while(1){
    if (~scanf("%s", tipe)){
        send(sock , tipe , strlen(tipe) , 0 );
    }
    char message[100];
    if (recv(sock, message, 100, 0)){
        if (strcmp(message, "-1") == 0) {
            printf("Connection rejected\n");
            return 0;
        }else {
            printf("%s\n",message);
        }
    }
}
```
- Potongan program diatas digunakan untuk membaca inputan user terus menerus. Apabila menerima inputan string maka inputan tersebut dikirim ke server.
- Setelah mengirim pesan maka client akan menunggu pesan response dari server.
- Apabila pesan yang diterima `-1` maka program keluar karena koneksi ditolak.

**Program Client Pembeli**
```c
#define PORT 8000
```
Server dan client pembeli menggunakan port `8000`

```c
while(1){
    if (~scanf("%s", tipe)){
        send(sock , tipe , strlen(tipe) , 0 );
    }
    char message[100];
    if (recv(sock, message, 100, 0)){
        if (strcmp(message, "-1") == 0) {
            printf("Connection rejected\n");
            return 0;
        }else {
            printf("%s\n",message);
        }
    }
}
```
- Fungsi utama dari client pembeli sama seperti client penjual.

**_Hasil_**  
Server penjual  
![serverpenjual](soal2/serverpenjualimage.PNG)  
Server pembeli  
![serverpembeli](soal2/serverpembeliimage.PNG)  
Client penjual  
![clientpenjual](soal2/clientpenjualimage.PNG)  
Client pembeli  
![clientpembeli](soal2/clientpembeliimage.PNG)  

## Soal-3
Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:

 a. Terdapat 2 karakter Agmal dan Iraj
 
 b. Kedua karakter memiliki status yang unik
 - Agmal mempunyai WakeUp_Status, di awal program memiliki status 0
 - Iraj memiliki Spirit_Status, di awal program memiliki status 100
 - Terdapat 3 Fitur utama
   - All Status, yaitu menampilkan status kedua sahabat
   - Ex: Agmal WakeUp_Status = 75   
     Iraj Spirit_Status = 30
   - “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
   - “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point
 - Terdapat Kasus yang unik dimana:
   - Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10        detik (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)
   - Jika Fitur  “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10        detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)
 - Program akan berhenti jika Salah Satu :
   - WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)
   - Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”)
 - Syarat Menggunakan Lebih dari 1 Thread
 
 **_Jawaban_**
 
 ```c
char orang1[10]="Agmal";
char orang2[10]="Iraj";
int WakeUp_Status;
int Spirit_Status;
int pilih, inputAgmal, inputIraj;

pthread_t tid1;
pthread_t tid2;
pthread_t tid3;
pthread_t tid4;
pthread_t tid5;
 ```
- Mendeklarasikan variabel **orang1** dan **orang2** untuk 2 karakter yaitu _Agmal_ dan _Iraj_.
- Terdapat variabel **WakeUp_Status** untuk menyimpan status dari karakter _Agmal_ sedangkan **Spirit_Status** untuk _Iraj_.
- Variabel **pilih** untuk memilih fitur yang ada, **inputAgmal** untuk menyimpan seberapa banyak fitur "Agmal Ayo Bangun"     dijalankan dan **inputIraj** untuk fitur "Iraj Ayo Tidur".
- Terdapat 5 thread yang dipakai

```c
void* fitur(void *arg)
{
	printf("\nFitur Utama:\n");
        printf("1. Tampilkan All Status\n");
        printf("2. Agmal Ayo Bangun\n");
        printf("3. Iraj Ayo Tidur\n");
	printf("4. Exit\n\n");
        printf("Pilih Fitur: "); scanf("%d", &pilih);
}
```
- Thread untuk memanggil menu awal

```c
void* AllStatus(void *arg)
{
	printf("\nSemua Status: \n");
	printf("Agmal : %d\n", WakeUp_Status);
	printf("Iraj : %d\n\n", Spirit_Status);
}
```
- Thread untuk Fitur Utama "All Status"

```c
void* Agmal_Bangun(void *arg)
{
	if(inputIraj == 3)
        {
                sleep(10);
		inputIraj = 0;
        }
	else
	{
		WakeUp_Status += 15;
	}
}
```
- Thread untuk Fitur Utama "Agmal Ayo Bangun"
- Thread ini akan disabled selama 10 detik apabila Fitur Utama "Iraj Ayo Tidur" telah dijalankan selama 3 kali, maka terdapat if condition untuk variabel **inputIraj==3** apabila benar maka akan **sleep(10)** dan set **inputIraj** menjadi 0 kembali
- Else, Fitur "Agmal Ayo Bangun" akan menambah **WakeUp_Status** sebanyak 15

```c
void* Iraj_Tidur(void *arg)
{
	if(inputAgmal == 3)
	{
		sleep(10);
		inputAgmal = 0;
	}
	else
	{
		Spirit_Status -= 20;
	}
}
```
- Thread untuk Fitur Utama "Iraj Ayo Tidur"
- Thread ini akan disabled selama 10 detik apabila Fitur Utama "Agmal Ayo Bangun" telah dijalankan selama 3 kali, maka terdapat if condition untuk variabel **inputAgmal==3** apabila benar maka akan **sleep(10)** dan set **inputAgmal** menjadi 0 kembali
- Else, Fitur "Iraj Ayo Tidur" akan mengurangi **Spirit_Status** sebanyak 20

```c
void* input(void *arg)
{
        if(inputIraj == 3)
        {
                printf("Fitur Agmal Ayo Bangun disabled 10 s\n");
                pthread_create(&(tid3), NULL, Agmal_Bangun, NULL);
        }

        else if(inputAgmal == 3)
        {
                printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
                pthread_create(&(tid4), NULL, Iraj_Tidur, NULL);
        }

}
```
- Thread untuk cek apakah salah satu variabel **inputIraj** atau **inputAgmal** memiliki nilai sebanyak 3.
- Apabila iya, maka tampilkan pesan "Fitur Agmal Ayo Bangun disabled 10 s" jika **inputIraj**=3 atau "Fitur Iraj Ayo Tidur disabled 10 s" jika **inputAgmal**=3. Dan masing-masing akan memanggil thread untuk fitur "Agmal Ayo Bangun" atau "Iraj Ayo Tidur" untuk menjalankan fungsi **sleep(10)**.

_**Pada fungsi main**_
```c
WakeUp_Status = 0;
Spirit_Status = 100;
```
- Awal mendeklarasikan variabel **WakeUp_Status**=0 dan **Spirit_Status**=100
- Lalu terdapat **while(1)** untuk input fitur berulang-ulang.

```c
if(WakeUp_Status >= 100)
{
	printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
	break;
}
else if(Spirit_Status <= 0)
{
	printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
	break;
}
```
- Cek apakah **WakeUp_Status** sudah melebihi 100 maka keluarkan pesan "Agmal Terbangun, mereka bangun pagi dan berolahraga" dan akan keluar dari program, begitu juga untuk **Spirit_Status** sudah kurang dari 0 akan keluar pesan "Iraj ikut tidur, dan bangun kesiangan bersama Agmal" dan keluar dari program.

```c
pthread_create(&(tid1), NULL, fitur, NULL);
pthread_join(tid1, NULL);
```
- Thread untuk selalu menampilkan Menu Awal berupa fitur-fitur utama

```c
if(pilih==1)
{
	pthread_create(&(tid2), NULL, AllStatus, NULL);
	pthread_join(tid2, NULL);
}

else if(pilih==2)
{
	if(inputIraj == 3)
	{

	}
	else
	{
		pthread_create(&(tid3), NULL, Agmal_Bangun, NULL);
		pthread_join(tid3, NULL);
		inputAgmal++;
	}
}

else if(pilih==3)
{
	if(inputAgmal == 3)
	{

	}
	else
	{
		pthread_create(&(tid4), NULL, Iraj_Tidur, NULL);
		pthread_join(tid4, NULL);
		inputIraj++;
	}
}
else if(pilih==4)
{
	exit(0);
}
else
{
	printf("Input Salah\n");
}

```
- If condition untuk memilih fitur, tiap fitur AllStatus, Agmal maupun Iraj dipanggil maka akan menjalankan thread untuk masing-masing fitur.
- Increment variabel **inputAgmal** dan **inputIraj** setiap Fitur Agmal atau Iraj dipanggil.

```c
pthread_create(&(tid5), NULL, input, NULL);
pthread_join(tid5, NULL);
```
- Di akhir **while(1)**, memanggil thread untuk mengecek input Agmal maupun Iraj

_**Hasil:**_

Apabila menampilkan AllStatus  
![AllStatus](soal3/AllStatus.PNG)  

Memanggil Fitur Agmal Ayo Bangun sebanyak 3 kali, lalu Fitur Iraj Disabled. Namun fitur Agmal tetap bisa dijalankan  
![Coba1](soal3/Coba1.PNG) 

Memanggil Fitur Agmal Ayo Bangun sebanyak 3 kali, lalu Fitur Iraj Disabled. Fitur Iraj dipanggil maka tidak bisa dijalankan 
![Coba2](soal3/Coba2.PNG) 

Memanggil Fitur Agmal Ayo Bangun sebanyak 3 kali, lalu Fitur Iraj Disabled. Lalu menunggu 10 detik dan jalankan fitur Iraj
![tunggu](soal3/tunggu.PNG) 

Contoh apabila variabel WakeUp_Status melebihi 100 dan keluar dari program
![exit](soal3/exit.PNG)  

## Soal-4
Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 

Dengan Syarat : 
- Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
- Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
- Ketika Mengekstrak file .zip juga harus secara bersama-sama
- Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik
  untuk mengekstrak kembali”
- Wajib Menggunakan Multithreading
- Boleh menggunakan system

**_Jawaban_**

Terdapat 6 thread 
- Thread untuk save ke file SimpanProses1.txt
  ```c
  void* save1 (void *arg)
  {
	status1 = 0;
	chdir("/home/jihan/Documents/FolderProses1");
	system("ps -aux | head -11 | tail -10 > SimpanProses1.txt");
	status1 = 1;
  }
  ```
  
- Thread untuk save ke file SimpanProses2.txt
  ```c
  void* save2 (void *arg)
  {
	status2 = 0;
	chdir("/home/jihan/Documents/FolderProses2");
	system("ps -aux | head -11 | tail -10 > SimpanProses2.txt");
	status2 = 1;
  }
  ```
  - Terdapat variabel **status1** dan **status2** sebagai flag, **status1** dan **status2** akan diset menjadi 1 apabila 10 list proses telah disimpan ke dalam file SimpanProses1.txt dan SimpanProses2.txt
  - Kami menggunakan system untuk membuka proses yang sedang berjalan _ps -aux_. 
  - Sedangkan _head -11 | tail -10_ untuk mengambil list dari 10 pertama, terdapat command _tail -10_ agar baris pertama saat memanggil _ps -aux_ yang hanya berisi nama kolom tidak tersimpan ke dalam file.
  - Simpan ke dalam file menggunakan command _> SimpanProses1.txt_ dan _> SimpanProses2.txt_
  
- Thread untuk zip file menjadi KompresProses1.zip dan menghapus filenya dari folder
  ```c
  void* zip1 (void *arg)
  {
	while(status1 != 1)
    	{

    	}
	chdir("/home/jihan/Documents/FolderProses1");
	system("zip KompresProses1.zip SimpanProses1.txt && rm SimpanProses1.txt");
	status1=2;
  }
  ```
- Thread untuk zip file menjadi KompresProses2.zip dan menghapus filenya dari folder
  ```c
  void* zip2 (void *arg)
  {
	while(status2 != 1)
        {

        }
	chdir("/home/jihan/Documents/FolderProses2");
	system("zip KompresProses2.zip SimpanProses2.txt && rm SimpanProses2.txt");
	status2=2;
  }
  ```
  - Menggunakan mutual exclusion untuk menunggu dari thread "save1" dan "save2", thread ini akan dijalankan apabila sebelumnya file SimpanProses1.txt dan SimpanProses2.txt telah berhasil dibuat. Dengan mengecek apakah variabel **status1** dan **status2** sudah bernilai 1.
  - Lalu terdapat command _zip_ untuk zip file menjadi KompresProses1.zip dan KompresProses2.zip lalu _rm_ "nama file" untuk menghapus file tersebut.
  - Set **status1** dan **status2** menjadi 2.

- Thread untuk unzip KompresProses1.zip
  ```c
  void* unzip1 (void *arg)
  {
        while(status1 != 2)
        {

        }
	sleep(15);
        chdir("/home/jihan/Documents/FolderProses1");
        system("unzip KompresProses1.zip");
  }
  ```

- Thread untuk unzip KompresProses2.zip
  ```c
  void* unzip2 (void *arg)
  {
        while(status2 != 2)
        {

        }
	sleep(15);
        chdir("/home/jihan/Documents/FolderProses2");
        system("unzip KompresProses2.zip");
  }
  ```
  - Thread ini juga menggunakan mutual exclusion, untuk menunggu apakah thread zip sebelumnya berhasil dijalankan atau tidak, dengan mengecek variabel **status1** dan **status2** sudah bernilai 2 atau belum.
  - Menggunakan **sleep(15)** untuk menunggu selama 15 detik sebelum KompresProses1.zip maupun KompresProses2.zip diekstrak.
  - Setelah itu terdapat command _unzip_ untuk mengekstrak dari KompresProses1.zip dan KompresProses2.zip.
  
**_Hasil :_**

Isi dari file SimpanProses1.txt
![file](soal4/file.PNG)

Proses saat zip

![zip](soal4/zip.PNG)

Proses saat unzip

![unzip](soal4/unzip.PNG)
  
## Soal-5
Angga, adik Jiwang akan berulang tahun yang ke sembilan pada tanggal 6 April besok. Karena lupa menabung, Jiwang tidak mempunyai uang sepeserpun untuk membelikan Angga kado. Kamu sebagai sahabat Jiwang ingin membantu Jiwang membahagiakan adiknya sehingga kamu menawarkan bantuan membuatkan permainan komputer sederhana menggunakan program C. Jiwang sangat menyukai idemu tersebut. Berikut permainan yang Jiwang minta.   
- Pemain memelihara seekor monster lucu dalam permainan. Pemain dapat  memberi nama pada monsternya.  
- Monster pemain memiliki hunger status yang berawal dengan nilai 200 (maksimalnya) dan nanti akan berkurang 5 tiap 10 detik.Ketika hunger status mencapai angka nol, pemain akan kalah. Hunger status dapat bertambah 15 apabila pemain memberi makan kepada monster, tetapi banyak makanan terbatas dan harus beli di Market.  
- Monster pemain memiliki hygiene status yang berawal dari 100 dan nanti berkurang 10 tiap 30 detik. Ketika hygiene status mencapai angka nol, pemain akan kalah. Hygiene status' dapat bertambah 30 hanya dengan memandikan monster. Pemain dapat memandikannya setiap 20 detik(cooldownnya 20 detik).  
- Monster pemain memiliki health status yang berawal dengan nilai 300. Variabel ini bertambah (regenerasi)daa 5 setiap 10 detik ketika monster dalam keadaan standby.
Monster pemain dapat memasuki keadaan battle. Dalam keadaan ini, food status(fitur b), hygiene status'(fitur c), dan ‘regenerasi’(fitur d) tidak akan berjalan. Health status dari monster dimulai dari darah saat monster pemain memasuki battle. Monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster pemain maupun NPC memiliki serangan sebesar 20. Monster pemain dengan monster musuh akan menyerang secara bergantian. 
- Fitur shop, pemain dapat membeli makanan sepuas-puasnya selama stok di toko masih tersedia.
    - Pembeli (terintegrasi dengan game)
        - Dapat mengecek stok makanan yang ada di toko.
        - Jika stok ada, pembeli dapat membeli makanan.
    - Penjual (terpisah)
        - Bisa mengecek stok makanan yang ada di toko
        - Penjual dapat menambah stok makanan.  

Spesifikasi program:
1. Program mampu mendeteksi input berupa key press. (Program bisa berjalan tanpa perlu menekan tombol enter)
2. Program terdiri dari 3 scene yaitu standby, battle, dan shop.
3. Pada saat berada di standby scene, program selalu menampilkan health status, hunger status, hygiene status, stok makanan tersisa, dan juga status kamar mandi (“Bath is ready” jika bisa digunakan, “Bath will be ready in [bath cooldown]s” jika sedang cooldown). Selain itu program selalu menampilkan 5 menu, yaitu memberi makan, mandi, battle, shop, dan exit. Contoh :  
Standby Mode  
Health : [health status]  
Hunger : [hunger status]  
Hygiene : [hygiene status]  
Food left : [your food stock]  
Bath will be ready in [cooldown]s  
Choices  
    1. Eat  
    2. Bath  
    3. Battle  
    4. Shop  
    5. Exit  

4. Pada saat berada di battle scene, program selalu menampilkan health status milik pemain dan monster NPC. Selain itu, program selalu menampilkan 2 menu yaitu serang atau lari. Contoh :  
Battle Mode  
Monster’s Health : [health status]  
Enemy’s Health : [enemy health status]  
Choices  
    1. Attack
    2. Run

5. Pada saat berada di shop scene versi pembeli, program selalu menampilkan food stock toko dan milik pemain. Selain itu, program selalu menampilkan 2 menu yaitu beli dan kembali ke standby scene. Contoh :  
Shop Mode  
Shop food stock : [shop food stock]  
Your food stock : [your food stock]  
Choices  
    1. Buy
    2. Back

6. Pada program penjual, program selalu menampilkan food stock toko. Selain itu, program juga menampilkan 2 menu yaitu restock dan exit. Contoh :  
Shop  
Food stock : [shop food stock]  
Choices  
    1. Restock
    2. Exit

7. Pastikan terminal hanya mendisplay status detik ini sesuai scene terkait (hint: menggunakan system(“clear”))

**_Jawaban_**

Terdapat 2 program yaitu  
1. Program game
2. Shop

Untuk program game
```c
typedef struct T_monster{
    char *nama;
    int hunger_status;
    int hygiene_status;
    int health_status;
}Monster;

Monster* Monster_new(char *nama){
    Monster* newMonster = (Monster*)malloc(sizeof(Monster));
    newMonster->nama = nama;
    newMonster->hunger_status = 200;
    newMonster->hygiene_status = 100;
    newMonster->health_status = 300;
}
```
Struktur **Monster** memiliki **nama**, **hunger_status**, **hygiene_status**, **health_status**.  
Fungsi `Monster_new(char *nama)` untuk menginsisialisasi monster dengan return type berupa pointer **Monster**.

Kemudian terdapat fungsi timer berupa **hungerStatusTimer**, .
```c
void *hungerStatusTimer(void *arg){
    int timeNow = 1;
    Monster *player = (Monster *)arg;
    while(1){
        timeNow = timeNow % 10;
        if (timeNow == 0){
            player->hunger_status = player->hunger_status - 5;
            if (player->hunger_status <= 0) alive = 0;
        }
        timeNow++;
        sleep(1);
    }
}

void *hygieneStatusTimer(void *arg){
    int timeNow = 1;
    Monster *player = (Monster *)arg;
    while(1){
        timeNow = timeNow % 30;
        if (timeNow == 0){
            player->hygiene_status = player->hygiene_status - 10;
            if (player->hygiene_status <= 0) alive = 0;
        }
        timeNow++;
        sleep(1);
    }
}

void *healthStatusTimer(void *arg){
    int timeNow = 1;
    Monster *player = (Monster *)arg;
    while(1){
        if (gameMode == 1){
            timeNow = timeNow % 10;
            if (timeNow == 0){
                player->health_status = player->health_status + 5;
            }
            timeNow++;
            sleep(1);
        }else{
            continue;
        }
    }
}
```
Fungsi - fungsi tersebut bertugas sebagai timer untuk mengurangi nilai - nilai dari monster sesuai dengan nama fungsinya sepert **hunger_status**, **hygiene_status**, **health_status**. Fungsi sleep setiap 1 detik untuk mensimulasikan timer 1 detik.   
Untuk fungsi **hungerStatusTimer** dan **healthStatusTimer** di modulus 10 artinya setiap 10 detik melakukan perhitungan sedangkan **hygieneStatusTimer** di modulus 30 artinya setiap 30 detik melakukan perhitungan.

```c
void *cooldown(void *arg){
    int *cd = (int *)arg;
    while(1){
        sleep(1);
        *cd = *cd - 1;
        if (*cd == 0) break;
    }
}
```
Fungsi **cooldown** digunakan untuk menghitung cooldown dari suatu aksi (contohnya Cooldown dari Bath).  
Fungsi tersebut mengambil nilai dari variabel cooldown dan mengambilnya sebagai argumen fungsi tersebut. Fungsi tersebut menggunakan pass by pointer sehingga merubah nilai cooldown pada fungsi tersebut akan merubah nilai asli dari variabel cooldown yang bersangkutan.

Kemudian terdapat fungsi **printMenu** yang digunakan untuk mencetak menu dari game sesuai dengan **gameMode** yang sedang berjalan.  
Pada program game kami variabel **gameMode** memiliki beberapa kemungkinan nilai yaitu:  
- gameMode = 1. Standby Mode
- gameMode = 2. Battle Mode
- gameMode = 3. Shop Mode
```c
void *printMenu(void *arg){
    Monster *player = (Monster*)arg;
    while(1){
        if (gameMode == 1){
            printf("Standby Mode\n");
            printf("Health : %d\n", player->health_status);
            printf("Hunger : %d\n", player->hunger_status);
            printf("Hygiene : %d\n", player->hygiene_status);
            printf("Food left : %d\n", foodStock); //shared memory
            if (bathCooldown == 0){
                printf("Bath is ready\n");
            }else{
                printf("Bath will be ready in %ds\n", bathCooldown);
            }
            printf("Choices\n");
            printf("1. Eat\n");
            printf("2. Bath\n");
            printf("3. Battle\n");
            printf("4. Shop\n");
            printf("5. Exit\n");
        }else if (gameMode == 2){
            printf("Battle Mode\n");
            printf("Monster's Health: %d\n", player->health_status);
            printf("Enemny's Health: %d\n", healthEnemy);
            printf("Choices\n");
            printf("1. Attack\n");
            printf("2. Run\n");
        }else if (gameMode == 3){
            printf("Shop Mode\n");
            printf("Shop food stock: %d\n", *shopStock);
            printf("Your food stock: %d\n", foodStock);
            printf("Choices\n");
            printf("1. Buy\n");
            printf("2. Back\n");
        }
        sleep(1);
        system("clear");
    }    
}
```
Fungsi tersebut akan di `sleep(1)` agar program mencetak menu setiap 1 detik.

Kemudian pada fungsi main.
```c
int main(){
    char nama[100];
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    shopStock = shmat(shmid, NULL, 0);
    printf("Masukkan nama monster: ");
    scanf("%s", nama);
    gameMode = 1;
    Monster *player = Monster_new(nama);
    bathCooldown = 0;
    system("clear");
    pthread_create(&menu, NULL, &printMenu, (void *)player);
    pthread_create(&hunger, NULL, &hungerStatusTimer, (void *)player);
    pthread_create(&health, NULL, &healthStatusTimer, (void *)player);
    pthread_create(&hygiene, NULL, &hygieneStatusTimer, (void *)player);
    healthEnemy = 100;
    alive = 1;
    foodStock = 10;
    *shopStock = 5;
    changemode(1);
    while(1){
        while(!kbhit()){
            if (gameMode == 1){
                char c = getchar();
                if (c == '3'){
                    gameMode = 2;
                }else if (c == '5'){
                    shmdt(shopStock);
                    shmctl(shmid, IPC_RMID, NULL);
                    changemode(0);
                    return 0;
                }else if (c == '1'){
                    if (foodStock > 0){
                        foodStock--;
                        player->hunger_status = player->hunger_status + 15;
                    }
                }else if (c == '2'){
                    player->hygiene_status = player->hygiene_status + 30;
                    bathCooldown = 20;
                    pthread_create(&bath, NULL, &cooldown, (void*)&bathCooldown);
                }else if (c == '4'){
                    gameMode = 3;
                }
            }else if (gameMode == 2){
                char c = getchar();
                if (c == '1'){
                    healthEnemy = healthEnemy - 20;
                    if (healthEnemy <= 0){
                        healthEnemy = 0;
                        gameMode = 1;
                        healthEnemy = 100;
                        continue;
                    }
                    player->health_status = player->health_status - 20;
                    if (player->health_status <= 0){
                        alive = 0;
                    }
                }else if (c == '2'){
                    gameMode = 1;
                    healthEnemy = 100;
                }
            }else if (gameMode == 3){
                char c = getchar();
                if (c == '1'){
                    if (shopStock > 0){
                        foodStock++;
                        *shopStock = *shopStock - 1;
                    }
                }else if (c == '2'){
                    gameMode = 1;
                }
            }
        }
        if (alive == 0){
            printf("Monster mati\n");
            shmdt(shopStock);
            shmctl(shmid, IPC_RMID, NULL);
            changemode(0);
            return 0;
        }
    }
    shmdt(shopStock);
    shmctl(shmid, IPC_RMID, NULL);
    changemode(0);
    return 0;
}
```

Detailnya
```c
key_t key = 1000;

int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
shopStock = shmat(shmid, NULL, 0);
```
Potongan program tersebut digunakan untuk mengambil memori dengan key 1000. Nilai alamat dari memori dengan key tersebut disimpan dalam variabel **shopStock**.

4 fungsi `pthread_create` digunakan untuk
```c
pthread_create(&menu, NULL, &printMenu, (void *)player);
pthread_create(&hunger, NULL, &hungerStatusTimer, (void *)player);
pthread_create(&health, NULL, &healthStatusTimer, (void *)player);
pthread_create(&hygiene, NULL, &hygieneStatusTimer, (void *)player);
```
1. thread **menu** diberikan untuk menjalankan fungsi **printMenu**
2. thread **hunger** diberikan untuk menjalankan fungsi **hungerStatusTimer**
3. thread **health** diberikan untuk menjalankan fungsi **healthStatusTimer**
4. thread **hygiene** diberikan untuk menjalankan fungsi **hygieneStatusTimer**

```c
if (foodStock > 0){
    foodStock--;
    player->hunger_status = player->hunger_status + 15;
}
```
Kemudian potongan code diatas digunakan untuk mensimulasikan command `Eat`.
```c
player->hygiene_status = player->hygiene_status + 30;
bathCooldown = 20;
pthread_create(&bath, NULL, &cooldown, (void*)&bathCooldown);
```
Potongan code diatas digunakan untuk memberikan cooldown pada bathCooldown.
```c
char c = getchar();
if (c == '1'){
    healthEnemy = healthEnemy - 20;
    if (healthEnemy <= 0){
        healthEnemy = 0;
        gameMode = 1;
        healthEnemy = 100;
        continue;
    }
    player->health_status = player->health_status - 20;
    if (player->health_status <= 0){
        alive = 0;
    }
}else if (c == '2'){
    gameMode = 1;
    healthEnemy = 100;
}
```
Potongan code diatas digunakan untuk mensimulasikan battle mode.

```c
char c = getchar();
if (c == '1'){
    if (shopStock > 0){
        foodStock++;
        *shopStock = *shopStock - 1;
    }
}else if (c == '2'){
    gameMode = 1;
}
```
Potongan code diatas digunakan untuk mensimulasikan shop pada player.
```c
if (alive == 0){
    printf("Monster mati\n");
    shmdt(shopStock);
    shmctl(shmid, IPC_RMID, NULL);
    changemode(0);
    return 0;
}
```
Potongan program diatas digunakan untuk memeriksa setiap saat apakah monster masih hidup atau tidak dengan bantuan variabel **alive**.

Untuk program shop
```c
void *printMenu(void *arg){
    int *stock = (int *)arg;
    while(1){
        printf("Shop\n");
        printf("Food stock: %d\n", *stock);
        printf("Choices\n");
        printf("1. Restock\n");
        printf("2. Exit\n");
        sleep(1);
        system("clear");
    }    
}
```
Sama halnya fungsi printMenu pada program game, fungsi printMenu pada program shop digunakan untuk mencetak menu setiap detiknya.
```c
int main(){
    key_t key = 1000;
    int *stock;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stock = shmat(shmid, NULL, 0);
    pthread_create(&menu, NULL, &printMenu, (void *)stock);
    changemode(1);
    while(1){
        while(!kbhit()){
            char c = getchar();
            if (c == '1'){
                *stock = *stock + 1;
            }else if (c == '2'){
                shmdt(stock);
                shmctl(shmid, IPC_RMID, NULL);
                changemode(0);
                return 0;
            }
        }
    }
    shmdt(stock);
    shmctl(shmid, IPC_RMID, NULL);
    changemode(0);
    return 0;
}
```
Kemudian pada fungsi main.
```c
key_t key = 1000;
int *stock;

int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
stock = shmat(shmid, NULL, 0);
```
Pada potongan ini digunakan untuk mengambil memori dengan key 1000 dan disimpan dalam variabel stock.  
`pthread_create(&menu, NULL, &printMenu, (void *)stock)`
Kemudian thread menu diberikan tugas untuk menjalankan fungsi **printMenu**.  

`*stock = *stock + 1;`
baris ini digunakan untuk menambah stock pada memori dengan key 1000.

**_Hasil_**  
![StandyMode](soal5/standy.PNG)  
![BattleMode](soal5/battle.PNG)  
![shop](soal5/shop1.PNG)   
![shopExternal](soal5/shopexternal.PNG)
