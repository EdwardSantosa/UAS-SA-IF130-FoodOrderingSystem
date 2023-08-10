#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RESTAURANTS 10
#define MAX_MENU_ITEMS 50
#define MAX_ORDERS 100

typedef struct {
    char username[50];
    char password[50];
    char restaurantName[50];
} Owner;

typedef struct {
    char username[50];
    char password[50];
    char deliveryAddress[100]; // Tambahkan member untuk alamat pengiriman
} Customer;


typedef struct {
    char itemName[50];
    char description[100];
    int price;
    int availableQuantity;
} MenuItem;

typedef struct {
    char itemName[50];
    int quantity;
} OrderItem;

typedef struct {
    char restaurantName[50];
    OrderItem items[MAX_MENU_ITEMS];
    int itemCount;
    int totalAmount;
    char deliveryAddress[100];
    int isFulfilled;
} Order;

Owner owners[MAX_RESTAURANTS];
int ownerCount = 0;

Customer customers[MAX_ORDERS];
int customerCount = 0;

MenuItem menus[MAX_RESTAURANTS][MAX_MENU_ITEMS];
int menuCount[MAX_RESTAURANTS] = {0};

Order orders[MAX_ORDERS];
int orderCount = 0;

int currentOwnerIndex = -1;
int currentCustomerIndex = -1;

void registerOwner() {
    Owner owner;
    printf("Masukkan nama pengguna: ");
    scanf("%s", owner.username);
    printf("Masukkan kata sandi: ");
    scanf("%s", owner.password);
    printf("Masukkan nama restoran: ");
    scanf("%s", owner.restaurantName);
    owners[ownerCount] = owner;
    ownerCount++;
    printf("Registrasi berhasil!\n");
}

void loginOwner() {
    char username[50];
    char password[50];
    printf("Masukkan nama pengguna: ");
    scanf("%s", username);
    printf("Masukkan kata sandi: ");
    scanf("%s", password);

    for (int i = 0; i < ownerCount; i++) {
        if (strcmp(owners[i].username, username) == 0 && strcmp(owners[i].password, password) == 0) {
            currentOwnerIndex = i;
            printf("Berhasil login sebagai pemilik restoran.\n");
            return;
        }
    }

    printf("Gagal login. Nama pengguna atau kata sandi salah.\n");
}

void logoutOwner() {
    currentOwnerIndex = -1;
    printf("Berhasil logout dari akun pemilik restoran.\n");
}

void registerCustomer() {
    Customer customer;
    printf("Masukkan nama pengguna: ");
    scanf("%s", customer.username);
    printf("Masukkan kata sandi: ");
    scanf("%s", customer.password);
    printf("Masukkan alamat pengiriman: ");
    scanf(" %[^\n]", customer.deliveryAddress); // Menggunakan %[^\n] untuk membaca keseluruhan baris alamat
    customers[customerCount] = customer;
    customerCount++;
    printf("Registrasi berhasil!\n");
}

void loginCustomer() {
    char username[50];
    char password[50];
    printf("Masukkan nama pengguna: ");
    scanf("%s", username);
    printf("Masukkan kata sandi: ");
    scanf("%s", password);

    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].username, username) == 0 && strcmp(customers[i].password, password) == 0) {
            currentCustomerIndex = i;
            printf("Berhasil login sebagai pelanggan.\n");
            return;
        }
    }

    printf("Gagal login. Nama pengguna atau kata sandi salah.\n");
}

void logoutCustomer() {
    currentCustomerIndex = -1;
    printf("Berhasil logout dari akun pelanggan.\n");
}

void addMenuItem() {
    if (currentOwnerIndex == -1) {
        printf("Anda harus login sebagai pemilik restoran untuk menambahkan menu.\n");
        return;
    }

    if (menuCount[currentOwnerIndex] >= MAX_MENU_ITEMS) {
        printf("Menu sudah penuh. Tidak bisa menambahkan lagi.\n");
        return;
    }

    MenuItem item;
    printf("Masukkan nama item: ");
    scanf("%s", item.itemName);
    printf("Masukkan deskripsi item: ");
    scanf("%s", item.description);
    printf("Masukkan harga (Rupiah): ");
    scanf("%d", &item.price);
    printf("Masukkan ketersediaan item: ");
    scanf("%d", &item.availableQuantity);

    menus[currentOwnerIndex][menuCount[currentOwnerIndex]] = item;
    menuCount[currentOwnerIndex]++;

    printf("Menu berhasil ditambahkan.\n");
}

void updateMenuItem() {
    if (currentOwnerIndex == -1) {
        printf("Anda harus login sebagai pemilik restoran untuk memperbarui menu.\n");
        return;
    }

    printf("Daftar Menu:\n");
    for (int i = 0; i < menuCount[currentOwnerIndex]; i++) {
        printf("%d. %s\n", i + 1, menus[currentOwnerIndex][i].itemName);
    }

    int choice;
    printf("Pilih nomor menu yang ingin diperbarui: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > menuCount[currentOwnerIndex]) {
        printf("Nomor menu tidak valid.\n");
        return;
    }

    MenuItem *item = &menus[currentOwnerIndex][choice - 1];

    printf("Masukkan deskripsi item baru: ");
    scanf("%s", item->description);
    printf("Masukkan harga baru (Rupiah): ");
    scanf("%d", &item->price);
    printf("Masukkan ketersediaan item baru: ");
    scanf("%d", &item->availableQuantity);

    printf("Menu berhasil diperbarui.\n");
}

void displayMenu() {
    if (currentCustomerIndex == -1) {
        printf("Anda harus login sebagai pelanggan untuk melihat menu.\n");
        return;
    }

    int restaurantChoice;
    printf("Pilih restoran:\n");
    for (int i = 0; i < ownerCount; i++) {
        printf("%d. %s\n", i + 1, owners[i].restaurantName);
    }
    printf("Pilih nomor restoran: ");
    scanf("%d", &restaurantChoice);

    if (restaurantChoice < 1 || restaurantChoice > ownerCount) {
        printf("Nomor restoran tidak valid.\n");
        return;
    }

    int ownerIndex = restaurantChoice - 1;

    printf("Menu %s:\n", owners[ownerIndex].restaurantName);
    for (int i = 0; i < menuCount[ownerIndex]; i++) {
        printf("%d. %s - %s - Harga: %d Rupiah - Tersedia: %d\n", i + 1,
               menus[ownerIndex][i].itemName, menus[ownerIndex][i].description,
               menus[ownerIndex][i].price, menus[ownerIndex][i].availableQuantity);
    }

    int orderChoice;
    printf("Pilih nomor menu yang ingin dipesan: ");
    scanf("%d", &orderChoice);

    if (orderChoice < 1 || orderChoice > menuCount[ownerIndex]) {
        printf("Nomor menu tidak valid.\n");
        return;
    }

    OrderItem orderItem;
    strcpy(orderItem.itemName, menus[ownerIndex][orderChoice - 1].itemName);
    printf("Masukkan jumlah yang ingin dipesan: ");
    scanf("%d", &orderItem.quantity);

    int totalAmount = menus[ownerIndex][orderChoice - 1].price * orderItem.quantity;
    strcpy(orders[orderCount].restaurantName, owners[ownerIndex].restaurantName);

    orders[orderCount].items[0] = orderItem;
    orders[orderCount].itemCount = 1;
    orders[orderCount].totalAmount = totalAmount;

    if (currentCustomerIndex != -1) {
    strcpy(orders[orderCount].deliveryAddress, customers[currentCustomerIndex].deliveryAddress);
}
    orderCount++;

    printf("Pesanan berhasil ditambahkan ke keranjang.\n");
}

void processOrders() {
    if (currentOwnerIndex == -1) {
        printf("Anda harus login sebagai pemilik restoran untuk memproses pesanan.\n");
        return;
    }

    printf("Pesanan Masuk:\n");
    for (int i = 0; i < orderCount; i++) {
        if (strcmp(orders[i].restaurantName, owners[currentOwnerIndex].restaurantName) == 0 && !orders[i].isFulfilled) {
            printf("Pesanan %d - Total: %d Rupiah - Tujuan: %s\n", i + 1, orders[i].totalAmount, orders[i].deliveryAddress);
        }
    }

    int orderChoice;
    printf("Pilih nomor pesanan yang ingin diproses: ");
    scanf("%d", &orderChoice);

    if (orderChoice < 1 || orderChoice > orderCount) {
        printf("Nomor pesanan tidak valid.\n");
        return;
    }

    orders[orderChoice - 1].isFulfilled = 1;

    printf("Pesanan telah diproses dan siap untuk diantar.\n");
}

void addOrder() {
    if (currentCustomerIndex == -1) {
        printf("Anda harus login sebagai pelanggan untuk menambah pesanan.\n");
        return;
    }

    int restaurantChoice;
    printf("Pilih restoran:\n");
    for (int i = 0; i < ownerCount; i++) {
        printf("%d. %s\n", i + 1, owners[i].restaurantName);
    }
    printf("Pilih nomor restoran: ");
    scanf("%d", &restaurantChoice);

    if (restaurantChoice < 1 || restaurantChoice > ownerCount) {
        printf("Nomor restoran tidak valid.\n");
        return;
    }

    int ownerIndex = restaurantChoice - 1;

    printf("Menu %s:\n", owners[ownerIndex].restaurantName);
    for (int i = 0; i < menuCount[ownerIndex]; i++) {
        printf("%d. %s - %s - Harga: %d Rupiah - Tersedia: %d\n", i + 1,
               menus[ownerIndex][i].itemName, menus[ownerIndex][i].description,
               menus[ownerIndex][i].price, menus[ownerIndex][i].availableQuantity);
    }

    int orderChoice;
    printf("Pilih nomor menu yang ingin dipesan: ");
    scanf("%d", &orderChoice);

    if (orderChoice < 1 || orderChoice > menuCount[ownerIndex]) {
        printf("Nomor menu tidak valid.\n");
        return;
    }

    OrderItem orderItem;
    strcpy(orderItem.itemName, menus[ownerIndex][orderChoice - 1].itemName);
    printf("Masukkan jumlah yang ingin dipesan: ");
    scanf("%d", &orderItem.quantity);

     int totalAmount = menus[ownerIndex][orderChoice - 1].price * orderItem.quantity;

    printf("\nPesanan Anda:\n");
    printf("Item: %s\n", orderItem.itemName);
    printf("Jumlah: %d\n", orderItem.quantity);
    printf("Total Harga: %d Rupiah\n", totalAmount); // Tampilkan total harga

    strcpy(orders[orderCount].restaurantName, owners[ownerIndex].restaurantName);
    orders[orderCount].items[0] = orderItem;
    orders[orderCount].itemCount = 1;
    orders[orderCount].totalAmount = totalAmount;

    if (currentCustomerIndex != -1) {
        strcpy(orders[orderCount].deliveryAddress, customers[currentCustomerIndex].deliveryAddress);
    }

    orderCount++;

    printf("Pesanan berhasil ditambahkan ke keranjang.\n");
}

int main() {
    int choice;

    do {
        printf("\n====== Food Ordering System ======\n");
        printf("1. Registrasi Pemilik Restoran\n");
        printf("2. Login Pemilik Restoran\n");
        printf("3. Registrasi Pelanggan\n");
        printf("4. Login Pelanggan\n");
        printf("5. Logout\n");
        printf("6. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerOwner();
                break;
            case 2:
                loginOwner();
                break;
            case 3:
                registerCustomer();
                break;
            case 4:
                loginCustomer();
                break;
            case 5:
                if (currentOwnerIndex != -1) {
                    logoutOwner();
                }
                if (currentCustomerIndex != -1) {
                    logoutCustomer();
                }
                break;
            case 6:
                printf("Terima kasih telah menggunakan Food Ordering System.\n");
                return 0;
            default:
                printf("Pilihan tidak valid.\n");
        }

        if (currentOwnerIndex != -1) {
            int ownerChoice;

            do {
                printf("\n====== Menu Pemilik Restoran ======\n");
                printf("1. Tambah Menu\n");
                printf("2. Perbarui Menu\n");
                printf("3. Proses Pesanan\n");
                printf("4. Logout\n");
                printf("Pilih menu: ");
                scanf("%d", &ownerChoice);

                switch (ownerChoice) {
                    case 1:
                        addMenuItem();
                        break;
                    case 2:
                        updateMenuItem();
                        break;
                    case 3:
                        processOrders();
                        break;
                    case 4:
                        logoutOwner();
                        break;
                    default:
                        printf("Pilihan tidak valid.\n");
                }
            } while (ownerChoice != 4);
        }

        if (currentCustomerIndex != -1) {
            int customerChoice;

            do {
                printf("\n====== Menu Pelanggan ======\n");
                printf("1. Lihat Menu\n");
                printf("2. Tambah Pesanan\n");
                printf("3. Logout\n");
                printf("Pilih menu: ");
                scanf("%d", &customerChoice);

                switch (customerChoice) {
                    case 1:
                        displayMenu();
                        break;
                    case 2:
                        addOrder();
                        break;
                    case 3:
                        logoutCustomer();
                        break;
                    default:
                        printf("Pilihan tidak valid.\n");
                }
            } while (customerChoice != 3);
        }

    } while (choice != 6);

    return 0;
}
