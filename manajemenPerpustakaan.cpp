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
				menuHapusBuku();
				break;
			default : //pilihan menu tidak valid
				cout << "Masukkan menu dengan BENAR!" << endl << endl;
				break;
		}
	} while (pilih != '0');
}

// fungsi Lihat Daftar Buku
void menuLihatDaftarBuku(){
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

//fungsi tambah buku
int menuTambahBuku(){
	Buku *bukuBaru = new Buku; //alokasi memori untuk buku baru

    //FILE *file = fopen("perpustakaan.txt", "a");
    //Buku bukuBaru; //deklarasi untuk menambah buku baru
 
    //user input buku baru
    cout << " === TAMBAH BUKU === " << endl;
    cout << "Masukkan ISBN: "; cin.getline(bukuBaru->ISBN, 20);
    cout << "Masukkan Judul: "; cin.getline(bukuBaru->judul, 100);
    cout << "Masukkan Penulis: "; cin.getline(bukuBaru->penulis, 100);
    cout << "Masukkan Tahun: "; cin >> bukuBaru->tahun;
    cout << "Masukkan Stok: "; cin >> bukuBaru->stok;
    bukuBaru->status = 1; //status buku baru selalu tersedia
	bukuBaru->next = NULL; //inisialisasi pengait buku baru

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
    return 1;
}

//fungsi edit buku
int menuEditBuku(){
	if (head == NULL) {
		cout << "Buku kosong. Tidak ada buku yang dapat diedit." << endl;
		return 0;	
	}

	char target[20];
	cout << "\n=== EDIT BUKU ===" << endl;
	cout << "Masukkan ISBN / judul buku yang ingin diedit: "; cin >> target;

	Buku *bantu =head;
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
			return 1; // jika berhasil edit 
		}
		bantu = bantu->next;
	}

	cout << "Buku dengan ISBN tersebut tidak ditemukan." << endl;
	return 0; //gagal nemu buku
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

// fungsi simpan FILE
void simpanFile(){
	FILE *file = fopen("perpustakaan.txt", "w");

	if (file == NULL){
		cout << "Gagal membuka file!\n";
		return;
	}

	Buku *temp = head;

	while (temp != NULL){
		fprintf(file, "%s\n", temp->ISBN);
		fprintf(file, "%s\n", temp->judul);
		fprintf(file, "%s\n", temp->penulis);
		fprintf(file, "%d\n", temp->tahun);
		fprintf(file, "%d\n", temp->stok);

		temp = temp->next;
	}

	fclose(file);
}

// fungsi load
void loadFile(){
	FILE *file = fopen("perpustakaan.txt", "r");

	if (file == NULL) return;

	while (!feof(file)) {
		Buku *bukuBaru = new Buku;

		if (fscanf(file, "%[^\n]\n", bukuBaru->ISBN) != 1) {
			break;
		} else {
			fscanf(file, "%[^\n]\n", bukuBaru->judul);
			fscanf(file, "%[^\n]\n", bukuBaru->penulis);
			fscanf(file, "%d\n", &bukuBaru->tahun);
			fscanf(file, "%d\n", &bukuBaru->stok);
		}

		bukuBaru->next = head;
		head = bukuBaru;
	}

	fclose(file);
}

int main(){
	
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
				menuLihatDaftarBuku();
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