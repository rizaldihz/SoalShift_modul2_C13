# SoalShift_modul2_C13
1. 17-0024 Mohammad Rizaldi Huzein Prastomo
2. 17-0122 Muhammad Husni Ridhart Azzikry

# Soal1
Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar.
Catatan : Tidak boleh menggunakan crontab.
#
## Jawab
Karena kita perlu mengecek sebuah file berekstensi **.png** ketika file tersebut dicopy ke sebuah folder, maka kita perlu membuat daemon untuk mengecek setiap waktu apakah terdapat sebuah file berekstensi **.png** di forlder tersebut. Demi kemudahan pengecekan, maka kami berasumsi bahwa file **.png** yang di cek hanya pada satu folder saja, maka kami membuat sebuah folder sementara untuk menampung gambar yang akan kami cek pada folder `/home/[user]/sisop/dummy-modul2`, berikut source code nya
```c
int checkpng(char* string)
{
  return !strcmp(string+(strlen(string)-4),".png") ? 1 : 0;
}

void checkandrename()
{   
    struct stat st = {0};
    if (stat("modul2", &st) == -1) {
        mkdir("modul2", 0777);
        mkdir("modul2/gambar", 0777);
    }

    char namafile[100];
    memset(namafile,'\0',sizeof(namafile));
    DIR* dir;
    struct dirent* rent;
    
    dir = opendir("sisop/dummy-modul2");
    if(dir==NULL){
        perror("Error: ");
        return;
    }
    else{
        // rent = readdir(dir);
        while((rent=readdir(dir))!=NULL){
            // rent=readdir(dir);
            printf("%s\n",rent->d_name);
            if(strlen(rent->d_name)<4){continue;}
            strcpy(namafile,rent->d_name);
            if(checkpng(namafile)){
                // printf("%s\n",namafile);
                char* temp = malloc((strlen(namafile))+10);
                char* from = malloc((strlen(namafile))+50);
                char* dest = malloc((strlen(namafile))+50);
                
                strcpy(from,"sisop/dummy-modul2/"); strcat(from,namafile);
                strncpy(temp,namafile,strlen(namafile)-4);
                strcpy(dest,"modul2/gambar/"); strcat(temp,"_grey.png");
                strcat(dest,temp);
                printf("%s\n%s\n",from,dest);
                if(rename(from,dest)<0) perror("Gagal ");
                memset(namafile,'\0',sizeof(namafile));
                memset(temp,'\0',sizeof(temp));
                memset(from,'\0',sizeof(from));
                memset(dest,'\0',sizeof(dest));
            }
        }
        closedir(dir);
    }
}

int main()
{
    /*
    daemon proc
    */
    while(1) {
      checkandrename();
      sleep(2);
    }
}
```
Pada main, terdapat prosedur daemon biasa seperti membuat child, setsid, close descriptor dan lain-lain. Daemon tersebut perlu memindahkan file **.png** ke direktori `/home/[user]/modul2/gambar/`, sehingga prosedur yang harus dilakukan oleh daemon tersebut yakni,
+ Membuat folder `/home/[user]/modul2/gambar/` apa bila belum terdapat file tersebut dengan `stat()` dan `mkdir()`, dimana `stat()` akan memberi nilai < 0 ketika tidak ditemukan file tersebut
```c
struct stat st = {0};
    if (stat("modul2", &st) == -1) {
        mkdir("modul2", 0777);
        mkdir("modul2/gambar", 0777);
    }
```
+ Mengecek isi directory tersebut dengan `readdir()`, yang akan kita akses menggunakan `struct dirent` yang menyimpan nama dari file tersebut.
+ Mengecek apakah extension merupakan `.png` dengan fungsi buatan yang mengcompare 4 string terakhir dengan `.png`
```c
int checkpng(char* string)
{
  return !strcmp(string+(strlen(string)-4),".png") ? 1 : 0;
}
void checkandrename()
{
  //...
  if(checkpng(namafile)){
  //...
}
```
+ Ketika merupakan `.png` maka rename file tersebut dengan tambahan `_grey.png` dibelakangnya
```c
strcpy(from,"sisop/dummy-modul2/"); strcat(from,namafile);
strncpy(temp,namafile,strlen(namafile)-4);
strcpy(dest,"modul2/gambar/"); strcat(temp,"_grey.png");
strcat(dest,temp);
if(rename(from,dest)<0) perror("Gagal ");
```

#
<br>

# Soal2
Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on.
Catatan: Tidak boleh menggunakan crontab
#
## Jawab
Karena kita perlu mengecek setiap 3 detik sekali maka secara otomatis kita perlu menggunakan daemon. Daemon tersebut akan bertugas untuk mencari file `elen.ku` pada direktori `hatiku`, lalu mencari tahu apakah `elen.ku` merupakan kepunyaan `www-data` baik user maupun grup. Apabila iya maka akan dihapus.
```c
void checkanddelete()
{   
    

    char namafile[100];
    DIR* dir;
    struct dirent* rent;
    chdir("/home/duhbuntu/sisop/hatiku/");
    dir = opendir(".");
    if(dir==NULL){
        perror("Error: ");
        return;
    }
    else{
        while((rent=readdir(dir))!=NULL){
            strcpy(namafile,rent->d_name);
            // printf("%s\n",namafile);
            if(strcmp(namafile,"elen.ku")==0){
                struct stat st = {0};
                if (stat(namafile, &st)<0) {
                    perror("Gagal ");
                    continue;
                }
                struct passwd *name = getpwuid(st.st_uid);
                struct group *grup = getgrgid(st.st_gid);
                // printf("%s %s\n",name->pw_name,grup->gr_name);
                char mode[]="0777";
                if((strcmp(name->pw_name,"www-data"))==0 && (strcmp(grup->gr_name,"www-data"))==0){
                    if(chmod(namafile,S_IRWXU|S_IRWXG|S_IRWXO)<0){
                        perror("Gagal ");
                    }
                    remove(namafile);
                }
            }
        }if((strcmp(name->pw_name,"www-data"))==0 && (strcmp(grup->gr_name,"www-data"))==0){
                    if(chmod(namafile,S_IRWXU|S_IRWXG|S_IRWXO)<0){
                        perror("Gagal ");
                    }
                    remove(namafile);
                }
        closedir(dir);
    }
}
int main()
{
    /*
    daemon proc
    */
    while(1) {
      checkanddelete();
      sleep(3);
    }
}
```
+ Pertama mengiterasi directory `hatiku` dengan `readdir()`
+ Kedua adalah meng-compare apakah nama dari file tersebut yang ada pada `struct dirent [d_name]` adalah `elen.ku`
```c
if(strcmp(namafile,"elen.ku")==0)
```
+ Ketiga adalah mengecek UID dan GID dari file `elen.ku` apakah milik `www-data` atau bukan dengan menggunakan `struct stat`, dan fungsi `stat()` untuk mengambil informasi `stat` dari file `elenku` dan mengambil UID dan GID dari variable `st_uid` dan `st_gid`
```c
struct stat st = {0};
if (stat(namafile, &st)<0) {
    perror("Gagal ");
    continue;
}
```
+ Selanjutnya adalah mengambil string dari UID dan GUID, menggunakan bantuan `struct passwd` untuk UID dan `struct group` untuk GID, lalu memanggil fungsi `getpwuid()` dan disimpan pada `struct passwd` untuk mendapatkan struct yang berisi nama dari UID tersebut, dan fungsi `getgrgid()` dan disimpan pada `struct group` untuk mendapatkan struct yang berisi nama dari GID tersebut
```c
struct passwd *name = getpwuid(st.st_uid);
struct group *grup = getgrgid(st.st_gid);
// printf("%s %s\n",name->pw_name,grup->gr_name);
```
+ Selanjutnya adalah mengcompare apakah `pw_name` dan `gr_name` sama dengan `www-data` dengan `strcmp()`, lalu kita `chmod()` file tersebut berdasarkan permission `0777`, yang berarti USER,GROUP,OTHER memiliki permission read, write, dan execute. 
`chmod(namafile,S_IRWXU|S_IRWXG|S_IRWXO)` berarti merubah permission dari file tersebut berdasarkan operasi or biner dari `S_IRWXU` yang berarti User Read-write-execute, `S_IRWXG` yang berarti Group Read-write-execute, dan `S_IRWXO` yang berarti Other Read-write-execute.
```c
if((strcmp(name->pw_name,"www-data"))==0 && (strcmp(grup->gr_name,"www-data"))==0){
    if(chmod(namafile,S_IRWXU|S_IRWXG|S_IRWXO)<0){
        perror("Gagal ");
    }
    remove(namafile);
}
```
+ Ketika selesai maka `remove()` file `elen.ku`
#
<br>

# Soal3
Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. 
Buatlah program C yang dapat :
1.  mengekstrak file zip tersebut.
2. menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt. 
<br>Catatan:  
+ Gunakan fork dan exec.
+ Gunakan minimal 3 proses yang diakhiri dengan exec.
+ Gunakan pipe
+ Pastikan file daftar.txt dapat diakses dari text editor

#
## Jawab
Pada nomor ini kita perlu mengekstrak `campur2.zip` terlebih dahulu, lalu kita harus menyimpan daftar file `*.txt` yang ada pada direktori `campur`. Karena kita diwajibkan menggunakan minimal 3 exec, maka kami memilih `unzip`, `ls`, dan `grep` sebagai exec dengan 2 pipe sebagai buffer deskriptor STDIN dan STDOUT.
```c
int main()
{
    chdir("/home/duhbuntu/sisop/");
    pid_t child1;
    pid_t child2;
    pid_t child3;
    int p1;
    int ag[4];
    char temp[100000];

    child1 = fork();

    if(child1 == 0){
        struct stat st = {0};
        if(stat("campur2",&st)!=0) execlp("unzip","unzip","campur2.zip", NULL);
        exit(1);
    }else{
        int p2;
        while(wait(&p1)>0);

        pipe(ag);
        pipe(ag + 2);
       
        child2 = fork();
        if(child2==0){
            close(ag[2]);
            close(ag[3]);
            close(ag[0]);
            dup2(ag[1],STDOUT_FILENO);
            close(ag[1]);
            execlp("ls","ls","campur2",NULL);
        }else{
            wait(NULL);
            child3 = fork();
            if(child3==0){
                close(ag[2]);
                dup2(ag[3],STDOUT_FILENO);
                close(ag[3]);
                
                close(ag[1]);
                dup2(ag[0],STDIN_FILENO);
                close(ag[0]);
                
                execlp("grep","grep",".txt$",NULL);
            }else{
                close(ag[3]);
                close(ag[0]);
                close(ag[1]);
                int byte = read(ag[2],temp,sizeof(temp));
                FILE* out;
                out = fopen("data.txt","w+");
                fputs(temp,out);
                fclose(out);
                close(ag[2]);
            }
            
        }
    }
}
```
Karena kita perlu menggunakan 3 exec, maka kita perlu membuat 3 child, oleh karena itu,
+ Membuat 1 proses child untuk `unzip`, dimana apabila belum terdapat folder `campur2` maka kita bisa mengunzipnya. Cek ada tidaknya menggunakan `stat()`
```c
child1 = fork();

if(child1 == 0){
    struct stat st = {0};
    if(stat("campur2",&st)!=0) execlp("unzip","unzip","campur2.zip", NULL);
    exit(1);
}
```
+ Lalu proses ke 2 adalah untuk `ls` daftar yang ada pada direktori `campur2`, lalu membuat `pipe` untuk buffer STDOUT dari `exec ls`  tersebut. Pipe sendiri terdiri atas 2 array `p[0]` sebagai read buffer, dan `p[1]` sebagai write buffer. Panggil `pipe(array of int)` untuk membuka pipe tersebut, dan ketika eksekusi, tutup seluruh deskriptor yang tidak dibutuhkan, pada kasus ini, kita hanya perlu menerima output, sehingga deskriptor yang harus diterima adalah `ag[1]` sebagai penerima *write output to buffer*, dengan `dup2(ag[1],STDOUT_FILENO)` yang berarti `ag[1]` dalam state WRITE dan `STDOUT_FILENO / 1` yang berarti file deskriptor untuk menulis hasil `syscall`.
```c
while(wait(&p1)>0);

pipe(ag);
pipe(ag + 2);

child2 = fork();
if(child2==0){
    close(ag[2]);
    close(ag[3]);
    close(ag[0]);
    dup2(ag[1],STDOUT_FILENO);
    close(ag[1]);
    execlp("ls","ls","campur2",NULL);
}
```
+ `ag[0],ag[1]` untuk buffer `ls` dan `ag[2],ag[3]` untuk buffer `grep`, sehingga langkah selanjutnya adalah membuat child proses lagi untuk melakukan `grep`. Sehingga karena kita akan menggunakan buffer dari ls `ag[0/1]`, dan karena kita sekarang dalam posisi read buffer, maka selain `ag[0]` akan kita tutup, lalu kita copy `ag[0]` menjadi file deskriptor input / STDIN_FILENO dengan `dup2(ag[0],STDIN_FILENO)`, sehingga file deskriptor input akan menjadi buffer / hasil `ls` tadi. Dan juga kita akan menyimpan hasil `grep` ke buffer lain yakni dengan `dup2(ag[3],STDOUT_FILENO)`, sehingga deskriptor STDOUT akan diambil oleh `ag[3]`
```c
wait(NULL);
child3 = fork();
if(child3==0){
  close(ag[2]);
  dup2(ag[3],STDOUT_FILENO);
  close(ag[3]);

  close(ag[1]);
  dup2(ag[0],STDIN_FILENO);
  close(ag[0]);

  execlp("grep","grep",".txt$",NULL);
}
```
+ Lalu dari buffer grep tersebut kita bisa membaca isinya dengan `ag[2]` sebagai buffer read kita dengan fungsi `read()` dimana ia akan membaca isi file deskriptor `ag[2]` dan menaruhnya di array of char hingga akhir buffer. Lalu kita bisa menaruh hasilnya di `data.txt` dengan membuat file `data.txt` bermode `w+` dan menaruh hasill array of char tadi ke file `data.txt` dengan `fputs()`.
```c
int byte = read(ag[2],temp,sizeof(temp));
FILE* out;
out = fopen("data.txt","w+");
fputs(temp,out);
fclose(out);
```
#
<br>

# Soal4
Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik).
Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.

Contoh: <br>
* File makan_enak.txt terakhir dibuka pada detik ke-1
* Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt

Catatan: <br>
* dilarang menggunakan crontab
* Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst
#
## Jawab
asd
#
<br>

# Soal5
Kerjakan poin a dan b di bawah: <br>
Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log <br>
Ket:
* Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
* Per menit memasukkan log#.log ke dalam folder tersebut
* ‘#’ : increment per menit. Mulai dari 1
* Buatlah program c untuk menghentikan program di atas.
<br>

NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.
#
## Jawab
asdasd
#
