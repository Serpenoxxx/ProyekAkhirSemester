#include <stdio.h>
#include <string.h>
/*Note: 
- setiap array kalo bisa dibuat linked list
- Proram masi khusus untuk enkirpsi, kalo bisa dibuat 
  jadi database
- Jan lupa parallel
*/

struct data 
{
  char nama[20];
  int umur;
  char alamat[30];
  struct data* next;
};
	struct data *head;


void encoding();
void decoding();




int main() 
{
  int select,k,i,umur,m;
  char alamat1[30];
  char name[30];
  struct data* temp2;
  head = NULL;
  printf("Masukan jumlah data yang ingin diinput:");
  scanf("%d",&k);

  for(i=0;i<k;i++){
		printf("Masukkan nama %d: ", i+1);
		scanf(" %[^\n]s", name);
		printf("Masukkan umur %d: ", i+1);
		scanf("%d", &umur);
		printf("Masukkan alamat %d: ", i+1);
		scanf(" %[^\n]s", alamat1);
		
		Node(umur,name,alamat1,m);
		m++; //input berapa kali 
	
	}


  //end of input

  printf("Pilih menu\n");
  printf("1. Enconding\n");
  printf("2. Decoding\n");
  printf("Pilihan: ");
  scanf("%d", &select);
  switch(select)
  {
    case 1:
       encoding();
      break;
      
    case 2:
       decoding();
      break;

    default:
      printf("Invalid Input!");
      break;
  }

}

void encoding(){
  int i, key;
  char fname[30];
  char data[1000];
  int kar;//int atau char perlu di config
  printf("Masukkan nama file (tambahkan .txt dibelakang) : ");
  gets(fname);
  
  FILE *fdata;
  fdata = fopen(fname, "w");
  if (fdata == NULL){
    printf("Error");
    }
    
  printf("Masukkan data: \n");
  gets(data);
  printf("Masukkan key: ");
  scanf("%d", &key);
  
  printf("Data hasil enkripsi: \n");
  for(i=0;data[i]!='\0';i++){
  	kar = data[i] + key; 
    //Menggunakan enkripsi dengan shift ASCII number
  	printf("%d ", kar);
    //Print hasil enkripsi
    fprintf(fdata,"%d ", kar);
    //Data disimpan
  }
  fclose(fdata);
}

void decoding(){
  char fname1[30],fname2[30];
  int num[1000] = {0};//Perlu dibuat dynamic
  int i=0;
  FILE* fdata1;
  FILE* fdata2;

 //get input from the user 
  printf("Masukkan nama file yang ingin dibuka: ");
  gets(fname1);
  printf("\nMasukkan nama file yang ingin dituju:");
  gets(fname2);

  //open both files
  fdata1 = fopen(fname1, "r");
  if(fdata1 == NULL)
    {
      return 0;
    }
    
  fdata2 = fopen(fname2, "w");
  if(fdata2 == NULL)
    {
      return 0;
    }

//decrypt algorithm
  i =0;
	while(fscanf(fdata1, "%d ", &num[i])!= EOF){
    num[i] = num[i] - 5;
    i++; 
	}








}

int Node(int umur, char name[20], char alamat1[30], int m){
	struct data *temp;	
	temp = malloc(sizeof(struct data));
	int i;
	strcpy(temp->alamat, alamat1);
	temp->umur = umur;
	strcpy(temp->nama, name);
	temp->next = NULL;
	if(m==1){
		temp->next = head;
		head = temp;
		return 0;	
	}
	struct data *temp1;
	temp1 = head;
	for(i=0;i<m-2;i++){
		temp1 = temp1->next;
	}
	temp->next = temp1->next;
	temp1->next = temp;
}