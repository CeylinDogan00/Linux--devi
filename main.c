#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GUNLUK_DOSYA "/var/log/syslog" // Gunluk dosyasinin yolu, ihtiyac halinde degistirilebilir
#define MAKS_SATIR_UZUNLUK 1024 // Maksimum satir uzunlugu

// Bagli liste dugumu yapisi
typedef struct GunlukKayit {
    char gunluk[MAKS_SATIR_UZUNLUK]; // Gunluk kaydini saklayan karakter dizisi
    struct GunlukKayit *sonraki; // Sonraki dugumu isaret eden gosterici
} GunlukKayit;

// Yeni bir dugum olusturma fonksiyonu
GunlukKayit *dugumOlustur(const char *gunlukSatir) {
    GunlukKayit *yeniDugum = (GunlukKayit *)malloc(sizeof(GunlukKayit)); // Yeni dugum icin bellek tahsisi
    if (yeniDugum == NULL) {
        perror("Bellek tahsisi basarisiz"); // Bellek tahsisi basarisiz olursa hata mesaji
        exit(EXIT_FAILURE);
    }
    strncpy(yeniDugum->gunluk, gunlukSatir, MAKS_SATIR_UZUNLUK); // Gunluk satirini dugume kopyala
    yeniDugum->gunluk[MAKS_SATIR_UZUNLUK - 1] = '\0'; // Tasmayi onlemek icin son karakteri null olarak ayarla
    yeniDugum->sonraki = NULL; // Yeni dugumun sonraki isaretcisini NULL yap
    return yeniDugum;
}

// Bagli listeye yeni bir dugum ekleme fonksiyonu
void gunlukEkle(GunlukKayit **bas, const char *gunlukSatir) {
    GunlukKayit *yeniDugum = dugumOlustur(gunlukSatir); // Yeni bir dugum olustur
    if (*bas == NULL) { // Eger liste bossa yeni dugumu basa ekle
        *bas = yeniDugum;
    } else {
        GunlukKayit *gecici = *bas;
        while (gecici->sonraki != NULL) { // Listenin sonuna kadar ilerle
            gecici = gecici->sonraki;
        }
        gecici->sonraki = yeniDugum; // Yeni dugumu listenin sonuna ekle
    }
}

// Bagli listedeki gunluk kayitlarini ekrana yazdirma fonksiyonu
void gunlukleriYazdir(GunlukKayit *bas) {
    while (bas != NULL) { // Listenin sonuna kadar ilerle
        printf("%s", bas->gunluk); // Gunluk kaydini ekrana yazdir
        bas = bas->sonraki; // Sonraki dugume gec
    }
}

// Bagli listeyi serbest birakma fonksiyonu (Bellek temizligi)
void gunlukleriSerbestBirak(GunlukKayit *bas) {
    GunlukKayit *gecici;
    while (bas != NULL) {
        gecici = bas;
        bas = bas->sonraki;
        free(gecici); // Bellegi serbest birak
    }
}

int main() {
    FILE *dosya = fopen(GUNLUK_DOSYA, "r"); // Gunluk dosyasini okuma modunda ac
    if (dosya == NULL) {
        perror("Gunluk dosyasi acilamadi"); // Dosya acilamazsa hata mesaji
        return EXIT_FAILURE;
    }

    GunlukKayit *gunlukListesi = NULL; // Bagli liste basini NULL olarak baslat
    char tampon[MAKS_SATIR_UZUNLUK]; // Gunluk satirlarini saklamak icin tampon bellek

    while (fgets(tampon, sizeof(tampon), dosya)) { // Dosyayi satir satir oku
        gunlukEkle(&gunlukListesi, tampon); // Okunan satiri bagli listeye ekle
    }
    fclose(dosya); // Dosyayi kapat

    printf("Sistem Gunlukleri:\n");
    gunlukleriYazdir(gunlukListesi); // Bagli listedeki gunlukleri ekrana yazdir

    gunlukleriSerbestBirak(gunlukListesi); // Bellegi temizle
    return EXIT_SUCCESS; // Program basariyla sonlandi
}

