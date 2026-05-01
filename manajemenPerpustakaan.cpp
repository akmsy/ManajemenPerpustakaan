#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cstring>
using namespace std;

struct Buku {
	char ISBN[20];
	char judul[100];
	char penulis[100];
	int tahun;
	int stok;
	int status; // 1 = tersedia, 0 = dipinjam
	Buku *next; // pengait untuk setiap buku
};

// variabel untuk login
string username, password;
int kesempatan;
string usernameTrue = "admin";
string passwordTrue = "admin123";
bool isLogin = false;

char konfirmMenu;
char optionMenu;

Buku *head = NULL ;

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

// cek apakah ISBN sudah terdaftar buat validasi duplikat
bool isbnSudahAda(const char *isbn) {
    Buku *bantu = head;
    while (bantu != NULL) {
        if (strcmp(bantu->ISBN, isbn) == 0) return true;
        bantu = bantu->next;
    }
    return false;
}

//fungsi tambah buku
int menuTambahBuku(Buku *&head){
	Buku *bukuBaru = new Buku; //alokasi memori untuk buku baru

    //FILE *file = fopen("perpustakaan.txt", "a");
    //Buku bukuBaru; //deklarasi untuk menambah buku baru
 
    //user input buku baru
    cout << " === TAMBAH BUKU === " << endl;
    cout << "Masukkan ISBN: "; cin.ignore(); cin.getline(bukuBaru->ISBN, 20);

	if (isbnSudahAda(bukuBaru->ISBN)){
		cout << "[!] ISBN sudah terdaftar! Buku tidak dapat ditambahkan.\n";
        delete bukuBaru;
        return 0;
	}

    cout << "Masukkan Judul: "; cin.getline(bukuBaru->judul, 100);
    cout << "Masukkan Penulis: "; cin.getline(bukuBaru->penulis, 100);
    cout << "Masukkan Tahun: "; cin >> bukuBaru->tahun;
    cout << "Masukkan Stok: "; cin >> bukuBaru->stok;
    bukuBaru->status = 1; //status buku baru selalu tersedia
	bukuBaru->next = NULL; //inisialisasi pengait buku baru

	// error handling stok negatif atau 0
    if (bukuBaru->stok < 1) {
        cout << "[!] Stok buku harus >= 1! Buku tidak dapat ditambahkan.\n";
        delete bukuBaru;
        return 0;
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
    return 1;
}

//fungsi edit buku
int menuEditBuku(Buku *head){
	if (head == NULL) {
		cout << "Buku kosong. Tidak ada buku yang dapat diedit." << endl;
		return 0;	
	}

	char target[100];
	cout << "\n=== EDIT BUKU ===" << endl;
	cin.ignore();
	cout << "Masukkan ISBN / judul buku: "; cin.getline(target, 100);

	Buku *bantu = head;

	while (bantu != NULL) {
		if (strcmp(bantu->ISBN, target) == 0 || strcmp(bantu->judul, target) == 0) {
			cout << "Buku ditemukan. Pilih data yang akan diubah:" << endl;
			cout << "[1] Judul" << endl;
			cout << "[2] Penulis" << endl;
			cout << "[3] Tahun" << endl;
			cout << "[4] Stok" << endl;
			cout << ">> "; 
			int pilihan; cin >> pilihan;

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

			cout << "Buku berhasil diedit!" << endl;
			simpanFile();
			return 1; // jika berhasil edit 
		}
		bantu = bantu->next;
	}

	cout << "Buku tidak ditemukan." << endl;
	return 0; //gagal nemu buku
}

//fungsi hapus buku
int menuHapusBuku(Buku *&head){
	if (head == NULL) {
		cout << "Buku kosong. Tidak ada buku yang dapat dihapus." << endl;
		return 0;	
	}

	char target[100];
	cout << "\n=== HAPUS BUKU ===" << endl;
	cin.ignore();
	cout << "Masukkan ISBN / judul buku: "; cin.getline(target, 100);

	Buku *bantu = head;
	Buku *prev = NULL;

	while (bantu != NULL) {
		if (strcmp(bantu->ISBN, target) == 0 || strcmp(bantu->judul, target) == 0) {
			// konfirmasi hapus
			cout << "Apakah Anda yalin akan menghapus buku \"" << bantu->judul << "\"? (y/n): ";
            char konfirm; cin >> konfirm;
            if (konfirm != 'y' && konfirm != 'Y') {
                cout << "Hapus buku dibatalkan.\n";
                return 0;
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
			return 1; // jika berhasil hapus
		}
		prev = bantu;
		bantu = bantu->next;
	}

	cout << "Buku tidak ditemukan." << endl;
	return 0; //gagal nemu buku
}

//fungsi tampilan menu awal
void tampilanMenuAwal(){	
	cout << "=== SISTEM MANAJEMEN PERPUSTAKAAN === " << endl;
	cout << "[1] Manajemen Buku" << endl;
	cout << "[2] Lihat Daftar Buku" << endl;
	cout << "[3] Cari Buku" << endl;
	cout << "[4] Sorting Buku" << endl;
	cout << "[5] Transaksi" << endl;
	cout << "[0] Keluar Program" << endl;
	cout << ">> "; cin >> optionMenu;
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

	cout << left << setw(15) << "ISBN"
		 << setw(25) << "Judul"
		 << setw(20) << "Penulis"
		 << setw(6) << "Tahun"
		 << setw(6) << "Stok"
		 << setw(10) << "Status" << endl;

	while (bantu != NULL){
		cout << left << setw(15) << bantu->ISBN
			<< setw(25) << bantu->judul
			<< setw(20) << bantu->penulis
			<< setw(6) << bantu->tahun
			<< setw(6) << bantu->stok
			<< setw(10) << bantu->status << endl;

		bantu = bantu->next;
	}
}

// searching buku (sequential search)
void menuCariBuku(){
	if (head == NULL){
        cout << "Data kosong.\n";
        return;
    }
 
    char target[100];
    cout << "\n=== CARI BUKU ===" << endl;
    cin.ignore();
    cout << "Masukkan ISBN / Judul buku: ";
    cin.getline(target, 100);
 
	cout <<	endl;

    Buku *bantu = head;
    bool ditemukan = false;
 
    while (bantu != NULL){
        if (strcmp(bantu->ISBN, target) == 0 || strcmp(bantu->judul, target) == 0){
            cout << "=== Detail Buku ===" << endl;
            cout << "ISBN    : " << bantu->ISBN    << endl;
            cout << "Judul   : " << bantu->judul   << endl;
            cout << "Penulis : " << bantu->penulis << endl;
            cout << "Tahun   : " << bantu->tahun   << endl;
            cout << "Stok    : " << bantu->stok    << endl;
            cout << "Status  : " << (bantu->status == 1 ? "Tersedia" : "Dipinjam") << endl;
            ditemukan = true;
            break;
        }
        bantu = bantu->next;
    }
 
    if (!ditemukan) cout << "Buku tidak ditemukan.\n";
}

void swapBuku(Buku *a, Buku *b){
    char tempStr[100];
    int tempInt;
 
    strcpy(tempStr,	a->ISBN);	strcpy(a->ISBN,	b->ISBN);   strcpy(b->ISBN, tempStr);
    strcpy(tempStr, a->judul);	strcpy(a->judul, b->judul);  strcpy(b->judul, tempStr);
    strcpy(tempStr, a->penulis);strcpy(a->penulis, b->penulis);strcpy(b->penulis, tempStr);
 
    tempInt = a->tahun;	a->tahun  = b->tahun;  b->tahun  = tempInt;
    tempInt = a->stok;	a->stok   = b->stok;   b->stok   = tempInt;
    tempInt = a->status;	a->status = b->status; b->status = tempInt;
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
    cout << "Buku berhasil diurutkan berdasarkan ISBN (A-Z)!\n";
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
	cout << "Buku berhasil diurutkan berdasarkan Judul (A-Z)!\n";
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
    cout << "Buku berhasil diurutkan berdasarkan Stok (Terbesar)!\n";
}

// fungsi sorting buku 
void menuSortingBuku(){
	char pilih;
	do { 
		cout << "=== SORTING BUKU ===" << endl << endl;
		cout << "Mau sorting berdasarkan apa?" << endl;
		cout << "[1] ISBN" << endl;
		cout << "[2] Judul A-Z" << endl;
		cout << "[3] Stok Terbesar" << endl;
		cout << "[0] Kembali" << endl;
		cout << ">>"; cin >> pilih;

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
		default : //pilihan menu tidak valid
			cout << "Masukkan menu dengan BENAR!" << endl << endl;
			break;
		}
	} while(pilih != '0');
}

//fungsi transaksi (pinjam & kembali)
void menuTransaksi(){
	if (head == NULL){
		cout << "Data buku kosong.\n";
		return;
	}

	char target[100];
	cout << "\n=== TRANSAKSI BUKU ===" << endl;
	cin.ignore();
	cout << "Masukkan ISBN / Judul buku: ";
	cin.getline(target, 100);

	Buku *bantu = head;

	while (bantu != NULL){
		if (strcmp(bantu->ISBN, target) == 0 || strcmp(bantu->judul, target) == 0){
			cout << "Buku ditemukan. Pilih jenis transaksi:" << endl;
			cout << "[1] Peminjaman Buku" << endl;
			cout << "[2] Pengembalian Buku" << endl;
			cout << ">> "; 
			char pilihan; 
			cin >> pilihan;

			switch (pilihan){
				//pinjam
				case '1':
					if (bantu->status == 1 && bantu->stok > 0){
						bantu->stok--;
						if (bantu->stok == 0) 
							bantu->status = 0; // jika stok habis, status jadi dipinjam
							simpanFile();
							cout << "Buku berhasil dipinjam!" << endl;
					} else {
						cout << "Maaf, buku sedang tidak tersedia untuk dipinjam." << endl;
					}
					break;
				//kembali
				case '2':
					bantu->stok++;
					bantu->status = 1; //mengemblikan status menjadi tersedia
					simpanFile();
					cout << "Buku berhasil dikembalikan!" << endl;
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
}

int main(){
	loadFile();
	
    cout << "== LOGIN ==" << endl;
	cout << endl;
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
			default : //pilihan menu tidak valid
				cout << "Masukkan menu dengan BENAR!" << endl << endl;
				break;
		    } 
	    } while (optionMenu != '0');
    } else {
		cout << "Anda berhasil Logout!";
	}
}