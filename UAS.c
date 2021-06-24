#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <omp.h>

//Inisialisasi linked list
struct data {
  char nama[20];
  int umur;
  char alamat[30];
  int urutan;
  struct data * next;
};

//prototype dari function encoding dan decoding
void encoding(int k,struct data* head);
int decoding();
void mainmenu();
int Node(struct data** head, int umur, char name[20], char alamat1[30], int m);
//Function mengubah angka ke dalam ASCII
int intToAscii(int number) {
  return '0' + number;
}

int main() {
  system("COLOR F0");
  int select, k, i, umur, m;
  char alamat1[30];
  char name[30];
  struct data * temp2;
  struct data* head = NULL;
  mainmenu();
  while (1) {
    scanf("%d", & select);
    switch (select) {
    case 1:
      system("CLS");
      printf("======================================\n");
      printf("Enter Number of People: ");
      scanf("%d", & k);
      system("CLS");
      for (i = 0; i < k; i++) {
        printf("==========Data For Person %d===========\n", i + 1);
        printf("Name %d: ", i + 1);
        scanf(" %[^\n]s", name);
        printf("Age %d: ", i + 1);
        scanf("%d", & umur);
        printf("Address %d: ", i + 1);
        scanf(" %[^\n]s", alamat1);

        Node(&head, umur, name, alamat1, m); //Fungsi node
        m++; //input berapa kali 
      }
      system("CLS");
      printf("==============Encyption===============\n");
      encoding(k, head);
      system("PAUSE");
      mainmenu();
      break;

    case 2:
      system("CLS");
      decoding();
      printf("\n");
      system("PAUSE");
      mainmenu();
      break;

    case 3:
      exit(0);
      break;

    default:
      printf("Invalid Input!\n");
      printf("Enter Correct Input: ");
    }
  }
  return 0;
}

void encoding(int k,struct data* head) {
  struct data * temp3 = head;
  int i, j, key, n1, n2, space, newline;
  char fname[30];

  int kar; 
  printf("Enter File Name (add .txt) : ");
  scanf(" %[^\n]s", fname);

  FILE * fdata;
  fdata = fopen(fname, "w");

  if (fdata == NULL) {
    perror("Error ");
  }

  //Kunci untuk enkripsi
  printf("Enter Key: ");
  scanf("%d", & key);

  printf("Encryption Result: \n");

  //temp3 = head;

  omp_set_num_threads(4); //Set jumlah thread

  #pragma omp parallel 
  {
    int numt, tid, awal, akhir;
    int l;
    numt = omp_get_num_threads();
    tid = omp_get_thread_num();
    awal = (k / numt) * tid;//awal loop
    akhir = (k / numt) * (tid + 1);//akhir loop

    if (tid == numt - 1) {
      akhir = k;
    }
	#pragma omp barrier
   //pencegahan race condition
    #pragma omp critical
	  {
      printf("\nThread %d (%d - %d): ", tid, awal, akhir);

      //Loop sampai urutan node sesuai dengan urutan data
      while ((temp3 -> urutan) < awal) {
        temp3 = temp3 -> next;
      }
      //print hasil enkripsi ke text file
      for (i = awal; i < akhir; i++) {
        //memasukkan nama ke local array
        char nama[30];
        strcpy(nama, temp3 -> nama);

        //memasukkan umur ke local array
        int umur;
        umur = temp3 -> umur;

        //memasukkan alamat ke local array
        char alamat[30];
        strcpy(alamat, temp3 -> alamat);
		
        //Memasukkan data nama kedalam text file
        for (j = 0; nama[j] != '\0'; j++) {
          kar = nama[j] + key;
          //Menggunakan enkripsi dengan shift ASCII number
          printf("%d ", kar);
          //Print hasil enkripsi
          fprintf(fdata, "%d ", kar);
          //Data disimpan         
        }

        //Memasukkan data umur kedalam text file
        if (umur >= 10) {
          //Mengambil digit pertama dan kedua dari umur
          n1 = umur / 10;
          n2 = umur % 10;

          kar = n1 + key;
          space = 32 + key;

          //Menggunakan enkripsi dengan shift ASCII number
          printf("%d %d ", space, intToAscii(kar));
          fprintf(fdata, "%d %d ", space, intToAscii(kar));
          kar = n2 + key;
          printf("%d %d ", intToAscii(kar), space);
          //Print hasil enkripsi
          fprintf(fdata, "%d %d ", intToAscii(kar), space);
          //Data disimpan   
		   
        } else if (umur < 10) {
          kar = umur + key;
          space = 32 + key;
          printf("%d %d %d ", space, intToAscii(kar), space);
          fprintf(fdata, "%d %d %d ", space, intToAscii(kar), space);
        }
		//Menggunakan enkripsi dengan shift ASCII number
        for (j = 0; alamat[j] != '\0'; j++) {
          kar = alamat[j] + key;
          //Print hasil enkripsi	
          printf("%d ", kar);
          //Data disimpan 
          fprintf(fdata, "%d ", kar);
        }
        //Print ASCII untuk new line
        newline = 10 + key;
        printf("%d", newline);
        fprintf(fdata, "%d", newline);

        //Lanjut ke node berikutnya
        temp3 = temp3 -> next;
        printf("\n");
        fprintf(fdata, "\n");
      }

    }

  }
  fclose(fdata);
  printf("\nFile Has Been Encrypted and Saved as %s\n", fname);
}

int decoding() {
  char fname1[30], fname2[30];
  char decode[1000] = {0};
  int i = 0, key;
  FILE * fdata1;
  FILE * fdata2;

  //Meminta input kepada user 
  printf("==============Decryption==============\n");
  printf("Exixting Filename (add .txt): ");
  scanf(" %[^\n]s", fname1);
  printf("Destination Filename (add .txt): ");
  scanf(" %[^\n]s", fname2);

  //Membuka file yang akan di decode
  fdata1 = fopen(fname1, "r");
  if (fdata1 == NULL) {
    perror("Error ");
    return 0;
  }

  //membuka file untuk menaruh hasil decoding ke dalam file
  fdata2 = fopen(fname2, "w");
  if (fdata2 == NULL) {
    perror("Error ");
    return 0;
  }

  printf("Enter Key: ");
  scanf("%d", & key);

  //decrypt algorithm
  printf("\nDecrypting...\n\n");
  i = 0;
  #pragma omp parallel
	{
		#pragma omp single
			{
			#pragma omp critical
			{
				  while (fscanf(fdata1, "%d", & decode[i]) != EOF) {
				    decode[i] = decode[i] - key;
				    printf("%c", decode[i]);
				    fprintf(fdata2, "%c", decode[i]);
				    i++;
				  }
			  }
			}
}
  fclose(fdata1);
  fclose(fdata2);
  printf("\nFile Has Been Decrypted and Saved as %s\n", fname2);
}
//Function main menu
void mainmenu() {
  system("CLS");
  printf("======================================\n");
  printf("     Data Encryptor and Decryptor\n");
  printf("======================================\n");
  printf("\t    Menu Selection\n");
  printf("\t    1. Data Input\n");
  printf("\t    2. Decryptor\n");
  printf("\t    3. Exit\n");
  printf("======================================\n");
  printf("Selection: ");
}
//Function untuk linked list
int Node(struct data** head, int umur, char name[20], char alamat1[30], int m) {
  struct data * temp = NULL;
  temp = malloc(sizeof(struct data));
  int i;

  //memasukkan data ke linked list
  temp -> urutan = m;
  strcpy(temp -> alamat, alamat1);
  temp -> umur = umur;
  strcpy(temp -> nama, name);
  temp -> next = NULL;

  if (m == 1) {
    temp -> next = *head;
    *head = temp;
    return 0;
  }

  struct data * temp1;
  temp1 = *head;

  for (i = 0; i < m - 2; i++) {
    temp1 = temp1 -> next;
  }

  temp -> next = temp1 -> next;
  temp1 -> next = temp;
}
