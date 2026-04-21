#include <iostream>
#include <iomanip>
using namespace std;

struct Buku {
	string ISBN;
	string judul;
	string penulis;
	int tahun;
};

// variabel untuk login
string username, password;
int kesempatan;
string usernameTrue = "admin";
string passwordTrue = "admin123";
bool isLogin = false;
char konfirmMenu;

char optionMenu;

//fungsi tampilan menu awal
void tampilanMenuAwal(){	
	cout << "=== SISTEM MANAJEMEN PERPUSTAKAAN === " << endl;
	cout << "[1] Lihat Daftar Buku" << endl;
	cout << "[2] Cari Buku" << endl;
	cout << "[3] Tambah Buku" << endl;
	cout << "[4] Edit Buku" << endl;
	cout << "[5] Hapus Buku" << endl;
	cout << "[6] Transaksi" << endl;
	cout << "[0] Keluar Program" << endl;
	cout << ">> "; cin >> optionMenu;
	system("cls");
}

int main(){
    cout << "== LOGIN =="s << endl;
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
				menuLihatDaftarBuku();
				break;
            case '2' :
				menuCariBuku();
				break;
            case '3' : //pilihan menu tambah buku
				menuTambahBuku();
				break;
			case '4' :
				menuEditBuku();
				break;
            case '5' :
				menuHapusBuku();
				break;
            case '6' :
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