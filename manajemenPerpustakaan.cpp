#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cstring>
#include <ctime>

using namespace std;

struct Buku {
	char ISBN[20], judul[100], penulis[100];
	int tahun, stok, status; // status untuk menyimpan jumlah yang dipinjam
	Buku *next; // pengait untuk setiap buku
};

struct Riwayat {
    char waktu[25], aksi[15], judul[100], nama[100];
    int jumlah;
    Riwayat *next;
};

// variabel untuk login
string username, password;
int kesempatan;
string usernameTrue = "admin";
string passwordTrue = "admin123";
bool isLogin = false;

char konfirmMenu, optionMenu;

Buku *head = NULL ;

//transaksi
int jumlahPinjam = 0; 
int jumlahKembali = 0;

// prentilan untuk membuat tabel biar rapi dan estetik di ambil dari kode/nilai extended ASCII characters
char tl = (char)218;  // ┌
char h = (char)196;   // ─
char tr = (char)191;  // ┐
char v = (char)179;   // │
char bl = (char)192;  // └
char br = (char)217;  // ┘
char pl = (char)195;  // ├
char pr = (char)180;  // ┤

// fungsi simpan FILE
void simpanFile(){
	FILE *file = fopen("perpustakaan.txt", "w");

	if (file == NULL){
		cout << "Gagal membuka file!\n";
		return;
	}

	Buku *temp = head;

	while (temp != NULL){
		fprintf(file, "%s|%s|%s|%d|%d|%d\n", // ubah pemisahnya jadi | aja, ada judul buku yg make koma soalnya
			temp->ISBN,
                temp->judul,
                temp->penulis,
                temp->tahun,
                temp->stok,
                temp->status);

		temp = temp->next;
	}

	fclose(file);
}

// fungsi load
void loadFile(){
	FILE *file = fopen("perpustakaan.txt", "r");

	if (file == NULL) return;
	Buku *tail = NULL; //buat append n jaga urutan asli
	Buku *bukuBaru = new Buku;

	while (fscanf(file, " %[^|]|%[^|]|%[^|]|%d|%d|%d\n",
                  bukuBaru->ISBN,
                  bukuBaru->judul,
                  bukuBaru->penulis,
                  &bukuBaru->tahun,
                  &bukuBaru->stok,
                  &bukuBaru->status) == 6)
	{
		bukuBaru->next = NULL;
		if (head == NULL) {
			head = bukuBaru;
            tail = bukuBaru;
        } else {
            tail->next = bukuBaru;
            tail = bukuBaru;
        }

        bukuBaru = new Buku;
	}

	delete bukuBaru; // hapus node terakhir yang tidak terpakai
	fclose(file);
}

// lanjut ke menu
void lanjutMenu(){
	cout << "\nTekan ENTER untuk lanjut...";
	cin.get();
}

// cek apakah ISBN sudah terdaftar buat validasi duplikat
bool isbnSudahAda(const char *isbn) {
    Buku *bantu = head;
    while (bantu != NULL) {
        if (strcmp(bantu->ISBN, isbn) == 0) return true;
        bantu = bantu->next;
    }
    return false;
}

// fungsi buat jadiin string jadi lowercase, jadi nnt pencarian judulnya bisa pake case-insensitive
void toLowerStr(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}

//fungsi tambah buku
int menuTambahBuku(Buku *&head){
	Buku *bukuBaru = new Buku; //alokasi memori untuk buku baru

    //FILE *file = fopen("perpustakaan.txt", "a");
    //Buku bukuBaru; //deklarasi untuk menambah buku baru
 
    //user input buku baru
    cout << right;
	cout << tl << setfill(h) << setw(70) << "" << tr << endl;
	cout << v << setfill(' ') << setw(40) << "TAMBAH BUKU" << setw(30) << "" << v << endl;
	cout << bl << setfill(h) << setw(70) << "" << br << endl;
    cout << "Masukkan ISBN: "; cin.getline(bukuBaru->ISBN, 20);

	if (isbnSudahAda(bukuBaru->ISBN)){
		cout << "[!] ISBN sudah terdaftar! Buku tidak dapat ditambahkan.\n";
        delete bukuBaru;
        return 0;
	}

    cout << "Masukkan Judul: "; cin.getline(bukuBaru->judul, 100);
    cout << "Masukkan Penulis: "; cin.getline(bukuBaru->penulis, 100);
    cout << "Masukkan Tahun: "; cin >> bukuBaru->tahun;
    cout << "Masukkan Stok: "; cin >> bukuBaru->stok;
    bukuBaru->status = 0; //status buku baru belum dipinjam
	bukuBaru->next = NULL; //inisialisasi pengait buku baru

	// error handling stok negatif atau 0
    if (bukuBaru->stok < 1) {
        cout << "[!] Stok buku harus >= 1! Buku tidak dapat ditambahkan.\n";
        delete bukuBaru;
		lanjutMenu();
        // return 0;
    }

	if (head == NULL) {
		head = bukuBaru;
	} else {
		Buku *bantu = head;
		while (bantu->next != NULL) {
			bantu = bantu->next;
		}
		bantu->next = bukuBaru;
	}

    //memasukkan data buku baru ke dalam file
    //fprintf(file, "%s;%s;%s;%d\n", bukuBaru->ISBN, bukuBaru->judul, bukuBaru->penulis, bukuBaru->tahun);

    //fclose(file);
    cout << "Buku berhasil ditambahkan!" << endl;
    simpanFile();
	cin.ignore();
	lanjutMenu();
    // return 1;
}

//fungsi edit buku
int menuEditBuku(Buku *head){
	if (head == NULL) {
		cout << "Buku kosong. Tidak ada buku yang dapat diedit." << endl;
		lanjutMenu();
		// return 0;	
	}

	char target[100];
	cout << right;
	cout << tl << setfill(h) << setw(70) << "" << tr << endl;
	cout << v << setfill(' ') << setw(40) << "EDIT BUKU" << setw(30) << "" << v << endl;
	cout << bl << setfill(h) << setw(70) << "" << br << endl;

	cout << "Masukkan ISBN / judul buku: "; cin.getline(target, 100);
	char targetLow[100];
	strcpy(targetLow, target);
	toLowerStr(targetLow);

	Buku *bantu = head;

	while (bantu != NULL) {
		char judulLow[100];
		strcpy(judulLow, bantu->judul);
		toLowerStr(judulLow);
		if (strcmp(bantu->ISBN, target) == 0 || strcmp(judulLow, targetLow) == 0) {
			cout << "Buku ditemukan. Pilih data yang akan diubah:" << endl;
			cout << "[1] Judul" << endl;
			cout << "[2] Penulis" << endl;
			cout << "[3] Tahun" << endl;
			cout << "[4] Stok" << endl;
			cout << ">> "; 
			int pilihan; cin >> pilihan;
			cin.ignore();

			switch (pilihan) {
				case 1:
					cout << "Masukkan Judul baru: "; cin.ignore(); cin.getline(bantu->judul, 100);
					break;
				case 2:
					cout << "Masukkan Penulis baru: "; cin.ignore(); cin.getline(bantu->penulis, 100);
					break;
				case 3:
					cout << "Masukkan Tahun baru: "; cin >> bantu->tahun;
					break;
				case 4:
					cout << "Masukkan Stok baru: "; cin >> bantu->stok;
					break;
				default:
					cout << "Pilihan tidak valid." << endl;
					return 0;
			}

			// konfirmasi unutk simpan perubahan
			cout << endl << "Yakin ingin menyimpan perubahan? (y/n): ";
			char konfirm; cin >> konfirm;
			if (konfirm != 'y' && konfirm != 'Y'){
				cout << "Perubahan dibatalkan!" << endl;
				lanjutMenu();
				// return 0;
			}

			cout << "Buku berhasil diedit!" << endl;
			simpanFile();
			lanjutMenu();
			// return 1; // jika berhasil edit 
		}
		bantu = bantu->next;
	}

	cout << "Buku tidak ditemukan." << endl;
	lanjutMenu();
	// return 0; //gagal nemu buku
}

//fungsi hapus buku
int menuHapusBuku(Buku *&head){
	if (head == NULL) {
		cout << "Buku kosong. Tidak ada buku yang dapat dihapus." << endl;
		lanjutMenu();
		// return 0;	
	}

	char target[100], targetLow[100];
	cout << "\n=== HAPUS BUKU ===" << endl;
	cout << "Masukkan ISBN / judul buku: "; cin.getline(target, 100);
	strcpy(targetLow, target);
	toLowerStr(targetLow);

	Buku *bantu = head;
	Buku *prev = NULL;

	while (bantu != NULL) {
		char judulLow[100];
		strcpy(judulLow, bantu->judul);
		toLowerStr(judulLow);
		if (strcmp(bantu->ISBN, target) == 0 || strcmp(judulLow, targetLow) == 0) {
			// konfirmasi hapus
			cout << "Apakah Anda yakin akan menghapus buku \"" << bantu->judul << "\"? (y/n): ";
            char konfirm; cin >> konfirm;
            if (konfirm != 'y' && konfirm != 'Y') {
                cout << "Hapus buku dibatalkan.\n";
				lanjutMenu();
                // return 0;
            }
			
			if (prev == NULL) { // jika buku yang dihapus adalah head
				head = bantu->next;
			} else {
				prev->next = bantu->next;
			}

			delete bantu; // menghapus buku dari memori
			bantu = NULL; 
			cout << "Buku berhasil dihapus!" << endl;
			simpanFile();
			lanjutMenu();
			// return 1; // jika berhasil hapus
		}
		prev = bantu;
		bantu = bantu->next;
	}

	cout << "Buku tidak ditemukan." << endl;
	lanjutMenu();
	// return 0; //gagal nemu buku
}

//fungsi tampilan menu awal
void tampilanMenuAwal(){	
	cout << "\n=== SISTEM MANAJEMEN PERPUSTAKAAN === " << endl;
	cout << "[1] Manajemen Buku" << endl;
	cout << "[2] Lihat Daftar Buku" << endl;
	cout << "[3] Cari Buku" << endl;
	cout << "[4] Sorting Buku" << endl;
	cout << "[5] Transaksi" << endl;
	cout << "[6] Riwayat Transaksi" << endl;
	cout << "[0] Keluar Program" << endl;
	cout << ">> "; cin >> optionMenu;
	cin.ignore();
	system("cls");
}

// fungsi untuk menu manajemen buku
void menuManajemenBuku(){
	char pilih;
	do {
		cout << "=== MANAJEMEN BUKU ===" << endl;
		cout << "[1] Tambah Buku" << endl;
		cout << "[2] Edit Buku" << endl;
		cout << "[3] Hapus Buku" << endl;
		cout << "[0] Kembali" << endl;
		cout << ">> "; cin >> pilih;
		cin.ignore();
		system("cls");

		switch (pilih) {
			case '1':
				menuTambahBuku(head);
				break;
			case '2':
				menuEditBuku(head);
				break;
			case '3':
				menuHapusBuku(head);
				break;
			case '0':
				cout << "Kembali ke menu utama..." << endl << endl;
				break;
			default : //pilihan menu tidak valid
				cout << "Masukkan menu dengan BENAR!" << endl << endl;
				break;
		}
	} while (pilih != '0');
}

// fungsi Lihat Daftar Buku
void menuLihatDaftarBuku(Buku *head){
	if (head == NULL){
		cout << "Data kosong" << endl;
		return;
	}

	Buku *bantu = head;

	cout << right;
	cout << tl << setfill(h) << setw(96) << "" << tr << endl;
	cout << v << setfill(' ') << setw(54) << "DAFTAR BUKU" << setw(42) << "" << v << endl;
	cout << bl << setfill(h) << setw(96) << "" << br << endl;
	cout << tl << setfill(h) << setw(96) << "" << tr << endl;
	cout << setfill(' ');
	cout << left 
		<< setw(2) << v
		<< setw(15) << "ISBN"
		<< setw(25) << "Judul"
		<< setw(28) << "Penulis"
		<< setw(6) << "Tahun"
		// << setw(6) << "Stok"
		// << setw(12) << "Status" << endl;
		<< setw(12) << "Tersedia"
		<< setw(9) << "Dipinjam" 
		<< v << endl;
	cout << pl << setfill(h) << setw(96) << "" << pr << endl;
	cout << setfill(' ');

	int totalJudul = 0;
	int totalStok = 0;
	while (bantu != NULL){
	
		cout << left 
			<< setw(2) << v
			<< setw(15) << bantu->ISBN
			<< setw(25) << bantu->judul
			<< setw(28) << bantu->penulis
			<< setw(6) << bantu->tahun
			// << setw(6) << bantu->stok
			// << setw(12) << "Tersedia: " << bantu->stok << " |Dipinjam: " << bantu->status <<endl; //status
			<< setw(12) << bantu->stok
			<< setw(9) << bantu->status 
			<< v << endl; 
		
		totalJudul++;
		totalStok += bantu->stok;

		bantu = bantu->next;
	}

	cout << bl << setfill(h) << setw(96) << "" << br << endl;
	cout << setfill(' ');
    cout << "Total Judul: " << totalJudul << " judul buku.\n";
    cout << "Total Stok Tersedia: " << totalStok << " buku.\n";

	lanjutMenu();
}

// searching buku (sequential search)
void menuCariBuku(){
	if (head == NULL){
        cout << "Data kosong.\n";
        return;
    }
 
    char target[100];
    cout << "\n=== CARI BUKU ===" << endl;
    cout << "Masukkan ISBN / Judul buku: ";
    cin.getline(target, 100);
 
	char targetLow[100];
    strcpy(targetLow, target);
    toLowerStr(targetLow);

	cout <<	endl;

    Buku *bantu = head;
    bool ditemukan = false;
 
    while (bantu != NULL){
		char judulLow[100];
        strcpy(judulLow, bantu->judul);
        toLowerStr(judulLow);

        if (strcmp(bantu->ISBN, target) == 0 || strstr(judulLow, targetLow) != NULL){
            cout << "=== Detail Buku ===" << endl;
            cout << "ISBN     : " << bantu->ISBN    << endl;
            cout << "Judul    : " << bantu->judul   << endl;
            cout << "Penulis  : " << bantu->penulis << endl;
            cout << "Tahun    : " << bantu->tahun   << endl;
            // cout << "Stok    : " << bantu->stok    << endl;
            cout << "Tersedia : " << bantu->stok    << endl;
            cout << "Dipinjam : " << bantu->status    << endl << endl;
            // cout << "Status  : " << "Tersedia: " << bantu->stok << " | Dipinjam: " << bantu->status << endl << endl;
            ditemukan = true;
            break;
        }
        bantu = bantu->next;
    }
 
    if (!ditemukan) cout << "Buku tidak ditemukan.\n";

	lanjutMenu();
}

void swapBuku(Buku *a, Buku *b){
    char tempStr[100];
    int tempInt;
 
    strcpy(tempStr,	a->ISBN);	strcpy(a->ISBN,	b->ISBN);      strcpy(b->ISBN, tempStr);
    strcpy(tempStr, a->judul);	strcpy(a->judul, b->judul);    strcpy(b->judul, tempStr);
    strcpy(tempStr, a->penulis);strcpy(a->penulis, b->penulis);strcpy(b->penulis, tempStr);
 
    tempInt = a->tahun;	a->tahun  = b->tahun;  b->tahun  = tempInt;
    tempInt = a->stok;	a->stok   = b->stok;   b->stok   = tempInt;
    tempInt = a->status;a->status = b->status; b->status = tempInt;
}

// sorting by ISBN
void sortISBN(){
	if (head == NULL){ cout << "Data kosong.\n"; return; }
    bool swapped;
    do {
        swapped = false;
        Buku *curr = head;
        while (curr->next != NULL){
            if (strcmp(curr->ISBN, curr->next->ISBN) > 0){
                swapBuku(curr, curr->next);
                swapped = true;
            }
            curr = curr->next;
        }
    } while (swapped);

    simpanFile();
    cout << "\nBuku berhasil diurutkan berdasarkan ISBN (A-Z)!\n\n";
	menuLihatDaftarBuku(head);
}

// sorting by Judul
void sortJudul(){
    if (head == NULL){ cout << "Data kosong.\n"; return; }
    bool swapped;
    do {
        swapped = false;
        Buku *curr = head;
        while (curr->next != NULL){
            if (strcmp(curr->judul, curr->next->judul) > 0){
                swapBuku(curr, curr->next);
                swapped = true;
            }
            curr = curr->next;
        }
    } while (swapped);

    simpanFile();
	cout << "\nBuku berhasil diurutkan berdasarkan Judul (A-Z)!\n\n";
	menuLihatDaftarBuku(head);
}

// sorting by Stok
void sortStok(){
    if (head == NULL){ cout << "Data kosong.\n"; return; }
    bool swapped;
    do {
        swapped = false;
        Buku *curr = head;
        while (curr->next != NULL){
            if (curr->stok < curr->next->stok){ // descending
                swapBuku(curr, curr->next);
                swapped = true;
            }
            curr = curr->next;
        }
    } while (swapped);
    simpanFile();
    cout << "\nBuku berhasil diurutkan berdasarkan Stok (Terbesar-Terkecil)!\n\n";
	menuLihatDaftarBuku(head);
}

// sorting by Tahun
void sortTahun() {
	if (head == NULL) { cout << "Data kosong.\n"; return; }
    bool swapped;
    do {
        swapped = false;
        Buku *curr = head;
        while (curr->next != NULL) {
            if (curr->tahun > curr->next->tahun) {
                swapBuku(curr, curr->next); swapped = true;
            }
            curr = curr->next;
        }
    } while (swapped);
    simpanFile();
    cout << "\nBuku berhasil diurutkan berdasarkan Tahun (Terlama-Terbaru).\n\n";
	menuLihatDaftarBuku(head);
}

// fungsi sorting buku 
void menuSortingBuku(){
	char pilih;
	do { 
		cout << "=== SORTING BUKU ===" << endl << endl;
		cout << "Mau sorting berdasarkan apa?" << endl;
		cout << "[1] ISBN (A-Z)" << endl;
		cout << "[2] Judul (A-Z)" << endl;
		cout << "[3] Stok Terbesar" << endl;
		cout << "[4] Tahun Terlama" << endl;
		cout << "[0] Kembali" << endl;
		cout << ">> "; cin >> pilih;
		cin.ignore();
		system("cls");

		switch (pilih){
		case '1':
			sortISBN();
			break;
		case '2':
			sortJudul();
			break;
		case '3':
			sortStok();
			break;
		case '4':
			sortTahun();
			break;
		case '0':
			cout << "Kembali ke menu utama..." << endl << endl;
			break;
		default : //pilihan menu tidak valid
			cout << "Masukkan menu dengan BENAR!" << endl << endl;
			break;
		}
	} while(pilih != '0');
}

// fungsi pencatatan riwayat
void catatRiwayat(const char *judul, const char *aksi, int jumlah, const char *nama) {
	FILE *file = fopen("riwayat.txt", "a");
	if (file == NULL) return;
 
	// ambil waktu pas melakukan transaksi
	time_t now = time(0);
	struct tm *t = localtime(&now);
	char waktu[25];
	strftime(waktu, sizeof(waktu), "%d/%m/%Y %H:%M:%S", t);
 
	fprintf(file, "%s|%s|%s|%d|%s\n", waktu, aksi, judul, jumlah, nama);
	fclose(file);
}

//fungsi transaksi (pinjam & kembali)
void menuTransaksi(){
	if (head == NULL){
		cout << "Data buku kosong.\n";
		return;
	}

	char target[100];
	char nama[100];
	cout << "\n=== TRANSAKSI BUKU ===" << endl;
	cout << "Masukkan ISBN / Judul buku: ";
	cin.getline(target, 100);

	char targetLow[100];
    strcpy(targetLow, target);
    toLowerStr(targetLow);

	Buku *bantu = head;

	while (bantu != NULL){
		char judulLow[100];
        strcpy(judulLow, bantu->judul);
        toLowerStr(judulLow);

		if (strcmp(bantu->ISBN, target) == 0 || strcmp(judulLow, targetLow) == 0){
			cout << "Buku ditemukan. Pilih jenis transaksi:" << endl;
			cout << "[1] Peminjaman Buku" << endl;
			cout << "[2] Pengembalian Buku" << endl;
			cout << "[0] Kembali" << endl;
			cout << ">> "; 
			char pilihan; 
			cin >> pilihan;
			cin.ignore();


			switch (pilihan){
				//pinjam
				case '1': {
					cout << "Berapa buku yang dipinjam(" << bantu->stok << ")? "; cin >> jumlahPinjam;
					cout << "Nama Peminjam : "; cin >> nama;
					cin.ignore();
					if (jumlahPinjam <= 0){
						cout << "Maaf, jumlah buku yang dipinjam minimal 1." << endl;
						lanjutMenu();
						// return;
					} else if (bantu->stok >= jumlahPinjam){
						bantu->stok -= jumlahPinjam; // kurangi stok sesuai jumlah yang dipinjam
						bantu->status += jumlahPinjam; // tambah status dipinjam sesuai jumlah yang dipinjam
						simpanFile();
						catatRiwayat(bantu->judul, "DIPINJAM", jumlahPinjam, nama);
						cout << "Anda berhasil meminjam buku " << bantu->judul << endl;
						cout << "Sisa stok buku " << bantu->judul << " sekarang: " << bantu->stok << endl;
						lanjutMenu();
					} else {
						cout << "Maaf, jumlah buku yang diminta melebihi stok yang tersedia." << endl;
						lanjutMenu();
						// return;
					}
					break;
				}
				//kembali 
				case '2':{
					cout << "Berapa buku yang dikembalikan(" << bantu->status << ")? "; cin >> jumlahKembali;
					cout << "Nama Pengembali : "; cin >> nama;
					if (bantu->status == 0) {
						cout << "Anda tidak meminjam buku ini. Tidak ada yang perlu dikembalikan.\n";
						lanjutMenu();
						// return;
					} else if (jumlahKembali <= 0) {
						cout << "Maaf, jumlah buku yang dikembalikan minimal 1." << endl;
						lanjutMenu();
						// return;
					} else if (jumlahKembali > bantu->status) {
						cout << "Gagal mengembalikan, karena jumlah yang dikembalikan lebih banyak dari yang dipinjam.\n";
						lanjutMenu();
						// return;
					}

					bantu->stok += jumlahKembali; // tambah stok sesuai jumlah yang dikembalikan
					bantu->status -= jumlahKembali; // kurangi status dipinjam sesuai jumlah yang dikembalikan
					simpanFile();
					catatRiwayat(bantu->judul, "DIKEMBALIKAN", jumlahKembali, nama);
					cout << "Buku " << bantu->judul << " berhasil dikembalikan!" << endl;
					if (jumlahKembali < jumlahPinjam) {
						cout << "Anda masih meminjam " << (jumlahPinjam - jumlahKembali) << " buku " << bantu->judul << ".\n";
						lanjutMenu();
					}else {
						cout << "Semua buku " << bantu->judul << " sudah dikembalikan. Stok buku sekarang: " << bantu->stok << endl;
						lanjutMenu();
					}
					break;
				}
				case '0':
					cout << "Kembali ke menu utama..." << endl << endl;
					break;
				default:
					cout << "Pilihan tidak valid." << endl;
					break;
				
			}
			return; // jika berhasil transaksi
		}
		bantu = bantu->next;
	}

	cout << "Buku tidak ditemukan." << endl;
	lanjutMenu();
}

void menuLihatRiwayatTransaksi() {
	FILE *file = fopen("riwayat.txt", "r");
	if (file == NULL) {
		cout << "Belum ada riwayat transaksi.\n";
		return;
	}

	cout << right;
	cout << tl << setfill(h) << setw(96) << "" << tr << endl;
	cout << v << setfill(' ') << setw(57) << "RIWAYAT TRANSAKSI" << setw(39) << "" << v << endl;
	cout << bl << setfill(h) << setw(96) << "" << br << endl;

	cout << tl << setfill(h) << setw(96) << "" << tr << endl;
	cout << setfill(' ');

	cout << left
		 << setw(2) << v
		 << setw(22) << "Waktu"
		 << setw(15) << "Status"
		 << setw(25) << "Judul"
		 << setw(10) << "Jumlah"
		 << setw(23) << "Nama" 
		 << v << endl;
	cout << pl << setfill(h) << setw(96) << "" << pr << endl;
	cout << setfill(' ');

	char line[400];
	int total = 0;
	int totalPinjam = 0;
	int totalKembali = 0;

	while (fgets(line, sizeof(line), file)) {

		// buat hapus enter
		int len = strlen(line);
		if (len > 0 && line[len-1] == '\n') {
			line[len-1] = '\0';
		}

		// parsing
		char waktu[25] = "", aksi[15] = "", judul[100] = "", jml[10] = "", nama[100] = "";
		char tmp[400];
		strcpy(tmp, line);

		char *tok = strtok(tmp, "|");
		if (tok) {
			strcpy(waktu, tok);
		}

		tok = strtok(NULL, "|");
		if (tok) {
			strcpy(aksi, tok);
		}

		tok = strtok(NULL, "|");
		if (tok) {
			strcpy(judul, tok);
		}

		tok = strtok(NULL, "|");
		if (tok) {
			strcpy(jml, tok);
		}

		tok = strtok(NULL, "|");
		if (tok) {
			strcpy(nama, tok);
		}

		cout << left
			 << setw(2) << v
			 << setw(22) << waktu
			 << setw(15) << aksi
			 << setw(25) << judul
			 << setw(10) << jml
			 << setw(23) << nama 
			 << setw(1) << v << endl;

		total++;

		if (strcmp(aksi, "DIPINJAM") == 0) {
			totalPinjam++;
		} else if (strcmp(aksi, "DIKEMBALIKAN") == 0) {
			totalKembali++;
		}
	}

	cout << bl << setfill(h) << setw(96) << "" << br << endl;
	cout << setfill(' ');

	cout << "Total Transaksi : " << total << endl;
	cout << "Total Pinjam    : " << totalPinjam << endl;
	cout << "Total Kembali   : " << totalKembali << endl;

	fclose(file);
	lanjutMenu();
}

int main(){
	loadFile();
	
	cout << tl << setfill(h) << setw(96) << "" << tr << endl;
	cout << v << setfill(' ') << setw(54) << "LOGIN" << setw(42) << "" << v << endl;
	cout << bl << setfill(h) << setw(96) << "" << br << endl;
    kesempatan = 3;

    // FORM LOGIN
	while (kesempatan > 0) {
		cout << "Masukkan username: "; cin >> username;
		cout << "Masukkan password: "; cin >> password;
        cout << endl;
		//system("cls");
	
		// cek username dan password
		if (username == usernameTrue && password == passwordTrue){
			cout << "Login Berhasil" << endl << endl; 
			isLogin = true;
			break;
		} else if (username != usernameTrue && password == passwordTrue){
			kesempatan--;
			cout << "Username salah." << endl;
		} else if (username == usernameTrue && password != passwordTrue){
			kesempatan--;
			cout << "Password salah." << endl;
		} else {
			kesempatan--;
			cout << "Username dan password salah." << endl;
		}

        // Jika kesempatan habis dan login gagal
		if (kesempatan > 0){
			cout << "Kesempatan tersisa " << kesempatan << " kali lagi." << endl << endl;
		} else {
			cout << "Kesempatan habis. Program keluar." << endl << endl;
		}
	}
	
    if (isLogin == true) {
        do {
		//menampilkan menu awal
		tampilanMenuAwal();

		switch (optionMenu) {
			case '0' : //pilihan menu keluar dari program
				cout << "Program Selesai";
				return 0;
			case '1' :
				menuManajemenBuku();
				break;
            case '2' :
				menuLihatDaftarBuku(head);
				break;
            case '3' : 
				menuCariBuku();
				break;
			case '4' :
				menuSortingBuku();
				break;
            case '5' :
				menuTransaksi();
				break;
            case '6' :
				menuLihatRiwayatTransaksi();
				break;
			default : //pilihan menu tidak valid
				cout << "Masukkan menu dengan BENAR!" << endl << endl;
				break;
		    } 
	    } while (optionMenu != '0');
    } else {
		cout << "Anda berhasil Logout!";
	}
}